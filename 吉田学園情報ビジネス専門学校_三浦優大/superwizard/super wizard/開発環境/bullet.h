#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//マクロ定義
#define MAX_BULLET_SIZE (20)
#define MAX_SIZE (15)
#define	MAX_POLYGON_SIZE (20)

//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;
	int nType;
	int nLife;//寿命
	bool bUse;//使用しているかどうか
}BULLET;

//
//プロトタイプ宣言
//
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int nType);
BULLET *GetBullet(void);
#endif