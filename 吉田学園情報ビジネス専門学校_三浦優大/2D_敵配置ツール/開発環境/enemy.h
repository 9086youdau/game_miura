//*****************************************************************************
//
//エネミー処理[enemy.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//エネミークラスの定義
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	//列挙型（敵の種類）
	typedef enum
	{
		ENEMYTYPE_NONE = 0,
		ENEMYTYPE_ONE,
		ENEMYTYPE_TWO,
		ENEMYTYPE_THREE,
		ENEMYTYPE_FOR,
		ENEMYTYPE_BOSS,
		ENEMYTYPE_MAX
	}ENEMYTYPE;

	//列挙型（敵ダメージの種類）
	typedef enum
	{
		ENEMY_DAMAGE_STATE_NONE = 0,
		ENEMY_DAMAGE_STATE_DAMAGE,
		ENEMY_DAMAGE_STATE_MAX
	}ENEMY_DAMAGE_STATE;

	CEnemy(int nPriority = OBJTYPE_ENEMY);    //コンストラクタ
	~CEnemy();                                //デストラクタ
	static CEnemy *Create(D3DXVECTOR3 pos, float fHight, float fWidth, ENEMYTYPE enemyType,float fMove, int nHoming,int nLife,int nDamage);//敵の生成処理

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType,float fMove, int nHoming, int nLife, int nDamage);       //初期化処理
	void Uninit(void);                       //終了処理
	void Update(void);						 //更新処理
	void Draw(void);                         //描画処理

	static HRESULT Load();					 //テクスチャのロード
	static void Unload(void);				 //テクスチャのアンロード

	void EnemyMoveOne(void);                 //敵の動き1
	void EnemyMoveTwo(void);                 //敵の動き2
	void EnemyMoveThree(void);               //敵の動き3
	void EnemyMoveFor(void);                 //敵の動き4
	void EnemyBossMove(void);                //敵のボスの動き

	void SetEnemyState(ENEMY_DAMAGE_STATE damage); //敵のダメージ計算処理

private:
	static D3DXVECTOR3 m_pos;                                  //位置情報
	D3DXVECTOR3 m_move;//移動量
	static LPDIRECT3DTEXTURE9 m_apTexture[ENEMYTYPE_MAX];	   //共有テクスチャのポインタ
	int m_nPatternAnim;									       //パターンNo	
	float m_fAngle;                                            //角度保存用
	float m_nCountParticle;                                    //敵の動きカウント
	float m_nEnemy;                                            //カウント用
	ENEMYTYPE m_enemyType;                                     //エネミーのタイプ
	int m_nBulletCount;                                        //カウント用
	float m_fRad;                                              //弾のカウント用
	D3DXVECTOR3 m_vec;                                         //ベクトル用
	float m_homingAngle;                                       //ホーミング用
	int m_BossCount;                                           //ボスのカウント用
	int m_SircleCnt;                                           //ボスのカウント用
	static ENEMY_DAMAGE_STATE m_EnemyState;                    //エネミーの状態用
	int m_nCountState;                                         //状態用カウント
	int m_nCountEnemy;                                         //状態用カウント
	int m_nLife;                                               //エネミーの体力
	float m_fMove;                                             //エネミーの移動量計算
	int m_HomingCount;                                         //エネミーのホーミング時間
	int m_HomingText;                                          //テキストから読み込まれたカウント時間
	int m_nLifeDamage;                                         //自機の弾のダメージ数
};
#endif
