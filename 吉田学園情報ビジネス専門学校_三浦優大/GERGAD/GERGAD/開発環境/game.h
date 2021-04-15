//-----------------------------------------------------------------
//
// ゲームの処理 (game.h)
// Author:Miurayudai
//
//-----------------------------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//======================================
//プロトタイプ宣言
//======================================
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
bool SetPause(bool bPause);
#endif
