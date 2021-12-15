//================================================
//
//マネージャー処理[manager.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "manager.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "fade.h"
#include "gamepad.h"

//=================================================
//静的メンバ変数宣言
//=================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputGamePad *CManager::m_pInputGamePad = NULL;
CSound *CManager::m_pSound = NULL;
CGame *CManager::m_pGame = NULL;
CManager::MODE CManager::m_mode = MODE_GAME;

//=============================================================================
//コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//マネージャーの初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow)
{
	//レンダラーの生成
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//キーボードの生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	//ゲームパッドの生成
	m_pInputGamePad = new CInputGamePad;
	m_pInputGamePad->Init(hInstance, hWnd);

	//サウンドの生成
	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	//モード設定
	SetMode(m_mode);

	return S_OK;
}

//=============================================================================
//マネージャーの終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//ゲームパッドの破棄
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}

	//キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//レンダラーの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//リリース関数
	CScene::ReleaseAll();
}

//=============================================================================
//マネージャーの更新処理
//=============================================================================
void CManager::Update(void)
{
	//キーボードの更新処理
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad->Update();
	}

	//キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//レンダラーの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
//マネージャーの描画処理
//=============================================================================
void CManager::Draw(void)
{
	//全ての描画設定
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
//画面遷移の処理
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//サウンドのポインタ宣言
	CSound *pSound;
	pSound = CManager::GetSound();

	//全ての要素を破棄
	CScene::ReleaseAll();

	//フェードをクリエイト
	CFade::Create(m_mode);

	//モード選択
	switch (mode)
	{
	case MODE_GAME:
		//ゲームのクリエイト
		m_pGame->Create();
		break;
	}

	//値を代入
	m_mode = mode;
}


