//=============================================================================
//
// ui処理 [ui.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "ui.h"
#include "manager.h"
#include "scene3D.h"
#include "title.h"
#include "renderer.h"
#include "sound.h"
#include "gamepad.h"
#include "input.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CUi::m_pTexture[UITYPE_MAX] = {}; //テクスチャ情報
int CUi::m_PointerCount = 0;                         //UIのポインタ情報
D3DXVECTOR3 CUi::m_posPause;                         //uiの位置情報
int  CUi::m_nPauseType = 0;                          //uiのタイプ情報

//=============================================================================
// コンストラクタ
//=============================================================================
CUi::CUi(int nPriority, int nType) : CScene2D(nPriority, nType)
{
	//メンバ変数初期化
	m_BlinkingCount = 0;
	m_nType = 0;
	m_PointerCount = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, float fHight, float fWidth,int nType)
{
	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//カラー設定関数
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//変数に代入
	m_nType = nType;
	m_pos = pos;
	m_firstpos = pos;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUi::Uninit(void)
{
	//終了処理
	CScene2D::Uninit();

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUi::Update(void)
{
	//関数呼び出し
	TitleUi();
}

//=============================================================================
// 描画処理
//=============================================================================
void CUi::Draw(void)
{
	//描画処理
	CScene2D::Draw();
}

//=============================================================================
// テクスチャ設定処理
//=============================================================================
void CUi::Load(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/trop.png", &m_pTexture[UITYPE_FREAM]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg.jpg", &m_pTexture[UITYPE_BG]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamestart.png", &m_pTexture[UITYPE_START]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutoriallogo.png", &m_pTexture[UITYPE_TUTORIAL]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/exit.png", &m_pTexture[UITYPE_EXIT]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pointer.png", &m_pTexture[UITYPE_POINTER]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamemes.png", &m_pTexture[UITYPE_GAMEMESSAGE]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rankingmenu.png", &m_pTexture[UITYPE_RESULTMENU]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/EnemyMa.png", &m_pTexture[UITYPE_ENEMYREMAINING]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultHp.png", &m_pTexture[UITYPE_RESULTHP]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/addscore.png", &m_pTexture[UITYPE_RESULTADD]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/equal-red3.png", &m_pTexture[UITYPE_RESULTEQUAL]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/1位.png", &m_pTexture[RANKING_FIRST]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/2位.png", &m_pTexture[RANKING_SECOND]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/3位.png", &m_pTexture[RANKING_THIRD]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/4位.png", &m_pTexture[RANKING_FOUR]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/5位.png", &m_pTexture[RANKING_FIVE]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamelogo.png", &m_pTexture[TITLE_LOGO]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture[TUTORIAL_MENU]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pausebg.png", &m_pTexture[PAUSE_FASE]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/PauseMenu.png", &m_pTexture[PAUSE_MENU]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ReturnTitle.png", &m_pTexture[PAUSE_TITLE]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GameRestart.png", &m_pTexture[PAUSE_RESTART]);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pausefream.png", &m_pTexture[PAUSE_FREAM]);
}


//=============================================================================
//uiのテクスチャ破棄処理
//=============================================================================
void CUi::Unload(void)
{
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
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
// タイトルのUI設定処理
//=============================================================================
void CUi::TitleUi(void)
{
	//サウンドを取得
	m_pSound = CManager::GetSound();

	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	if (CTitle::GetNotMove() == false)
	{
		//uiのタイプがタイトルのポインタだった場合
		if (m_nType == UITYPE_POINTER)
		{
			//Sキーが押された場合
			if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN) == true)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				m_pos.y += 120.0f;        //位置情報をプラスする
				m_PointerCount++;         //カウントを回す
				if (m_PointerCount == 3)  //カウントが3になった場合
				{
					m_PointerCount = 0;   //カウントを0に戻してあげる
					m_pos = m_firstpos;   //最初の位置に戻す
				}
			}
			//Sキーが押された場合
			else if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP) == true)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				if (m_PointerCount == 0)  //カウントが0になった場合
				{
					m_PointerCount = 3;   //カウントを3にしてあげる
					m_pos.y = m_firstpos.y + 120.0f * 3;  //一番最後の位置にする
				}
				m_pos.y -= 120.0f;        //位置をずらす
				m_PointerCount--;         //カウントを引いていく
			}
			//位置情報を引数から読み込み反映させる
			SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	//uiのタイプがタイトルの場合とポインタのカウントが0の時
	if (m_nType == UITYPE_START && m_PointerCount == 0)
	{
		//間隔のカウントアップ
		m_BlinkingCount++;

		if (m_BlinkingCount % 30 == 0)//間隔調整
		{
			//透明にする
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		else if (m_BlinkingCount % 15 == 0 && m_BlinkingCount % 30)//間隔調整
		{
			//元に戻す
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//uiのタイプがチュートリアルだった場合
	else if (m_nType == UITYPE_TUTORIAL && m_PointerCount == 1)
	{
		//間隔のカウントアップ
		m_BlinkingCount++;

		if (m_BlinkingCount % 30 == 0)//間隔調整
		{
			//透明にする
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		else if (m_BlinkingCount % 15 == 0 && m_BlinkingCount % 30)//間隔調整
		{
			//元に戻す
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//uiのタイプがイグジットだった場合
	else if (m_nType == UITYPE_EXIT && m_PointerCount == 2)
	{
		//間隔のカウントアップ
		m_BlinkingCount++;

		if (m_BlinkingCount % 30 == 0)//間隔調整
		{
			//透明にする
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		else if (m_BlinkingCount % 15 == 0 && m_BlinkingCount % 30)//間隔調整
		{
			//元に戻す
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//ポインタのカウントが1の場合
	if (m_PointerCount == 1)
	{
		//uiタイプがスタートかもしくはイグジットの
		if (m_nType == UITYPE_START || m_nType == UITYPE_EXIT)
		{
			//カラーを元に戻す
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//ポインタのカウントが2の場合
	else if (m_PointerCount == 2)
	{
		//uiタイプがチュートリアルかもしくはスタートの
		if (m_nType == UITYPE_TUTORIAL || m_nType == UITYPE_START)
		{
			//カラーを元に戻す
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//ポインタのカウントが0の場合
	else if (m_PointerCount == 0)
	{
		//uiタイプがイグジットかもしくはチュートリアルの
		if (m_nType == UITYPE_EXIT || m_nType == UITYPE_TUTORIAL)
		{
			//カラーを元に戻す
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
//=============================================================================
// ポーズのUI設定処理
//=============================================================================
void CUiPause::PauseUi(void)
{
	//サウンドを取得
	m_pSound = CManager::GetSound();

	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//ポーズ画面
	if (CManager::GetPause() == true)
	{
		//uiのタイプがタイトルのポインタだった場合
		if (m_nType == PAUSE_FREAM)
		{
			//Sキーが押された場合
			if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN) == true)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				m_pos.y += 210.0f;        //位置情報をプラスする
				m_PointerCount++;         //カウントを回す
				if (m_PointerCount == 2)  //カウントが2になった場合
				{
					m_PointerCount = 0;   //カウントを0に戻してあげる
					m_pos = m_firstpos;   //最初の位置に戻す
				}
			}
			//Sキーが押された場合
			else if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP) == true)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				if (m_PointerCount == 0)  //カウントが0になった場合
				{
					m_PointerCount = 2;   //カウントを2にしてあげる
					m_pos.y = m_firstpos.y + 210.0f * 2;  //一番最後の位置にする
				}
				m_pos.y -= 210.0f;        //位置をずらす
				m_PointerCount--;         //カウントを引いていく
			}
			//位置情報を引数から読み込み反映させる
			SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
}
//=============================================================================
// UIを動かす設定処理
//=============================================================================
void CUi::MoveUi(D3DXVECTOR3 pos, int nType)
{
	//位置情報取得
	m_posPause = pos;
	//タイプ情報取得
	m_nPauseType = nType;
}

//=============================================================================
// 生成処理
//=============================================================================
CUi * CUi::Create(D3DXVECTOR3 pos, float fHight, float fWidth,int nType)
{
	//モデルを生成
	CUi *pUi = NULL;
	pUi = new CUi;

	//NULLチェック
	if (pUi != NULL)
	{
		//オブジェクトの初期化処理
		pUi->Init(pos, fHight, fWidth, nType);

		//テクスチャ処理
		pUi->BindTexture(m_pTexture[nType]);
	}

	return pUi;
}


//=============================================================================
//
// メイン処理 [uipause.cpp]
// Author : Miura Yudai
//
//=============================================================================

//ポーズ画面に表示させたい場合

//=============================================================================
// コンストラクタ
//=============================================================================
CUiPause::CUiPause(int nPriority, int nType) : CUi(nPriority, nType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CUiPause::~CUiPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUiPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	//初期化処理
	CScene2D::Init(pos, fSizeY, fSizeX);

	//カラー設定関数
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//変数に代入
	m_nType = nType;
	m_pos = pos;
	m_firstpos = pos;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUiPause::Uninit(void)
{
	//終了処理
	CScene2D::Uninit();

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUiPause::Update(void)
{
	//関数呼び出し
	PauseUi();
}

//=============================================================================
// 描画処理
//=============================================================================
void CUiPause::Draw(void)
{
	//描画処理
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CUiPause * CUiPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// ローカル変数宣言
	CUiPause *pUiPause = NULL;
	pUiPause = new CUiPause;

	// 初期化
	if (pUiPause != NULL)
	{
		pUiPause->Init(pos, fSizeX, fSizeY, nType);
	}

	// テクスチャ割り当て
	pUiPause->BindTexture(m_pTexture[nType]);

	return pUiPause;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CUiPause::Unload(void)
{
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (m_pTexture != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
