#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//位置
	int nType;
	int nLife;
	bool bUse;//使用しているかどうか
	int g_nCounterAnim;//アニメーション
	int g_nPatternAnim;//アニメーション
	int g_nseetAnim;//アニメーション
}ITEM;
//
//プロトタイプ宣言
//
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos,int nType, D3DXVECTOR3 move);
ITEM *GetItem(void);
#endif
