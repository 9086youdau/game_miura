//================================================
//
//プレイヤー処理[Player.h]
// Author; miura yuudai
//
//================================================
#include "Player.h"
#include "bullet.h"
#include "sound.h"
#include "effect.h"
#include "input.h"
#include "enemy.h"
#include "bullet.h"
#include "fade.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTexturePlayer[2] = {};//テクスチャ情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;//頂点情報
PLAYER g_posPlayer;//プレイヤー情報
D3DXVECTOR3 g_rotPlayer;//回転量
D3DXVECTOR3 g_movePlayer;//移動量
int g_nCounterAnim;//アニメーション
int g_nPatternAnim;//アニメーション
int g_nseetAnim;//アニメーション
float g_fLengthPlayer;//対角線の長さ
float g_fAnglePlayer;//対角線の角度
int g_nCnt;//弾のカウント変数
int g_aData = GetEnemyNumber();
int g_nDataa;

//====================
//ポリゴンの初期化処理
//====================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ddd.png", &g_pTexturePlayer[0]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/slow.png", &g_pTexturePlayer[1]);

	//変数初期化
	g_nCounterAnim = 0;
	g_nPatternAnim = 0;
	g_nseetAnim = 0;
	g_posPlayer.bUse = true;
	g_posPlayer.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPlayer.nLife = 4;
	g_posPlayer.nType = 0;
	g_nCnt = 0;
	g_nDataa = 1;
	g_posPlayer.state = PLAYERSTATE_NOMAL;

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
	g_posPlayer.Pos = D3DXVECTOR3(550, 770, 0.0f);

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

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//==================
//プレイヤーの終了処理
//==================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < 2; nCntPlayer++)
	{
		//テクスチャの開放
		if (g_pTexturePlayer[nCntPlayer] != NULL)
		{
			g_pTexturePlayer[nCntPlayer]->Release();
			g_pTexturePlayer[nCntPlayer] = NULL;
		}
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
	BULLET pBullet;
	PLAYER *pPolygon;
	pPolygon = &g_posPlayer;
	g_nCnt++;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//位置更新
	g_posPlayer.Pos.x += g_posPlayer.move.x;
	g_posPlayer.Pos.y += g_posPlayer.move.y;

	if (g_posPlayer.bUse == true)//使用しているか
	{
		//対角線の長さ
		g_fLengthPlayer = sqrtf(100 * 100 + 100 * 100);
		//対角線の角度
		g_fAnglePlayer = atan2f(100, 100);

		pPolygon->nType = 0;

		if (GetKeyboardPress(DIK_D) == true)//右に進む
		{
			//移動量
			g_posPlayer.Pos.x += sinf(D3DX_PI*0.5f) * 5.5f;
			if (GetKeyboardPress(DIK_LSHIFT) == true)//右に進む
			{
				pPolygon->nType = 1;

				//移動量
				g_posPlayer.Pos.x += sinf(D3DX_PI*0.5f) * -2.5f;
			}
		}

		if (GetKeyboardPress(DIK_A) == true)//左に進む
		{
			//移動量
			g_posPlayer.Pos.x += -sinf(D3DX_PI*0.5f) * 5.5f;
			if (GetKeyboardPress(DIK_LSHIFT) == true)//右に進む
			{
				pPolygon->nType = 1;

				//移動量
				g_posPlayer.Pos.x += sinf(D3DX_PI*0.5f) * 2.5f;
			}
		}

		if (GetKeyboardPress(DIK_S) == true)//下に進む
		{
			//移動量
			g_posPlayer.Pos.y += sinf(D3DX_PI*0.5f) * 5.5f;
			if (GetKeyboardPress(DIK_LSHIFT) == true)//右に進む
			{
				pPolygon->nType = 1;

				//移動量
				g_posPlayer.Pos.y += sinf(D3DX_PI*0.5f) * -2.5f;
			}
		}

		if (GetKeyboardPress(DIK_W) == true)//上に進む
		{
			//移動量
			g_posPlayer.Pos.y += -sinf(D3DX_PI*0.5f) * 5.5f;
			if (GetKeyboardPress(DIK_LSHIFT) == true)//右に進む
			{
				pPolygon->nType = 1;

				//移動量
				g_posPlayer.Pos.y += sinf(D3DX_PI*0.5f) * 2.5f;
			}
		}

		if (GetKeyboardPress(DIK_K) == true)//プレイヤーの回転
		{
			//回転量
			g_rotPlayer.z += (D3DX_PI / 18);
			if (g_rotPlayer.z > D3DX_PI)
			{
				g_rotPlayer.z -= D3DX_PI*2.0f;
			}
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)//弾を打つ処理
		{
			if (GetKeyboardPress(DIK_LSHIFT) == true)//
			{
				for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++)
				{
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					D3DXCOLOR col;
					float fAngle;

					col = D3DXCOLOR(1.0f, 0.4f, 0.4f, 0.5f);//色
					pos = g_posPlayer.Pos;//発射位置
					nLife = 300;//弾の寿命
					fAngle = (float)(rand() % 628 - 314) / 100.0f;//弾の範囲
					move.x = sinf(fAngle)*7.5f;//移動量
					move.y = cosf(fAngle)*7.5f;//移動量
					SetBullet(pos, move, nLife, col, BULLETTYPE_ENEMY_HOMING);//弾発射
				}
			}
			else if(GetKeyboardTrigger(DIK_SPACE) == true)//弾の間隔
			{
					D3DXVECTOR3 pos;
					int nLife;
					D3DXVECTOR3 move;
					D3DXCOLOR col;

					col = D3DXCOLOR(1.0f, 0.4f, 0.4f, 0.5f);//色
					pos = g_posPlayer.Pos;//発射位置
					nLife = 200;//弾の寿命
					move.x = sinf(D3DX_PI)*7.5f;//移動量
					move.y = cosf(D3DX_PI)*7.5f;//移動量
					SetBullet(pos, move, nLife, col, BULLETTYPE_ENEMY_HOMING);//弾発射
			}
		}

		//画面端の爆発処理
		if (g_posPlayer.Pos.x + 20 >= 1220)
		{
			//パーティクル処理
			for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
			{
				D3DXVECTOR3 pos;
				int nLife;
				D3DXVECTOR3 move;
				float fAngle;

				pos = pPolygon->Pos;//位置
				nLife = 10 + rand() % 20;//寿命をランダム
				fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
				move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
				move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
				SetEffect(pos, nLife, move);//エフェクト処理
			}
			pPolygon->bUse = false;//使用していない
			SetFade(FADE_OUT, MODE_RESULT);
		}

		//画面端の爆発処理
		else if (g_posPlayer.Pos.x - 20 <= 50)
		{
			//パーティクル処理
			for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
			{
				D3DXVECTOR3 pos;
				int nLife;
				D3DXVECTOR3 move;
				float fAngle;

				pos = pPolygon->Pos;//位置
				nLife = 10 + rand() % 20;//寿命をランダム
				fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
				move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
				move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
				SetEffect(pos, nLife, move);//エフェクト処理
			}
			pPolygon->bUse = false;//使用していない
			if (g_aData == 1)
			{
				SetFade(FADE_OUT, MODE_RESULT);
			}
		}

		//画面端の爆発処理
		if (g_posPlayer.Pos.y - 30 <= 50)
		{
			//パーティクル処理
			for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
			{
				D3DXVECTOR3 pos;
				int nLife;
				D3DXVECTOR3 move;
				float fAngle;

				pos = pPolygon->Pos;//位置
				nLife = 10 + rand() % 20;//寿命をランダム
				fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
				move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
				move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
				SetEffect(pos, nLife, move);//エフェクト処理
			}
			pPolygon->bUse = false;//使用していない
			SetFade(FADE_OUT, MODE_RESULT);
		}

		//画面端の爆発処理
		else if (g_posPlayer.Pos.y + 30 >= 1030)
		{
			//パーティクル処理
			for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
			{
				D3DXVECTOR3 pos;
				int nLife;
				D3DXVECTOR3 move;
				float fAngle;

				pos = pPolygon->Pos;//位置
				nLife = 10 + rand() % 20;//寿命をランダム
				fAngle = (float)(rand() % 628 - 314) / 100.0f;//爆発の範囲
				move.x = sinf(fAngle)*2.5f;//x軸の弾の速度
				move.y = cosf(fAngle)*2.5f;//y軸の弾の速度
				SetEffect(pos, nLife, move);//エフェクト処理
			}

			pPolygon->bUse = false;//使用していない
			SetFade(FADE_OUT, MODE_RESULT);
		}

		//プレイヤーの頂点座標
		pVtx[0].pos.x = g_posPlayer.Pos.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[0].pos.y = g_posPlayer.Pos.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posPlayer.Pos.x - sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[1].pos.y = g_posPlayer.Pos.y - cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posPlayer.Pos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[2].pos.y = g_posPlayer.Pos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posPlayer.Pos.x - sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[3].pos.y = g_posPlayer.Pos.y - cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[3].pos.z = 0.0f;

		switch (g_posPlayer.state)
		{
		case ENEMYSTATE_DAMAGE:
			g_posPlayer.nCounterState--;
			if (g_posPlayer.nCounterState <= 0)
			{
				g_posPlayer.state = PLAYERSTATE_NOMAL;
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

}

//====================
//プレイヤーの描画処理
//====================
void DrawPlayer(void)
{
	PLAYER *pPolygon;
	pPolygon = &g_posPlayer;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer[pPolygon->nType]);

	if (g_posPlayer.bUse == true)//プレイヤーを使用しているかどうか
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}

//=================
//プレイヤーの設定
//=================
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife)
{
	PLAYER *pPolygon;
	VERTEX_2D *pVtx;

	pPolygon = &g_posPlayer;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (pPolygon->bUse == true)//プレイヤーを使用しているかどうか
	{
		//位置設定
		g_posPlayer.Pos = pos;

		//頂点座標の設定
		pVtx[0].pos.x = g_posPlayer.Pos.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[0].pos.y = g_posPlayer.Pos.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posPlayer.Pos.x - sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[1].pos.y = g_posPlayer.Pos.y - cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posPlayer.Pos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[2].pos.y = g_posPlayer.Pos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posPlayer.Pos.x - sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[3].pos.y = g_posPlayer.Pos.y - cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[3].pos.z = 0.0f;


		//弾の使用
		pPolygon->bUse = true;

	}

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

}

bool HitPlayer(int nDamage)
{
	PLAYER *pPlayer;
	pPlayer = &g_posPlayer;
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	g_posPlayer.nLife -= nDamage;//体力からダメージを引く

	if (g_posPlayer.nLife <= 0)//体力が0以下になったら
	{
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		g_posPlayer.bUse = false;
		g_nDataa--;

		for (int nCntAppear = 0; nCntAppear < 100; nCntAppear++)
		{
			D3DXVECTOR3 pos;
			int nLife;
			D3DXVECTOR3 move;
			float fAngle;

			pos = pPlayer->Pos;
			nLife = 100 + rand() % 200;
			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			move.x = sinf(fAngle)*1.5f;
			move.y = cosf(fAngle)*1.5f;
			SetEffect(pos, nLife, move);
		}
		if (g_nDataa == 0)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}

		return true;
	}
	else
	{
		g_posPlayer.state = PLAYERSTATE_DAMAGE;
		g_posPlayer.nCounterState = 20;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	}
	return false;

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=====================
//構造体プレイヤーの設定
//=====================
PLAYER *GetPlayer(void)
{
	return &g_posPlayer;
}

