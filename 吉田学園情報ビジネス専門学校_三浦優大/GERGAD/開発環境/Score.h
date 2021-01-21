#ifndef _SCORE_H_
#define _SCORE_H_

#define MAX_SCORE (8)//敵の最大数
#include "main.h"


//===============
//プロトタイプ宣言
//===============
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(void);
void AddScore(int nValue);
#endif