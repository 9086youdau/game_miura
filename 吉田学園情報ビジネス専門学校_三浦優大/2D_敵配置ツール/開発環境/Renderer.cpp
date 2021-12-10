//================================================
//
//レンダラー処理[Renderer.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "Renderer.h"
#include "Scene.h"
#include "manager.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "debugproc.h"

//=================================================
//静的メンバ変数宣言
//=================================================
CPause *CRenderer::m_apPause = NULL;
bool CRenderer::m_Pause = false;
CCamera *CRenderer::m_pCamera = NULL;
CMeshfield *CRenderer::m_pField = NULL;
CMeshwall *CRenderer::m_pWall = NULL;
CLight *CRenderer::m_pLight = NULL;
CDebugProc *CRenderer::m_pDebug = NULL;

//=================================================
//コンストラクタ
//=================================================
CRenderer::CRenderer()
{
	
}
//=================================================
//デストラクタ
//=================================================
CRenderer::~CRenderer()
{

}

//=================================================
//初期化処理
//=================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 35, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFont);

		m_dwGameStartTime = timeGetTime();//ゲーム開始時刻

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	//ローカル変数宣言
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;

	//残像処理
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//レンダリングターゲットのテクスチャ生成
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_apTextureMT[nCnt], NULL);

		//テクスチャレンダリング用のインターフェースの生成
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRendererMT[nCnt]);

		//Zバッファの生成
		m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_apBuffMT[nCnt], NULL);

		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		m_pD3DDevice->SetRenderTarget(0, m_apRendererMT[nCnt]);

		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		//レンダリングターゲットのテクスチャクリア
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	//フィードバック用のポリゴン生成
	VERTEX_2D *pVtx;

	//頂点バッファの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
	}

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f - 0.5f, SCREEN_HEIGHT - 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f - 0.5f, 0.0f - 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 0.5f, SCREEN_HEIGHT - 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 0.5f, 0.0f - 0.5f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuffMT->Unlock();

	//デバックの設定
	m_pDebug = new CDebugProc;
	m_pDebug->Init();

	return S_OK;
}

//=================================================
//終了処理
//=================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// 情報表示用フォントの破棄
	if (m_pTimeFont != NULL)
	{
		m_pTimeFont->Release();
		m_pTimeFont = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	//頂点バッファの開放
	if (m_pVtxBuffMT != NULL)
	{
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = NULL;
	}

	//テクスチャインターフェースの破棄
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}

		if (m_apRendererMT[nCnt] != NULL)
		{
			m_apRendererMT[nCnt]->Release();
			m_apRendererMT[nCnt] = NULL;
		}

		if (m_apBuffMT[nCnt] != NULL)
		{
			m_apBuffMT[nCnt]->Release();
			m_apBuffMT[nCnt] = NULL;
		}
	}
}

//=================================================
//更新処理
//=================================================
void CRenderer::Update(void)
{
	//シーンの全てのアップデート
	CScene::UpdateAll();

	//boolの値を返す
	m_Pause = CGame::GetPause();

	//モード代入
	m_mode = CManager::GetMode();
}

//=================================================
//描画処理
//=================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255),
		1.0f,
		0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ポリゴンの描画処理
		CScene::DrawAll();

		//デバックデータの文字列
		m_pDebug->Print("エネミーカウント【%d】\nエネミーのサブカウント【%d / %d】\n指定したカウントで再生【F8】\nエネミーカウントを初期化【F5】\nエネミーカウントを調整【F4】【F6】\nポーズ状態【P】",CGame::GetEnemyCount(),CGame::GetEnemySubCount(), CGame::GetEnemyMaxCount());

		//デバックプロックの描画
		m_pDebug->Draw();

#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// FPS計算処理
//=============================================================================
#ifdef _DEBUG
void CRenderer::DrawFPS(void)
{
	int nCnt = GetFPS();
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", nCnt);
	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif
//=============================================================================
// DrawTime計算処理
//=============================================================================
void CRenderer::DrawTime(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//char str[256];

	if (m_bEndGame == false)
	{
		//sprintf(str, "TIME:%.3f\n", (float)m_dwGameTime / 1000.f);
	}

	// テキスト描画
	//m_pTimeFont->DrawText(NULL, str, -1, &rect, DT_CENTER, D3DCOLOR_ARGB(0xff, 0x00, 0xff, 0xff));
}
