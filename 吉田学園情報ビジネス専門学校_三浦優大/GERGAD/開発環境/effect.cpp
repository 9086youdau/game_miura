//================================================
//
//エフェクト処理[Player.h]
// Author; miura yuudai
//
//================================================
#include "effect.h"
#include "Player.h"

//マクロ定義
#define MAX_EFFECT_SIZE (20)//エフェクトのサイズ
#define MAX_EFFECT (7096)//配列

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect;//頂点ポインタ
EFFECT g_aEffect[MAX_EFFECT];//弾の情報
D3DXCOLOR g_colorEffect;

//==================
//エフェクトの初期化処理
//==================
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = g_colorEffect;
		g_aEffect[nCntEffect].bUse = false;
		g_colorEffect = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aEffect[nCntEffect].nLife= 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - MAX_EFFECT_SIZE, g_aEffect[nCntEffect].pos.y + MAX_EFFECT_SIZE, 0.0f);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - MAX_EFFECT_SIZE, g_aEffect[nCntEffect].pos.y - MAX_EFFECT_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + MAX_EFFECT_SIZE, g_aEffect[nCntEffect].pos.y + MAX_EFFECT_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + MAX_EFFECT_SIZE, g_aEffect[nCntEffect].pos.y - MAX_EFFECT_SIZE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_colorEffect;
		pVtx[1].col = g_colorEffect;
		pVtx[2].col = g_colorEffect;
		pVtx[3].col = g_colorEffect;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		 pVtx += 4; //4頂点ごとずらす
	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();

	return  S_OK;
}

//====================
//エフェクトの終了処理
//====================
void UninitEffect(void)
{
	//テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
	
	//頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//====================
//エフェクトの更新処理
//====================
void UpdateEffect(void)
{
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)//エフェクトを使用しているかどうか
		{
			pEffect->pos += pEffect->move;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - MAX_EFFECT_SIZE, pEffect->pos.y + MAX_EFFECT_SIZE, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - MAX_EFFECT_SIZE, pEffect->pos.y - MAX_EFFECT_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + MAX_EFFECT_SIZE, pEffect->pos.y + MAX_EFFECT_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + MAX_EFFECT_SIZE, pEffect->pos.y - MAX_EFFECT_SIZE, 0.0f);


			//頂点カラーの設定
			/*pVtx[0].col = g_colorEffect;
			pVtx[1].col = g_colorEffect;
			pVtx[2].col = g_colorEffect;*/

			//寿命チェック
			pEffect->nLife--;
			if (pEffect->nLife == 5)//寿命が0になったら
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - 17, pEffect->pos.y + 17, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - 17, pEffect->pos.y - 17, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + 17, pEffect->pos.y + 17, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + 17, pEffect->pos.y - 17, 0.0f);
			}

			else if (pEffect->nLife == 4)//寿命が0になったら
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - 13, pEffect->pos.y + 13, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - 13, pEffect->pos.y - 13, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + 13, pEffect->pos.y + 13, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + 13, pEffect->pos.y - 13, 0.0f);
			}

			else if (pEffect->nLife == 3)//寿命が0になったら
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - 9, pEffect->pos.y + 9, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - 9, pEffect->pos.y - 9, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + 9, pEffect->pos.y + 9, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + 9, pEffect->pos.y - 9, 0.0f);
			}

			else if (pEffect->nLife == 2)//寿命が0になったら
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - 5, pEffect->pos.y + 5, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - 5, pEffect->pos.y - 5, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + 5, pEffect->pos.y + 5, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + 5, pEffect->pos.y - 5, 0.0f);
			}

			else if (pEffect->nLife == 1)//寿命が0になったら
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - 1, pEffect->pos.y + 1, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - 1, pEffect->pos.y - 1, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + 1, pEffect->pos.y + 1, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + 1, pEffect->pos.y - 1, 0.0f);
			}


			if (pEffect->nLife == 0)//寿命が0になったら
			{
				pEffect->bUse = false;//使用しない
			}

		}
			//4頂点ごとずらす
			pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//====================
//エフェクトの描画処理
//====================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];

	//デバイスの取得
	pDevice = GetDevice();

	//レンダーステートの設定(加算合成）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEffect);

		pDevice->SetTexture(0, g_pTextureEffect);
	
		if (g_aEffect[nCntEffect].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect*4, 2);
		}
	}

	//レンダーステートの設定(元に戻す）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//================
//エフェクトの設定
//================
void SetEffect(D3DXVECTOR3 pos,int nLife, D3DXVECTOR3 move)
{
	EFFECT *pEffect;
	VERTEX_2D *pVtx;

	pEffect = &g_aEffect[0];

	// 頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)//弾を使用しているかどうか
		{
				//位置設定
				pEffect->pos = pos;

				pEffect->nLife = nLife;

				pEffect->move = move;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - MAX_EFFECT_SIZE, pEffect->pos.y + MAX_EFFECT_SIZE, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - MAX_EFFECT_SIZE, pEffect->pos.y - MAX_EFFECT_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + MAX_EFFECT_SIZE, pEffect->pos.y + MAX_EFFECT_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + MAX_EFFECT_SIZE, pEffect->pos.y - MAX_EFFECT_SIZE, 0.0f);

				//弾の使用
				pEffect->bUse = true;
				break;
		}
			//4頂点ごとずらす
			pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}
EFFECT *GetEffect(void)
{
	return &g_aEffect[0];
}
