//================================================
//
//プレイヤー処理[Player.h]
// Author; miura yuudai
//
//================================================

#include "Block.h"
#include "Player.h"
#include "input.h"
#include "Explosion.h"
#include "bullet.h"
#include "fade.h"
#include "enemy.h"
#include "Timer.h"
#include "particle.h"
#include "effect.h"

//=========================
//マクロ定義
//=========================
#define MAX_BLOCK (256)

//================
//グローバル変数
//================
LPDIRECT3DTEXTURE9 g_apTextureBlock[10] = {};//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;//頂点バッファへのポインタ
BLOCK g_aBlock[MAX_BLOCK];//敵の情報
D3DXVECTOR3 g_rotBlock;//回転量
int g_nData = 1;
int nCountParticle;
int nCountParticleL;
int g_ppData = 0;


//==========================
//ブロックの初期化処理
//==========================
HRESULT InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	VERTEX_2D *pVtx;

	BLOCK *pBlock;

	pBlock = &g_aBlock[0];

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pattern-1929506_1280.png", &g_apTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.jpg", &g_apTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block200.jpg", &g_apTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipe000.png", &g_apTextureBlock[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block400.jpg", &g_apTextureBlock[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipe000.png", &g_apTextureBlock[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block400.jpg", &g_apTextureBlock[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.jpg", &g_apTextureBlock[7]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.jpg", &g_apTextureBlock[8]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block400.jpg", &g_apTextureBlock[9]);


	//初期化
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].nType = 0;
		int g_nData = 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x - pBlock->fWidth, g_aBlock[nCntBlock].Pos.y + pBlock->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x - pBlock->fWidth, g_aBlock[nCntBlock].Pos.y - pBlock->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x + pBlock->fWidth, g_aBlock[nCntBlock].Pos.y + pBlock->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x + pBlock->fWidth, g_aBlock[nCntBlock].Pos.y - pBlock->fHeight, 0.0f);

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
	g_pVtxBuffBlock->Unlock();

	return S_OK;
}

