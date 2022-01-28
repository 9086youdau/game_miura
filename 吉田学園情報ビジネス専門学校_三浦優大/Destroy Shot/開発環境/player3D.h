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

//==============================================================================
// マクロ定義
//==============================================================================
#define MODEL_PARTS		(30)	// モデルパーツの最大数
#define TEXT_LENGTH		(128)	// テキスト読み込みの最大数
#define DATA_LENGTH		(64)	// データ読み込みの最大数
#define MAX_PLAYER_PARTS (17)   // モデルのパーツ数
#define MAX_MOTION       (20)   // モーションの最大数

//==============================================================================
// 前方宣言
//==============================================================================
class CHp;
class CPasserby;
class CSound;
class CBullet;
class CModel;

//==============================================================================
// モーションの種類
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// ニュートラルモーション
	MOTIONTYPE_MOVE,		// 移動モーション
	MOTIONTYPE_SHOTMOVE,	// 撃つモーション
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
	void LoadMotion(void);														//外部読み込み                                                   //外部書き込み
	void UpdateMotion(int MOTIONTYPE,bool bUse);
	void PlayerMove(void);                                                      //プレイヤーの移動処理
	void CollisionPlayer(void);                                                 //プレイヤーの当たり判定

	static D3DXVECTOR3	GetPosition(void);						//位置取得用
	static D3DXVECTOR3	GetRotation(void);						//向き取得用

	static CBullet *GetBullet (void) { return m_pBullet; }
	static bool GetParticle(void) {return m_bReset;}

private:
	static CModel   *m_apModel[MODEL_PARTS];				//モデルへのポインタ
	char			*m_pTextName[MODEL_PARTS];				//名前保存
	int				m_aIndexParent[MODEL_PARTS];			//モデルの親
	float	m_aOffset[MODEL_PARTS][6];				//モデルの位置向きを設置
	float	m_aOffsetWrite[10][MODEL_PARTS][6];		//書き込み用の位置向きを設定用
	float			m_aOldOffset[MODEL_PARTS][6];			//モデルの位置向きを設置保存用
	float			m_aOffsetMemo[MODEL_PARTS][6];			//保存用モデルの位置向きを設置

	//ワールドマトリックス関連
	LPDIRECT3DTEXTURE9	m_pTexture;		//テクスチャのポインタ
	LPD3DXMESH				m_pMesh;			//メッシュ
	LPD3DXBUFFER			m_pBuffMat;			//バッファ
	DWORD					m_nNumMat;			//マトリックス
	D3DXMATRIX				m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		        m_pos;				//位置
	static D3DXVECTOR3		m_getpos;				//受け渡し用位置情報
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
	bool					m_Loop[MAX_MOTION];
	int						m_NumKey[MAX_MOTION];
	int						m_SetParent[MODEL_PARTS];
	float					m_Player3D[MAX_MOTION][10][MODEL_PARTS][6];		//各モーションの位置向きを記憶場所
	int				m_nFrame[MAX_MOTION][10];								//各モーションのフレーム記憶
	int				m_nFrameTotal[MAX_MOTION];								//各モーションのフレーム記憶
	int			            m_Key;														//現在のモーションをカウント
	int			    m_nCounterMotion;											//モーションのカウンター
	int              m_GetKey;
	int				m_KeySet[MAX_MOTION];									//最大フレーム数を記憶
	int       	    m_MotionType;
	int      				m_MotionTypeOld;
	int                     m_nMoveCnt;                                                 //モーション用カウント変数

	int              m_nNumModel;                                                //モデルのパーツ数
	int              m_nNumKeyKeep;                                              //モデルのキー保存用
	bool                    m_bLoopKeep;                                                //ループするかしないか保存用
	bool                    m_bMotionChange;                                            //モーション変化用
	int                     m_nBrendFrame;                                              //ブレンドモーションフレーム
	char                    m_pModelName[MODEL_PARTS][128];                        //Xファイル読み込み用
	D3DXVECTOR3             m_posMemo;                                                  //保存用
	int                     m_nBrendCount;
	int              m_MotionTypeMax;                                            //モーションの最大値
	bool                    m_bUse;
	int                     m_nCnt;                                                     //F2キーの処理
	int              m_MotionChange;                                             //モーション切り替え
	int                     m_AddKey;                                                   //キー数を増やす
	int                     m_TotalKey;
	int                     m_nCntMat;                                                  //マテリアルの色変更カウント
	int                     m_nCntMatOld;                                               //マテリアルの昔のカウント保存用
	char             *m_SelectData[MODEL_PARTS + 1][256];                   //デバックログ表示用
	bool                    m_bData;                                                    //ログデータ用フラグ
	bool                    m_bData2;                                                   //ログデータ用フラグ
	int                     m_DataCount;                                                //カウントデータ
	int                     m_OldDataCount;                                             //カウントデータ保存用
	bool                    m_bMove;                                                    //移動するためのフラグ
	int                     m_nCounter;
	bool                    m_nNextKey;
	int                     m_MotionMax;
	bool                    m_bColision;
	static CBullet *m_pBullet;

	//当たり判定用メンバ変数
	D3DXVECTOR3 m_aPosColision[4];//当たり判定の矩形用
	D3DXVECTOR3 m_aVec[4];        //ベクトル用
	D3DXVECTOR3 m_bVec[4];        //ベクトル用2
	float m_fPlayerVec[4];        //プレイヤーのベクトル保存用

	CHp *m_pHp[4];                //HP用ポインタ変数
	CPasserby *m_pPasserby;       //敵のポインタ保存用
	int m_nHpCount;               //HPのカウント用
	static bool m_bReset;         //プレイヤーが死んだときのフラグ
	int m_nResetCount;            //プレイヤーが死んだときのカウント
	int m_nBulletCount;           //弾の発射速度
	int m_nParticlecount;         //パーティクルが発生するカウント   
	bool m_bParticle;             //パーティクルが発生するフラグ
	CSound		*m_pSound;		  //サウンドポインタ
};

#endif