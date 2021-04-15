#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=============================
//カメラの構造体
//=============================
typedef struct
{
	D3DXVECTOR3 PosV;//視点
	D3DXVECTOR3 PosR;//注視点
	D3DXVECTOR3 VecU;//上方向ベクトル
	D3DXVECTOR3 moveV;//視点
	D3DXVECTOR3 PosVDest;//注視点
	D3DXVECTOR3 moveR;//注視点
	D3DXVECTOR3 PosRDest;//注視点
	D3DXMATRIX mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX mtxView;//ビューマトリックス
	D3DXVECTOR3 rot;//カメラの向き
	float fLength;//距離
}Camera;

//==========================
//プロトタイプ宣言
//==========================
void InitCamera(void);//初期化処理
void UninitCamera(void);//終了処理
void UpdateCamera(void);//更新処理
void SetCamera(void);//カメラの設定,Drawの一番上で定義
Camera GetCamera(void);
#endif