//====================
//ブロックの終了処理
//====================
void UninitBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < 10; nCntBlock++)
	{
		//テクスチャの開放
		if (g_apTextureBlock[nCntBlock] != NULL)
		{
			g_apTextureBlock[nCntBlock]->Release();
			g_apTextureBlock[nCntBlock] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//==================
//ブロックの更新処理
//==================
void UpdateBlock(void)
{
	BLOCK *pBlock;
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;
	ITEM *pExplosion;
	BULLET *pBullet;
	ENEMY *pEnemy;

	nCountParticle++;
	nCountParticleL--;
	nCountParticleL--;
	g_ppData++;


	pBlock = &g_aBlock[0];
	pPlayer = GetPlayer();
	pExplosion = GetItem();
	pBullet = GetBullet();
	pEnemy = GetEnemy();

	// 頂点バッファをロック
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	//位置更新
	pPlayer->Pos.x += pPlayer->move.x;
	pPlayer->Pos.y += pPlayer->move.y;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++ ,pBlock++,pExplosion++,pEnemy++,pBullet++)
	{
		if (pBlock->bUse == true)//弾を使用しているかどうか
		{
			if (GetKeyboardPress(DIK_D) == true)//右に進む
			{
				//プレイヤーが動いたときブロックも
				pBlock->move.x -= 2.0f;

				pExplosion->move.x -= 2.0f;

				pEnemy->move.x -= 4.75f;

				pBullet->move.x -= 0.1f;
			}
			
			if (GetKeyboardPress(DIK_A) == true)//左に進む
			{
				//プレイヤーが動いたときブロックも
				pBlock->move.x += 2.0f;

				pExplosion->move.x += 2.0f;

				pEnemy->move.x += 4.75f;

			}

			//画面が止まる処理
			//if (g_aBlock[60].Pos.x <= 0)
			//{
			//	//プレイヤーが動いたときブロックも
			//	pBlock->move.x = 0.0f;

			//	pExplosion->move.x = 0.0f;

			//	pEnemy->move.x = 0.00f;

			//	if (GetKeyboardPress(DIK_D) == true)//右に進む
			//	{
			//		pPlayer->move.x += sinf(D3DX_PI*0.5f) * 4.0f;
			//	}
			//	if (GetKeyboardPress(DIK_A) == true)//右に進む
			//	{
			//		pPlayer->move.x += -sinf(D3DX_PI*0.5f) * 4.0f;
			//	}

			//	//ブロック左の処理
			//	if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth + 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth + 30 &&
			//		pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
			//	{
			//		pPlayer->move.x = 0.0f;//重力リセット
			//		pPlayer->Pos.x = pBlock->Pos.x - pBlock->fWidth - 30;//左の当たり判定
			//		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pExplosion++)
			//		{
			//			g_aBlock[nCntBlock].move.x -= 2.0f;
			//		}
			//		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pExplosion++)
			//		{
			//			pExplosion->move.x -= 5.0f;
			//		}
			//	}
			//	//ブロック右の処理
			//	else if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth - 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth - 30 &&
			//			 pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
			//	{
			//		pPlayer->move.x = 0.0f;//重力リセット
			//		pPlayer->Pos.x = pBlock->Pos.x + pBlock->fWidth + 30;//右の当たり判定
			//		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pExplosion++)
			//		{
			//			g_aBlock[nCntBlock].move.x += 2.0f;
			//		}
			//		pExplosion->move.x -= 5.0f;
			//	}

			//}

			//上下処理

			//ブロック上の処理
			if (pBlock->bUse == true)
			{
				if (pBlock->Pos.x >= pPlayer->oldPos.x - pBlock->fWidth - 20 && pBlock->Pos.x <= pPlayer->oldPos.x + pBlock->fWidth + 20 &&
					pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight + pPlayer->g_fLengthPlayer + 20 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight + pPlayer->g_fLengthPlayer + 20)
				{
					pPlayer->move.y = 0.0f;//重力リセット
					pPlayer->Pos.y = pBlock->Pos.y - pBlock->fHeight - pPlayer->g_fLengthPlayer + 20;//上の当たり判定
				}

				//ブロック下の処理
				else if (pBlock->Pos.x >= pPlayer->oldPos.x - pBlock->fWidth - 20 && pBlock->Pos.x <= pPlayer->oldPos.x + pBlock->fWidth + 20 &&
						 pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - pPlayer->g_fLengthPlayer + 5 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - pPlayer->g_fLengthPlayer + 5)
				{
					pPlayer->move.y = 0.0f;//重力リセット
					pPlayer->Pos.y = pBlock->Pos.y + pBlock->fHeight + pPlayer->g_fLengthPlayer - 5;//下の当たり判定
				}


				//左右処理
				//ブロック左の処理
				if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth + 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth + 30 &&
					pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
				{
					pPlayer->move.x = 0.0f;//重力リセット
					pPlayer->Pos.x = pBlock->Pos.x - pBlock->fWidth - 30;//左の当たり判定
					for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
					{
						g_aBlock[nCntBlock].move.x += 2.0f;
					}
				}

				//ブロック右の処理
				else if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth - 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth - 30 &&
						 pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
					{
						pPlayer->move.x = 0.0f;//重力リセット
						pPlayer->Pos.x = pBlock->Pos.x + pBlock->fWidth + 30;//右の当たり判定
						for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
						{
							g_aBlock[nCntBlock].move.x -= 2.0f;
						}
					}

				}

			//位置更新
			pBlock->Pos.x += pBlock->move.x;
			pBlock->Pos.y += pBlock->move.y;

			pExplosion->pos.x += pExplosion->move.x;
			pExplosion->pos.y += pExplosion->move.y;


		//アイテム処理
			//ブロックタイプが2の時
			if (pBlock->nType == 2)
			{
				if (pBlock->bUse == true)
				{
					//ブロック下の処理
					if (pBlock->Pos.x >= pPlayer->oldPos.x - pBlock->fWidth - 20 && pBlock->Pos.x <= pPlayer->oldPos.x + pBlock->fWidth + 20 &&
					pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - pPlayer->g_fLengthPlayer + 5 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - pPlayer->g_fLengthPlayer + 5)
					{
						pPlayer->move.y = 0.0f;//重力リセット
						pPlayer->Pos.y = pBlock->Pos.y + pBlock->fHeight + pPlayer->g_fLengthPlayer - 5;//下の当たり判定

						//アイテム
						SetItem(pBlock->Pos, 1, pExplosion->move);
						
						//ブロックテクスチャ切り替え
						pBlock->nType = 4;

					}
				}
			}

			//剣の処理
			if (pExplosion->nType == 4)
			{
				if (pExplosion->pos.x >= pPlayer->Pos.x - 50 && pExplosion->pos.x <= pPlayer->Pos.x + 50 &&
					pExplosion->pos.y >= pPlayer->Pos.y - 50 && pExplosion->pos.y <= pPlayer->Pos.y + 50)
				{
					pExplosion->bUse = false;
					for (int nCntBlockfalse = 0; nCntBlockfalse < 55; nCntBlockfalse++)
					{
						g_aBlock[nCntBlockfalse].bUse = false;
					}
					
						SetFade(FADE_OUT, MODE_RESULT);
					
				}
			}

			
			//土管処理
			if (pBlock->nType == 3)
			{
				if (pBlock->Pos.x >= pPlayer->oldPos.x - pBlock->fWidth - 20 && pBlock->Pos.x <= pPlayer->oldPos.x + pBlock->fWidth + 20 &&
					pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight + pPlayer->g_fLengthPlayer + 15 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight + pPlayer->g_fLengthPlayer + 15)
				{
					if (GetKeyboardPress(DIK_S) == true)
					{
						pPlayer->bUse = false;
						SetFade(FADE_OUT, MODE_GAME2);
					}
				}
			}

			//ゴール土管処理
			if (pBlock->nType == 5)
			{
				if (pBlock->Pos.x >= pPlayer->oldPos.x - pBlock->fWidth - 20 && pBlock->Pos.x <= pPlayer->oldPos.x + pBlock->fWidth + 20 &&
					pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight + pPlayer->g_fLengthPlayer + 15 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight + pPlayer->g_fLengthPlayer + 15)
				{
					if (GetKeyboardPress(DIK_S) == true)
					{
						SetFade(FADE_OUT, MODE_RESULT);
					}
				}
			}
			

		//ファイアーバー処理
		if(pBlock->nType == 6)
		{
			D3DXVECTOR3 pos;
			D3DXVECTOR3 pos2;
			int nLife;
			D3DXVECTOR3 move;
			float fAngle;
			D3DXCOLOR col;

			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{
				pos = pBlock->Pos;
				///nLife = 2 + rand() % 5;
				nLife = 5;
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//カラー
				fAngle = (float)(nCountParticle) / 100.0f + D3DX_PI*0.5f;
				move.x = sinf(fAngle)*55.5f;
				move.y = cosf(fAngle)*55.5f;

				SetEffect(pos, nLife, 3, move);
			}
		}
		
		if (pEnemy->nType == 0)
		{
			for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
			{
				if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth - 20 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth + 20 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight + 40 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight + 40)
				{
					pEnemy->move.y = 0.0f;//重力リセット
					pEnemy->Pos.y = g_aBlock[nCntBlock].Pos.y - g_aBlock[nCntBlock].fHeight - 40;//上の当たり判定
				}

				else if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth - 20 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth + 20 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight + 5 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight + 5)
				{
					pEnemy->move.y = 0.0f;//重力リセット
					pEnemy->Pos.y = g_aBlock[nCntBlock].Pos.y - g_aBlock[nCntBlock].fHeight + 5;//上の当たり判定
				}

				//左右処理
				//ブロック左の処理
				if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth + 60 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth + 60 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight - 90 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight - 90)
				{
					pEnemy->move.x = 0.0f;//重力リセット
					pEnemy->Pos.x = g_aBlock[nCntBlock].Pos.x - g_aBlock[nCntBlock].fWidth - 60;//左の当たり判定
				}

				//ブロック右の処理
				else if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth - 30 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth - 30 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight - 90 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight - 90)
				{
					pEnemy->move.x = 0.0f;//重力リセット
					pEnemy->Pos.x = g_aBlock[nCntBlock].Pos.x + g_aBlock[nCntBlock].fWidth + 30;//右の当たり判定
				}
			}
		}

		if (pEnemy->nType == 1)
		{
			for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
			{
				if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth - 20 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth + 20 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight + 80 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight + 80)
				{
					pEnemy->move.y = 0.0f;//重力リセット
					pEnemy->Pos.y = g_aBlock[nCntBlock].Pos.y - g_aBlock[nCntBlock].fHeight - 80;//上の当たり判定
				}

				//左右処理
				//ブロック左の処理
				if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth + 60 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth + 60 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight - 90 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight - 90)
				{
					pEnemy->move.x = 0.0f;//重力リセット
					pEnemy->Pos.x = g_aBlock[nCntBlock].Pos.x - g_aBlock[nCntBlock].fWidth - 60;//左の当たり判定
				}

				//ブロック右の処理
				else if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth - 30 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth - 30 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight - 90 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight - 90)
				{
					pEnemy->move.x = 0.0f;//重力リセット
					pEnemy->Pos.x = g_aBlock[nCntBlock].Pos.x + g_aBlock[nCntBlock].fWidth + 30;//右の当たり判定
				}
			}
		}
		
		if (pEnemy->nType == 2)
		{
			for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
			{
				if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth - 20 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth + 20 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight + 55 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight + 55)
				{
					pEnemy->move.y = 0.0f;//重力リセット
					pEnemy->Pos.y = g_aBlock[nCntBlock].Pos.y - g_aBlock[nCntBlock].fHeight - 55;//上の当たり判定
				}

				//左右処理
				//ブロック左の処理
				if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth + 60 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth + 60 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight - 90 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight - 90)
				{
					pEnemy->move.x = 0.0f;//重力リセット
					pEnemy->Pos.x = g_aBlock[nCntBlock].Pos.x - g_aBlock[nCntBlock].fWidth - 60;//左の当たり判定
				}

				//ブロック右の処理
				else if (g_aBlock[nCntBlock].Pos.x >= pEnemy->Pos.x - g_aBlock[nCntBlock].fWidth - 30 && g_aBlock[nCntBlock].Pos.x <= pEnemy->Pos.x + g_aBlock[nCntBlock].fWidth - 30 &&
					g_aBlock[nCntBlock].Pos.y >= pEnemy->Pos.y - g_aBlock[nCntBlock].fHeight - 90 && g_aBlock[nCntBlock].Pos.y <= pEnemy->Pos.y + g_aBlock[nCntBlock].fHeight - 90)
				{
					pEnemy->move.x = 0.0f;//重力リセット
					pEnemy->Pos.x = g_aBlock[nCntBlock].Pos.x + g_aBlock[nCntBlock].fWidth + 30;//右の当たり判定
				}
			}
		}


		if (pBlock->nType == 9)
		{
			D3DXVECTOR3 pos;
			D3DXVECTOR3 pos2;
			int nLife;
			D3DXVECTOR3 move;
			float fAngle;
			D3DXCOLOR col;

			for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			{
				pos = pBlock->Pos;
				///nLife = 2 + rand() % 5;
				nLife = 5;
				col = D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f);//カラー
				fAngle = (float)(nCountParticleL) / 100.0f + D3DX_PI*0.5f;
				move.x = sinf(fAngle)*55.5f;
				move.y = cosf(fAngle)*55.5f;

				SetEffect(pos, nLife, 3, move);
			}
		}

		if (pPlayer->state == PLAYERSTATE_STAR)
		{
			//ブロック左の処理
			if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth + 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth + 30 &&
				pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
			{
				pPlayer->move.x = 0.0f;//重力リセット
				pPlayer->Pos.x = pBlock->Pos.x - pBlock->fWidth - 30;//左の当たり判定
				for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
				{
					g_aBlock[nCntBlock].move.x += 2.25f;
				}
			}

			else if (pBlock->Pos.x >= pPlayer->Pos.x - pBlock->fWidth - 30 && pBlock->Pos.x <= pPlayer->Pos.x + pBlock->fWidth - 30 &&
				pBlock->Pos.y >= pPlayer->Pos.y - pBlock->fHeight - 10 && pBlock->Pos.y <= pPlayer->Pos.y + pBlock->fHeight - 10)
			{
				pPlayer->move.x = 0.0f;//重力リセット
				pPlayer->Pos.x = pBlock->Pos.x + pBlock->fWidth + 30;//右の当たり判定
				for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
				{
					g_aBlock[nCntBlock].move.x -= 2.25f;
				}
			}
		}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x - pBlock->fWidth, g_aBlock[nCntBlock].Pos.y + pBlock->fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x - pBlock->fWidth, g_aBlock[nCntBlock].Pos.y - pBlock->fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x + pBlock->fWidth, g_aBlock[nCntBlock].Pos.y + pBlock->fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].Pos.x + pBlock->fWidth, g_aBlock[nCntBlock].Pos.y - pBlock->fHeight, 0.0f);
			
			// 移動量の減衰
			pBlock->move.x += (0.0f - pBlock->move.x) *0.5f;

			// 移動量の減衰
			pExplosion->move.x += (0.0f - pExplosion->move.x) *0.67f;

			// 移動量の減衰
			pEnemy->move.x += (0.0f - pEnemy->move.x) *0.4f;

			// 移動量の減衰
			pPlayer->move.x += (0.0f - pPlayer->move.x) *0.5f;

			// 移動量の減衰
			//pBullet->move.x += (0.0f - pBullet->move.x) *0.05f;

		}

		//4頂点ごとずらす
			pVtx += 4;
	}
		
	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}

