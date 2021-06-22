#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//========================
//マクロ定義
//========================
#define MAXITEM (256)
#define MAXITEMVER (MAXITEM * 4)

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
	D3DXVECTOR3 aPos[MAXITEMVER];
	D3DXVECTOR3 Vec[MAXITEMVER];
	D3DXVECTOR3 g_Vec[MAXITEMVER];
	float PlayerVec[MAXITEMVER];
	bool bUse;
	int nType;
	//テクスチャへのポインタ
	LPDIRECT3DTEXTURE9 g_apTextureItem = NULL;
	int nCntScore;
}Item;

//==========================
//プロトタイプ宣言
//==========================
void InitItem(void);//初期化処理
void UninitItem(void);//終了処理
void UpdateItem(void);//更新処理
void DrawItem(void);//カメラの設定,Drawの一番上で定義
Item *GetItem(void);
bool CollisionVecItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
int nCnt(void);
#endif
