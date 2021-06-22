#ifndef _STONE_H_
#define _STONE_H_

#include "main.h"

//======================================
//マクロ定義
//======================================
#define MAX_OBJECT_TYPE (2)
#define MAXOBJECT (256)
#define MAXOBJECTVER (MAXOBJECT * 4)

//======================================
//構造体の定義
//======================================
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 rotmove;//向き
	D3DXVECTOR3 rotDest;//目的の向き
	D3DXVECTOR3 MaxObject;//最大値
	D3DXVECTOR3 MinObject;//最小値
	D3DXVECTOR3 aPosObject[MAXOBJECTVER];
	D3DXVECTOR3 VecObject[MAXOBJECTVER];
	D3DXVECTOR3 aVecObject[MAXOBJECTVER];
	float ObjectVec[MAXOBJECTVER];
	bool bUse;
	int nType;
}OBJECT;

//==========================
//プロトタイプ宣言
//==========================
void InitObject(void);//初期化処理
void UninitObject(void);//終了処理
void UpdateObject(void);//更新処理
void DrawObject(void);//描画処理
OBJECT *GetObject(void);
bool CollisionVecObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
#endif

