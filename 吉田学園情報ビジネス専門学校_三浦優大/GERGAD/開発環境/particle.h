#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "enemy.h"

//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;
	D3DXCOLOR color;
	int g_nCounterAnim;
	int g_nPatternAnim;
	int g_nseetAnim;
	float fRadius;
	bool bUse;//使用しているかどうか
	int nLife;
}PARTICLE;
//
//プロトタイプ宣言
//
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos,int nLife, D3DXVECTOR3 move);

#endif
