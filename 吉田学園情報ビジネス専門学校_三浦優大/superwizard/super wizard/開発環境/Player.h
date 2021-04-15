#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//===================
//プレイヤーの状態
//===================
typedef enum
{
	PLAYERSTATE_NOMAL = 0,//通常時
	PLAYERSTATE_DAMAGE,//ダメージ状態
	PLAYERSTATE_MUTEKI,//無敵状態
	PLAYERSTATE_FIRE,//火の状態
	PLAYERSTATE_FREAZ,//氷の状態
	PLAYERSTATE_STAR,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 oldPos;//位置
	D3DXVECTOR3 move;//移動量
	int g_nCounterAnim;//アニメーション
	int g_nPatternAnim;//アニメーション
	int g_nseetAnim;//アニメーション
	PLAYERSTATE state;//状態
	int nCounterState;//状態カウンター
	int nCounterStateStar;
	int nType;//種類
	int nLife;//寿命
	bool bUse;//使用しているかどうか
	float g_fLengthPlayer;//対角線の長さ
	float g_fAnglePlayer;//対角線の角度
}PLAYER;

//===============
//プロトタイプ宣言
//===============
HRESULT InitPlayer(void);//初期化処理
void UninitPlayer(void);//終了処理
void UpdatePlayer(void);//更新処理
void DrawPlayer(void);//描画処理
PLAYER *GetPlayer(void);
bool HitPlayer(int nDamage);
#endif
