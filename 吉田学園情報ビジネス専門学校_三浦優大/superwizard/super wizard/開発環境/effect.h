#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
//#include "enemy.h"


//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	int nType;
	bool bUse;//使用しているかどうか
	int nLife;
}EFFECT;
//
//プロトタイプ宣言
//
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos,int nLife,int nType, D3DXVECTOR3 move);
EFFECT *GetEffect(void);

#endif
