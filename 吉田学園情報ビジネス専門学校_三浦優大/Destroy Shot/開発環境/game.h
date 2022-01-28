//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "passerby.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CObject3D;
class CPlayer3D;
class CHp;
class CSound;

//=============================================================================
//マクロ定義
//=============================================================================
#define TEXT_LENGTH (128)
#define DATA_GAME_LENGTH (248)
#define MAX_DATA (224)

//=============================================================================
// ゲーム画面クラスの定義
//=============================================================================
//継承先描画クラス
class CGame : public CScene
{
public:
	CGame();									//コンストラクタ
	~CGame();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理

	static CObject3D *GetObject(int nCnt) { return m_pObject3D[nCnt]; }
	static CPlayer3D *GetPlayer(void) { return m_pPlayer3D; }
	static CHp *GetHp(int nCnt) { return m_pHp[nCnt]; }

	void LoadEnemy(void);                                       //敵のテキスト読み込み
	void LoadObject(void);                                      //オブジェクトの位置読み込み
	void LoadParticle(void);                                    //パーティクル情報を読み込み

	//生成処理
	static CGame *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	static int GetEnemyPurpose(void) { return m_nCountEnemyText; }
	static void SetCount(int nCnt);
	
	static void GameParticle(D3DXVECTOR3 pos,int nType);
	static int GetLife(void) { return m_nHpCount; }
	static void SetLife(int nLife);

private:
	LPD3DXFONT	m_pTimeFontGame;					//表示用フォント
	static CObject3D *m_pObject3D[256];				//時間Get
	static CPlayer3D *m_pPlayer3D;				    //プレイヤーのポインタ情報
	static CHp *m_pHp[4];                           //HPのポインタ情報
	static float m_fAngle;                          //パーティクルの角度変更
	static float m_fWidth;                          //パーティクルの横幅
	static float m_fHight;                          //パーティクルの縦幅
	static float m_fParicleMove[DATA_GAME_LENGTH];       //パーティクルの速さ
	static D3DXCOLOR m_col[DATA_GAME_LENGTH];            //パーティクルの色
	int m_nLife[DATA_GAME_LENGTH];                       //パーティクルの寿命
	static int m_nCnt;                              //エネミーのカウント変数

	//敵の配置用テキストメンバ変数
	static int m_nCountEnemyText;                        //何個読み込んだかのカウント
	int m_nStartEnemyText[DATA_GAME_LENGTH];                  //いつ出現させるかのカウント
	D3DXVECTOR3 m_EnemyPos[DATA_GAME_LENGTH];                 //敵の位置情報
	D3DXVECTOR3 m_EnemySize[DATA_GAME_LENGTH];                //敵のサイズ情報

	//オブジェクト配置用メンバ変数
	int m_nCountObjectText;                         //何個読み込んだかのカウント
	D3DXVECTOR3 m_ObjectPos[DATA_GAME_LENGTH];           //オブジェクトの位置情報
	D3DXVECTOR3 m_ObjectRot[DATA_GAME_LENGTH];           //オブジェクトの位置情報
	int m_nObjectCount;                             //オブジェクトの数                          
	char m_sObjectName[30][128];                    //オブジェクトのXファイル読み込み
	char m_sTextureName[30][128];                   //オブジェクトのテクスチャ読み込み
	int m_nModelType[DATA_GAME_LENGTH];                  //モデルのタイプ
	int m_nTextureType[DATA_GAME_LENGTH];                //モデルのテクスチャタイプ

	//パーティクル用
	static int m_nGParicleMax[DATA_GAME_LENGTH];               //パーティクルの発生数
	int m_nGParicleText;								  //何個読み込んだかのカウント
	int m_nGCountText;									  //パーティクルの数
	static float m_GSizeX[DATA_GAME_LENGTH];                   //パーティクルのサイズ
	static float m_GSizeY[DATA_GAME_LENGTH];                   //パーティクルのサイズ
	static float m_fGSpeed[DATA_GAME_LENGTH];                  //パーティクルのスピード
	static D3DXCOLOR m_GParticlecol[DATA_GAME_LENGTH];         //パーティクルのカラー
	static int m_nGLife[DATA_GAME_LENGTH];                     //パーティクルの寿命
	static float m_fGGravity[DATA_GAME_LENGTH];                //パーティクルの重力

	static int m_nHpCount;                                //プレイヤーのHPカウント
	CSound		*m_pSound;								//サウンドポインタ
};

#endif