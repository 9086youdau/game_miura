#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PARTS		(16)	// モデルパーツの最大数
#define MAX_KEY			(8)		// キー数の最大数
#define FRAME_NUM		(50)	// フレームの最大数
#define MODEL_TEX_NUM	(8)		// モデルのテクスチャ数
#define MBF_NEU_MOVE	(3)		// モーションブレンドフレーム：ニュートラルから移動
#define MBF_MOVE_NEU	(15)	// モーションブレンドフレーム：移動からニュートラル
#define MBF_NEU_ACT		(15)	// モーションブレンドフレーム：ニュートラルからアクション
#define MBF_ACT_NEU		(40)	// モーションブレンドフレーム：アクションからニュートラル
#define MBF_LND_NEU		(20)	// モーションブレンドフレーム：着地からニュートラル

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_MODELNEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MODELMOVE,		// 移動モーション
	MOTIONTYPE_MODELACTION,       //アクションモーション
	MOTIONTYPE_JUMP,
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

// 近接攻撃の種類
typedef enum
{
	SHORT_SLASH = 0,		// 剣
	SHORT_MAX
}ShortRange;

//==============================================================================
// 武器の保有状態
//==============================================================================
typedef enum
{
	WEAPON_NONE = 0,	// 素手
	WEAPON_MAX
}Weapon;

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
	D3DXVECTOR3 posReset;	
	D3DXVECTOR3 posMemo;								// 保存用位置情報
	D3DXVECTOR3 rotMemo;								// 保存用角度情報
}Model;

//==============================================================================
// 近接攻撃の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
	D3DXMATRIX mtxWorld;			// 当たり判定のマトリックス
	int nParts;						// 発生するパーツ番号
	float fRadius;					// 判定半径
	int nStartFrame;				// 開始フレーム
	int nEndFrame;					// 終了フレーム
	int nCntFrameTotal;				// 攻撃フレーム数
	bool bUse;						// 攻撃の有効化
	float fGetDamage[4];
} Attack;

// 攻撃球体
typedef struct
{
	int nParts;
	int nStartFrame;
	int nEndFrame;
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posold;		//過去の位置
	D3DXVECTOR3 rot;		// 角度
	D3DXCOLOR col;			// 色	
	D3DXVECTOR3 move;		// 位置
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	LPD3DXMESH pMeshSphere;
	LPD3DXBUFFER pBuffMatSphere;	// マテリアルへのポインタ
	float fRadius;
	int nSlice;
	int nStack;
	int nCntFrameTotal;		// 攻撃フレーム数
	bool bUse;				// 使用状態
	D3DXVECTOR3 vtxMinSphere; //モデルの最小値
	D3DXVECTOR3	vtxMaxSphere;	// モデルの各座標の最大値
	D3DXVECTOR3 aPosSph[4];
	D3DXVECTOR3 VecSph[4];
	float SphVec[4];
} AttackSphere;

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
	AttackSphere as;
	bool bUse;
	float fOutRadius;							// プレイヤーの外半径
	float fHeight;								// プレイヤーの高さ
	int nFrameTotal[MOTIONTYPE_MODELMAX];			// 各モーションの合計フレーム数
	bool bMotionChange;							// モーション変化時の判定
	int nBrendFrame;							// モーションブレンド時のフレーム数
	MOTIONMODELTYPE motionTypeOld;					// 直前のモーションタイプ
	ShortRange sr;								// 近接攻撃の種類
	int nMoveCnt;								// 移動モーションカウント
	Attack attack;
	Weapon weapon;
} Player;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitModel(void);	// プレイヤーの初期化処理
void UninitModel(void);	// プレイヤーの終了処理
void UpdateModel(void);	// プレイヤーの更新処理
void DrawModel(void);		// プレイヤーの描画処理
Player *GetModel(void);	// プレイヤーの取得
void UpdateMotion(MOTIONMODELTYPE motionType);	// モーションの更新
void LoadMotion(void);						// .txtフォルダの読み込み
void AttackParts(void);
//bool CollisionVecSph(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);
//bool CollisionVecEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);//外積の当たり判定
#endif
