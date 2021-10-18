//*****************************************************************************
//
//バレット処理[bullet.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CEnemy;

//*****************************************************************************
//バレットクラスの定義
//*****************************************************************************
class CBullet : public CScene2D
{
public:
	//列挙型（弾の種類）
	typedef enum
	{
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX
	}BULLETTYPE;
	//列挙型（弾の種類）
	typedef enum
	{
		BULLETTYPEDAMAGE_NONE = 0,
		BULLETTYPEDAMAGE_NOMALSHOT,
		BULLETTYPEDAMAGE_BOMB,
		BULLETTYPEDAMAGE_ENEMYNOMALSHOT,
		BULEETTYPEDAMAGE_SIRCLESHOT,
		BULLETTYPEDAMAGE_ENEMYMOVESIRCLE,
		BULLETTYPEDAMAGE_ENEMYCOUNT,
		BULLETTYPEDAMAGE_ENEMYHOMING,
		BULLETTYPEDAMAGE_MAX
	}BULLETTYPEDAMAGE;
	CBullet(int nPriority = OBJTYPE_ENEMY_BULLET);
	~CBullet();
	static CBullet *Create(D3DXVECTOR3 pos,BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,BULLETTYPEDAMAGE bulletbomb,float omega,float fHight, float fWidth, float fAngle);
	HRESULT Init(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb,float omega,float fAngle);//初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理
	void SetBullet(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType, BULLETTYPEDAMAGE bulletbomb, float omega,float fHight, float fWidth);//弾の移動処理
	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード
	void MoveLoopingBullet2(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);
	void MoveNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);
	void MoveEnemyNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);
	void MoveEnemySircleBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);
	void MoveEnemyLoopingBullet2(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);
	void MoveEnemyCountBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float & R, float VR,float angle);
	void MoveHomingBullet(float& x, float& y, float& vx, float& vy, float& rx, float& ry, float ex, float ey, float fAngle);
	static int GetLife(void) { return m_nLifeCount; }
	static void SetLife(int Damage);
	static void SetLifeRecovery(int Recovery);
	static bool GetPlayerAttack(void) { return m_playerattack; }
private:
	D3DXVECTOR3 m_move;//弾の移動量
	D3DXVECTOR3 m_pos;//弾の位置
	D3DXVECTOR3 m_posBullet;//弾の位置
	D3DXVECTOR3 m_posplayerBullet;
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//共有テクスチャのポインタ
	int m_nLife;//弾の体力
	static BULLETTYPE m_bulletplayerType;//弾の種類
	static BULLETTYPE m_bulletenemyType;
	BULLETTYPEDAMAGE m_bulletbomb;
	float m_fAngle;
	int m_nCountGraze;//カウント用
	static CEnemy *m_pEnemy;
	static int m_nLifeCount;
	int m_nBomb;
	D3DXVECTOR3 m_han;
	D3DXVECTOR3 m_han2;
	float m_omega;
	D3DXVECTOR3 m_vec;
	float m_fHight;
	float m_fWidth;
	float m_CountSircle;
	float m_vecangle;
	D3DXVECTOR3 m_BombBullet;
	static bool m_playerattack;
};
#endif


