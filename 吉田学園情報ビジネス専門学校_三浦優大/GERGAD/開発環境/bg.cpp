//================================================
//
//バックグラウンド処理[Player.h]
// Author; miura yuudai
//
//================================================
#include "bg.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureBG[5] = {};//テクスチャ情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;//頂点情報
BG g_bg[5];//バックグラウンド情報

//====================
//バックグラウンドの初期化処理
//====================
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/14.png", &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/14.png", &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &g_pTextureBG[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ddd.png", &g_pTextureBG[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hplogo.png", &g_pTextureBG[4]);


	//変数初期化
	for (int nCount = 0; nCount < 5; nCount++)
	{
		g_bg[nCount].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_bg[nCount].nType = 0;
		g_bg[nCount].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 5,
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
	
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(200, 1030, 0.0f);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(200, 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1220, 1030, 0.0f);
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

	return S_OK;
}
//==================
//バックグラウンドの終了処理
//==================
void UninitBG(void)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//テクスチャの開放
		if (g_pTextureBG[nCount] != NULL)
		{
			g_pTextureBG[nCount]->Release();
			g_pTextureBG[nCount] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

}
//==================
//バックグラウンドの更新処理
//==================
void UpdateBG(void)
{

}

//==================
//バックグラウンドの描画処理
//==================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < 5; nCount++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCount]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}
//==================
//バックグラウンドの設定処理
//==================
void SetBG(D3DXVECTOR3 Pos, int nType)
{
	BG *pbg;
	VERTEX_2D *pVtx;

	pbg = &g_bg[0];

	// 頂点バッファをロック
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < 5; nCount++, pbg++)
	{
		if (g_bg[nCount].bUse == false)//バックグラウンドを使用しているかどうか
		{
			if (nType == 0)//タイプが0の時
			{
				//位置設定
				g_bg[nCount].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
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

		if (nType == 1)//タイプが1の時
		{
			//位置設定
			g_bg[nCount].Pos = Pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(50, 1030, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(50, 50, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(1220, 1030, 0.0f);
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

		if (nType == 2)//タイプが2の時
		{
			//位置設定
			g_bg[nCount].Pos = Pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_bg[nCount].Pos.x - 150, g_bg[nCount].Pos.y + 100, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_bg[nCount].Pos.x - 150, g_bg[nCount].Pos.y - 100, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_bg[nCount].Pos.x + 600, g_bg[nCount].Pos.y + 100, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_bg[nCount].Pos.x + 600, g_bg[nCount].Pos.y - 100, 0.0f);//2Dは0.0固定

			//タイプ設定
			pbg->nType = nType;

			//バックグラウンドの使用
			pbg->bUse = true;
			break;
		}

		if (nType == 3)//タイプが2の時
		{
			//位置設定
			g_bg[nCount].Pos = Pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_bg[nCount].Pos.x - 150, g_bg[nCount].Pos.y + 150, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_bg[nCount].Pos.x - 150, g_bg[nCount].Pos.y - 150, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_bg[nCount].Pos.x + 150, g_bg[nCount].Pos.y + 150, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_bg[nCount].Pos.x + 150, g_bg[nCount].Pos.y - 150, 0.0f);//2Dは0.0固定

			//タイプ設定
			pbg->nType = nType;

			//バックグラウンドの使用
			pbg->bUse = true;
			break;
		}

		if (nType == 4)//タイプが2の時
		{
			//位置設定
			g_bg[nCount].Pos = Pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_bg[nCount].Pos.x - 25, g_bg[nCount].Pos.y + 25, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_bg[nCount].Pos.x - 25, g_bg[nCount].Pos.y - 25, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_bg[nCount].Pos.x + 25, g_bg[nCount].Pos.y + 25, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_bg[nCount].Pos.x + 25, g_bg[nCount].Pos.y - 25, 0.0f);//2Dは0.0固定

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

