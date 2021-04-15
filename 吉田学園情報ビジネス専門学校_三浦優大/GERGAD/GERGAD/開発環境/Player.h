//-----------------------------------------------------------------
//
// プレイヤー画面の処理 (player.h)
// Author:Miurayudai
//
//-----------------------------------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//======================
//プレイヤーの状態
//======================
typedef enum
{
	PLAYERSTATE_NOMAL = 0,//通常時
	PLAYERSTATE_DAMAGE,//ダメージ状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;//移動量
	int nType;//種類
	int nLife;//寿命
	PLAYERSTATE state;//状態
	int nCounterState;
	bool bUse;//使用しているかどうか
	D3DXVECTOR3 rotPlayer;//回転量
	D3DXVECTOR3 movePlayer;//移動量
	int nCounterAnim;//アニメーション
	int nPatternAnim;//アニメーション
	int nseetAnim;//アニメーション
	float fLengthPlayer;//対角線の長さ
	float fAnglePlayer;//対角線の角度
}PLAYER;

//===============
//プロトタイプ宣言
//===============
HRESULT InitPlayer(void);//初期化処理
void UninitPlayer(void);//終了処理
void UpdatePlayer(void);//更新処理
void DrawPlayer(void);//描画処理
bool HitPlayer(int nDamage);
PLAYER *GetPlayer(void);

#endif
