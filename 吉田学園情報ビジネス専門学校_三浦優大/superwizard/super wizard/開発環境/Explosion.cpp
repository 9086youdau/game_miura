#include "Explosion.h"
#include "Block.h"
#include "Player.h"
#include "Score.h"
#include "particle.h"
#include "effect.h"
#include "input.h"
#include "bullet.h"

//===========
//マクロ定義
//===========
#define MAX_ITEM (256)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureItem[5] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem;//頂点ポインタ
ITEM g_aItem[MAX_ITEM];//弾の情報
D3DXCOLOR g_colorItem;

HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/577px-FireFlowerMK8.png", &g_pTextureItem[0]);
	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/577px-FireFlowerMK8.png", &g_pTextureItem[1]);
	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/coin000.png", &g_pTextureItem[2]);
	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/スター.png", &g_pTextureItem[3]);
	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/soad2.png", &g_pTextureItem[4]);


	for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++)
	{
		g_aItem[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntExplosion].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
		g_colorItem = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		g_aItem[nCntExplosion].nType = 0;
		g_aItem[nCntExplosion].bUse = false;
		g_aItem[nCntExplosion].nLife = 0;
		g_aItem[nCntExplosion].g_nCounterAnim = 0;
		g_aItem[nCntExplosion].g_nPatternAnim = 0;
		g_aItem[nCntExplosion].g_nseetAnim = 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffItem, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntExplosion].pos.x - 60, g_aItem[nCntExplosion].pos.y + 60, 0.0f);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntExplosion].pos.x - 60, g_aItem[nCntExplosion].pos.y - 60, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntExplosion].pos.x + 60, g_aItem[nCntExplosion].pos.y + 60, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntExplosion].pos.x + 60, g_aItem[nCntExplosion].pos.y - 60, 0.0f);


		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_colorItem;
		pVtx[1].col = g_colorItem;
		pVtx[2].col = g_colorItem;
		pVtx[3].col = g_colorItem;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}

	g_pVtxBuffItem->Unlock();

	return  S_OK;
}
void UninitItem(void)
{
	for (int nCntExplosion = 0; nCntExplosion < 5; nCntExplosion++)
	{
		//テクスチャの開放
		if (g_pTextureItem[nCntExplosion] != NULL)
		{
			g_pTextureItem[nCntExplosion]->Release();
			g_pTextureItem[nCntExplosion] = NULL;
		}
	}
	//頂点バッファの開放
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}
void UpdateItem(void)
{
	ITEM *pExplosion;

	pExplosion = &g_aItem[0];

	VERTEX_2D *pVtx;

	BLOCK *pBlock;

	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	pBlock = GetBlock();


	// 頂点バッファをロック
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++, pExplosion++,pBlock++)
	{
		if (pExplosion->bUse == true)//弾を使用しているかどうか
		{
			if (pExplosion->nType == 0)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 30, pExplosion->pos.y + 30, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 30, pExplosion->pos.y - 30, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 30, pExplosion->pos.y + 30, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 30, pExplosion->pos.y - 30, 0.0f);

				if (pExplosion->pos.x >= pPlayer->Pos.x - 60 && pExplosion->pos.x <= pPlayer->Pos.x + 60 &&
					pExplosion->pos.y >= pPlayer->Pos.y - 60 && pExplosion->pos.y <= pPlayer->Pos.y + 60)
				{
					//パーティクル処理
					for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
					{
						D3DXVECTOR3 pos;
						int nLife;
						D3DXVECTOR3 move;
						float fAngle;

						pos = pPlayer->Pos;//位置
						nLife = 20 + rand() % 30;//寿命をランダム
						fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
						move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
						move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
						SetEffect(pos, nLife, 0, move);//エフェクト処理
					}
					HitPlayer(1);
					pPlayer->state = PLAYERSTATE_FREAZ;
					pExplosion->bUse = false;
				}

				pExplosion->move.y += sinf(D3DX_PI*0.5)*1.5f;

				if (pExplosion->pos.y <= 540)
				{
					pExplosion->move.y = 0.0f;
				}
			}

			if (pExplosion->nType == 3)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 40, pExplosion->pos.y + 40, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 40, pExplosion->pos.y - 40, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 40, pExplosion->pos.y + 40, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 40, pExplosion->pos.y - 40, 0.0f);

				if (pExplosion->pos.x >= pPlayer->Pos.x - 60 && pExplosion->pos.x <= pPlayer->Pos.x + 60 &&
					pExplosion->pos.y >= pPlayer->Pos.y - 60 && pExplosion->pos.y <= pPlayer->Pos.y + 60)
				{
					//パーティクル処理
					for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
					{
						D3DXVECTOR3 pos;
						int nLife;
						D3DXVECTOR3 move;
						float fAngle;

						pos = pPlayer->Pos;//位置
						nLife = 20 + rand() % 30;//寿命をランダム
						fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
						move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
						move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
						SetEffect(pos, nLife, 0, move);//エフェクト処理
					}
					HitPlayer(1);
					pPlayer->state = PLAYERSTATE_STAR;
					pExplosion->bUse = false;
				}
				
				
				pExplosion->move.y += sinf(D3DX_PI*0.5)*1.5f;

				if (pExplosion->pos.y <= 540)
				{
					pExplosion->move.y = 0.0f;
				}

			}

			if (pExplosion->nType == 1)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 30, pExplosion->pos.y + 30, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 30, pExplosion->pos.y - 30, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 30, pExplosion->pos.y + 30, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 30, pExplosion->pos.y - 30, 0.0f);

				if (pExplosion->pos.x >= pPlayer->Pos.x - 60 && pExplosion->pos.x <= pPlayer->Pos.x + 60 &&
					pExplosion->pos.y >= pPlayer->Pos.y - 60 && pExplosion->pos.y <= pPlayer->Pos.y + 60)
				{
					//パーティクル処理
					for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
					{
						D3DXVECTOR3 pos;
						int nLife;
						D3DXVECTOR3 move;
						float fAngle;

						pos = pPlayer->Pos;//位置
						nLife = 20 + rand() % 30;//寿命をランダム
						fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
						move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
						move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
						SetEffect(pos, nLife, 0, move);//エフェクト処理
					}
					HitPlayer(1);
					pPlayer->state = PLAYERSTATE_FIRE;
					pExplosion->bUse = false;
				}

				pExplosion->move.y += sinf(D3DX_PI*0.5)*1.5f;
				
				if (pExplosion->pos.y <= 540)
				{
					pExplosion->move.y = 0.0f;
				}

			}

			if (pExplosion->nType == 2)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 30, pExplosion->pos.y + 50, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 30, pExplosion->pos.y - 50, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 30, pExplosion->pos.y + 50, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 30, pExplosion->pos.y - 50, 0.0f);

				pExplosion->g_nCounterAnim++;//アニメーションがプラスされていく

				if ((pExplosion->g_nCounterAnim % 7) == 0)//10で割り切れる
				{
					pExplosion->g_nPatternAnim++;

					pExplosion->g_nseetAnim = (pExplosion->g_nseetAnim + 1) % 8;

					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f*pExplosion->g_nseetAnim, 1.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.25f*pExplosion->g_nseetAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.25f + 0.25f*pExplosion->g_nseetAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f*pExplosion->g_nseetAnim, 0.0f);
				}

				if (pExplosion->bUse = true)
				{
					if (pExplosion->pos.x >= pPlayer->Pos.x - 50 && pExplosion->pos.x <= pPlayer->Pos.x + 50 &&
						pExplosion->pos.y >= pPlayer->Pos.y - 50 && pExplosion->pos.y <= pPlayer->Pos.y + 50)
					{
						pExplosion->bUse = false;

						//パーティクル処理
						for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
						{
							D3DXVECTOR3 pos;
							int nLife;
							D3DXVECTOR3 move;
							float fAngle;

							pos = pExplosion->pos;//位置
							nLife = 1 + rand() % 30;//寿命をランダム
							fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
							move.x = sinf(fAngle)*1.5f;//x軸の弾の速度
							move.y = cosf(fAngle)*1.5f;//y軸の弾の速度
							SetEffect(pos, nLife, 0, move);//エフェクト処理
						}

						AddScore(100);
					}
				}
			}

			if (pExplosion->nType == 4)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 50, pExplosion->pos.y + 50, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 50, pExplosion->pos.y - 50, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 50, pExplosion->pos.y + 50, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 50, pExplosion->pos.y - 50, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = g_colorItem = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = g_colorItem = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = g_colorItem = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = g_colorItem = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			}

			for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++, pBlock++)
			{
				if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth + 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth + 30 &&
					pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
				{
					for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++)
					{
						g_aItem[nCntExplosion].move.x += 1.33f;
					}
				}

				else if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth - 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth - 30 &&
						pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
				{
					for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++)
					{
						g_aItem[nCntExplosion].move.x -= 1.33f;
					}
				}


				if (pPlayer->state == PLAYERSTATE_STAR)
				{
					if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth + 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth + 30 &&
						pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
					{
						for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++)
						{
							g_aItem[nCntExplosion].move.x += 2.25f;
						}
					}
					else if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth - 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth - 30 &&
							pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
					{
						for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++)
						{
							g_aItem[nCntExplosion].move.x -= 2.25f;
						}
					}
				}
			}
			
	
		//移動量の減衰
		 pExplosion->move.y += (0.0f - pExplosion->move.y) *1.5f;

		 //位置更新
		 pExplosion->pos.x += pExplosion->move.x;
		 pExplosion->pos.y += pExplosion->move.y;

		}
		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

