#ifndef _CAVEGAME_H_
#define _CAVEGAME_H_

#include "main.h"

//===============
//プロトタイプ宣言
//===============
HRESULT InitCaveGame(void);
void UninitCaveGame(void);
void UpdateCaveGame(void);
void DrawCaveGame(void);
bool SetCavePause(bool bPause);
int GetcaveGameNumber(void);
#endif