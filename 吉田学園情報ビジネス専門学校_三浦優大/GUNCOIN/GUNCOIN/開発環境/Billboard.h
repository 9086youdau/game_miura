#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXCOLOR color;//色
	int nType;//種類
	int nLife;//寿命
	bool bUse;//使用しているかどうか
	LPDIRECT3DTEXTURE9 pTextureBillboard = NULL;
}BILLBOARD;

//===============
//プロトタイプ宣言
//===============
HRESULT InitBillboard(void);//初期化処理
void UninitBillboard(void);//終了処理
void UpdateBillboard(void);//更新処理
void DrawBillboard(void);//描画処理
void SetBillboard(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 move,int nType);
BILLBOARD *GetBillboard(void);
#endif
