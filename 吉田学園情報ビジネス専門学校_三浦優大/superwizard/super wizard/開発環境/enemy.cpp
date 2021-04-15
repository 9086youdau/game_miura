//================================================
//
//敵の処理[Player.h]
// Author; miura yuudai
//
//================================================
#include "input.h"
#include "enemy.h"
#include "bullet.h"
#include "Player.h"
#include "Explosion.h"
#include "enemy.h"
#include "particle.h"
#include "fade.h"
#include "Block.h"
#include "effect.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[3] = {};//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;//頂点バッファへのポインタ
ENEMY g_aEnemy[256];//敵の情報
D3DXVECTOR3 g_rotEnemy;//敵のサイズ情報
int g_nCntEnemy;

//==================
//エネミーの初期化処理
//==================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy006.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/クッパ.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hannma.png", &g_apTextureEnemy[2]);

	//初期化
	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 3000;//敵の体力
		g_aEnemy[nCntEnemy].g_nCounterAni = 0;
		g_aEnemy[nCntEnemy].g_nPatternAni = 0;
		g_aEnemy[nCntEnemy].g_nseetAni = 0;
		int g_nCntEnemy = 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 256,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);

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

		pVtx += 4;//4頂点ごとずらす
	}

	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//==================
//エネミーの終了処理
//==================
void UninitEnemy(void)
{
	//テクスチャの開放
	for (int nCount = 0; nCount <  MAX_TYPE_ENEMY; nCount++)
	{
		if (g_apTextureEnemy[nCount] != NULL)
		{
			g_apTextureEnemy[nCount]->Release();
			g_apTextureEnemy[nCount] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//==================
//エネミーの更新処理
//==================
void UpdateEnemy(void)
 {
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	BLOCK *pBlock;
	EFFECT *pEffect;
	BULLET *pBullet;
	pEnemy = &g_aEnemy[0];
	pPlayer = GetPlayer();
	pBlock = GetBlock();
	pEffect = GetEffect();
	pBullet = GetBullet();
	g_nCntEnemy++;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++,pEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)//敵が使用されているかどうか
		{
			//対角線の長さ
			pEnemy->g_fLengthEnemy = sqrtf(MAX_ENEMY_SIZE *MAX_ENEMY_SIZE + MAX_ENEMY_SIZE * MAX_ENEMY_SIZE);
			//対角線の角度
			pEnemy->g_fAngleEnemy = atan2f(MAX_ENEMY_SIZE, MAX_ENEMY_SIZE);

			if (pEnemy->nType == 1)
			{
				if (g_nCntEnemy % 100 == 0)
				{
					float fAngle;
					
					fAngle = (float)(258) / 100.0f;//弾の範囲
					pEnemy->move.y += cosf(fAngle) * 30.5f;

				}
				float fmove;

				fmove = (float)(rand() % 78 - 39) / 100.0f;
				pEnemy->move.x -= sinf(fmove*0.5f) * 12.5f;

				if (g_nCntEnemy % 150 == 0)
				{
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;
					D3DXCOLOR col;

					nLife = 2200;
					col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					fAngle = (float)(rand() % 628 - 314) / 100.0f;//弾の範囲
					move.x = sinf(D3DX_PI*0.5)*-5.0f;//弾の移動量
					move.y = cosf(D3DX_PI*0.5)*-5.0f;//弾の移動量

					SetBullet(pEnemy->Pos, move, nLife, col, 0);

				}
			}

			if (pEnemy->nType == 0)
			{
				pEnemy->move.x -= sinf(D3DX_PI*0.5f) * 0.5f;

				//対角線の長さ
				pEnemy->g_fLengthEnemy = sqrtf(40 * 40 + 40 * 40);
				//対角線の角度
				pEnemy->g_fAngleEnemy = atan2f(40, 40);

				pEnemy->g_nCounterAni++;//アニメーションがプラスされていく

				if ((pEnemy->g_nCounterAni % 13) == 0)//10で割り切れる
				{
					pEnemy->g_nPatternAni++;

					pEnemy->g_nseetAni = (pEnemy->g_nseetAni + 1) % 8;

					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f*pEnemy->g_nseetAni, 1.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f*pEnemy->g_nseetAni, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f + 0.5f*pEnemy->g_nseetAni, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f*pEnemy->g_nseetAni, 0.0f);
				}

			}
			if (pEnemy->nType == 2)
			{
				//対角線の長さ
				pEnemy->g_fLengthEnemy = sqrtf(60 * 60 + 60 * 60);
				//対角線の角度
				pEnemy->g_fAngleEnemy = atan2f(60, 60);

				pEnemy->move.x -= sinf(D3DX_PI*0.5f) * 0.2f;

				if (g_nCntEnemy % 50 == 0)
				{
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;
					D3DXCOLOR col;

					nLife = 200;
					col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
					fAngle = (float)(-220) / 100.0f;//弾の範囲
					move.x = sinf(fAngle)*14.0f;//弾の移動量
					move.y = cosf(fAngle)*14.0f;//弾の移動量

					SetBullet(pEnemy->Pos, move, nLife, col, 3);

				}
			}

			//エネミーの移動調整
			for (int nCntExplosion = 0; nCntExplosion < 256; nCntExplosion++, pBlock++)
			{
				if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth + 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth + 30 &&
					pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
				{
					for (int nCntExplosion = 0; nCntExplosion < 256; nCntExplosion++)
					{
						g_aEnemy[nCntExplosion].move.x += 3.0f;
					}
				}

				else if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth - 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth - 30 &&
						pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
				{
					for (int nCntExplosion = 0; nCntExplosion < 256; nCntExplosion++)
					{
						g_aEnemy[nCntExplosion].move.x -= 3.0f;
					}
				}
			}


			//エネミーの重力
			pEnemy->move.y += 3.5f;

			//位置更新
			g_aEnemy[nCntEnemy].Pos.x += pEnemy->move.x;
			g_aEnemy[nCntEnemy].Pos.y += pEnemy->move.y;


			//移動量の減衰
			pEnemy->move.x += (0.0f - pEnemy->move.x) *0.5f;


			//移動時の頂点情報
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].Pos.x + sinf(g_rotEnemy.z - pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].Pos.y + cosf(g_rotEnemy.z - pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].Pos.x - sinf(g_rotEnemy.z + pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].Pos.y - cosf(g_rotEnemy.z + pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].Pos.x + sinf(g_rotEnemy.z + pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].Pos.y + cosf(g_rotEnemy.z + pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].Pos.x - sinf(g_rotEnemy.z - pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].Pos.y - cosf(g_rotEnemy.z - pEnemy->g_fAngleEnemy) * pEnemy->g_fLengthEnemy;
			pVtx[3].pos.z = 0.0f;


			//switch (g_aEnemy[nCntEnemy].state)
			//{
			//case ENEMYSTATE_DAMAGE:
			//	g_aEnemy[nCntEnemy].nCounterState--;
			//	if (g_aEnemy[nCntEnemy].nCounterState <= 0)
			//	{
			//		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;
			//		//頂点カラーの設定
			//		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//	}
			//	break;
			//}

			//移動量の減衰
			//pEnemy->move.x += (0.0f - pEnemy->move.x) *0.5f;

		}
		pVtx += 4;//4頂点ごとずらす
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
  }
