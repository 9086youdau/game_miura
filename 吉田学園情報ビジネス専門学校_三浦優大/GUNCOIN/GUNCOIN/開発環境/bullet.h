#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//弾の構造体
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR col;
	int nLife;//寿命
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	bool bUse;//使用しているかどうか
}BULLET;

//==================
//プロトタイプ宣言
//==================
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife);
BULLET *GetBullet(void);
#endif
