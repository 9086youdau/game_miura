#ifndef _COIN_H_
#define _COIN_H_

#define MAX_COIN (2)//スコアの最大数
#include "main.h"

//弾の構造体
typedef struct
{
	int nCoin;//時間
	LPDIRECT3DTEXTURE9 g_apTextureCoin = NULL;//テクスチャへポインタ
}COIN;

//===============
//プロトタイプ宣言
//===============
HRESULT InitCoin(void);
void UninitCoin(void);
void UpdateCoin(void);
void DrawCoin(void);
COIN *GetCoin(void);
void AddCoin(int nValue);
#endif