#ifndef _STONE_H_
#define _STONE_H_

#include "main.h"

//========================
//マクロ定義
//========================
#define MAXSTONE (256)
#define MAXSTONEVER (MAXSTONE * 4)

//===============
//構造体の定義
//===============
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 rotmove;//向き
	D3DXVECTOR3 rotDest;//目的の向き
	D3DXVECTOR3 MaxStone;//最大値
	D3DXVECTOR3 MinStone;//最小値
	D3DXVECTOR3 aPos[MAXSTONEVER];
	D3DXVECTOR3 Vec[MAXSTONEVER];
	D3DXVECTOR3 g_Vec[MAXSTONEVER];
	float PlayerVec[MAXSTONEVER];
	bool bUse;
	int nType;
	//テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 g_apTextureStone[3];

}STONE;

//==========================
//プロトタイプ宣言
//==========================
void InitStone(void);//初期化処理
void UninitStone(void);//終了処理
void UpdateStone(void);//更新処理
void DrawStone(void);//カメラの設定,Drawの一番上で定義
STONE *GetStone(void);
bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);
void SetStone(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
#endif

