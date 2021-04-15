#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define ENEMY_PARTS		(16)	// モデルパーツの最大数
#define MAX_KEY			(8)		// キー数の最大数
#define FRAME_NUM		(50)	// フレームの最大数
#define ENEMYMMAX (56)
#define ENEMYMAXVEC (ENEMYMMAX * 4)

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MOVE,		// 移動モーション
	MOTIONTYPE_ACTION,       //アクションモーション
	MOTIONTYPE_MAX			// モーションの最大数
} MOTIONTYPE;

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
} ENEMYKEY;

//==============================================================================
// キー情報の構造体
//==============================================================================
typedef struct
{
	int nFrame;				// 再生フレーム			
	ENEMYKEY aKey[ENEMY_PARTS];	// 各モデルのキー要素 
} ENEMYKEY_SET;

//==============================================================================
// モージョン情報の構造体
//==============================================================================
typedef struct
{
	bool bLoop;				// ループするか
	int nNumKey;			// キーの総数
	ENEMYKEY_SET aKeySet[10];	// キー情報
} ENEMYMOTION_INFO;

//==============================================================================
// モデルの構造体
//==============================================================================
typedef struct
{
	LPD3DXMESH pMesh;		// メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;	// マテリアルへのポインタ
	DWORD nNumMat;			// マテリアルの数
	LPDIRECT3DTEXTURE9 pTextureEnemy;//テクスチャへのポインタ
	D3DXVECTOR3 Pos;			// 位置(オフセット)
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	int nIdxModelParent;		// 親モデルのインデックス
	char aFileName[128];
	bool bUse;
	int nType;
}ENEMYModel;

//==============================================================================
// プレイヤーの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 OldPos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	D3DXVECTOR3 rotmove;
	int nIdx;
	ENEMYModel aModel[ENEMY_PARTS];					// モデル(パーツ)
	int nNumModel/* = MODEL_PARTS*/;			// モデル(パーツ)数
	ENEMYMOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	// モーション情報
	int nNumMotion = MOTIONTYPE_MAX;			// モーション数
	MOTIONTYPE motionType;						// モーションタイプ
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キー数
	int nKey;									// キー№
	int nCounterMotion;							// モーションのカウンター
	D3DXVECTOR3 aPos[ENEMYMAXVEC];
	D3DXVECTOR3 Vec[ENEMYMAXVEC];
	D3DXVECTOR3 g_Vec[ENEMYMAXVEC];
	float PlayerVec[ENEMYMAXVEC];
	D3DXVECTOR3 MinModel; //モデルの最小値
	D3DXVECTOR3	MaxModel;	// モデルの各座標の最大値
	bool bUse;
} ENEMYPLAYER;
//==========================
//プロトタイプ宣言
//==========================
void InitEnemy(void);//初期化処理
void UninitEnemy(void);//終了処理
void UpdateEnemy(void);//更新処理
void DrawEnemy(void);//カメラの設定,Drawの一番上で定義
ENEMYPLAYER *GetEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
void SetEnemyMotion(MOTIONTYPE motionType);		// モーションの設定 
void UpdateEnemyMotion(MOTIONTYPE motionType);	// モーションの更新
//bool CollisionVecModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld,float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);
bool CollisionVecSord(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);
#endif

