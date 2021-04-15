#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PARTS		(16)	// モデルパーツの最大数
#define MAX_KEY			(8)		// キー数の最大数
#define FRAME_NUM		(50)	// フレームの最大数

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_MODELNEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MODELMOVE,		// 移動モーション
	MOTIONTYPE_MODELACTION,       //アクションモーション
	MOTIONTYPE_MODELMAX			// モーションの最大数
} MOTIONMODELTYPE;

//==============================================================================
// キーの構造体
//==============================================================================
typedef struct
{
	float fPosX;	// 位置X
	float fPosY;	// 位置Y
	float fPosZ;	// 位置Z
	float fRotX;	// 向きX
	float fRotY;	// 向きY
	float fRotZ;	// 向きZ
} KEY;

//==============================================================================
// キー情報の構造体
//==============================================================================
typedef struct
{
	int nFrame;				// 再生フレーム			
	KEY aKey[MODEL_PARTS];	// 各モデルのキー要素 
} KEY_SET;

//==============================================================================
// モージョン情報の構造体
//==============================================================================
typedef struct
{
	bool bLoop;				// ループするか
	int nNumKey;			// キーの総数
	KEY_SET aKeySet[10];	// キー情報
} MOTION_INFO;

//==============================================================================
// モデルの構造体
//==============================================================================
typedef struct
{
	LPD3DXMESH pMeshModel;		// メッシュへのポインタ
	LPD3DXBUFFER pBuffMatModel;	// マテリアルへのポインタ
	DWORD nNumMatModel;			// マテリアルの数
	LPDIRECT3DTEXTURE9 pTextureModel;//テクスチャへのポインタ
	D3DXVECTOR3 pos;			// 位置(オフセット)
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 posOld;			// 昔の位置(オフセット)
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	int nIdxModelParent;		// 親モデルのインデックス
	char aFileName[128];
	bool bUse;
	D3DXVECTOR3 g_vtxMinSord; //モデルの最小値
	D3DXVECTOR3	g_vtxMaxSord;	// モデルの各座標の最大値
}Model;

//==============================================================================
// プレイヤーの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	int nIdx;
	Model aModel[MODEL_PARTS];					// モデル(パーツ)
	int nNumModel/* = MODEL_PARTS*/;			// モデル(パーツ)数
	MOTION_INFO aMotionInfo[MOTIONTYPE_MODELMAX];	// モーション情報
	int nNumMotion = MOTIONTYPE_MODELMAX;			// モーション数
	MOTIONMODELTYPE motionType;						// モーションタイプ
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キー数
	int nKey;									// キー№
	int nCounterMotion;							// モーションのカウンター
	D3DXVECTOR3 aPos[4];
	D3DXVECTOR3 Vec[4];
	D3DXVECTOR3 g_Vec[4];
	float PlayerVec[4];
	D3DXVECTOR3 g_vtxMinPlayer; //モデルの最小値
	D3DXVECTOR3	g_vtxMaxPlayer;	// モデルの各座標の最大値
	bool bUse;
} Player;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitModel(void);	// プレイヤーの初期化処理
void UninitModel(void);	// プレイヤーの終了処理
void UpdateModel(void);	// プレイヤーの更新処理
void DrawModel(void);		// プレイヤーの描画処理
Player *GetModel(void);	// プレイヤーの取得
void SetMotion(MOTIONMODELTYPE motionType);		// モーションの設定 
void UpdateMotion(MOTIONMODELTYPE motionType);	// モーションの更新
void LoadMotion(void);						// .txtフォルダの読み込み
//bool CollisionVecEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);//外積の当たり判定
#endif
