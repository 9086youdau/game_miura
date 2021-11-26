//=============================================================================
//
// プレイヤー3D処理 [player3D.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _PLAYER3D_H_
#define _PLAYER3D_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "main.h"
#include "scene.h"
#include "model.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PARTS		(20)	// モデルパーツの最大数
#define TEXT_LENGTH		(128)	// テキスト読み込みの最大数
#define DATA_LENGTH		(64)	// データ読み込みの最大数
#define MAX_PLAYER_PARTS (17)   // モデルのパーツ数

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MOVE,		// 移動モーション
	MOTIONTYPE_LEFTMOVE,	// 左移動モーション
	MOTIONTYPE_RIGHTMOVE,	// 右移動モーション
	MOTIONTYPE_MAX			// モーションの最大数
} MOTIONTYPE;

//=============================================================================
// ポリゴンクラスの定義
//=============================================================================
//継承先描画クラス
class CPlayer3D : public CScene
{
public:

	CPlayer3D(int nPriority = 5, int nType = 0);								//コンストラクタ
	virtual ~CPlayer3D();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//初期化処理
	void Uninit(void);															//終了処理
	void Update(void);															//更新処理
	void Draw(void);															//描画処理

	static CPlayer3D *Create();													//生成処理
	void LoadMotion(void);														//外部読み込み
	void WriteMotion(void);                                                     //外部書き込み
	void UpdateMotion(int MOTIONTYPE,bool bUse);
	void MotionTool(void);                                                      //モーションツール用操作関数
	void PlayerMove(void);                                                      //プレイヤーの移動処理

	static D3DXVECTOR3	GetPosition(void);						//位置取得用
	static D3DXVECTOR3	GetRotation(void);						//向き取得用
	//モーションツール用
	static MOTIONTYPE   GetMotionTypeMax(void) { return m_MotionTypeMax; } //モーションの最大数取得
	static int   GetMotionType(void) { return m_MotionChange; } //現在のモーション取得
	static int   GetKeyMax(void) { return m_nNumKeyKeep; } //モーションの最大数取得
	static int   GetKey(void) { return m_GetKey; } //モーションの最大数取得
	static int   GetFrame(void) { return m_nFrame[m_MotionChange][m_GetKey]; }
	static int   GetFrameTotal(void) { return m_nFrameTotal[m_MotionChange]; }
	static int   GetCounter(void) { return m_nCounterMotion; }
	static int   GetNumModel(void) { return m_nNumModel; }
	static D3DXVECTOR3 GetModelParentRot(int nCnt) { return m_apModel[nCnt]->GetRot(); }
	static D3DXVECTOR3 GetModelParentPos(int nCnt) { return m_apModel[nCnt]->GetPos(); }
	static char *GetSelectData(int nCnt) { return m_SelectData[nCnt][256]; }
	static int   GetModelCount(void) { return m_DataCount; }
	static float GetFirstModel(int nCnt) { return m_aOffset[0][nCnt]; }

private:
	static CModel   *m_apModel[MAX_PLAYER_PARTS];				//モデルへのポインタ
	char			*m_pTextName[MAX_PLAYER_PARTS];				//名前保存
	int				m_aIndexParent[MAX_PLAYER_PARTS];			//モデルの親
	static float	m_aOffset[MAX_PLAYER_PARTS][6];				//モデルの位置向きを設置
	float			m_aOldOffset[MAX_PLAYER_PARTS][6];			//モデルの位置向きを設置保存用
	float			m_aOffsetMemo[MAX_PLAYER_PARTS][6];			//保存用モデルの位置向きを設置

	//ワールドマトリックス関連
	LPDIRECT3DTEXTURE9	m_pTexture;		//テクスチャのポインタ
	LPD3DXMESH				m_pMesh;			//メッシュ
	LPD3DXBUFFER			m_pBuffMat;			//バッファ
	DWORD					m_nNumMat;			//マトリックス
	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	static D3DXVECTOR3		m_pos;				//位置
	static D3DXVECTOR3		m_rot;				//向き
	D3DXVECTOR3             m_rotDest;          //向きの慣性
	int						m_nNumVtx;			//頂点数
	float					m_fRot;				//回転
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_MaxPlayer;//最大値
	D3DXVECTOR3 m_MinPlayer;//最小値
	D3DXVECTOR3 m_posold;
	D3DXVECTOR3 m_posold2;
	int nCntPlayer;
	D3DXVECTOR3	m_RotDest;

	//モーション使用
	bool					m_Loop[MOTIONTYPE_MAX];
	int						m_NumKey[MOTIONTYPE_MAX];
	int						m_SetParent[MAX_PLAYER_PARTS];
	float					m_Player3D[MOTIONTYPE_MAX][10][MAX_PLAYER_PARTS][6];		//各モーションの位置向きを記憶場所
	static int				m_nFrame[MOTIONTYPE_MAX][10];								//各モーションのフレーム記憶
	static int				m_nFrameTotal[MOTIONTYPE_MAX];								//各モーションのフレーム記憶
	int			            m_Key;														//現在のモーションをカウント
	static int			    m_nCounterMotion;											//モーションのカウンター
	static int              m_GetKey;
	static int				m_KeySet[MOTIONTYPE_MAX];									//最大フレーム数を記憶
	static int       	    m_MotionType;
	int      				m_MotionTypeOld;
	int                     m_nMoveCnt;                                                 //モーション用カウント変数

	static int              m_nNumModel;                                                //モデルのパーツ数
	static int              m_nNumKeyKeep;                                              //モデルのキー保存用
	bool                    m_bLoopKeep;                                                //ループするかしないか保存用
	bool                    m_bMotionChange;                                            //モーション変化用
	int                     m_nBrendFrame;                                              //ブレンドモーションフレーム
	char                    m_pModelName[MAX_PLAYER_PARTS][128];                        //Xファイル読み込み用
	D3DXVECTOR3             m_posMemo;                                                  //保存用
	int                     m_nBrendCount;
	static MOTIONTYPE       m_MotionTypeMax;                                            //モーションの最大値
	bool                    m_bUse;
	int                     m_nCnt;                                                     //F2キーの処理
	static int              m_MotionChange;                                             //モーション切り替え
	int                     m_AddKey;                                                   //キー数を増やす
	int                     m_TotalKey;
	int                     m_nCntMat;                                                  //マテリアルの色変更カウント
	int                     m_nCntMatOld;                                               //マテリアルの昔のカウント保存用
	static char             *m_SelectData[MAX_PLAYER_PARTS + 1][256];                   //デバックログ表示用
	bool                    m_bData;                                                    //ログデータ用フラグ
	bool                    m_bData2;                                                   //ログデータ用フラグ
	static int              m_DataCount;                                                //カウントデータ
	int                     m_OldDataCount;                                             //カウントデータ保存用
	bool                    m_bMove;                                                    //移動するためのフラグ
};

#endif