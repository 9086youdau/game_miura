#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "input.h"
#include "fade.h"


typedef enum
{
	TITLETYPE_TITLENOMAL = 0,
	TITLETYPE_BG,
	TITLETYPE_TITLE,
	TITLETYPE_PRESSENTER,
	TITLETYPE_MAX
}TITLETYPE;

typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXCOLOR col;
	int nType;//種類
	bool bUse;//使用しているかどうか
}TITLE;

//===============
//プロトタイプ宣言
//===============

HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitle(int nCount);
#endif

