//================================================
//
//バックグラウンド処理[BG.cpp]
// Author; miura yuudai
//
//================================================

//================================================
//ヘッダー読み込み
//================================================
#include "bg.h"

//============================================
//マクロ定義
//============================================
#define MAXBG (5)//バックグラウンドの最大数

//============================================
//グローバル変数
//============================================
LPDIRECT3DTEXTURE9 g_pTextureBG[MAXBG] = {};//テクスチャ情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;//頂点情報
BG g_bg[MAXBG];//バックグラウンド情報

//========================================
//バックグラウンドの初期化処理
//========================================
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/14.png", &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/14.png", &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &g_pTextureBG[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ShipType04.png", &g_pTextureBG[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hplogo.png", &g_pTextureBG[4]);


	//グローバル変数初期化
	for (int nCount = 0; nCount < MAXBG; nCount++)
	{
		g_bg[nCount].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_bg[nCount].nType = 0;
		g_bg[nCount].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAXBG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	
	//for文ループ
	for (int nCountBg = 0; nCountBg < MAXBG; nCountBg++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(200, 1030, 0.0f);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(200, 50, 0.0f);//2Dは0.0固定
		pVtx[2].pos = D3DXVECTOR3(1220, 1030, 0.0f);//2Dは0.0固定
		pVtx[3].pos = D3DXVECTOR3(1220, 50, 0.0f);//2Dは0.0固定

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;//頂点情報をずらす

	}
	//頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();

	//返り値
	return S_OK;
}
//=================================
//バックグラウンドの終了処理
//=================================
void UninitBG(void)
{
	for (int nCountBg = 0; nCountBg < MAXBG; nCountBg++)
	{
		//テクスチャの開放
		if (g_pTextureBG[nCountBg] != NULL)
		{
			g_pTextureBG[nCountBg]->Release();
			g_pTextureBG[nCountBg] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

}
//=====================================
//バックグラウンドの更新処理
//=====================================
void UpdateBG(void)
{

}

//======================================
//バックグラウンドの描画処理
//======================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCountBg = 0; nCountBg < MAXBG; nCountBg++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCountBg]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCountBg * 4, 2);
	}
}
//=========================================
//バックグラウンドの設定処理
//=========================================
void SetBG(D3DXVECTOR3 Pos, int nType)
{
	//ポインタ変数
	BG *pbg;
	pbg = &g_bg[0];

	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountBg = 0; nCountBg < MAXBG; nCountBg++, pbg++)
	{
		if (g_bg[nCountBg].bUse == false)//バックグラウンドを使用しているかどうか
		{
			if (nType == 0)//タイプが0の時
			{
				//位置設定
				g_bg[nCountBg].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);//2Dは0.0固定
				pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);//2Dは0.0固定

				 //頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 150);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 150);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 150);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 150);

				//タイプ設定
				pbg->nType = nType;

				//バックグラウンドの使用
				pbg->bUse = true;
				break;
			}

			else if (nType == 1)//タイプが1の時
			{
				//位置設定
				g_bg[nCountBg].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(50, 1030, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(50, 50, 0.0f);//2Dは0.0固定
				pVtx[2].pos = D3DXVECTOR3(1220, 1030, 0.0f);//2Dは0.0固定
				pVtx[3].pos = D3DXVECTOR3(1220, 50, 0.0f);//2Dは0.0固定

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				//タイプ設定
				pbg->nType = nType;

				//バックグラウンドの使用
				pbg->bUse = true;
				break;
			}

			else if (nType == 2)//タイプが2の時
			{
				//位置設定
				g_bg[nCountBg].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x - 150, g_bg[nCountBg].Pos.y + 100, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x - 150, g_bg[nCountBg].Pos.y - 100, 0.0f);//2Dは0.0固定
				pVtx[2].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x + 600, g_bg[nCountBg].Pos.y + 100, 0.0f);//2Dは0.0固定
				pVtx[3].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x + 600, g_bg[nCountBg].Pos.y - 100, 0.0f);//2Dは0.0固定

				//タイプ設定
				pbg->nType = nType;

				//バックグラウンドの使用
				pbg->bUse = true;
				break;
			}

			else if (nType == 3)//タイプが2の時
			{
				//位置設定
				g_bg[nCountBg].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x - 50, g_bg[nCountBg].Pos.y + 50, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x - 50, g_bg[nCountBg].Pos.y - 50, 0.0f);//2Dは0.0固定
				pVtx[2].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x + 50, g_bg[nCountBg].Pos.y + 50, 0.0f);//2Dは0.0固定
				pVtx[3].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x + 50, g_bg[nCountBg].Pos.y - 50, 0.0f);//2Dは0.0固定

				//タイプ設定
				pbg->nType = nType;

				//バックグラウンドの使用
				pbg->bUse = true;
				break;
			}

			else if (nType == 4)//タイプが2の時
			{
				//位置設定
				g_bg[nCountBg].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x - 25, g_bg[nCountBg].Pos.y + 25, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x - 25, g_bg[nCountBg].Pos.y - 25, 0.0f);//2Dは0.0固定
				pVtx[2].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x + 25, g_bg[nCountBg].Pos.y + 25, 0.0f);//2Dは0.0固定
				pVtx[3].pos = D3DXVECTOR3(g_bg[nCountBg].Pos.x + 25, g_bg[nCountBg].Pos.y - 25, 0.0f);//2Dは0.0固定

				//タイプ設定
				pbg->nType = nType;

				//バックグラウンドの使用
				pbg->bUse = true;
				break;
			}
		}
		pVtx += 4;//4頂点ごとずらす
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();
}

