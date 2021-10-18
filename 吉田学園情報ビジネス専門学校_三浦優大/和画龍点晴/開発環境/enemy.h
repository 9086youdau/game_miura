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
	CEnemy(int nPriority = OBJTYPE_ENEMY);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType);       //初期化処理
	void Uninit(void);                       //終了処理
	void Update(void);						 //更新処理
	void Draw(void);                         //描画処理
	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード
	static CEnemy *Create(D3DXVECTOR3 pos,float fHight, float fWidth,ENEMYTYPE enemyType);
	void EnemyMove2(void);//敵の動き2
	void EnemyMove3(void);//敵の動き3
	void EnemyMove4(void);//敵の動き4
	void EnemyMove5(void);//敵の動き4
	void EnemyBossMove(void);
	void SetEnemyState(ENEMY_DAMAGE_STATE damage,int nLifeDamage);
	void SetEnemyBossState(ENEMY_DAMAGE_STATE damage);
	int GetEnemy(void) { return m_nLife; }
private:
	static D3DXVECTOR3 m_pos;//位置情報
	D3DXVECTOR3 m_move;//移動量
	D3DXVECTOR3 m_enemymove;
	static LPDIRECT3DTEXTURE9 m_apTexture[ENEMYTYPE_MAX];				 //共有テクスチャのポインタ
	int m_nCounterAnim;									 //カウンター
	int m_nPatternAnim;									 //パターンNo	
	int m_nCnt;
	float m_fAngle;
	float m_nCountParticle;
	float m_nEnemy;
	ENEMYTYPE m_enemyType;
	float m_nEnemyCnt;
	int m_nBulletCount;
	float m_fRad;
	D3DXVECTOR3 m_vec;
	float m_homingAngle;
	int m_BossCount;
	int m_SircleCnt;
	static ENEMY_DAMAGE_STATE m_EnemyState;
	int m_nCountState;
	int m_nCountEnemy;
	int m_nLife;
};
#endif
