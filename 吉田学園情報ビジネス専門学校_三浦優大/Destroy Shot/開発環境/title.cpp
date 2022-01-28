//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : Miura Yudai
//
//=============================================================================

//==============================================================================
// インクルードファイル読み込み
//==============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"
#include "gamepad.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
bool CTitle::m_bExit = false;
bool CTitle::m_bNotMove = false;

//==============================================================================
// コンストラクタ
//==============================================================================
CTitle::CTitle()
{
	//メンバ変数初期化
	m_bNotMove = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CTitle::~CTitle()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//サウンドを取得
	m_pSound = CManager::GetSound();

	//サウンドBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM000);

	//uiのテクスチャロード
	CUi::Load();

	//ui生成
	CUi::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 1);

	for (int nCnt = 2; nCnt < 5; nCnt++)
	{
		//ui生成
		CUi::Create(D3DXVECTOR3(760.0f, 450.0f + (nCnt * 120.0f), 0.0f), 100.0f, 450.0f, nCnt);
	}

	//ui生成
	CUi::Create(D3DXVECTOR3(650.0f, 730.0f, 0.0f), 50.0f, 250.0f, 5);

	//ui生成
	CUi::Create(D3DXVECTOR3(725.0f, 550.0f, 0.0f), 100.0f, 500.0f, 6);

	//ui生成
	CUi::Create(D3DXVECTOR3(725.0f, 50.0f, 0.0f), 500.0f, 500.0f, 17);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CTitle::Uninit(void)
{
	//BGMの破棄
	m_pSound->Stop(CSound::SOUND_LABEL_BGM000);

	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CTitle::Update(void)
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
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true  || pGamepad->IsButtonDown(CInput::A) == true)
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
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);
		}
	}

	//uiのポインタが終了の所だったら
	if (CUi::GetPointCount() == 2)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true)
		{
			//停止フラグ
			m_bNotMove = true;
			//サウンドBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
			// ウィンドウを破棄するよう指示する
			m_bExit = true;
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CTitle::Draw(void)
{

}

//==============================================================================
// 生成処理
//==============================================================================
CTitle * CTitle::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CTitle *pTitle;
	pTitle = new CTitle;

	// NULLチェック
	if (pTitle != NULL)
	{
		pTitle->Init(pos, fSizeX, fSizeY);
	}

	return pTitle;
}
