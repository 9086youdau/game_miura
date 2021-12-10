//================================================
//
//ランキング処理[ranking.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "ranking.h"
#include "number.h"
#include "score.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "gamepad.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "result.h"
#include "Graze.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CRanking::m_pTexture[4] = {};
CScore *CRanking::m_pScore = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CRanking::CRanking()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
//ランキングの初期化処理
//=============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{ 
	//ランキングテクスチャロード
	Load();

	//スコアのテクスチャロード
	CNumber::Load();

	//関数呼び出し
	SetVertex();

	//ランキングスコア表示
	m_pScore = CScore::Create(CScore::SCORETYPE_RANKING_SCORE);

	//グレイズの初期化
	CGraze::CalculatioGraze(0);

	//返り値
	return S_OK;
}

//=============================================================================
//ランキングの終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	//テクスチャ終了処理
	Unload();

	//スコアのテクスチャロード
	CNumber::Unload();

	//終了処理
	m_pScore->Uninit();
	m_pScore = NULL;

	//破棄関数
	Release();
}

//=============================================================================
//ランキングの更新処理
//=============================================================================
void CRanking::Update(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		//ゲーム画面へ遷移
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//=============================================================================
//ランキングの描画処理
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
//ランキングの設定・作成処理
//=============================================================================
CRanking * CRanking::Create(void)
{
	//インスタンス生成
	CRanking *pRanking = NULL;
	pRanking = new CRanking;

	//初期化呼び出し
	pRanking->Init(D3DXVECTOR3(0.0f,0.0f,0.0f), MAX_SCORE_SIZE_Y, MAX_SCORE_SIZE_X);

	return pRanking;
}

//==============================================================================
//順位設定処理
//==============================================================================
void CRanking::SetVertex(void)
{
	float m_Rank = 0;

	//ランキングの順位インスタンス生成
	m_pScene2D[0] = new CScene2D;
	m_pScene2D[0]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[0]->BindTexture(m_pTexture[0]);

	for (int nCnt = 1; nCnt < 6; nCnt++)
	{
		//ランキングの順位インスタンス生成
		m_pScene2D[nCnt] = new CScene2D;
		m_pScene2D[nCnt]->Init(D3DXVECTOR3((nCnt * 55.0f) + 100.0f,(nCnt * 155.0f) + 120.0f, 1.0f), 120.0f, 250.0f);
		m_pScene2D[nCnt]->BindTexture(m_pTexture[1]);
		m_pScene2D[nCnt]->SetRanking(m_Rank);

		m_Rank += 0.2f;
	}

	//ランキングの順位インスタンス生成
	m_pScene2D[6] = new CScene2D;
	m_pScene2D[6]->Init(D3DXVECTOR3(350.0f, 0.0f, 1.0f), 300.0f, 1200.0f);
	m_pScene2D[6]->BindTexture(m_pTexture[2]);

	int nCntY = 0;

	for (int nCnt = 7; nCnt < 12; nCnt++)
	{
		//ランキングの順位インスタンス生成
		m_pScene2D[nCnt] = new CScene2D;
		m_pScene2D[nCnt]->Init(D3DXVECTOR3((nCnt * 55.0f) + 210.0f, (nCntY * 155.0f) + 270.0f, 1.0f), 120.0f, 900.0f);
		m_pScene2D[nCnt]->BindTexture(m_pTexture[3]);

		//カウントアップ
		nCntY += 1;
	}
}
//=============================================================================
//ランキングのテクスチャロード処理
//=============================================================================
HRESULT CRanking::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlebg3.jpg", &m_pTexture[0]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Ranking_Rank2.png", &m_pTexture[1]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RankingMenu.png", &m_pTexture[2]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TITLECOVER.png", &m_pTexture[3]);

	return S_OK;
}
//=============================================================================
//ランキングのテクスチャアンロード処理
//=============================================================================
void CRanking::Unload(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
