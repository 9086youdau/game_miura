//=============================================================================
//
// メイン処理 [main.h]
// Author : Miura　Yudai
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド再生に必要
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define CLASS_NAME "WindowClass"			//ウィンドウのクラスの名前
//#define WINDOW_NAME "WARNING"				//ウィンドウの名前
#define ID_BUTTON000	(101)				//ボタンのID
#define ID_EDIT000		(111)				//エディットのID
#define WINDOW_POS_X	(40)				//
#define WINDOW_POS_Y	(40)				//
#define ID_TIMER		(121)				//
#define TIMER_INTERVAL	(1000/60)			//
#define LOOP_PLAYER		(3)					//
#define SCREEN_WIDTH	(1920)				//画面の幅
#define SCREEN_HEIGHT	(1080)				//画面の高さ
#define MAX_POLYGON		(300)				//描画できる数
#define MAX_LIFE		(5)					//プレイヤーのライフ数

//*****************************************************************************
// 関数定義
//*****************************************************************************
int GetFPS(void);

//前方宣言
class CRenderer;
class CManager;

//プロトタイプ宣言
//CRenderer *GetRenderer(void);
CManager *GetManager(void);

#endif