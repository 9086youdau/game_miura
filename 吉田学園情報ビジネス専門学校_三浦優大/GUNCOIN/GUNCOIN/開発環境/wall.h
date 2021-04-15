#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//========================
//マクロ定義
//========================
#define MAXWALL (256)
#define MAXWALLVER (MAXWALL * 4)

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
	D3DXVECTOR3 aPos[MAXWALLVER];
	D3DXVECTOR3 Vec[MAXWALLVER];
	D3DXVECTOR3 g_Vec[MAXWALLVER];
	float PlayerVec[MAXWALLVER];
	bool bUse;
	int nType;
	//テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 g_apTextureWall = NULL;
}WALL;

//==========================
//プロトタイプ宣言
//==========================
void InitWall(void);//初期化処理
void UninitWall(void);//終了処理
void UpdateWall(void);//更新処理
void DrawWall (void);//カメラの設定,Drawの一番上で定義
WALL *GetWall(void);
bool CollisionVecWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType);
#endif


