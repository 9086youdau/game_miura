//=============================================================================
//
//ポーズ処理[pause.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "pause.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "game.h"
#include "gamepad.h"
#include "bullet.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_pTexture[PAUSETYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 CPause::m_pVtxBuff = NULL;
CPause::PAUSETYPE CPause::m_pauseType = PAUSETYPE_RETURN_GAME;

//=============================================================================
//コンストラクタ
//=============================================================================
CPause::CPause()
{
	//メンバ変数初期化
	m_pauseType = PAUSETYPE_RETURN_GAME;
}
//=============================================================================
//デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//ポーズの初期化処理
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//テキスチャロード
	Load();

	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < PAUSETYPE_MAX; nCntTitle++,pVtx += 4)
	{
		if (nCntTitle == PAUSETYPE_BG)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);
		}

		else if (nCntTitle == PAUSETYPE_PAUSE)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(1300.0f, 200.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1300.0f, 400.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(600.0f, 200.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 400.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);

			//テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		else if (nCntTitle == PAUSETYPE_RETURN_GAME)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(1300.0f, 550.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1300.0f, 700.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(600.0f, 550.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 700.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[1].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[2].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[3].col = D3DXCOLOR(255, 0, 0, 255);

			//テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		else if (nCntTitle == PAUSETYPE_RETURN_TITLE)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(1300.0f, 750.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1300.0f, 900.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(600.0f, 750.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 900.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);

			//テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//ポーズの終了処理
//=============================================================================
void CPause::Uninit(void)
{
	//テクスチャアンロード
	Unload();

	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//タイトルの更新処理
//=============================================================================
void CPause::Update(void)
{
	//関数呼び出し
	SelectPause();
}

//=============================================================================
//ポーズの描画処理
//=============================================================================
void CPause::Draw(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntPause]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//=============================================================================
//ポーズの作成・設定処理
//=============================================================================
CPause * CPause::Create(void)
{
	//インスタンス生成
	CPause *pPause = NULL;
	pPause = new CPause;

	//初期化呼び出し
	pPause->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pPause;
}

//=============================================================================
//ポーズのテクスチャロード処理
//=============================================================================
HRESULT CPause::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pausebg.png", &m_pTexture[PAUSETYPE_BG]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/PAUSEMENU.png", &m_pTexture[PAUSETYPE_PAUSE]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RESETGAME.png", &m_pTexture[PAUSETYPE_RETURN_GAME]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RETURNTITLE.png", &m_pTexture[PAUSETYPE_RETURN_TITLE]);

	return S_OK;
}

//=============================================================================
//ポーズのテクスチャアンロード処理
//=============================================================================
void CPause::Unload(void)
{
	//ループ
	for (int nCnt = 0; nCnt < PAUSETYPE_MAX; nCnt++)
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
//ポーズのセレクト処理
//=============================================================================
void CPause::SelectPause(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//モード更新
	CManager::MODE mode;
	mode = CManager::GetMode();

	//boolの値を返す
	bool bPause = CGame::GetPause();

	if (m_pauseType == PAUSETYPE_RETURN_GAME)
	{
		//モードがタイトルの時
		if (mode == CManager::MODE_GAME)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				//ゲーム画面へ遷移
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
				//体力をリセットする
				CBullet::SetLifeRecovery(3);
			}
		}
	}
	else if (m_pauseType == PAUSETYPE_RETURN_TITLE)
	{
		//モードがタイトルの時
		if (mode == CManager::MODE_GAME)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				//ゲーム画面へ遷移
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
			}
		}
	}


	//ポインタの構造体
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_DOWN) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		if (m_pauseType == PAUSETYPE_RETURN_GAME)
		{
			//頂点カラーの設定
			pVtx[8].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[9].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[10].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[11].col = D3DXCOLOR(255, 255, 255, 255);

			//頂点カラーの設定
			pVtx[12].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[13].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[14].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[15].col = D3DXCOLOR(255, 0, 0, 255);

			m_pauseType = PAUSETYPE_RETURN_TITLE;
		}
		else if (m_pauseType == PAUSETYPE_RETURN_TITLE)
		{
			//頂点カラーの設定
			pVtx[12].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[13].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[14].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[15].col = D3DXCOLOR(255, 255, 255, 255);

			//頂点カラーの設定
			pVtx[8].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[9].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[10].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[11].col = D3DXCOLOR(255, 0, 0, 255);

			m_pauseType = PAUSETYPE_RETURN_GAME;
		}
	}

	else if (pInputGamePad->IsButtonDown(CInput::KEYINFO_UP) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		if (m_pauseType == PAUSETYPE_RETURN_GAME)
		{
			//頂点カラーの設定
			pVtx[8].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[9].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[10].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[11].col = D3DXCOLOR(255, 255, 255, 255);

			//頂点カラーの設定
			pVtx[12].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[13].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[14].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[15].col = D3DXCOLOR(255, 0, 0, 255);

			//タイプ変更
			m_pauseType = PAUSETYPE_RETURN_TITLE;
		}
		else if (m_pauseType == PAUSETYPE_RETURN_TITLE)
		{
			//頂点カラーの設定
			pVtx[12].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[13].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[14].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[15].col = D3DXCOLOR(255, 255, 255, 255);

			//頂点カラーの設定
			pVtx[8].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[9].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[10].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[11].col = D3DXCOLOR(255, 0, 0, 255);

			//タイプ変更
			m_pauseType = PAUSETYPE_RETURN_GAME;
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

