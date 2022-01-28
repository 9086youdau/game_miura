//=============================================================================
//
// 通行人処理 [passerby.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _PASSERBY_H_
#define _PASSERBY_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "main.h"
#include "scene.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PASSERBY_PARTS (30)	// モデルパーツの最大数
#define TEXT_LENGTH		(128)	// テキスト読み込みの最大数
#define DATA_LENGTH		(64)	// データ読み込みの最大数
#define MAX_PASSERBY_PARTS (13)

//=============================================================================
// 前方宣言
//=============================================================================
class CSound;
class CHp;
class CModel;

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_PASSERBY_NEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_PASSERBY_MOVE,		// 移動モーション
	MOTIONTYPE_PASSERBY_LEFTMOVE,	// 左移動モーション
	MOTIONTYPE_PASSERBY_RIGHTMOVE,	// 右移動モーション
	MOTIONTYPE_PASSERBY_JUMP,		// ジャンプモーション
	MOTIONTYPE_PASSERBY_MAX			// モーションの最大数
} MOTIONTYPE_PASSERBY;

//==============================================================================
// 歩行者の種類
//==============================================================================
typedef enum
{
	PASSERBY_TYPE_NEUTRAL = 0,  // 何もなし
	PASSERBY_TYPE_MEN,		    // 男の人
	PASSERBY_TYPE_WOMEN,	    // 女の人
	PASSERBY_TYPE_CAT,	        // 猫
	PASSERBY_TYPE_MAX			// モーションの最大数
} PASSERBY_TYPE;

//=============================================================================
// 歩行者クラスの定義
//=============================================================================
//継承先描画クラス
class CPasserby : public CScene
{
public:

	CPasserby(int nPriority = OBJTYPE_ENEMY, int nType = 0);								//コンストラクタ
	virtual ~CPasserby();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//初期化処理
	void Uninit(void);															//終了処理
	void Update(void);															//更新処理
	void Draw(void);															//描画処理

	static CPasserby *Create(D3DXVECTOR3 pos,PASSERBY_TYPE passerbytype,int nType);//生成処理
	
	void LoadMotion(PASSERBY_TYPE passerbytype);														//外部読み込み
	void UpdateMotion(MOTIONTYPE_PASSERBY passerby);
	void PasserbyMove(void);
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	void CollisionPlayer(void);
	D3DXVECTOR3 GetPasserbyPos(void) { return m_pos; }

private:
	CModel			*m_apModel[MODEL_PASSERBY_PARTS];				//モデルへのポインタ
	char			*m_pTextName[MODEL_PASSERBY_PARTS];				//名前保存
	int				m_aIndexParent[MODEL_PASSERBY_PARTS];			//モデルの親
	float			m_aOffset[MODEL_PASSERBY_PARTS][6];				//モデルの位置向きを設置
	float			m_aOffsetMemo[MODEL_PASSERBY_PARTS][6];				//保存用モデルの位置向きを設置

	//ワールドマトリックス関連
	LPDIRECT3DTEXTURE9	m_pTexture;		//テクスチャのポインタ
	LPD3DXMESH				m_pMesh;			//メッシュ
	LPD3DXBUFFER			m_pBuffMat;			//バッファ
	DWORD					m_nNumMat;			//マトリックス
	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		m_pos;				        //位置
	D3DXVECTOR3     m_poskeep;                  //位置を記憶する変数
	static D3DXVECTOR3   m_posHoming;           //可視化を追尾する
	D3DXVECTOR3		m_rot;				//向き
	D3DXVECTOR3             m_rotDest;          //向きの慣性
	int						m_nNumVtx;			//頂点数
	float					m_fRot;				//回転
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_MaxPasserby;//最大値
	D3DXVECTOR3 m_MinPasserby;//最小値
	D3DXVECTOR3 m_posold;
	D3DXVECTOR3 m_posold2;
	int nCntPlayer;
	D3DXVECTOR3	m_RotDest;
	D3DXVECTOR3 m_startpos;

	//モーション使用
	bool					m_Loop[MOTIONTYPE_PASSERBY_MAX];
	int						m_NumKey[MOTIONTYPE_PASSERBY_MAX];
	int						m_SetParent[MODEL_PASSERBY_PARTS];
	float					m_Passerby[MOTIONTYPE_PASSERBY_MAX][10][MODEL_PASSERBY_PARTS][6];		//各モーションの位置向きを記憶場所
	int						m_nFrame[MOTIONTYPE_PASSERBY_MAX][10];								//各モーションのフレーム記憶
	int						m_nFrameTotal[MOTIONTYPE_PASSERBY_MAX];								//各モーションのフレーム記憶
	int						m_Key;														//現在のモーションをカウント
	float					m_fCounterMotion;											//モーションのカウンター
	int						m_KeySet[MOTIONTYPE_PASSERBY_MAX];									//最大フレーム数を記憶
	MOTIONTYPE_PASSERBY		m_MotionType;
	MOTIONTYPE_PASSERBY		m_MotionTypeOld;
	int                     m_nMoveCnt;                                                 //モーション用カウント変数

	int                     m_nNumModel;                                                //モデルのパーツ数
	int                     m_nNumKeyKeep;                                              //モデルのキー保存用
	bool                    m_bLoopKeep;                                                //ループするかしないか保存用
	bool                    m_bMotionChange;                                            //モーション変化用
	int                     m_nBrendFrame;                                              //ブレンドモーションフレーム
	char                    m_pModelName[MODEL_PASSERBY_PARTS][128];                      //Xファイル読み込み用
	D3DXVECTOR3             m_posMemo;                                                  //保存用
	bool                    m_bGoal[5];                                                 //目的地判定用
	PASSERBY_TYPE           m_passerbytype;                                             //歩行者の種類
	int                     m_nPasserbyCount;                                           //歩行者の別々の動き変数
	int                     m_nRamdom[2];                                               //ランダムを可視化する
	D3DXVECTOR3          	m_aPos[10][4];                                              //当たり判定の外積用
	bool                    m_fUseScene3D;
	bool                    m_EnemyMove;

	//当たり判定用メンバ変数
	D3DXVECTOR3 m_aPosColision[4];
	D3DXVECTOR3 m_aVec[4];
	D3DXVECTOR3 m_bVec[4];
	float m_fPlayerVec[4];

	CHp *m_pHp[4];

	bool m_bParticle;
	static bool m_bParticle2;
	bool m_bParticle3;
	bool m_bReset;
	int m_nParticlecount;
	CSound		*m_pSound;								//サウンドポインタ
};

#endif
