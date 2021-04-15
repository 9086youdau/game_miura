//-----------------------------------------------------------------
//
// リザルト画面の処理 (result.h)
// Author:Miurayudai
//
//-----------------------------------------------------------------

#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//==================================================
//構造体の定義
//==================================================
typedef struct
{
	int nType;
	int nData;
}RESULT;

//====================================================
//プロトタイプ宣言
//====================================================
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetVertexTextureResult(int nCntResult);
#endif


