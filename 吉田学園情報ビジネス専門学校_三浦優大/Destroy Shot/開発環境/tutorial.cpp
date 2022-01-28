//=============================================================================
//
// チュートリアル画面処理 [tutorial.cpp]
// Author : Miura Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "tutorial.h"
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"
#include "ranking.h"
#include "number.h"
#include "ui.h"
#include "gamepad.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//ui生成
	CUi::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 18);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//画面遷移	決定ボタンを押した場合
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
	{
		//モード設定
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);		// シーン遷移
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTutorial * CTutorial::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// リザルト画面の生成
	CTutorial *pTutorial;
	pTutorial = new CTutorial;

	// NULLチェック
	if (pTutorial != NULL)
	{
		//チュートリアルの初期化処理
		pTutorial->Init(pos, fSizeX, fSizeY);
	}

	return pTutorial;
}
