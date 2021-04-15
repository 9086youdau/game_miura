#ifndef _TUTOLIAL_H_
#define _TUTOLIAL_H_

#include "main.h"
#include "input.h"
#include "fade.h"



typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXCOLOR col;
	int nType;//種類
	bool bUse;//使用しているかどうか
}TUTOLIAL;

//===============
//プロトタイプ宣言
//===============
HRESULT InitTutolial(void);
void UninitTutolial(void);
void UpdateTutolial(void);
void DrawTutolial(void);
void SetTutolial(int nCount);
#endif
