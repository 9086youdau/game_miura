//=============================================================================
//
//リザルト処理[result.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "result.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "Renderer.h"
#include "fade.h"
#include "number.h"
#include "score.h"
#include "gamepad.h"
#include "Graze.h"
#include "sound.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CResult::m_pTexture[8] = {};
CNumber *CResult::m_apNumber[2][8] = {};
int CResult::m_nResultScore = 0;
int CResult::m_nResultAllScore = 0;
int CResult::m_nNumber[8] = {};
int CResult::m_nAllNumber[8] = {};
CScore *CResult::m_pScore = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CResult::CResult()
{
	//メンバ変数初期化
	m_nCounterAnim = 0.0f;
}
//=============================================================================
//デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//リザルトの初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//サウンドのポインタ宣言
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGM設定
	pSound->Stop(pSound->SOUND_LABEL_BGM001);

	//BGM設定
	pSound->Play(pSound->SOUND_LABEL_BGM002);

	//テクスチャロード
	Load();

	//スコアのテクスチャロード
	CNumber::Load();

	//ゲームのスコアを代入
	m_nResultScore = CScore::GetScore();

	//関数呼び出し
	SetVertex();

	//グレイズ表示
	m_pGraze = CGraze::Create(1300.0f, 550.0f, 40.0f, 30.0f);

	//スコアの作成・設定関数
	m_pScore = CScore::Create(CScore::SCORETYPE_RESULT_SCORE);

	//返り値
	return S_OK;
}

//=============================================================================
//リザルトの終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//テクスチャアンロード
	Unload();

	//スコアのテクスチャロード
	CNumber::Unload();

	//終了処理
	m_pGraze->Uninit();
	m_pGraze = NULL;

	//終了処理
	m_pScore->Uninit();
	m_pScore = NULL;

	//破棄関数
	Release();
}

//=============================================================================
//リザルトの更新処理
//=============================================================================
void CResult::Update(void)
{
	//関数呼び出し
	TimesGraze();

	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//モードがタイトルの時
	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			//ランキング画面へ遷移
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RANKING);
		}
	}

	//カウントを進める
	m_nCounterAnim -= 0.0008f;

	for (int nCnt = 1; nCnt < 3; nCnt++)
	{
		//背景のテクスチャ処理
		m_pScene2D[nCnt]->SetBg(1.5f, m_nCounterAnim * (nCnt + 1));
	}
}

//=============================================================================
//リザルトの描画処理
//=============================================================================
void CResult::Draw(void)
{

}
//=============================================================================
//リザルトのテクスチャ処理
//=============================================================================
HRESULT CResult::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlebg3.jpg", &m_pTexture[0]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultMoveBg.png", &m_pTexture[1]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultMoveBg2.png", &m_pTexture[2]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultLogo.png", &m_pTexture[3]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MaxScore.png", &m_pTexture[4]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MaxGraze.png", &m_pTexture[5]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Times.png", &m_pTexture[6]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultScore.png", &m_pTexture[7]);

	return S_OK;
}
//=============================================================================
//リザルトのテクスチャアンロード処理
//=============================================================================
void CResult::Unload(void)
{
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
//リザルトの作成・設定処理
//=============================================================================
CResult * CResult::Create(void)
{
	//インスタンス生成
	CResult *pResult = NULL;
	pResult = new CResult;

	//初期化呼び出し
	pResult->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pResult;
}

//=============================================================================
//グレイズの計算処理
//=============================================================================
void CResult::TimesGraze(void)
{
	//計算処理
	m_nResultAllScore = m_nResultScore * CGraze::GetGraze();
}

//=============================================================================
//リザルトの設定処理
//=============================================================================
void CResult::SetVertex(void)
{
	//リザルトのインスタンス生成
	m_pScene2D[0] = new CScene2D;
	m_pScene2D[0]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[0]->BindTexture(m_pTexture[0]);

	//リザルトのインスタンス生成
	m_pScene2D[1] = new CScene2D;
	m_pScene2D[1]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[1]->BindTexture(m_pTexture[1]);

	//リザルトのインスタンス生成
	m_pScene2D[2] = new CScene2D;
	m_pScene2D[2]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[2]->BindTexture(m_pTexture[2]);

	//リザルトのインスタンス生成
	m_pScene2D[3] = new CScene2D;
	m_pScene2D[3]->Init(D3DXVECTOR3(350.0f, 0.0f, 1.0f), 300.0f, 1200.0f);
	m_pScene2D[3]->BindTexture(m_pTexture[3]);

	//リザルトのインスタンス生成
	m_pScene2D[4] = new CScene2D;
	m_pScene2D[4]->Init(D3DXVECTOR3(200.0f, 350.0f, 1.0f), 150.0f, 600.0f);
	m_pScene2D[4]->BindTexture(m_pTexture[4]);

	//リザルトのインスタンス生成
	m_pScene2D[5] = new CScene2D;
	m_pScene2D[5]->Init(D3DXVECTOR3(1050.0f, 350.0f, 1.0f), 150.0f, 600.0f);
	m_pScene2D[5]->BindTexture(m_pTexture[5]);

	//リザルトのインスタンス生成
	m_pScene2D[6] = new CScene2D;
	m_pScene2D[6]->Init(D3DXVECTOR3(550.0f, 450.0f, 1.0f), 200.0f, 900.0f);
	m_pScene2D[6]->BindTexture(m_pTexture[6]);

	//リザルトのインスタンス生成
	m_pScene2D[7] = new CScene2D;
	m_pScene2D[7]->Init(D3DXVECTOR3(550.0f, 650.0f, 1.0f), 200.0f, 800.0f);
	m_pScene2D[7]->BindTexture(m_pTexture[7]);
}
