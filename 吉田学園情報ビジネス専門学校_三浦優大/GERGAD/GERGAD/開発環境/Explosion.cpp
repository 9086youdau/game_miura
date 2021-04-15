//================================================
//
//爆発アニメーション処理[Explosion.cpp]
// Author; miura yuudai
//
//================================================

//================================================
//ヘッダー読み込み
//================================================
#include "Explosion.h"

//================================================
//マクロ定義
//================================================
#define MAX_EXPLOSION_SIZE (60)
#define MAXEXPLOSION (256)

//================================================
//グローバル変数
//================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//頂点情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion;//頂点ポインタ
EXPLOSION g_aExplosion[MAXEXPLOSION];//弾の情報

//=======================================
//爆発アニメーションの初期化処理
//=======================================
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAXEXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAXEXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	//頂点バッファのロックする
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAXEXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y + MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y - MAX_EXPLOSION_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y + MAX_EXPLOSION_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y - MAX_EXPLOSION_SIZE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aExplosion[nCntExplosion].col;
		pVtx[1].col = g_aExplosion[nCntExplosion].col;
		pVtx[2].col = g_aExplosion[nCntExplosion].col;
		pVtx[3].col = g_aExplosion[nCntExplosion].col;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}

	//頂点情報をアンロックする
	g_pVtxBuffExplosion->Unlock();

	//返り値
	return  S_OK;
}
//=======================================
//爆発アニメーションの終了処理
//=======================================
void UninitExplosion(void)
{
	//テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//=======================================
//爆発アニメーションの更新処理
//=======================================
void UpdateExplosion(void)
{
	//ポインタ変数
	EXPLOSION *pExplosion;
	pExplosion = &g_aExplosion[0];

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAXEXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == true)//アニメーションを使用しているかどうか
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y + MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y - MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y + MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y - MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定

			//アニメーション処理
			g_aExplosion[nCntExplosion].g_nCounterAnim++;
			if ((g_aExplosion[nCntExplosion].g_nCounterAnim % 3) == 0)
			{
				g_aExplosion[nCntExplosion].g_nPatternAnim++;
				
				g_aExplosion[nCntExplosion].g_nseetAnim = (g_aExplosion[nCntExplosion].g_nseetAnim + 1) % 10;

				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f*g_aExplosion[nCntExplosion].g_nseetAnim, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + 0.125f*g_aExplosion[nCntExplosion].g_nseetAnim, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.125f + 0.125f*g_aExplosion[nCntExplosion].g_nseetAnim, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f*g_aExplosion[nCntExplosion].g_nseetAnim, 0.0f);
				if (g_aExplosion[nCntExplosion].g_nPatternAnim > 4)
				{
					pExplosion->bUse = false;
					g_aExplosion[nCntExplosion].g_nPatternAnim = 0;//初期化
					g_aExplosion[nCntExplosion].g_nCounterAnim = 0;//初期化
				}
			}
			
		}
		//4頂点ごとずらす
		pVtx += 4;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=======================================
//爆発アニメーションの描画処理
//=======================================
void DrawExplosion (void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAXEXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)//使われているか
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}
//=======================================
//爆発アニメーションの設定処理
//=======================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	//ポインタ変数
	EXPLOSION *pBullet;
	pBullet = &g_aExplosion[0];

	//構造体のポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAXEXPLOSION; nCntExplosion++, pBullet++)
	{
		if (pBullet->bUse == false)//弾を使用しているかどうか
		{
			//位置設定
			g_aExplosion[nCntExplosion].pos = pos;

			//カラー設定
			g_aExplosion[nCntExplosion].col = col;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y + MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y - MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y + MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + MAX_EXPLOSION_SIZE, g_aExplosion[nCntExplosion].pos.y - MAX_EXPLOSION_SIZE, 0.0f);//2Dは0.0固定

			//弾の使用
			pBullet->bUse = true;
			break;
		}

		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}