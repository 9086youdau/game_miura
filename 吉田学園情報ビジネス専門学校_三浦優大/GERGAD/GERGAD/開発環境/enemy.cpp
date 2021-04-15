//================================================
//
//敵の処理[enemy.cpp]
// Author; miura yuudai
//
//================================================

//================================================
//ヘッダー読み込み
//================================================
#include "input.h"
#include "enemy.h"
#include "bullet.h"
#include "Player.h"
#include "Explosion.h"
#include "sound.h"
#include "enemy.h"
#include "particle.h"
#include "effect.h"
#include "fade.h"
#include "Score.h"

//================================================
//マクロ定義
//================================================
#define MAX_ENEMY (256)//敵の最大数
#define MAX_TYPE_ENEMY (3)//敵の種類数
#define MAX_ENEMY_SIZE (145)//敵のサイズ
#define MAX_BULLET_SIZE (80)//弾のサイズ

//================================================
//グローバル変数
//================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = {};//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;//頂点バッファへのポインタ
ENEMY g_aEnemy[MAX_ENEMY];//敵の情報
int g_nData = 1;//カウント変数

//====================================
//エネミーの初期化処理
//====================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/200.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy001.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy002.png", &g_apTextureEnemy[2]);

	//グローバル変数初期化
	g_nData = 1;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;//敵のタイプ
		g_aEnemy[nCntEnemy].bUse = false;//使われているかどうか
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;//エネミーの状態
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 1000;//敵の体力
		g_aEnemy[nCntEnemy].nCounterAni = 0;//カウンターアニメーション
		g_aEnemy[nCntEnemy].nPatternAni = 0;//パターンアニメーション
		g_aEnemy[nCntEnemy].nseetAni = 0;//シートアニメーション
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
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

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
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
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}

	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

	//返り値
	return S_OK;
}

//====================================
//エネミーの終了処理
//====================================
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

//=====================================
//エネミーの更新処理
//=====================================
void UpdateEnemy(void)
 {
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	//ポインタ変数
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	pEnemy = &g_aEnemy[0];
	pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//アニメーション処理
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//対角線の長さ
		g_aEnemy[nCntEnemy].fLengthEnemy = sqrtf(MAX_ENEMY_SIZE *MAX_ENEMY_SIZE + MAX_ENEMY_SIZE * MAX_ENEMY_SIZE);
		//対角線の角度
		g_aEnemy[nCntEnemy].fAngleEnemy = atan2f(MAX_ENEMY_SIZE, MAX_ENEMY_SIZE);

		if (g_aEnemy[nCntEnemy].bUse == true)//敵が使用されているかどうか
		{
			g_aEnemy[nCntEnemy].nCounterAni++;//アニメーションがプラスされていく

			if ((g_aEnemy[nCntEnemy].nCounterAni % 10) == 0)//10で割り切れる
			{
				g_aEnemy[nCntEnemy].nPatternAni++;//アニメーションがプラス

				g_aEnemy[nCntEnemy].nseetAni = (g_aEnemy[nCntEnemy].nseetAni + 1) % 10;

				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f*g_aEnemy[nCntEnemy].nseetAni, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + 0.1f*g_aEnemy[nCntEnemy].nseetAni, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f + 0.1f*g_aEnemy[nCntEnemy].nseetAni, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f*g_aEnemy[nCntEnemy].nseetAni, 0.0f);

			}

			if (g_aEnemy[nCntEnemy].nPatternAni % 10 == 0)//アニメーションが10の倍数の時
			{
				for (int nCntAppear = 0; nCntAppear < 7; nCntAppear++)//パーティクル処理
				{
					//変数
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					float fAngle;
					D3DXCOLOR col;

					//設定
					col = D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f);//カラー
					pos = pEnemy->Pos;//位置
					nLife = 200 + rand() % 300;//ランダムで寿命
					fAngle = (float)(rand() % 628 - 314) / 100.0f;//弾の範囲
					move.x = sinf(fAngle)*4.0f;//弾の移動量
					move.y = cosf(fAngle)*4.0f;//弾の移動量
					SetBullet(pos, move, nLife, col, BULLETTYPE_ENEMY_HOMING2);//弾発射
				}

				if(g_aEnemy[nCntEnemy].nPatternAni % 40 == 0)//アニメーションが40の倍数の時
				{
					for (int nCntAppear = 0; nCntAppear < 2; nCntAppear++)//パーティクル処理
					{
						//変数
						D3DXVECTOR3 pos;
						int nLife;
						D3DXVECTOR3 move;
						float fAngle;
						D3DXCOLOR col;

						//設定
						pos = pEnemy->Pos;//位置
						nLife = 550;//ランダムで寿命
						fAngle = (float)(rand() % 157 - 78) / 100.0f;//弾の範囲
						move.x = sinf(fAngle)*2.0f;//弾の移動量
						move.y = cosf(fAngle)*2.0f;//弾の移動量
						SetBullet(pos, move, nLife, col, BULLETTYPE_ENEMY_HOMING2);//弾発射
					}
				}
			}

			if (g_aEnemy[nCntEnemy].nPatternAni >= 100)//アニメーションが100になったら
			{
				float fAnglee;
				fAnglee = (float)(rand() % 628 - 314) / 100.0f;

				//位置更新
				g_aEnemy[nCntEnemy].Pos.x += pEnemy->move.x;
				g_aEnemy[nCntEnemy].Pos.y += pEnemy->move.y;

				//エネミーが移動
				pEnemy->move.x += sinf(fAnglee) * 0.2f;
				pEnemy->move.y += cosf(fAnglee) * 0.2f;

				//画面端の処理
				if (g_aEnemy[nCntEnemy].Pos.y + 20 >= 1030)
				{
					pEnemy->move.y *= -1;
				}
				else if (g_aEnemy[nCntEnemy].Pos.y - 20 <= 50)
				{
					pEnemy->move.y *= -1;
				}

				//画面端の処理
				if (g_aEnemy[nCntEnemy].Pos.x + 20 >= 1220)
				{
					pEnemy->move.x *= -1;
				}
				else if (g_aEnemy[nCntEnemy].Pos.x - 20 <= 50)
				{
					pEnemy->move.x *= -1;
				}

			//プレイヤーと敵が衝突した時の判定
			else if (pEnemy->bUse == true)
			{
				if (pPlayer->Pos.x >= pEnemy->Pos.x - MAX_BULLET_SIZE && pPlayer->Pos.x <= pEnemy->Pos.x + MAX_BULLET_SIZE &&
					pPlayer->Pos.y >= pEnemy->Pos.y - MAX_BULLET_SIZE && pPlayer->Pos.y <= pEnemy->Pos.y + MAX_BULLET_SIZE)
				{
					//パーティクル処理
					for (int nCntAppear = 0; nCntAppear < 100; nCntAppear++)
					{
						//変数
						D3DXVECTOR3 pos;
						int nLife;
						D3DXVECTOR3 move;
						float fAngle;

						//設定
						pos = pPlayer->Pos;//位置
						nLife = 10 + rand() % 20;//寿命をランダム
						fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
						move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
						move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
						SetEffect(pos, nLife, move);//エフェクト処理
					}
					pPlayer->bUse = false;//使われているか
					SetFade(FADE_OUT, MODE_RESULT);//当たったらリザルトにフェードアウト
				}
			}
		}

			//移動時の頂点情報
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].Pos.x + sinf(g_aEnemy[nCntEnemy].rotEnemy.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].Pos.y + cosf(g_aEnemy[nCntEnemy].rotEnemy.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].Pos.x - sinf(g_aEnemy[nCntEnemy].rotEnemy.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].Pos.y - cosf(g_aEnemy[nCntEnemy].rotEnemy.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].Pos.x + sinf(g_aEnemy[nCntEnemy].rotEnemy.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].Pos.y + cosf(g_aEnemy[nCntEnemy].rotEnemy.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].Pos.x - sinf(g_aEnemy[nCntEnemy].rotEnemy.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].Pos.y - cosf(g_aEnemy[nCntEnemy].rotEnemy.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.z = 0.0f;

			//敵の状態
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_DAMAGE://ダメージ状態になったら色を変更
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;
					//頂点カラーの設定
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			}
		}
		pVtx += 4;//4頂点ごとずらす
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
  }
