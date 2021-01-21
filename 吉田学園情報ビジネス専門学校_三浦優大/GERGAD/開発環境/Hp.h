#ifndef _HP_H_
#define _HP_H_

#define MAX_HP (8)

#include "main.h"

//===============
//プロトタイプ宣言
//===============
HRESULT InitHp(void);
void UninitHp(void);
void UpdateHp(void);
void DrawHp(void);
void SetHp(void);
void AddHp(int nValue);
#endif
