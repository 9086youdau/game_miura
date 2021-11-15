//=============================================================================
//
//チュートリアル処理[tutorial.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "tutorial.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "gamepad.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[2] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{
	//メンバ変数初期化
	m_nCount = 255;
}
//=============================================================================
//デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
//タイトルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//テキスチャロード
	Load();

	//タイトルのインスタンス生成
	m_pScene2D[0] = new CScene2D;
	m_pScene2D[0]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[0]->BindTexture(m_pTexture[0]);
	m_pScene2D[0]->SetTex(1.0f, 0);

	//タイトルのインスタンス生成
	m_pScene2D[1] = new CScene2D;
	m_pScene2D[1]->Init(D3DXVECTOR3(1250.0f, 50.0f, 0.0f), 80.0f, 500.0f);
	m_pScene2D[1]->BindTexture(m_pTexture[1]);
	m_pScene2D[1]->SetTex(1.0f, 0);

	return S_OK;
}

//=============================================================================
//タイトルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	//テクスチャアンロード
	Unload();

	//破棄関数
	Release();
}

//=============================================================================
//タイトルの更新処理
//=============================================================================
void CTutorial::Update(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//モードを持ってくる
	CManager::MODE mode;
	mode = CManager::GetMode();

	//カウントアップ
	m_nCount++;

	if (m_nCount % 30 == 0)//間隔
	{
		//カラー変更
		m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}

	else if (m_nCount % 15 == 0 && m_nCount % 30)//間隔
	{
		//カラー変更
		m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//モードがタイトルの時
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			//ランキング画面へ遷移
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
		}
	}
}

//=============================================================================
//タイトルの描画処理
//=============================================================================
void CTutorial::Draw(void)
{
}

//=============================================================================
//タイトルの作成・設定処理
//=============================================================================
CTutorial * CTutorial::Create(void)
{
	//インスタンス生成
	CTutorial *pTutorial = NULL;
	pTutorial = new CTutorial;

	//初期化呼び出し
	pTutorial->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pTutorial;
}

//=============================================================================
//タイトルのテクスチャロード処理
//=============================================================================
HRESULT CTutorial::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture[0]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pressabutton.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
//タイトルのテクスチャアンロード処理
//=============================================================================
void CTutorial::Unload(void)
{
	//ループ
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}