//-----------------------------------------------------------------
//
// スコアの処理 (Score.h)
// Author:Miurayudai
//
//-----------------------------------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//================================================
//プロトタイプ宣言
//================================================
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(void);
void AddScore(int nValue);
#endif