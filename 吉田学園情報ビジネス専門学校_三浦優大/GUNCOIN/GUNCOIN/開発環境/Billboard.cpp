//================================================
//
//エフェクト処理[Player.h]
// Author; miura yuudai
//
//================================================
#include "Billboard.h"
#include "input.h"

//マクロ定義
#define MAX_EFFECT_SIZE (5)//エフェクトのサイズ
#define MAX_EFFECT (7096)//配列

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard;//頂点ポインタ
BILLBOARD g_abillboard[MAX_EFFECT];//弾の情報
D3DXCOLOR g_colorBillboard;
D3DXMATRIX g_mtxWorldBillboard;//ワールドマトリックス

//==================
//エフェクトの初期化処理
//==================
HRESULT InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_abillboard[0].pTextureBillboard);

	//初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_abillboard[nCntEffect].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abillboard[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abillboard[nCntEffect].bUse = false;
		g_colorBillboard = D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f);
		g_abillboard[nCntEffect].nLife = 0;
		g_abillboard[nCntEffect].nType = 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBillboard, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x - MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.y + MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.z);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x + MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.y + MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x - MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.y - MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x + MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.y - MAX_EFFECT_SIZE, g_abillboard[nCntEffect].Pos.z);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = g_colorBillboard;
		pVtx[1].col = g_colorBillboard;
		pVtx[2].col = g_colorBillboard;
		pVtx[3].col = g_colorBillboard;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //4頂点ごとずらす
	}

	//頂点バッファをアンロック
	g_pVtxBuffBillboard->Unlock();

	return  S_OK;
}

//====================
//エフェクトの終了処理
//====================
void UninitBillboard(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//テクスチャの開放
		if (g_abillboard[nCntEffect].pTextureBillboard != NULL)
		{
			g_abillboard[nCntEffect].pTextureBillboard->Release();
			g_abillboard[nCntEffect].pTextureBillboard = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//====================
//エフェクトの更新処理
//====================
void UpdateBillboard(void)
{
	BILLBOARD *pBillboard;
	pBillboard = &g_abillboard[0];

	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pBillboard++)
	{
		if (pBillboard->bUse == true)//エフェクトを使用しているかどうか
		{
			if (pBillboard->nType == 0)
			{
				pBillboard->Pos += pBillboard->move;

				pBillboard->color = g_colorBillboard;

				pBillboard->move.y += 1.0f;

				//寿命チェック
				pBillboard->nLife--;
				if (pBillboard->nLife == 0)//寿命が0になったら
				{
					pBillboard->bUse = false;//使用しない
				}
			}
			else if (pBillboard->nType == 1)
			{
				pBillboard->Pos += pBillboard->move;

				pBillboard->color = g_colorBillboard;

				////頂点座標の設定
				//pVtx[0].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x - 25, g_abillboard[nCntEffect].Pos.y + 25, g_abillboard[nCntEffect].Pos.z);//2Dは0.0固定
				//pVtx[1].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x + 25, g_abillboard[nCntEffect].Pos.y + 25, g_abillboard[nCntEffect].Pos.z);
				//pVtx[2].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x - 25, g_abillboard[nCntEffect].Pos.y - 25, g_abillboard[nCntEffect].Pos.z);
				//pVtx[3].pos = D3DXVECTOR3(g_abillboard[nCntEffect].Pos.x + 25, g_abillboard[nCntEffect].Pos.y - 25, g_abillboard[nCntEffect].Pos.z);

				//寿命チェック
				pBillboard->nLife--;
				if (pBillboard->nLife == 0)//寿命が0になったら
				{
					pBillboard->bUse = false;//使用しない
				}
			}
		}
		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboard->Unlock();
}

//====================
//エフェクトの描画処理
//====================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	BILLBOARD *pBillboard;
	pBillboard = &g_abillboard[0];

	//デバイスの取得
	pDevice = GetDevice();

	//ビューマトリックス取得用
	D3DXMATRIX mtxView;

	//計算用マトリックス
	D3DXMATRIX mtxTrans;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_abillboard[nCntEffect].bUse == true)
		{
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//レンダーステートの設定(加算合成）
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBillboard);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);//逆行列を求める
			g_mtxWorldBillboard._41 = 0.0f;
			g_mtxWorldBillboard._42 = 0.0f;
			g_mtxWorldBillboard._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_abillboard[nCntEffect].Pos.x, g_abillboard[nCntEffect].Pos.y, g_abillboard[nCntEffect].Pos.z);

			D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

			//テクスチャの設定
			pDevice->SetTexture(0, g_abillboard[0].pTextureBillboard);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//レンダーステートの設定(元に戻す）
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//================
//エフェクトの設定
//================
void SetBillboard(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 move,int nType)
{
	VERTEX_3D *pVtx;

	BILLBOARD *pBillboard;
	pBillboard = &g_abillboard[0];

	// 頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pBillboard++)
	{
		if (pBillboard->bUse == false)//弾を使用しているかどうか
		{
			if (nType == 0)
			{
				//位置設定
				pBillboard->Pos = pos;

				pBillboard->nLife = nLife;

				pBillboard->move = move;

				pBillboard->nType = nType;

				//頂点座標の設定
				//pVtx[0].pos = D3DXVECTOR3(pBillboard->Pos.x - MAX_EFFECT_SIZE, pBillboard->Pos.y + MAX_EFFECT_SIZE, 0.0f);//2Dは0.0固定
				//pVtx[1].pos = D3DXVECTOR3(pBillboard->Pos.x + MAX_EFFECT_SIZE, pBillboard->Pos.y + MAX_EFFECT_SIZE, 0.0f);
				//pVtx[2].pos = D3DXVECTOR3(pBillboard->Pos.x - MAX_EFFECT_SIZE, pBillboard->Pos.y - MAX_EFFECT_SIZE, 0.0f);
				//pVtx[3].pos = D3DXVECTOR3(pBillboard->Pos.x + MAX_EFFECT_SIZE, pBillboard->Pos.y - MAX_EFFECT_SIZE, 0.0f);

				//弾の使用
				pBillboard->bUse = true;
				break;
			}

			else if(nType == 1)
			{
				//位置設定
				pBillboard->Pos = pos;

				pBillboard->nLife = nLife;

				pBillboard->move = move;

				pBillboard->nType = nType;

				//頂点カラーの設定
				pVtx[0].col = g_colorBillboard = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = g_colorBillboard = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = g_colorBillboard = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = g_colorBillboard = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);


				//頂点座標の設定
				//pVtx[0].pos = D3DXVECTOR3(pBillboard->Pos.x - MAX_EFFECT_SIZE, pBillboard->Pos.y + MAX_EFFECT_SIZE, 0.0f);//2Dは0.0固定
				//pVtx[1].pos = D3DXVECTOR3(pBillboard->Pos.x + MAX_EFFECT_SIZE, pBillboard->Pos.y + MAX_EFFECT_SIZE, 0.0f);
				//pVtx[2].pos = D3DXVECTOR3(pBillboard->Pos.x - MAX_EFFECT_SIZE, pBillboard->Pos.y - MAX_EFFECT_SIZE, 0.0f);
				//pVtx[3].pos = D3DXVECTOR3(pBillboard->Pos.x + MAX_EFFECT_SIZE, pBillboard->Pos.y - MAX_EFFECT_SIZE, 0.0f);

				//弾の使用
				pBillboard->bUse = true;
				break;
			}
		}
		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboard->Unlock();
}
BILLBOARD *GetBillboard(void)
{
	return &g_abillboard[0];
}