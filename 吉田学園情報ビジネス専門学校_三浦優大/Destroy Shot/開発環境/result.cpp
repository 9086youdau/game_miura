//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : Miura Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "result.h"
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"
#include "ranking.h"
#include "number.h"
#include "ui.h"
#include "sound.h"
#include "gamepad.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CScore *CResult::m_pScore[5];

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//サウンドを取得
	m_pSound = CManager::GetSound();

	//サウンドBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM000);

	//uiのテクスチャロード
	CUi::Load();

	//スコアのテクスチャロード
	CNumber::Load();

	//ui生成
	CUi::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 1);

	//ui生成
	CUi::Create(D3DXVECTOR3(600.0f, 50.0f, 0.0f), 150.0f, 750.0f, 7);

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//ui生成
		CUi::Create(D3DXVECTOR3(750.0f, 450.0f + (nCnt * 120.0f), 0.0f), 100.0f, 500.0f, 0);
	}

	//ui生成
	CUi::Create(D3DXVECTOR3(670.0f, 280.0f, 0.0f), 150.0f, 650.0f, 9);

	//ui生成
	CUi::Create(D3DXVECTOR3(650.0f, 300.0f, 0.0f), 100.0f, 100.0f, 10);

	//ui生成
	CUi::Create(D3DXVECTOR3(1230.0f, 300.0f, 0.0f), 100.0f, 100.0f, 11);

	// 順位ロゴ
	CUi::Create(D3DXVECTOR3(550.0f, 460.0f, 0.0f), 90, 120, 12);
	CUi::Create(D3DXVECTOR3(550.0f, 580.0f, 0.0f), 90, 120, 13);
	CUi::Create(D3DXVECTOR3(550.0f, 700.0f, 0.0f), 90, 120, 14);
	CUi::Create(D3DXVECTOR3(560.0f, 820.0f, 0.0f), 65, 90, 15);
	CUi::Create(D3DXVECTOR3(560.0f, 940.0f, 0.0f), 65, 90, 16);

	//プレイヤースコア
	CScore::Create(D3DXVECTOR3(150.0f, 350.0f, 0.0f));
	//ゲーム時のスコアを呼び出す
	CScore::SetScore(CScore::GetScore());

	//ランキングデータの管理　読み込み＆並び替え
	CRanking::Create(D3DXVECTOR3(1180.0f, 500.0f, 0.0f), 100.0f, 100.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//サウンドの破棄
	m_pSound->Stop(CSound::SOUND_LABEL_BGM000);

	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
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
void CResult::Draw(void)
{

}

//=============================================================================
// 生成処理
//=============================================================================
CResult * CResult::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// リザルト画面の生成
	CResult *pResult;
	pResult = new CResult;

	// NULLチェック
	if (pResult != NULL)
	{
		pResult->Init(pos, fSizeX, fSizeY);
	}

	return pResult;
}
