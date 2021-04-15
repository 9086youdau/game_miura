#include "cavegame.h"
#include "bullet.h"
#include "Player.h"
#include "Explosion.h"
#include "bg.h"
#include "particle.h"
#include "input.h"
#include "Block.h"
#include "result.h"
#include "effect.h"
#include "enemy.h"
#include "Score.h"
#include "Pause.h"
#include "Timer.h"

//グローバル変数
bool g_bbPause = false;//ポーズ中かどうか
int g_apData;

HRESULT InitCaveGame(void)
{
	ITEM *pExplosion;
	pExplosion = GetItem();

	g_apData = 1;

	if (g_apData == 1)
	{
		//背景の初期化
		InitBG();
	}

	//弾の初期化処理
	InitBullet();

	//敵の初期化処理
	InitEnemy();

	SETENEMY(D3DXVECTOR3(4100.0f, 200.0f, 0.0f), 1);
	SETENEMY(D3DXVECTOR3(1200.0f, 200.0f, 0.0f), 0);

	//ブロックの初期化
	InitBlock();

	for (int nCntBlock = 0; nCntBlock < 15; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(650.0f + (nCntBlock * 60.0f), 800.0f, 0.0f), 30, 30, 8);
		SetBlock(D3DXVECTOR3(650.0f + (nCntBlock * 60.0f), 300.0f, 0.0f), 30, 30, 8);
	}

	for (int nCntBlock = 0; nCntBlock < 25; nCntBlock++)
	{
		SetBlock(D3DXVECTOR3(3000.0f + (nCntBlock * 60.0f), 970.0f, 0.0f), 30, 30, 0);
		//SetBlock(D3DXVECTOR3(3000.0f + (nCntBlock * 60.0f), 970.0f, 0.0f), 30, 30, 8);
	}

	SetBlock(D3DXVECTOR3(950.0f, 970.0f, 0.0f), 1000, 30, 1);
	SetBlock(D3DXVECTOR3(950.0f, 1030.0f, 0.0f), 1000, 30, 1);

	SetBlock(D3DXVECTOR3(400.0f, 900.0f, 0.0f), 30, 30, 8);

	//クッパ戦の左側ブロック
	SetBlock(D3DXVECTOR3(2960.0f, 970.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(2960.0f, 910.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(2900.0f, 970.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(2900.0f, 910.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(2960.0f, 850.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(2900.0f, 850.0f, 0.0f), 30, 30, 8);

	//クッパ戦の右側ブロック
	SetBlock(D3DXVECTOR3(4560.0f, 970.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4560.0f, 910.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4500.0f, 970.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4500.0f, 910.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4560.0f, 850.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4560.0f, 790.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4560.0f, 730.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4560.0f, 670.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4500.0f, 850.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4500.0f, 790.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4500.0f, 730.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4500.0f, 670.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4610.0f, 970.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4660.0f, 970.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4720.0f, 970.0f, 0.0f), 30, 30, 8);
	SetBlock(D3DXVECTOR3(4780.0f, 970.0f, 0.0f), 30, 30, 8);



	SetBlock(D3DXVECTOR3(40.0f, 400.0f, 0.0f), 30, 540, 7);
	SetBlock(D3DXVECTOR3(-20.0f, 400.0f, 0.0f), 30, 540, 7);

	//SetBlock(D3DXVECTOR3(300.0f, 400.0f, 0.0f), 30, 540, 7);

	SetBlock(D3DXVECTOR3(1050.0f, 600.0f, 0.0f), 30, 30, 2);


	SetBlock(D3DXVECTOR3(590.0f, 300.0f, 0.0f), 30, 30, 6);
	SetBlock(D3DXVECTOR3(1550.0f, 300.0f, 0.0f), 30, 30, 6);
	SetBlock(D3DXVECTOR3(590.0f, 800.0f, 0.0f), 30, 30, 9);
	SetBlock(D3DXVECTOR3(1550.0f, 800.0f, 0.0f), 30, 30, 9);

	//ファイアーバーゾーン
	SetBlock(D3DXVECTOR3(2100.0f, 870.0f, 0.0f), 30, 30, 6);
	SetBlock(D3DXVECTOR3(2300.0f, 870.0f, 0.0f), 30, 30, 9);
	SetBlock(D3DXVECTOR3(2500.0f, 870.0f, 0.0f), 30, 30, 9);
	SetBlock(D3DXVECTOR3(2700.0f, 830.0f, 0.0f), 30, 30, 6);




	//ポリゴンの初期化処理
	InitPlayer();

	//爆発の初期化
	InitItem();

	SetItem(D3DXVECTOR3(4700.0f, 900.0f, 0.0f), 4, pExplosion->move);

	//エフェクトの初期化
	InitEffect();

	//パーティクルの初期化
	InitParticle();

	//スコアの初期化
	InitScore();

	//ポーズの初期化
	InitPause();

	//タイマーの初期化
	InitTimer();

	return S_OK;
}

void UninitCaveGame(void)
{

	g_apData = 0;

	//BGMを終了する処理
	//StopSound();

	//背景の終了処理
	UninitBG();

	//ブロックの終了処理
	UninitBlock();

	//ポリゴンの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitItem();

	//敵の終了処理
	UninitEnemy();

	//スコアの終了処理
	UninitScore();

	//ポーズの終了処理
	UninitPause();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//タイマーの終了処理
	UninitTimer();
}

void UpdateCaveGame(void)
{
	//g_aData--;
	//ポーズ画面の処理
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bbPause = g_bbPause ? false : true;
	}
	if (g_bbPause == true)
	{
		UpdatePause();
	}
	else
	{
		//背景の更新処理
		UpdateBG();
			
		//ブロック更新処理
		UpdateBlock();

		//ポリゴンの更新処理
		UpdatePlayer();

		//弾の更新処理
		UpdateBullet();

		//爆発の更新処理
		UpdateItem();

		//敵の更新処理
		UpdateEnemy();

		//スコアの更新処理
		UpdateScore();

		//エフェクトの更新処理
		UpdateEffect();

		//パーティクルの更新処理
		UpdateParticle();

		//タイマーの更新処理
		UpdateTimer();
	}
}

void DrawCaveGame(void)
{

	//背景の描画処理
	DrawBG();

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

	//背景の描画処理
	DrawScore();

	//エフェクトの描画処理
	DrawEffect();

	//パーティクルの描画処理
	DrawParticle();

	//タイマーの描画処理
	DrawTimer();

	if (g_bbPause == true)
	{
		DrawPause();
	}
}
int GetcaveGameNumber(void)
{
	return g_apData;
}