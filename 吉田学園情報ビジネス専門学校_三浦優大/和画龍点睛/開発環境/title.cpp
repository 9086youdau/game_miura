//=============================================================================
//
//タイトル処理[title.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "title.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "gamepad.h"
#include "sound.h"
#include "SceneX.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture[TITLETYPE_MAX] = {};
bool CTitle::m_bExit = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	//メンバ変数初期化
	m_nCount = 255;
	m_titleType = TITLETYPE_GAME;
	m_nCounterAnim = 0.0f;
	m_pos = D3DXVECTOR3(950.0f, 0.0f, 0.0f);
	m_posui = D3DXVECTOR3(-120.0f, 950.0f, 0.0f);
	m_moveui = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
//デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//テキスチャロード
	Load();

	//関数呼び出し
	SetVertex();

	//サウンドのポインタ宣言
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGM設定
	pSound->Stop(pSound->SOUND_LABEL_BGM002);

	//BGM設定
	pSound->Play(pSound->SOUND_LABEL_BGM000);

	//返り値
	return S_OK;
}

//=============================================================================
//タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//テクスチャアンロード
	Unload();

	//破棄関数
	Release();
}

//=============================================================================
//タイトルの更新処理
//=============================================================================
void CTitle::Update(void)
{
	//多重背景処理
	MoveBg();

	//セレクト処理
	TitleSelect();

	//タイトルロゴの移動
	m_pos.y += 3.0f;

	//タイトルのuiの移動
	m_moveui.x = cosf(-35) * -5.5f;
	m_moveui.y = sinf(-35) * -5.5f;

	//ある位置まできたらストップ
	if (m_pos.y >= 250)
	{
		m_pos.y = 250.0f;
	}

	//ある位置まできたらストップ
	if (m_posui.y <= 750)
	{
		m_moveui.x = 0.0f;
		m_moveui.y = 0.0f;
	}

	//位置更新
	m_posui += m_moveui;

	//位置更新
	m_pScene2D[TITLETYPE_TITLELOGO]->SetPosition(m_pos,D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_pScene2D[TITLETYPE_UI]->SetPosition(m_posui,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//タイトルの描画処理
//=============================================================================
void CTitle::Draw(void)
{
}

//=============================================================================
//タイトルの作成・設定処理
//=============================================================================
CTitle * CTitle::Create(void)
{
	//インスタンス生成
	CTitle *pTitle = NULL;
	pTitle = new CTitle;

	//初期化呼び出し
	pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pTitle;
}

//=============================================================================
//タイトルのテクスチャロード処理
//=============================================================================
HRESULT CTitle::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlebg3.jpg", &m_pTexture[TITLETYPE_BG]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlemovebg.png", &m_pTexture[TITLETYPE_MOVEBG]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlemovebg2.png", &m_pTexture[TITLETYPE_MOVEBG2]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlesircle.png", &m_pTexture[TITLETYPE_UI]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlelogo.png", &m_pTexture[TITLETYPE_TITLELOGO]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleStartCover.png", &m_pTexture[TITLETYPE_GAME]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleExitCover.png", &m_pTexture[TITLETYPE_EXIT]);

	return S_OK;
}

//=============================================================================
//タイトルのテクスチャアンロード処理
//=============================================================================
void CTitle::Unload(void)
{
	//ループ
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
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
//タイトルの描画設定処理
//=============================================================================
void CTitle::SetVertex(void)
{
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++)
	{
		if (nCntTitle == TITLETYPE_BG)
		{
			//タイトルのインスタンス生成
			m_pScene2D[TITLETYPE_BG] = new CScene2D;
			m_pScene2D[TITLETYPE_BG]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
			m_pScene2D[TITLETYPE_BG]->BindTexture(m_pTexture[TITLETYPE_BG]);
		}

		else if (nCntTitle == TITLETYPE_MOVEBG)
		{
			//タイトルのインスタンス生成
			m_pScene2D[TITLETYPE_MOVEBG] = new CScene2D;
			m_pScene2D[TITLETYPE_MOVEBG]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
			m_pScene2D[TITLETYPE_MOVEBG]->BindTexture(m_pTexture[TITLETYPE_MOVEBG]);
		}

		else if (nCntTitle == TITLETYPE_MOVEBG2)
		{
			//タイトルのインスタンス生成
			m_pScene2D[TITLETYPE_MOVEBG2] = new CScene2D;
			m_pScene2D[TITLETYPE_MOVEBG2]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
			m_pScene2D[TITLETYPE_MOVEBG2]->BindTexture(m_pTexture[TITLETYPE_MOVEBG2]);
		}

		else if (nCntTitle == TITLETYPE_UI)
		{
			//タイトルのインスタンス生成
			m_pScene2D[TITLETYPE_UI] = new CScene2D;
			m_pScene2D[TITLETYPE_UI]->Init(m_posui, 300.0f, 300.0f);
			m_pScene2D[TITLETYPE_UI]->BindTexture(m_pTexture[TITLETYPE_UI]);
		}

		else if (nCntTitle == TITLETYPE_TITLELOGO)
		{
			//タイトルのインスタンス生成
			m_pScene2D[TITLETYPE_TITLELOGO] = new CScene2D;
			m_pScene2D[TITLETYPE_TITLELOGO]->Init(m_pos, 300.0f, 800.0f);
			m_pScene2D[TITLETYPE_TITLELOGO]->BindTexture(m_pTexture[TITLETYPE_TITLELOGO]);
		}

		else if (nCntTitle == TITLETYPE_GAME)
		{
			//タイトルのインスタンス生成
			m_pScene2D[TITLETYPE_GAME] = new CScene2D;
			m_pScene2D[TITLETYPE_GAME]->Init(D3DXVECTOR3(1300.0f, 750.0f, 0.0f), 120.0f, 600.0f);
			m_pScene2D[TITLETYPE_GAME]->BindTexture(m_pTexture[TITLETYPE_GAME]);
			m_pScene2D[TITLETYPE_GAME]->SetTex(1.0f, 0);
		}

		else if (nCntTitle == TITLETYPE_EXIT)
		{
			//タイトルのインスタンス生成
			m_pScene2D[TITLETYPE_EXIT] = new CScene2D;
			m_pScene2D[TITLETYPE_EXIT]->Init(D3DXVECTOR3(1300.0f, 900.0f, 0.0f), 120.0f, 600.0f);
			m_pScene2D[TITLETYPE_EXIT]->BindTexture(m_pTexture[TITLETYPE_EXIT]);
			m_pScene2D[TITLETYPE_EXIT]->SetTex(1.0f, 0);
		}
	}
}
//=============================================================================
//タイトルの多重背景処理
//=============================================================================
void CTitle::MoveBg(void)
{
	//カウントを進める
	m_nCounterAnim -= 0.0008f;

	for (int nCnt = 1; nCnt < 3; nCnt++)
	{
		//背景のテクスチャ処理
		m_pScene2D[nCnt]->SetBg(1.0f, m_nCounterAnim * (nCnt + 1));
	}
}

//=============================================================================
//タイトルの選択処理
//=============================================================================
void CTitle::TitleSelect(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//モード代入
	CManager::MODE mode;
	mode = CManager::GetMode();

	//タイトルタイプがstartgameの時
	if (m_titleType == TITLETYPE_GAME)
	{
		//カウントアップ
		m_nCount++;

		if (m_nCount % 30 == 0)//間隔
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 0, 0, 0.5f));
		}

		else if (m_nCount % 15 == 0 && m_nCount % 30)//間隔
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 0, 0, 1.0f));
		}

		//モードがタイトルの時
		if (mode == CManager::MODE_TITLE)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				//ゲーム画面へ遷移
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);
			}
		}
	}
	//タイトルタイプがexitの時
	else if (m_titleType == TITLETYPE_EXIT)
	{
		//カウントアップする
		m_nCount++;

		if (m_nCount % 30 == 0)//間隔
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 0, 0, 0.5f));
		}

		else if (m_nCount % 15 == 0 && m_nCount % 30)//間隔
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 0, 0, 1.0f));
		}

		//モードがタイトルの時
		if (mode == CManager::MODE_TITLE)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				// ウィンドウを破棄するよう指示する
				m_bExit = true;
			}
		}
	}

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_UP) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		if (m_titleType == TITLETYPE_GAME)
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_EXIT;
		}
		else if (m_titleType == TITLETYPE_EXIT)
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_GAME;
		}
	}

	else if (pInputGamePad->IsButtonDown(CInput::KEYINFO_DOWN) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		if (m_titleType == TITLETYPE_GAME)
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_EXIT;
		}
		else if (m_titleType == TITLETYPE_EXIT)
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_GAME;
		}
	}
}
