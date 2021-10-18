//=============================================================================
//
// メイン処理 [main.h]
// Author : MIURA YUDAI
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"//入力処理に必要
#include "xaudio2.h"
#include <stdio.h>

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
#define ID_BUTTON000 (101)//ボタンのID
#define ID_EDIT000 (111)//エディットのID
#define TIMER_INTERVAL (1000/60)//FPS計算用
#define LOOP_POLYGON (1356)//ループ配列用
#define SCREEN_WIDTH (1920)//画面の幅
#define SCREEN_HEIGHT (1080)//画面の高さ
#define SCREEN_CENTER_X (1920 / 2)//画面の半分の幅
#define SCREEN_CENTER_Y (1080 / 2)//画面の半分の高さ
#define MAXSIZEPLAYER (100) //サイズの大きさ
#define MAXTEXTURE (1.0f)//テクスチャのサイズ
#define MAX_EXPLOSION_TEXTURE (0.125f)//テクスチャのサイズ
#define MAX_EXPLOSION_SIZE (30)//爆発のサイズ
#define MAX_ENEMY_SIZE_X (50)//エネミーのサイズ
#define MAX_ENEMY_SIZE_Y (40)//エネミーのサイズ
#define MAX_ENEMY2_SIZE_X (40)//エネミーのサイズ
#define MAX_ENEMY2_SIZE_Y (60)//エネミーのサイズ
#define MAX_ENEMY_TEXTURE (0.1f)//エネミーのサイズ
#define MAX_SCORE_SIZE_X (20)//スコアのXサイズ
#define MAX_SCORE_SIZE_Y (20)//スコアのYサイズ
#define MAX_EFFECT_SIZE (130)//エフェクトのサイズ
#define MAX_PARTICLE_SIZE (6)//パーティクルのサイズ
#define MAX_LIFE_TEXTURE_SIZE (30)//ライフのテクスチャサイズ
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CRenderer;
class CScene;

//*****************************************************************************
//構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_3D;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GetFPS(void);                                                 
#endif