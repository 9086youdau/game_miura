#ifndef _BG_H_
#define _BG_H_

#include "main.h"

//===============
//構造体の定義
//===============
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;
	int nType;//種類
	bool bUse;//使用しているかどうか
	int g_nCounterAnim;//アニメーション
	int g_nPatternAnim;//アニメーション
	int g_nseetAnim;//アニメーション
}BG;

//===============
//プロトタイプ宣言
//===============
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void SetVertexBG(int nCntBG);
#endif
