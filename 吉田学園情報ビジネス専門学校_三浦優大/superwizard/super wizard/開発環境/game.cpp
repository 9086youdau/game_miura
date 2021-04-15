#include "game.h"
#include "bullet.h"
#include "Player.h"
#include "Explosion.h"
#include "bg.h"
#include "particle.h"
#include "input.h"
#include "Block.h"
#include "result.h"
#include "enemy.h"
#include "Timer.h"
#include "Score.h"
#include "effect.h"
#include "Pause.h"

//グローバル変数
bool g_bPause = false;//ポーズ中かどうか
int g_pData;
//int g_aapData;
int nLife;
int nCount;
float fAngle;
D3DXVECTOR3 move;

HRESULT InitGame(void)
{
	g_pData = 1;

	//g_aapData = 0;

	nCount++;

	ITEM *pExplosion;
	pExplosion = GetItem();

	if (g_pData == 1)
	{
		//背景の初期化
		InitBG();
	}
	g_bPause = false;

	//弾の初期化処理
	InitBullet();

	//敵の初期化処理
	InitEnemy();

	SETENEMY(D3DXVECTOR3(2800.0f, 200.0f, 0.0f), 0);
	SETENEMY(D3DXVECTOR3(1200.0f, 200.0f, 0.0f), 0);
	SETENEMY(D3DXVECTOR3(5400.0f, 500.0f, 0.0f), 2);
	SETENEMY(D3DXVECTOR3(5200.0f, 200.0f, 0.0f), 2);

	//ブロックの初期化
	InitBlock();

	//通路ブロック
	for (int nCntBlock = 0; nCntBlock < 18; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(650.0f + (nCntBlock * 60.0f), 600.0f, 0.0f), 30, 30, 0);
		SetBlock(D3DXVECTOR3(650.0f + (nCntBlock * 60.0f), 300.0f, 0.0f), 30, 30, 0);
		SetBlock(D3DXVECTOR3(2400.0f + (nCntBlock * 60.0f), 950.0f, 0.0f), 30, 30, 0);
		SetBlock(D3DXVECTOR3(4200.0f + (nCntBlock * 60.0f), 600.0f, 0.0f), 30, 30, 0);
		SetBlock(D3DXVECTOR3(4200.0f + (nCntBlock * 60.0f), 300.0f, 0.0f), 30, 30, 0);
	}

	for (int nCntBlock = 0; nCntBlock < 35; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(3600.0f + (nCntBlock * 60.0f), 950.0f, 0.0f), 30, 30, 0);
	}

	SetBlock(D3DXVECTOR3(2400.0f, 150.0f, 0.0f),30,30,0);

	for (int nCntBlock = 0; nCntBlock < 12; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2460.0f + (nCntBlock * 60.0f), 890.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 11; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2520.0f + (nCntBlock * 60.0f), 830.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 10; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2580.0f + (nCntBlock * 60.0f), 770.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 9; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2640.0f + (nCntBlock * 60.0f), 710.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 8; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2700.0f + (nCntBlock * 60.0f), 650.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 7; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2760.0f + (nCntBlock * 60.0f), 590.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 6; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2820.0f + (nCntBlock * 60.0f), 530.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 5; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2880.0f + (nCntBlock * 60.0f), 470.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 4; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(2940.0f + (nCntBlock * 60.0f), 410.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 3; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(3000.0f + (nCntBlock * 60.0f), 350.0f, 0.0f), 30, 30, 0);
	}
	for (int nCntBlock = 0; nCntBlock < 2; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(3060.0f + (nCntBlock * 60.0f), 290.0f, 0.0f), 30, 30, 0);
	}

	SetBlock(D3DXVECTOR3(950.0f, 950.0f, 0.0f), 1000, 30, 1);

	SetBlock(D3DXVECTOR3(5500.0f, 830.0f, 0.0f), 90, 90, 3);


	SetBlock(D3DXVECTOR3(2280.0f, 830.0f, 0.0f), 30, 30, 6);
	SetBlock(D3DXVECTOR3(2080.0f, 890.0f, 0.0f), 30, 30, 6);

	SetBlock(D3DXVECTOR3(1280.0f, 430.0f, 0.0f), 30, 30, 6);

	SetBlock(D3DXVECTOR3(500.0f, 600.0f, 0.0f), 30, 30, 2);

	SetBlock(D3DXVECTOR3(500.0f, 890.0f, 0.0f), 30, 30, 0);

	//ポリゴンの初期化処理
	InitPlayer();

	//爆発の初期化
	InitItem();

	for (int nCntItem = 0; nCntItem < 15; nCntItem++)
	{
		SetItem(D3DXVECTOR3(1500.0f + (nCntItem * 60.0f), 410.0f, 0.0f), 2, pExplosion->move);
	}

	//エフェクトの初期化
	InitEffect();

	//パーティクルの初期化
	InitParticle();

	//タイマーの初期化
	InitTimer();

	//スコアの初期化
	InitScore();

	//ポーズの初期化
	InitPause();

	return S_OK;
}

void UninitGame(void)
{
	//BGMを終了する処理
	//StopSound();

	//g_pData--;
	//if (g_pData == 1)
	{
		//背景の終了処理
		UninitBG();
	}

	//ブロックの終了処理
	UninitBlock();

	//ポリゴンの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();

	//敵の終了処理
	UninitEnemy();

	//爆発の終了処理
	UninitItem();

	//敵の終了処理
	//UninitEnemy();

	//タイマーの終了処理
	UninitTimer();

	//スコアの終了処理
	UninitScore();

	//ポーズの終了処理
	UninitPause();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();
}

void UpdateGame(void)
{ 
	//ポーズ画面の処理
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}
	if (g_bPause == true)
	{
		UpdatePause();
	}
	else
	{
		//g_pData--;
		//if (g_pData == 1)
		{
			//背景の更新処理
			UpdateBG();

			//UpdateResult();
		}

		//ブロック更新処理
		UpdateBlock();

		//ポリゴンの更新処理
		UpdatePlayer();

		//弾の更新処理
		UpdateBullet();

		//敵の終了処理
		UpdateEnemy();

		//爆発の更新処理
		UpdateItem();

		//敵の更新処理
		//UpdateEnemy();

		//タイマーの更新処理
		UpdateTimer();

		//エフェクトの更新処理
		UpdateEffect();

		//スコアの終了処理
		UpdateScore();

		//パーティクルの更新処理
		UpdateParticle();
	}
}

void DrawGame(void)
{
	//if (g_pData == 1)
	{
		//背景の描画処理
		DrawBG();
		//DrawResult();
	}

	//弾の描画処理
	DrawBullet();

	//ブロックの描画処理
	DrawBlock();

	//ポリゴンの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//爆発の描画処理
	DrawItem();

	//タイマーの描画処理
	DrawTimer();

	//スコアの描画処理
	DrawScore();

	//エフェクトの描画処理
	DrawEffect();

	//パーティクルの描画処理
	DrawParticle();

	if (g_bPause == true)
	{
		DrawPause();
		
	}
}
int GetGameNumber(void)
{
	return g_pData;
}
int GetGameNumber2(void)
{
	return S_OK; //g_aapData;
}
