//================================================
//
//プレイヤー処理[Player.h]
// Author; miura yuudai
//
//================================================
#include "Player.h"
#include "input.h"
#include "bg.h"
#include "Explosion.h"
#include "Block.h"
#include "bullet.h"
#include "fade.h"
#include "enemy.h"
#include "effect.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//テクスチャ情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;//頂点情報
PLAYER g_posPlayer;//プレイヤー情報
D3DXVECTOR3 g_rotPlayer;//回転量
D3DXVECTOR3 g_movePlayer;//移動量
int g_nCnt;


//====================
//ポリゴンの初期化処理
//====================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ShipType04.png", &g_pTexturePlayer);

	//変数初期化
	g_posPlayer.bUse = true;
	g_posPlayer.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPlayer.oldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPlayer.nLife = 1000;
	g_posPlayer.state = PLAYERSTATE_NOMAL;
	g_posPlayer.nCounterState = 100;//無敵時間
	g_posPlayer.nCounterStateStar = 1000;
	g_posPlayer.g_nCounterAnim = 0;
	g_posPlayer.g_nPatternAnim = 0;
	g_posPlayer.g_nseetAnim = 0;
	g_posPlayer.g_fAnglePlayer = 0;
	g_posPlayer.g_fLengthPlayer = 0;
	int g_nCnt = 0;


	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーの初期位置
	g_posPlayer.Pos = D3DXVECTOR3(400, 800, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.25f, 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//==================
//プレイヤーの終了処理
//==================
void UninitPlayer(void)
{
	//テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

}
//==================
//プレイヤーの更新処理
//==================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;

	PLAYER *pPlayer;
	ITEM * pExplosion;
	BLOCK *pBlock;
	ENEMY *pEnemy;
	EFFECT *pEffect;
	BULLET *pBullet;

	pPlayer = GetPlayer();
	pExplosion = GetItem();
	pBlock = GetBlock();
	pEnemy = GetEnemy();
	pEffect = GetEffect();
	pBullet = GetBullet();
	g_nCnt++;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		if (pPlayer->bUse == true)//使用しているか
		{
			g_posPlayer.oldPos.x = g_posPlayer.Pos.x;
			g_posPlayer.oldPos.y = g_posPlayer.Pos.y;

			if (g_posPlayer.state == PLAYERSTATE_NOMAL)
			{
				//対角線の長さ
				g_posPlayer.g_fLengthPlayer = sqrtf(40 * 40 + 50 * 50);

				//対角線の角度
				g_posPlayer.g_fAnglePlayer = atan2f(40, 50);

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++)
				{
					//死亡処理
					if (pPlayer->Pos.x >= pEnemy->Pos.x - 50 && pPlayer->Pos.x <= pEnemy->Pos.x + 50 &&
						pPlayer->Pos.y >= pEnemy->Pos.y - 50 && pPlayer->Pos.y <= pEnemy->Pos.y + 50)
					{
						//パーティクル処理
						for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)
						{
							D3DXVECTOR3 pos;
							int nLife;
							D3DXVECTOR3 move;
							float fAngle;

							pos = pPlayer->Pos;//位置
							nLife = 100 + rand() % 200;//寿命をランダム
							fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
							move.x = sinf(fAngle)*1.5f;//x軸の弾の速度
							move.y = cosf(fAngle)*1.5f;//y軸の弾の速度
							SetEffect(pos, nLife, 0, move);//エフェクト処理
						}
						pPlayer->bUse = false;
						pBlock->move.x = 0.0f;
						pPlayer->move.x = 0.0f;
						SetFade(FADE_OUT, MODE_RESULT);
					}
				}
				//死亡処理
				if (pEffect->nType == 3)
				{
					if (pEffect->bUse == true)
					{
						for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEffect++)
						{
							if (pPlayer->Pos.x >= pEffect->pos.x - 30 && pPlayer->Pos.x <= pEffect->pos.x + 30 &&
								pPlayer->Pos.y >= pEffect->pos.y - 30 && pPlayer->Pos.y <= pEffect->pos.y + 30)
							{
								pPlayer->bUse = false;
								//パーティクル処理
								for (int nCntAppear = 0; nCntAppear < 5; nCntAppear++)
								{
									D3DXVECTOR3 pos;
									int nLife;
									D3DXVECTOR3 move;
									float fAngle;

									pos = pPlayer->Pos;//位置
									nLife = 100 + rand() % 200;//寿命をランダム
									fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
									move.x = sinf(fAngle)*1.5f;//x軸の弾の速度
									move.y = cosf(fAngle)*1.5f;//y軸の弾の速度
									SetEffect(pos, nLife, 0, move);//エフェクト処理
								}
								pBlock->move.x = 0.0f;
								pPlayer->move.x = 0.0f;
								SetFade(FADE_OUT, MODE_RESULT);
							}
						}
					}
				}
				//死亡処理
				if (pBullet->nType == 0)
				{
					if (pBullet->bUse == true)
					{
						for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pBullet++)
						{
							if (pPlayer->Pos.x >= pBullet->pos.x - 90 && pPlayer->Pos.x <= pBullet->pos.x + 90 &&
								pPlayer->Pos.y >= pBullet->pos.y - 90 && pPlayer->Pos.y <= pBullet->pos.y + 90)
							{
								pPlayer->bUse = false;
								//パーティクル処理
								for (int nCntAppear = 0; nCntAppear < 5; nCntAppear++)
								{
									D3DXVECTOR3 pos;
									int nLife;
									D3DXVECTOR3 move;
									float fAngle;

									pos = pPlayer->Pos;//位置
									nLife = 100 + rand() % 200;//寿命をランダム
									fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
									move.x = sinf(fAngle)*1.5f;//x軸の弾の速度
									move.y = cosf(fAngle)*1.5f;//y軸の弾の速度
									SetEffect(pos, nLife, 0, move);//エフェクト処理
								}
								pBlock->move.x = 0.0f;
								pPlayer->move.x = 0.0f;
								SetFade(FADE_OUT, MODE_RESULT);
							}
						}
					}
				}
			}

			if (g_posPlayer.state == PLAYERSTATE_DAMAGE)
			{
				//無敵時間に突入
				if (pEnemy->bUse == true)
				{
					for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++)
					{
						if (pPlayer->oldPos.x >= pEnemy->Pos.x - 50 && pPlayer->oldPos.x <= pEnemy->Pos.x + 50 &&
							pPlayer->Pos.y >= pEnemy->Pos.y - 50 && pPlayer->Pos.y <= pEnemy->Pos.y + 50)
						{
							g_posPlayer.state = PLAYERSTATE_MUTEKI;
						}
					}
				}
				//無敵時間に突入
				if (pEffect->nType == 3)
				{
					if (pEffect->bUse == true)
					{
						for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEffect++)
						{
							if (pPlayer->Pos.x >= pEffect->pos.x - 30 && pPlayer->Pos.x <= pEffect->pos.x + 30 &&
								pPlayer->Pos.y >= pEffect->pos.y - 30 && pPlayer->Pos.y <= pEffect->pos.y + 30)
							{
								g_posPlayer.state = PLAYERSTATE_MUTEKI;
							}
						}
					}
				}
				//無敵時間に突入
				if (pEffect->nType == 5)
				{
					if (pEffect->bUse == true)
					{
						for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEffect++)
						{
							if (pPlayer->Pos.x >= pEffect->pos.x - 30 && pPlayer->Pos.x <= pEffect->pos.x + 30 &&
								pPlayer->Pos.y >= pEffect->pos.y - 30 && pPlayer->Pos.y <= pEffect->pos.y + 30)
							{
								g_posPlayer.state = PLAYERSTATE_MUTEKI;
							}
						}
					}
				}
			}

			if (g_posPlayer.state == PLAYERSTATE_MUTEKI)
			{
				g_posPlayer.nCounterState--;

				//対角線の長さ
				g_posPlayer.g_fLengthPlayer = sqrtf(40 * 40 + 40 * 40);

				//対角線の角度
				g_posPlayer.g_fAnglePlayer = atan2f(40, 40);

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

				pPlayer->bUse = true;

				if (g_posPlayer.nCounterState <= 0)
				{
					g_posPlayer.state = PLAYERSTATE_NOMAL;
				}
			}

			//ファイヤー状態
			if (g_posPlayer.state == PLAYERSTATE_FIRE)
			{
				//対角線の長さ
				pPlayer->g_fLengthPlayer = sqrtf(40 * 40 + 80 * 80);

				//対角線の角度
				pPlayer->g_fAnglePlayer = atan2f(40, 70);

				//if (GetKeyboardTrigger(DIK_F) == true)//プレイヤーの回転
				//{
				//	//プレイヤーの弾処理
				//	for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)//パーティクル処理
				//	{
				//		D3DXVECTOR3 pos;
				//		int nLife;
				//		D3DXVECTOR3 move;
				//		float fAngle;
				//		D3DXCOLOR col;

				//		col = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);//カラー
				//		pos = pPlayer->Pos;//位置
				//		nLife = 100 + rand() % 200;//ランダムで寿命
				//		fAngle = (float)(rand() % 158 - 78) / 100.0f;//弾の範囲
				//		move.x = sinf(D3DX_PI*0.5)*4.0f;//弾の移動量
				//		move.y = cosf(D3DX_PI*0.5)*4.0f;//弾の移動量
				//		SetBullet(pos, move, nLife ,col,4);
				//	}
				//}

				//敵とプレイヤーの処理
				if (pEnemy->bUse == true)
				{
					for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++)
					{
						if (pPlayer->oldPos.x >= pEnemy->Pos.x - 50 && pPlayer->oldPos.x <= pEnemy->Pos.x + 50 &&
							pPlayer->Pos.y >= pEnemy->Pos.y - 50 && pPlayer->Pos.y <= pEnemy->Pos.y + 50)
						{
							g_posPlayer.state = PLAYERSTATE_DAMAGE;
						}
					}
				}
				
				//プレイヤーとファイアーバーの判定
				if (pEffect->nType == 3)
				{
					if (pEffect->bUse == true)
					{
						for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEffect++)
						{
							if (pPlayer->Pos.x >= pEffect->pos.x - 40 && pPlayer->Pos.x <= pEffect->pos.x + 40 &&
								pPlayer->Pos.y >= pEffect->pos.y - 40 && pPlayer->Pos.y <= pEffect->pos.y + 40)
							{
								g_posPlayer.state = PLAYERSTATE_DAMAGE;
							}
						}
					}
				}
			}

			else if (g_posPlayer.state == PLAYERSTATE_FREAZ)
			{
				//対角線の長さ
				pPlayer->g_fLengthPlayer = sqrtf(30 * 30 + 70 * 70);

				//対角線の角度
				pPlayer->g_fAnglePlayer = atan2f(30, 70);

				if (GetKeyboardTrigger(DIK_F) == true)//プレイヤーの回転
				{
					for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)//パーティクル処理
					{
						D3DXVECTOR3 pos;
						int nLife;
						D3DXVECTOR3 move;
						float fAngle;
						D3DXCOLOR col;

						col = D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f);//カラー
						pos = pPlayer->Pos;//位置
						nLife = 200 + rand() % 300;//ランダムで寿命
						fAngle = (float)(rand() % 628 - 314) / 100.0f;//弾の範囲
						move.x = sinf(D3DX_PI*0.5)*4.0f;//弾の移動量
						move.y = cosf(D3DX_PI*0.5)*4.0f;//弾の移動量
						SetBullet(pos, move, nLife, col, 1);
					}
				}
				if (pEnemy->bUse == true)
				{
					for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++)
					{
						if (pPlayer->oldPos.x >= pEnemy->Pos.x - 50 && pPlayer->oldPos.x <= pEnemy->Pos.x + 50 &&
							pPlayer->Pos.y >= pEnemy->Pos.y - 50 && pPlayer->Pos.y <= pEnemy->Pos.y + 50)
						{
							g_posPlayer.state = PLAYERSTATE_DAMAGE;
						}
					}
				}
			}

			else if (g_posPlayer.state == PLAYERSTATE_STAR)
			{
				pPlayer->nCounterStateStar--;
				pPlayer->bUse = true;
				if (GetKeyboardPress(DIK_D) == true)//右に進む
				{
					//移動量
					for (int nCntExplosion = 0; nCntExplosion < 256; nCntExplosion++, pBlock++,pExplosion++)
					{
						//プレイヤーが動いたときブロックも
						pBlock->move.x -= 2.25f;

						pExplosion->move.x -= 2.25f;
					}
				}
				if (GetKeyboardPress(DIK_A) == true)//左に進む
				{
					//移動量
					for (int nCntExplosion = 0; nCntExplosion < 256; nCntExplosion++, pBlock++, pExplosion++)
					{
						//プレイヤーが動いたときブロックも
						pBlock->move.x += 2.25f;

						pExplosion->move.x += 2.25f;
					}
				}
				if (pPlayer->nCounterStateStar <= 0)
				{
					pPlayer->state = PLAYERSTATE_NOMAL;
				}

				D3DXVECTOR3 pos;
				int nLife;
				D3DXVECTOR3 move;
				float fAngle;
				D3DXCOLOR col;

				col = D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f);//カラー
				pos = pPlayer->Pos;//位置
				nLife = 10 + rand() % 30;//ランダムで寿命
				fAngle = (float)(rand() % 628 - 314) / 100.0f;//弾の範囲
				move.x = sinf(fAngle)*4.0f;//弾の移動量
				move.y = cosf(fAngle)*4.0f;//弾の移動量
				SetEffect(pos, nLife, 0, move);
			}


			if (GetKeyboardPress(DIK_D) == true)//右に進む
			{
				//移動量
				//g_posPlayer.move.x += sinf(D3DX_PI*0.5f) * 1.0f;

				g_posPlayer.g_nCounterAnim++;//アニメーションがプラスされていく

				if ((g_posPlayer.g_nCounterAnim % 5) == 0)//10で割り切れる
				{
					g_posPlayer.g_nPatternAnim++;

					g_posPlayer.g_nseetAnim = (g_posPlayer.g_nseetAnim + 1) % 8;

					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f*g_posPlayer.g_nseetAnim, 0.5f);
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.25f*g_posPlayer.g_nseetAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.25f + 0.25f*g_posPlayer.g_nseetAnim, 0.5f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f*g_posPlayer.g_nseetAnim, 0.0f);
				}
			}

			if (GetKeyboardPress(DIK_A) == true)//左に進む
			{
				//移動量
				//g_posPlayer.move.x += -sinf(D3DX_PI*0.5f) * 2.0f;

				g_posPlayer.g_nCounterAnim++;//アニメーションがプラスされていく

				if ((g_posPlayer.g_nCounterAnim % 5) == 0)//10で割り切れる
				{
					g_posPlayer.g_nPatternAnim++;

					g_posPlayer.g_nseetAnim = (g_posPlayer.g_nseetAnim + 1) % 8;

					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f*g_posPlayer.g_nseetAnim, 1.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.25f*g_posPlayer.g_nseetAnim, 0.5f);
					pVtx[2].tex = D3DXVECTOR2(0.25f + 0.25f*g_posPlayer.g_nseetAnim, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f*g_posPlayer.g_nseetAnim, 0.5f);
				}
			}

			if (g_posPlayer.move.y == 0.0f)
			{
				if (GetKeyboardTrigger(DIK_SPACE) == true)//弾を打つ処理
				{
					//頂点情報の設定
					pVtx[0].tex = D3DXVECTOR2(0.25f, 0.5f);
					pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f, 0.5f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 0.0f);

					//移動量
					g_posPlayer.move.y -= sinf(D3DX_PI*0.5f) * 28.5f;
				}
			}

			
			//重力
			g_posPlayer.move.y += 1.25f;

			//移動量の減衰
			g_posPlayer.move.x += (0.0f - g_posPlayer.move.x) *0.5f;

			//位置更新
			g_posPlayer.Pos.x += g_posPlayer.move.x;
			g_posPlayer.Pos.y += g_posPlayer.move.y;


			if (g_posPlayer.Pos.y >= 3080)
			{
				g_posPlayer.bUse = false;
				SetFade(FADE_OUT, MODE_RESULT);
			}
		

			//プレイヤーの頂点座標
			pVtx[0].pos.x = g_posPlayer.Pos.x + sinf(g_rotPlayer.z - g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[0].pos.y = g_posPlayer.Pos.y + cosf(g_rotPlayer.z - g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_posPlayer.Pos.x - sinf(g_rotPlayer.z + g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[1].pos.y = g_posPlayer.Pos.y - cosf(g_rotPlayer.z + g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_posPlayer.Pos.x + sinf(g_rotPlayer.z + g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[2].pos.y = g_posPlayer.Pos.y + cosf(g_rotPlayer.z + g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_posPlayer.Pos.x - sinf(g_rotPlayer.z - g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[3].pos.y = g_posPlayer.Pos.y - cosf(g_rotPlayer.z - g_posPlayer.g_fAnglePlayer) * g_posPlayer.g_fLengthPlayer;
			pVtx[3].pos.z = 0.0f;

		}

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

}

//====================
//プレイヤーの描画処理
//====================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_posPlayer.bUse == true)//プレイヤーを使用しているかどうか
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}
PLAYER *GetPlayer(void)
{
	return &g_posPlayer;
}
bool HitPlayer(int nDamage)
{
	PLAYER *pPlayer;
	pPlayer = &g_posPlayer;

	ITEM *pExplosion;
	pExplosion = GetItem();

	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	g_posPlayer.nLife -= nDamage;//体力からダメージを引く

	if (g_posPlayer.nLife -= nDamage)//体力が0以下になったら
	{
		g_posPlayer.state = PLAYERSTATE_DAMAGE;
	}
	return false;

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