//==================
//ブロックの描画処理
//==================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	BLOCK *pBlock;
	pBlock = &g_aBlock[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBlock[pBlock->nType]);

		if (g_aBlock[nCntBlock].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	
	}
}

//====================
//ブロック設置処理
//====================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight,int nType)
{
	BLOCK *pBlock;
	VERTEX_2D *pVtx;

	pBlock = &g_aBlock[0];

	// 頂点バッファをロック
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == false)//弾を使用しているかどうか
		{
			if (nType == 0)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

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

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 1)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(30.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(30.0f, 0.0f);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 2)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

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

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 3)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

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

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 4)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

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

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 5)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 0, 255);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 6)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 255, 255);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 7)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 20.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 20.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 8)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

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

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			if (nType == 9)
			{
				//位置設定
				pBlock->Pos = pos;

				pBlock->fHeight = fHeight;

				pBlock->fWidth = fWidth;

				pBlock->nType = nType;

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 255, 255);

				//頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pBlock->Pos.x - pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y + pBlock->fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pBlock->Pos.x + pBlock->fWidth, pBlock->Pos.y - pBlock->fHeight, 0.0f);

				//弾の使用
				pBlock->bUse = true;
				break;
			}
			
		}
			//4頂点ごとずらす
			pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}
BLOCK *GetBlock(void)
{
	return &g_aBlock[60];
}
int GetBlockNumber(void)
{
	return g_ppData;
}


