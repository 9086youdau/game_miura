#include "game.h"
#include "bullet.h"
#include "Player.h"
#include "enemy.h"
#include "Explosion.h"
#include "bg.h"
#include "sound.h"
#include"effect.h"
#include "Pause.h"
#include "particle.h"
#include "Score.h"
#include "input.h"
#include "Hp.h"

//グローバル変数
bool g_bPause = false;//ポーズ中かどうか

HRESULT InitGame(void)
{
	//背景の初期化
	InitBG();

	//弾の初期化処理
	InitBullet();

	//敵の初期化処理
	InitEnemy();

	//for (int nCntEnemy = 0; nCntEnemy < 20; nCntEnemy++)
	{
		//SETENEMY(D3DXVECTOR3(600 + (nCntEnemy * 75.0f), 100, 0.0f), 0);
	}

	SETENEMY(D3DXVECTOR3(570 ,350, 0.0f), 0);

	//ポリゴンの初期化処理
	InitPlayer();

	//爆発の初期化
	InitExplosion();

	//エフェクトの初期化
	InitEffect();

	//パーティクルの初期化
	InitParticle();

	//スコアの初期化
	InitScore();

	//ポーズの初期化
	InitPause();

	SetBG(D3DXVECTOR3(0, 0, 0.0f), 0);
	SetBG(D3DXVECTOR3(0, 0, 0.0f), 1);
	SetBG(D3DXVECTOR3(1400, 900, 0.0f), 2);
	SetBG(D3DXVECTOR3(1640, 400, 0.0f), 3);
	SetBG(D3DXVECTOR3(1725, 400, 0.0f), 4);

	InitHp();

	return S_OK;
}

void UninitGame(void)
{
	//BGMを終了する処理
	StopSound();

	//背景の終了処理
	UninitBG();

	//ポリゴンの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

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

	UninitHp();
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
		//背景の更新処理
		UpdateBG();

		//ポリゴンの更新処理
		UpdatePlayer();

		//弾の更新処理
		UpdateBullet();

		//爆発の更新処理
		UpdateExplosion();

		//敵の更新処理
		UpdateEnemy();

		//スコアの更新処理
		UpdateScore();

		//エフェクトの更新処理
		UpdateEffect();

		//パーティクルの更新処理
		UpdateParticle();

		UpdateHp();

	}
}

void DrawGame(void)
{
	//背景の描画処理
	DrawBG();

	//弾の描画処理
	DrawBullet();

	//ポリゴンの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//爆発の描画処理
	DrawExplosion();

	//背景の描画処理
	DrawScore();

	//エフェクトの描画処理
	DrawEffect();

	//パーティクルの描画処理
	DrawParticle();

	DrawHp();

	if (g_bPause == true)
	{
		DrawPause();
		
	}
	
}