#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//マクロ定義
#define MAX_BULLET_SIZE (100)
#define MAX_SIZE (40)
#define	MAX_POLYGON_SIZE (10)

typedef enum
{
	BULLETTYPE_ENEMY=0,
	BULLETTYPE_ENEMY_HOMING,
	BULLETTYPE_ENEMY_HOMING2,
	BULLETTYPE_ENEMY_HOMING3,
	BULLETTYPE_MAX
}BULLETTYPE;

//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;
	BULLETTYPE nType;
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
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, BULLETTYPE nType);

#endif