//-----------------------------------------------------------------
//
// バックグラウンドの処理 (bg.h)
// Author:Miurayudai
//
//-----------------------------------------------------------------

#ifndef _BG_H_
#define _BG_H_

#include "main.h"

//======================================
//構造体の定義
//======================================
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	int nType;//種類
	bool bUse;//使用しているかどうか
}BG;

//==========================================
//プロトタイプ宣言
//==========================================
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void SetBG(D3DXVECTOR3 Pos, int nType);
#endif