void DrawItem (void)
{
	LPDIRECT3DDEVICE9 pDevice;

	ITEM *pExplosion;

	pExplosion = &g_aItem[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++,pExplosion++)
	{
		if (pExplosion->bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItem[pExplosion->nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion*4, 2);
		}
	}
}
void SetItem(D3DXVECTOR3 pos,int nType, D3DXVECTOR3 move)//弾の設定
{
	VERTEX_2D *pVtx;

	ITEM *pExplosion;
	pExplosion = &g_aItem[0];

	// 頂点バッファをロック
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_ITEM; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)//弾を使用しているかどうか
		{
			if (nType == 0)
			{
				//位置設定
				pExplosion->pos = pos;

				pExplosion->nType = nType;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 60, pExplosion->pos.y + 60, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 60, pExplosion->pos.y - 60, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 60, pExplosion->pos.y + 60, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 60, pExplosion->pos.y - 60, 0.0f);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = g_colorItem = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[1].col = g_colorItem = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[2].col = g_colorItem = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[3].col = g_colorItem = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);


				//弾の使用
				pExplosion->bUse = true;
				break;
			}

			else if (nType == 1)
			{
				//位置設定
				pExplosion->pos = pos;

				pExplosion->nType = nType;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 60, pExplosion->pos.y + 60, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 60, pExplosion->pos.y - 60, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 60, pExplosion->pos.y + 60, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 60, pExplosion->pos.y - 60, 0.0f);
				
				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//弾の使用
				pExplosion->bUse = true;
				break;
			}

			else if (nType == 2)
			{
				//位置設定
				pExplosion->pos = pos;

				pExplosion->nType = nType;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 60, pExplosion->pos.y + 60, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 60, pExplosion->pos.y - 60, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 60, pExplosion->pos.y + 60, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 60, pExplosion->pos.y - 60, 0.0f);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//弾の使用
				pExplosion->bUse = true;
				break;
			}
			else if (nType == 3)
			{
				//位置設定
				pExplosion->pos = pos;

				pExplosion->nType = nType;

				pExplosion->move = move;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 50, pExplosion->pos.y + 50, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 50, pExplosion->pos.y - 50, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 50, pExplosion->pos.y + 50, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 50, pExplosion->pos.y - 50, 0.0f);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//弾の使用
				pExplosion->bUse = true;
				break;
			}
			else if (nType == 4)
			{
				//位置設定
				pExplosion->pos = pos;

				pExplosion->nType = nType;

				pExplosion->move = move;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - 50, pExplosion->pos.y + 50, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - 50, pExplosion->pos.y - 50, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + 50, pExplosion->pos.y + 50, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + 50, pExplosion->pos.y - 50, 0.0f);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//弾の使用
				pExplosion->bUse = true;
				break;
			}
		}

		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}
ITEM *GetItem(void)
{
	return &g_aItem[0];
}