//==================
//エネミーの描画処理
//==================
void DrawEnemy(void)
{
	ENEMY *pEnemy;
	LPDIRECT3DDEVICE9 pDevice;

	pEnemy = &g_aEnemy[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCutEnemy = 0; nCutEnemy < 256; nCutEnemy++,pEnemy++)
	{
		pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

		if(pEnemy->bUse == true)//弾を使用しているかどうか
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCutEnemy*4, 2);
		}
	}
}

//==================
//エネミーの設定
//==================
void SETENEMY(D3DXVECTOR3 Pos, int nType)
{
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	pEnemy = &g_aEnemy[0];

	// 頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == false)//弾を使用しているかどうか
		{
			if (nType == 0)
			{
				//位置設定
				g_aEnemy[nCntEnemy].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);


				//タイプ設定
				pEnemy->nType = nType;

				//弾の使用
				pEnemy->bUse = true;
				break;
			}
			else if (nType == 1)
			{
				//位置設定
				g_aEnemy[nCntEnemy].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);

				//タイプ設定
				pEnemy->nType = nType;

				//弾の使用
				pEnemy->bUse = true;
				break;
			}
			else if (nType == 2)
			{
				//位置設定
				g_aEnemy[nCntEnemy].Pos = Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x - MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y + MAX_ENEMY_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].Pos.x + MAX_ENEMY_SIZE, g_aEnemy[nCntEnemy].Pos.y - MAX_ENEMY_SIZE, 0.0f);

				//タイプ設定
				pEnemy->nType = nType;

				//弾の使用
				pEnemy->bUse = true;
				break;
			}
		}

		pVtx += 4;//4頂点ごとずらす
	}
		// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//==================
//エネミーの構造体設定
//==================
ENEMY *GetEnemy(void) 
{
	return &g_aEnemy[0];
}

////==================
////エネミーの弾ヒット時の設定
////==================
//bool HitEnemy(int nIdx, int nDamage)
//{
//	ENEMY *pEnemy;
//	pEnemy = &g_aEnemy[nIdx];
//	VERTEX_2D *pVtx;
//
//	//頂点バッファをロックし、頂点情報のポインタを取得
//	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
//
//	g_aEnemy[nIdx].nLife -= nDamage;//体力からダメージを引く
//
//	if (g_aEnemy[nIdx].nLife <= 0)//体力が0以下になったら
//	{
//		PlaySound(SOUND_LABEL_SE_EXPLOSION);
//		AddScore(99999999);
//		g_aEnemy[nIdx].bUse = false;
//		g_nData--;
//
//		for (int nCntAppear = 0; nCntAppear < 300; nCntAppear++)
//		{
//			D3DXVECTOR3 pos;
//			int nLife;
//			D3DXVECTOR3 move;
//			float fAngle;
//
//			pos = pEnemy->Pos;
//			nLife = 100 + rand() % 200;
//			fAngle = (float)(rand() % 628 - 314) / 100.0f;
//			move.x = sinf(fAngle)*1.5f;
//			move.y = cosf(fAngle)*1.5f;
//			SetEffect(pos, nLife, 0, move);
//		}
//			if (g_nData == 0)
//			{
//				SetFade(FADE_OUT, MODE_RESULT);
//			}
//		
//		return true;
//	}
//	else
//	{
//			AddScore(99);
//			g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
//			g_aEnemy[nIdx].nCounterState = 10;
//
//			//頂点カラーの設定
//			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
//			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
//			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
//			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
//
//	}
//		return false;
//	
//	// 頂点バッファをアンロック
//	g_pVtxBuffEnemy->Unlock();
//}