#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//==========================
//プロトタイプ宣言
//==========================
void InitLight(void);//初期化処理
void UninitLight(void);//終了処理
void UpdateLight(void);//更新処理
D3DLIGHT9 *GetLight(void);
#endif
