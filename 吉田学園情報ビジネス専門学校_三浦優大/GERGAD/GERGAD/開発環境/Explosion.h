//-----------------------------------------------------------------
//
// 爆発アニメーションの処理 (Explosion.h)
// Author:Miurayudai
//
//-----------------------------------------------------------------

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//===================================
//弾の構造体
//===================================
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXCOLOR col;
	int g_nCounterAnim;
	int g_nPatternAnim;
	int g_nseetAnim;
	bool bUse;//使用しているかどうか
}EXPLOSION;

//=======================================
//プロトタイプ宣言
//=======================================
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif
