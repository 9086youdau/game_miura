//-----------------------------------------------------------------
//
// 敵の処理 (enemy.h)
// Author:Miurayudai
//
//-----------------------------------------------------------------

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//==============================
//敵の状態
//==============================
typedef enum
{
	ENEMYSTATE_NOMAL = 0 ,//通常時
	ENEMYSTATE_DAMAGE,//ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//===================================
//敵の構造体
//===================================
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;//移動量
	int nType;//種類
	bool bUse;//使用しているかどうか
	int nCounterAni;//アニメーション
	int nPatternAni;//アニメーション
	int nseetAni;//アニメーション
	ENEMYSTATE state;//状態
	int nCounterState;//状態カウンター
	int nLife;//体力
	D3DXVECTOR3 rotEnemy;//敵のサイズ情報
	float fLengthEnemy;//対角線の長さ
	float fAngleEnemy;//対角線の角度
}ENEMY;

//==========================================
//プロトタイプ宣言
//==========================================
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SETENEMY(D3DXVECTOR3 Pos, int nType);
ENEMY *GetEnemy(void);
bool HitEnemy(int nIdx, int nDamage);
int GetEnemyNumber(void);
#endif