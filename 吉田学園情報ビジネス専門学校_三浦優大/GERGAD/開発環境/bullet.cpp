#include "bullet.h"
#include "Explosion.h"
#include "sound.h"
#include "effect.h"
#include "particle.h"
#include "enemy.h"
#include "Score.h"
#include "Player.h"
#include "fade.h"
#include "Hp.h"


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturebullet[3] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet;//頂点ポインタ
BULLET g_aBullet[702];//弾の情報
D3DXCOLOR g_colorBullet;


HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/10.png", &g_pTexturebullet[BULLETTYPE_ENEMY_HOMING]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/10.png", &g_pTexturebullet[BULLETTYPE_ENEMY_HOMING2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/10.png", &g_pTexturebullet[BULLETTYPE_ENEMY_HOMING3]);

	for (int nCntBullet = 0; nCntBullet < 702; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].col = g_colorBullet;
		g_colorBullet = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].nType = BULLETTYPE_ENEMY;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 702, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < 702; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - MAX_SIZE, g_aBullet[nCntBullet].pos.y + MAX_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - MAX_SIZE, g_aBullet[nCntBullet].pos.y - MAX_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + MAX_SIZE, g_aBullet[nCntBullet].pos.y + MAX_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + MAX_SIZE, g_aBullet[nCntBullet].pos.y - MAX_SIZE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		////頂点カラーの設定
		pVtx[0].col = g_colorBullet;
		pVtx[1].col = g_colorBullet;
		pVtx[2].col = g_colorBullet;
		pVtx[3].col = g_colorBullet;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}

	g_pVtxBuffBullet->Unlock();
	return  S_OK;
}
void UninitBullet(void)
{
	//テクスチャの開放
	for (int nCntBullet = 0; nCntBullet < 3; nCntBullet++)
	{
		if (g_pTexturebullet[nCntBullet] != NULL)
		{
			g_pTexturebullet[nCntBullet]->Release();
			g_pTexturebullet[nCntBullet] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
void UpdateBullet(void)
{
	BULLET *pBullet;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	EFFECT *pEffect;
	pBullet = &g_aBullet[0];
	VERTEX_2D *pVtx;
	int nLife = 10;
	

	// 頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < 702; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)//弾を使用しているかどうか
		{
			if (pBullet->nLife <= 50)
			{
				//位置更新
				if (pBullet->nType == BULLETTYPE_ENEMY_HOMING2)
				{
					D3DXVECTOR3 vec;
					float fAngle;
					D3DXVECTOR3 move;
					pPlayer = GetPlayer();

					vec = pPlayer->Pos - pBullet->pos;//現在地から目的地へのベクトル
					fAngle = atan2f(vec.x, vec.y);//ベクトルの角度を求める
					move.x = sinf(fAngle) * 1.0f;//移動量を求める
					move.y = cosf(fAngle) * 1.0f;//移動量を求める

					pBullet->pos += move;

				}
			}
			else
			{
				pBullet->pos += pBullet->move;
			}

		D3DXVECTOR3 vec;
		float fAngle;
		D3DXVECTOR3 move;
		pPlayer = GetPlayer();

		vec = pPlayer->Pos - pBullet->pos;//現在地から目的地へのベクトル
		fAngle = atan2f(vec.x, vec.y);//ベクトルの角度を求める
		move.x = sinf(fAngle) * 1.0f;//移動量を求める
		move.y = cosf(fAngle) * 1.0f;//移動量を求める

		
		if (pBullet->nType == BULLETTYPE_ENEMY_HOMING)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 15, g_aBullet[nCntBullet].pos.y + 15, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 15, g_aBullet[nCntBullet].pos.y - 15, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 15, g_aBullet[nCntBullet].pos.y + 15, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 15, g_aBullet[nCntBullet].pos.y - 15, 0.0f);

			pVtx[0].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);
			pVtx[1].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);
			pVtx[2].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);
			pVtx[3].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);

			//エフェクト処理
			SetEffect(pBullet->pos, nLife, move);
		}

		else if (pBullet->nType == BULLETTYPE_ENEMY_HOMING2)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 35, g_aBullet[nCntBullet].pos.y + 35, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 35, g_aBullet[nCntBullet].pos.y - 35, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 35, g_aBullet[nCntBullet].pos.y + 35, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 35, g_aBullet[nCntBullet].pos.y - 35, 0.0f);

			pVtx[0].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.3f);
			pVtx[1].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.3f);
			pVtx[2].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.3f);
			pVtx[3].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.3f);

			//エフェクト処理
			SetEffect(pBullet->pos, nLife, move);
		}

		else if (pBullet->nType == BULLETTYPE_ENEMY_HOMING3)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 35, g_aBullet[nCntBullet].pos.y + 35, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 35, g_aBullet[nCntBullet].pos.y - 35, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 35, g_aBullet[nCntBullet].pos.y + 35, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 35, g_aBullet[nCntBullet].pos.y - 35, 0.0f);

			pVtx[0].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);
			pVtx[1].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);
			pVtx[2].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);
			pVtx[3].col = g_colorBullet = g_colorBullet = D3DXCOLOR(0.3f, 0.3f, 1.0f, 0.3f);

			//エフェクト処理
			SetEffect(pBullet->pos, nLife, move);
		}

			//寿命チェック
			pBullet->nLife--;
			if (pBullet->nLife == 0)
			{
				for (int nCntAppear = 0; nCntAppear < 100; nCntAppear++)
				{
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;
					pos = pBullet->pos;
					nLife = 10 + rand() % 20;
					fAngle = (float)(rand() % 628 - 314) / 20.0f;
					move.x = sinf(fAngle)*2.5f;
					move.y = cosf(fAngle)*2.5f;
					SetEffect(pos, nLife, move);
					pBullet->bUse = false;
				}
			}

			//画面外チェック 爆発エフェクト
			if (g_aBullet[nCntBullet].pos.y + 20 >= 1030)
			{
				for (int nCntAppear = 0; nCntAppear < 100; nCntAppear++)
				{
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;
					pos = pBullet->pos;
					nLife = 10 + rand() % 20;
					fAngle = (float)(rand() % 628 - 314) / 100.0f;
					move.x = sinf(fAngle)*2.5f;
					move.y = cosf(fAngle)*2.5f;
					SetEffect(pos, nLife, move);
				}
				pBullet->bUse = false;
			}
			else if (g_aBullet[nCntBullet].pos.y - 20 <= 50)
			{
				for (int nCntAppear = 0; nCntAppear < 100; nCntAppear++)
				{
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;
					D3DXCOLOR col;
					col = D3DXCOLOR(0.01f, 0.01f, 1.0f, 1.0f);
					pos = pBullet->pos;
					nLife = 10 + rand() % 20;
					fAngle = (float)(rand() % 628 - 314) / 100.0f;
					move.x = sinf(fAngle)*2.5f;
					move.y = cosf(fAngle)*2.5f;
					SetEffect(pos, nLife,move);
				}
				pBullet->bUse = false;
			}

			if (g_aBullet[nCntBullet].pos.x + 20 >= 1220)
			{
				for (int nCntAppear = 0; nCntAppear < 100; nCntAppear++)
				{
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;
					pos = pBullet->pos;
					nLife = 10 + rand() % 20;
					fAngle = (float)(rand() % 628 - 314) / 100.0f;
					move.x = sinf(fAngle)*2.5f;
					move.y = cosf(fAngle)*2.5f;
					SetEffect(pos, nLife, move);
				}
				pBullet->bUse = false;
			}
			else if (g_aBullet[nCntBullet].pos.x - 20 <= 50)
			{
				for (int nCntAppear = 0; nCntAppear < 100; nCntAppear++)
				{
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;

					pos = pBullet->pos;
					nLife = 10 + rand() % 20;
					fAngle = (float)(rand() % 628 - 314) / 100.0f;
					move.x = sinf(fAngle)*2.5f;
					move.y = cosf(fAngle)*2.5f;
					SetEffect(pos, nLife, move);
				}
				pBullet->bUse = false;
			}
			
			pEnemy = GetEnemy();
			pPlayer = GetPlayer();
			pEffect = GetEffect();

			//敵との当たり判定
				if (pBullet->nType == BULLETTYPE_ENEMY_HOMING2)//エネミーの弾
				{
					if (pPlayer->bUse == true)
					{
						if (pBullet->pos.x >= pPlayer->Pos.x - MAX_POLYGON_SIZE && pBullet->pos.x <= pPlayer->Pos.x + MAX_POLYGON_SIZE &&
							pBullet->pos.y >= pPlayer->Pos.y - MAX_POLYGON_SIZE && pBullet->pos.y <= pPlayer->Pos.y + MAX_POLYGON_SIZE)
						{
							HitPlayer(1);
							AddHp(1);
							pBullet->bUse = false;
							for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
							{
								D3DXVECTOR3 pos;
								int nLife;
								D3DXVECTOR3 move;
								float fAngle;
								pos = pBullet->pos;
								nLife = 10 + rand() % 20;
								fAngle = (float)(rand() % 628 - 314) / 100.0f;
								move.x = sinf(fAngle)*2.5f;
								move.y = cosf(fAngle)*2.5f;
								SetEffect(pos, nLife, move);
							}
						}
					}
				}
				for (int nCntEnemy = 0; nCntEnemy < 702; nCntEnemy++, pEnemy++)
				{
					if (pBullet->nType == BULLETTYPE_ENEMY_HOMING)//ポリゴンの弾
					{
						if (pEnemy->bUse == true)
						{
							if (pBullet->pos.x >= pEnemy->Pos.x - MAX_BULLET_SIZE && pBullet->pos.x <= pEnemy->Pos.x + MAX_BULLET_SIZE &&
								pBullet->pos.y >= pEnemy->Pos.y - MAX_BULLET_SIZE && pBullet->pos.y <= pEnemy->Pos.y + MAX_BULLET_SIZE)
							{
								HitEnemy(nCntEnemy, 60);
								pBullet->bUse = false;
							}
						}
					}

				}
		}
	//4頂点ごとずらす
				pVtx += 4;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	int Count;
	Count = 0;
	BULLET *pBullet;
	pBullet = &g_aBullet[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBullet = 0; nCntBullet < 702; nCntBullet++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturebullet[pBullet->nType]);
		pDevice->SetTexture(0, g_pTexturebullet[pBullet[nCntBullet].nType]);

		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, BULLETTYPE nType)//弾の設定
{
	BULLET *pBullet;
	VERTEX_2D *pVtx;

	pBullet = &g_aBullet[0];

	// 頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet <702; nCntBullet++, pBullet++)
	{
		if(pBullet->bUse == false)//弾を使用しているかどうか
		{
			if (nType == BULLETTYPE_ENEMY_HOMING)
			{
				//位置設定
				pBullet->pos = pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - MAX_SIZE, pBullet->pos.y + MAX_SIZE, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x - MAX_SIZE, pBullet->pos.y - MAX_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x + MAX_SIZE, pBullet->pos.y + MAX_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + MAX_SIZE, pBullet->pos.y - MAX_SIZE, 0.0f);

				//移動量を設定
				pBullet->move = move;

				//寿命を設定
				pBullet->nLife = nLife;

				pBullet->nType = nType;

				pBullet->col = col;

				pVtx[0].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.3f);
				pVtx[1].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.3f);
				pVtx[2].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.3f);
				pVtx[3].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.3f);

				//弾の使用
				pBullet->bUse = true;
				//AddScore(10);
				break;
			}
			else if (nType == BULLETTYPE_ENEMY_HOMING2)
			{
				//位置設定
				pBullet->pos = pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - MAX_SIZE, pBullet->pos.y + MAX_SIZE, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x - MAX_SIZE, pBullet->pos.y - MAX_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x + MAX_SIZE, pBullet->pos.y + MAX_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + MAX_SIZE, pBullet->pos.y - MAX_SIZE, 0.0f);

				//移動量を設定
				pBullet->move = move;

				//寿命を設定
				pBullet->nLife = nLife;

				pBullet->nType = nType;

				pBullet->col = col;

				pVtx[0].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
				pVtx[1].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
				pVtx[2].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
				pVtx[3].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);

				//弾の使用
				pBullet->bUse = true;
				break;
			}
			else if (nType == BULLETTYPE_ENEMY_HOMING3)
			{
				//位置設定
				pBullet->pos = pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - MAX_SIZE, pBullet->pos.y + MAX_SIZE, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x - MAX_SIZE, pBullet->pos.y - MAX_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x + MAX_SIZE, pBullet->pos.y + MAX_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + MAX_SIZE, pBullet->pos.y - MAX_SIZE, 0.0f);

				//移動量を設定
				pBullet->move = move;

				//寿命を設定
				pBullet->nLife = nLife;

				pBullet->nType = nType;

				pBullet->col = col;
				pVtx[0].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
				pVtx[1].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
				pVtx[2].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);
				pVtx[3].col = g_colorBullet = g_colorBullet = D3DXCOLOR(1.0f, 0.3f, 1.0f, 1.0f);

				//弾の使用
				pBullet->bUse = true;
				break;
			}
		}

		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

