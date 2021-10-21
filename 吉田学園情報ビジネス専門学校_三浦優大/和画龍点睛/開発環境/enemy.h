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

	CEnemy(int nPriority = OBJTYPE_ENEMY);//コンストラクタ
	~CEnemy();//デストラクタ
	static CEnemy *Create(D3DXVECTOR3 pos, float fHight, float fWidth, ENEMYTYPE enemyType);//敵の生成処理

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType);       //初期化処理
	void Uninit(void);                       //終了処理
	void Update(void);						 //更新処理
	void Draw(void);                         //描画処理

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

	void EnemyMove2(void);//敵の動き
	void EnemyMove3(void);//敵の動き
	void EnemyMove4(void);//敵の動き
	void EnemyMove5(void);//敵の動き
	void EnemyBossMove(void);//敵のボスの動き

	void SetEnemyState(ENEMY_DAMAGE_STATE damage,int nLifeDamage);//敵の状態処理
	void SetEnemyBossState(ENEMY_DAMAGE_STATE damage);//ボスの状態管理
	int GetEnemy(void) { return m_nLife; }//ライフ管理

private:
	static D3DXVECTOR3 m_pos;//位置情報
	D3DXVECTOR3 m_move;//移動量
	static LPDIRECT3DTEXTURE9 m_apTexture[ENEMYTYPE_MAX];				 //共有テクスチャのポインタ
	int m_nPatternAnim;									 //パターンNo	
	float m_fAngle;//角度保存用
	float m_nCountParticle;//敵の動きカウント
	float m_nEnemy;//カウント用
	ENEMYTYPE m_enemyType;//エネミーのタイプ
	int m_nBulletCount;//カウント用
	float m_fRad;//弾のカウント用
	D3DXVECTOR3 m_vec;//ベクトル用
	float m_homingAngle;//ホーミング用
	int m_BossCount;//ボスのカウント用
	int m_SircleCnt;//ボスのカウント用
	static ENEMY_DAMAGE_STATE m_EnemyState;//エネミーの状態用
	int m_nCountState;//状態用カウント
	int m_nCountEnemy;//状態用カウント
	int m_nLife;//エネミーの体力
};
#endif
