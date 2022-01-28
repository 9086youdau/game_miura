﻿//=============================================================================
//
// レンダラー処理 [renderer.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "sound.h"
#include "manager.h"
#include "debugproc.h"
#include "player3D.h"
#include "camera.h"
#include "passerby.h"
#include "scene2D.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CDebugProc *CRenderer::m_pDebug = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	//メンバ変数初期化
	m_nCntMaxPart = 10;
	m_nCntOldMax = 1;
	m_nCntOld = 1;
	m_nForMin = 0;
	m_nForMax = 10;
	m_nMaxPlayerPart = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
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
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	)))
	{

		if (FAILED(m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice
		)))
		{

			if (FAILED(m_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

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
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小時に補間
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大時に補間
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U値の繰り返し
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V値の繰り返し

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // α値の合成
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//レンダリングターゲット用のテクスチャの生成
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT[nCnt], NULL);

		//テクスチャレンダリング用インターフェースの生成
		m_pTextureMT[nCnt]->GetSurfaceLevel(0, &m_pRenderMT[nCnt]);

		//テクスチャレンダリング用Zバッファの生成
		m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT[nCnt], NULL);

		//現在のレンダリングターゲットを取得
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//現在のZバッファを取得
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		//レンダリングターゲットを設定
		m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[nCnt]);

		//Zバッファを設定
		m_pD3DDevice->SetDepthStencilSurface(m_pBuffMT[nCnt]);

		//テクスチャ0をクリアする
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//現在のレンダリングターゲットを戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//現在のZバッファを戻す
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	//ビューポートを設定
	m_ViewportMT.X = (DWORD)0.0f;
	m_ViewportMT.Y = (DWORD)0.0f;
	m_ViewportMT.Width = SCREEN_WIDTH;
	m_ViewportMT.Height = SCREEN_HEIGHT;
	m_ViewportMT.MinZ = 0.0f;
	m_ViewportMT.MaxZ = 1.0f;

	// 頂点バッファの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,							// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffMT->Unlock();

	//デバックの設定
	m_pDebug = new CDebugProc;
	m_pDebug->Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
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

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{

		if (m_pTextureMT != NULL)
		{
			m_pTextureMT[nCnt]->Release();
			m_pTextureMT[nCnt] = NULL;
		}

		if (m_pRenderMT != NULL)
		{
			m_pRenderMT[nCnt]->Release();
			m_pRenderMT[nCnt] = NULL;
		}

		if (m_pBuffMT != NULL)
		{
			m_pBuffMT[nCnt]->Release();
			m_pBuffMT[nCnt] = NULL;
		}
	}

	if (m_pDebug != NULL)
	{
		m_pDebug->Uninit();
		m_pDebug = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	//シーンのオールアップデート
	CScene::UpdateAll();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	CManager *pManager = GetManager();

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ポリゴンの描画処理
		CScene::DrawAll();

		// 経過時間
		//pManager->DrawTime();

//#ifdef _DEBUG
		// FPS表示
		CRenderer::DrawFPS();

		//デバックログ
		//m_pDebug->Print("+----------------------------------------------------+\n");
		//m_pDebug->Print("|キーフレーム数変更 [N] : 【%d】                     |\n", CPlayer3D::GetFrame());
		//m_pDebug->Print("|FRAME                  : 【%d / %d】                 |\n", CPlayer3D::GetCounter(), CPlayer3D::GetFrame());
		//m_pDebug->Print("+----------------------------------------------------+\n");


		//デバックプロックの描画
		m_pDebug->Draw();
//#endif
		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// 描画処理(残像あり)
//=============================================================================
void CRenderer::Draw2(void)
{
	CManager *pManager = GetManager();

	LPDIRECT3DSURFACE9 pRenderWk;
	LPDIRECT3DTEXTURE9 pTextureWk;

	//マルチターゲット
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//現在の情報を記憶
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//現在の情報を記憶
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		//レンダリングターゲットをテクスチャ0に設定
		m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[0]);

		//バッファをテクスチャ0に設定
		m_pD3DDevice->SetDepthStencilSurface(m_pBuffMT[0]);

		//テクスチャ0をクリアする
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// ポリゴンの描画処理
		CScene::DrawAll();

		// 経過時間
		pManager->DrawTime();

#ifdef _DEBUG
		// FPS表示
		//CRenderer::DrawFPS();
#endif
		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		//フィードバック用のポリゴンにテクスチャ1を貼り付ける
		m_pD3DDevice->SetTexture(0, m_pTextureMT[1]);

		// ポリゴンの描画処理
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//不透明のポリゴンを表示する
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		//元に戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//元に戻す
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);

		// 頂点バッファをデータストリームに設定
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		//フィードバック用のポリゴンにテクスチャ0を貼り付ける
		m_pD3DDevice->SetTexture(0, m_pTextureMT[0]);

		// ポリゴンの描画処理
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//不透明のポリゴンを表示から戻す
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//テクスチャ0とテクスチャ1を入れ替える
		pRenderWk = m_pRenderMT[0];
		m_pRenderMT[0] = m_pRenderMT[1];
		m_pRenderMT[1] = pRenderWk;

		//テクスチャ0とテクスチャ1を入れ替える
		pTextureWk = m_pTextureMT[0];
		m_pTextureMT[0] = m_pTextureMT[1];
		m_pTextureMT[1] = pTextureWk;

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	//記憶用
	D3DXMATRIX mtxView, mtxProjection;
	float fAspert;

	//レンダラーを設置
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT[0]);

	//バッファを設定
	m_pD3DDevice->SetDepthStencilSurface(m_pBuffMT[0]);

	//ビューポートを設置
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &vecU);

	//ビューマトリックスの生成
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	fAspert = ((float)m_ViewportMT.Width) / (float)m_ViewportMT.Height;

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&mtxProjection,
		D3DXToRadian(45.0f),													// 画角の設定
		fAspert,																// 画面比率の設定 ビューポートの幅高さ		
		10.0f,																	// 手前限界の描画距離
		1000.0f);																// 奥限界の描画距離

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//=============================================================================
// FPSに関する処理
//=============================================================================
void CRenderer::DrawFPS()
{
#ifdef _DEBUG
	int nCountFPS = GetFPS();
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	//画面に表示
	wsprintf(str, "FPS:%d\n", nCountFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
#endif
}
