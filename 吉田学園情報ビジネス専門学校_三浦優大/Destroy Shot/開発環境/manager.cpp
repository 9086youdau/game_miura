//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "sound.h"
#include "gamepad.h"
#include "fade.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "pause.h"
#include "camera.h"
#include "texture.h"
#include "light.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CRenderer		*CManager::m_pRenderer = NULL;
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;
CInput			*CManager::m_pInput = NULL;
CSound			*CManager::m_pSound = NULL;
CScene			*CManager::m_pScene = NULL;
CInputGamePad	*CManager::m_pGamepad = NULL;
CGame			*CManager::m_pGame = NULL;
CCamera			*CManager::m_pCamera = NULL;
CLight			*CManager::m_pLight = NULL;
CSceneX			*CManager::m_pSceneX = NULL;
CTexture		*CManager::m_pTexture = NULL;
CTitle			*CManager::m_pTitle = NULL;
CResult			*CManager::m_pResult = NULL;
CTutorial		*CManager::m_pTutorial = NULL;
CPause			*CManager::m_pPause = NULL;

//初期の画面
CManager::MODE	CManager::m_mode = MODE_TITLE;
//経過時間を表示させる
bool	CManager::m_bEndGame = false;
bool	g_bPause = false;			//ポーズ状態

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	//初期化
	m_pTimeFont = NULL;
	m_dwGameStartTime = 0;
	m_dwGameTime = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//レンダラー作成
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, bWindow);
	}

	//キーボード生成
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	//ゲームパッド生成
	if (m_pGamepad == NULL)
	{
		m_pGamepad = new CInputGamePad;
		m_pGamepad->Init(hInstance, hWnd);
	}

	//サウンド生成
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		m_pSound->Init(hWnd);
	}

	//カメラ生成
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
		m_pCamera->Init();
	}

	//ライト生成
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
		m_pLight->Init();
	}

	//テクスチャクラスを生成
	if (m_pTexture == NULL)
	{
		m_pTexture = new CTexture;
		m_pTexture->Load();
	}

	//モード設定
	SetMode(m_mode);

	// タイマーの初期化
	InitTime();

	return S_OK;
}

//==============================================================================
// モード選択
//==============================================================================
void CManager::SetMode(MODE mode)
{
	//終了処理
	CScene::ReleaseAll();

	//フェードをクリエイト
	CFade::Create(m_mode);

	//画面遷移時に初期化
	g_bPause = false;

	switch (mode)
	{
	case MODE_TITLE:
		//タイトル画面の初期化処理
		m_pTitle->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_TUTORIAL:
		//チュートリアル画面の初期化処理
		m_pTutorial->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_GAME:
		//ゲーム画面の初期化処理
		m_pGame->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		//ポーズ画面の生成
		m_pPause->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_RESULT:
		//リザルト画面の初期化処理
		m_pResult->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;
	}
	
	//モード変更
	m_mode = mode;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//オブジェクトクラスの破棄
	CScene::ReleaseAll();

	//テクスチャクラスの破棄
	m_pTexture->Unload();
	m_pTexture = NULL;

	//入力処理の破棄
	if (m_pInput != NULL)
	{
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = NULL;
	}

	//サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//レンダリングクラスの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//ゲームパッドクラスの破棄
	if (m_pRenderer != NULL)
	{
		m_pGamepad->Uninit();
		delete m_pGamepad;
		m_pGamepad = NULL;
	}

	// タイマーフォントの破棄
	UninitTime();
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//カメラの更新
	m_pCamera->Update();

	//キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//ゲームパッドの更新処理
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Update();
	}

	//レンダラーの更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//ゲーム画面時のみポーズ
	if (CManager::m_mode == MODE_GAME)
	{
		//ポーズの切り替えオン
		if (g_bPause == false)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true || pGamepad->IsButtonDown(CInput::START))
			{
				g_bPause = true;
			}
		}

		//ポーズの切り替えオフ
		else if (g_bPause == true)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true || pGamepad->IsButtonDown(CInput::START))
			{
				g_bPause = false;
			}
		}
	}

	// タイマー更新
	UpdateTime();
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	//全てのレンダラーの描画
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//==============================================================================
// タイマー初期化
//==============================================================================
HRESULT CManager::InitTime(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetRenderer()->GetDevice();

	// フォントオブジェクトの生成
	D3DXCreateFont(
		pDevice, 36, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pTimeFont);

	// 開始時刻取得
	m_dwGameStartTime = timeGetTime();

	return S_OK;
}

//==============================================================================
// タイマー更新
//==============================================================================
void CManager::UninitTime(void)
{
	if (m_pTimeFont != NULL)
	{
		m_pTimeFont->Release();
		m_pTimeFont = NULL;
	}
}

//==============================================================================
// タイマー更新
//==============================================================================
void CManager::UpdateTime(void)
{
	if (m_bEndGame == false)
	{
		// 経過時間更新
		m_dwGameTime = timeGetTime() - m_dwGameStartTime;
	}
}

//==============================================================================
// タイマー描画
//==============================================================================
void CManager::DrawTime(void)
{
#ifdef _DEBUG
	// ローカル変数宣言
	RECT rect = { 0,30,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	if (m_bEndGame == false)
	{
		//経過時間をメンバ関数に入れる
		m_fTime = (float)m_dwGameTime / 1000;
		
		//画面に表示させる
		sprintf(&aStr[0], "経過時間 : %.3f\n", m_fTime);

		// テキストの描画
		m_pTimeFont->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
#endif
}

//=============================================================================
// キーボードのGet関数
//=============================================================================
CInputKeyboard * CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// レンダラーのGet関数
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// サウンドのGet関数
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// カメラのGet関数
//=============================================================================
CCamera * CManager::GetCamera(void)
{
	return m_pCamera;
}
//=============================================================================
// テクスチャのGet関数
//=============================================================================
CTexture * CManager::GetTexture(void)
{
	return m_pTexture;
}
//=============================================================================
// マネージャーのGet関数
//=============================================================================
bool CManager::GetPause()
{
	return g_bPause;
}
