#ifndef _TIMER_H_
#define _TIMER_H_

#define MAX_TIMER (3)//スコアの最大数
#include "main.h"

//弾の構造体
typedef struct
{
	int nTimer;//時間
}TIMER;

//===============
//プロトタイプ宣言
//===============
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
TIMER *GetTimer(void);
void AddTimer(int nValue);
#endif