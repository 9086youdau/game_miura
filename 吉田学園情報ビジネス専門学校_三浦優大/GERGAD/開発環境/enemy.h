#ifndef _ENEMY_H_
#define _ENEMY_H_

#define MAX_ENEMY (256)//敵の最大数
#define MAX_TYPE_ENEMY (3)//敵の種類数
#define MAX_ENEMY_SIZE (145)//敵のサイズ

#include "main.h"

//========
//敵の状態
//========
typedef enum
{
	ENEMYSTATE_NOMAL = 0 ,//通常時
	ENEMYSTATE_DAMAGE,//ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//===========
//敵の構造体
//===========
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;//移動量
	int nType;//種類
	bool bUse;//使用しているかどうか
	int g_nCounterAni;//アニメーション
	int g_nPatternAni;//アニメーション
	int g_nseetAni;//アニメーション
	ENEMYSTATE state;//状態
	int nCounterState;//状態カウンター
	int nLife;//体力
}ENEMY;

//===============
//プロトタイプ宣言
//===============
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SETENEMY(D3DXVECTOR3 Pos, int nType);
ENEMY *GetEnemy(void);
bool HitEnemy(int nIdx, int nDamage);
int GetEnemyNumber(void);
#endif