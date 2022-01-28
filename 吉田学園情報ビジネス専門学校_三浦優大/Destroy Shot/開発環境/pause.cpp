//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Miura Yudai
//
//=============================================================================

//==============================================================================
// インクルードファイル読みこみ
//==============================================================================
#include "pause.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"
#include "ui.h"
#include "gamepad.h"
#include "sound.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
bool CPause::m_bNotMove = false;

//==============================================================================
// マクロ定義
//==============================================================================
#define MENU_PAUSE_MAX (2)				// 選択できるモードの数
#define SCREEN_TRANSITION_SPEED (30)	// 画面遷移の速度

#define LOGO_RESTART_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f))		// リスタートのロゴの位置
#define LOGO_MENU_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f))			// メニューののロゴの位置
#define LOGO_EXIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f))			// 終了のロゴの位置	

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause(int nPriority, int nType) : CScene(nPriority, nType)
{
	// 初期化
	m_nFadeCntPause = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ui生成
	CUiPause::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 19);

	//ui生成
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, 0);

	//ui生成
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.05f, SCREEN_HEIGHT / 2.1f, 0.0f), 500.0f, 100.0f, 23);

	//ui生成
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f, SCREEN_HEIGHT / 8, 0.0f), 800.0f, 150.0f, 20);

	//ui生成
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f, SCREEN_HEIGHT / 1.65f, 0.0f), 800.0f, 150.0f, 21);

	//ui生成
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f, SCREEN_HEIGHT / 2.45f, 0.0f), 800.0f, 150.0f, 22);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	//サウンドを取得
	m_pSound = CManager::GetSound();

	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//uiのポインタがゲームスタートの所だったら
	if (CUi::GetPointCount() == 0)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true)
		{
			//停止フラグ
			m_bNotMove = true;
			//サウンドBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
			//ゲームに移動する
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
		}
	}

	//uiのポインタがチュートリアルの所だったら
	if (CUi::GetPointCount() == 1)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true)
		{
			//停止フラグ
			m_bNotMove = true;
			//サウンドBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
			//チュートリアルに移動する
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPause * CPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CPause *pPause;
	pPause = new CPause;

	// NULLチェック
	if (pPause != NULL)
	{
		pPause->Init(pos, fSizeX, fSizeY);
	}

	return pPause;
}