//====================================
//エネミーの描画処理
//====================================
void DrawEnemy(void)
{
	//ポインタ変数
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCutEnemy = 0; nCutEnemy < MAX_ENEMY; nCutEnemy++,pEnemy++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

		if(pEnemy->bUse == true)//弾を使用しているかどうか
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCutEnemy*4, 2);
		}
	}
}

//======================================
//エネミーの設定
//======================================
void SETENEMY(D3DXVECTOR3 Pos, int nType)
{
	//ポインタ変数
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == false)//弾を使用しているかどうか
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

		pVtx += 4;//4頂点ごとずらす
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//===================================
//エネミーの構造体設定
//===================================
ENEMY *GetEnemy(void) 
{
	return &g_aEnemy[0];
}

//==========================================
//エネミーの弾ヒット時の設定
//==========================================
bool HitEnemy(int nIdx, int nDamage)
{
	//ポインタ変数
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[nIdx];

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nIdx].nLife -= nDamage;//体力からダメージを引く

	if (g_aEnemy[nIdx].nLife <= 0)//体力が0以下になったら
	{
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		AddScore(99999999);//スコアが99999999入る
		g_aEnemy[nIdx].bUse = false;//trueからfalseに
		g_nData--;//カウント変数

		//倒した後にでるエフェクト
		for (int nCntAppear = 0; nCntAppear < 300; nCntAppear++)
		{
			//変数
			D3DXVECTOR3 pos;
			int nLife;
			D3DXVECTOR3 move;
			float fAngle;

			//設定
			pos = pEnemy->Pos;
			nLife = 100 + rand() % 200;
			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			move.x = sinf(fAngle)*1.5f;
			move.y = cosf(fAngle)*1.5f;
			SetEffect(pos, nLife, move);
		}
		//0になったらフェードアウト
		if (g_nData == 0)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
		
		return true;
	}
	else
	{
		//敵に弾が当たったら100スコア
			AddScore(100);
			g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nIdx].nCounterState = 10;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	}
		return false;
	
	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}
int GetEnemyNumber(void)
{
	return g_nData;
}
