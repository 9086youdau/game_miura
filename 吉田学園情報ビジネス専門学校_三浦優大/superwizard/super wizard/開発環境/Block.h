#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

typedef enum
{
	BLOCKTYPE_BLOCK = 0,
	BLOCKTYPE_BLOCK_FILED,
	BLOCKTYPE_BLOCK_BLOCK,
	BLOCKTYPE_BLOCK_ITEM,
	BLOCKTYPE_MAX
}BLOCKTYPE;

//===============
//構造体の定義
//===============
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	D3DXVECTOR3 oldPos;//位置
	D3DXVECTOR3 move;
	float fWidth;//幅
	float fHeight;//高さ
	int nType;
	bool bUse;//使われているか
}BLOCK;

//===============
//プロトタイプ宣言
//===============
HRESULT InitBlock(void);//初期化処理
void UninitBlock(void);//終了処理
void UpdateBlock(void);//更新処理
void DrawBlock(void);//描画処理
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight,int nType);//ブロック設置処理
BLOCK *GetBlock(void);
int GetBlockNumber(void);
#endif
