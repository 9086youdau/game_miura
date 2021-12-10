//*****************************************************************************
//
//レーザー処理[laser.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _LASER_H_
#define _LASER_H_

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
class CLaser : public CScene2D
{
public:
	//列挙型（弾の種類）
	typedef enum
	{
		LASERTYPE_NONE = 0,
		LASERTYPE_ENEMY,
		LASERTYPE_MAX
	}LASERTYPE;

	//列挙型（弾の種類）
	typedef enum
	{
		LASERTYPEDAMAGE_NONE = 0,
		LASERTYPEDAMAGE_ENEMYNOMALSHOT,
		LASERTYPEDAMAGE_ENEMYNOTICE,
		LASERTYPEDAMAGE_MAX
	}LASERTYPEDAMAGE;

	CLaser(int nPriority = OBJTYPE_ENEMY_LASER);
	~CLaser();
	static CLaser *Create(D3DXVECTOR3 pos, LASERTYPE laserType, LASERTYPEDAMAGE laserdamage, float omega, float fHight, float fWidth, float fAngle, D3DXVECTOR3 rot,int nLife);

	HRESULT Init(D3DXVECTOR3 pos, LASERTYPE laserType, float fHight, float fWidth, LASERTYPEDAMAGE laserdamage, float omega, float fAngle, D3DXVECTOR3 rot,int nLife);//初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理

	void SetLaser(D3DXVECTOR3 pos, LASERTYPE laserType, LASERTYPEDAMAGE laserdamage, float omega, float fHight, float fWidth,D3DXVECTOR3 rot,int nLife);//弾の移動処理
	void CollisionLaser(void);							 //弾の移動処理

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

	void MoveEnemyNomalLaser(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);

	static bool GetPlayerAttack(void) { return m_Laserplayerattack; }//アタックする所のフラグ管理
	static void SetPlayerAttack(bool bUse);

private:
	D3DXVECTOR3 m_move;//弾の移動量
	D3DXVECTOR3 m_pos;//弾の位置
	D3DXVECTOR3 m_posLaser;//弾の位置
	static LPDIRECT3DTEXTURE9 m_pTexture;//共有テクスチャのポインタ
	int m_nLife;//弾の体力
	static LASERTYPE m_laserType;//弾の種類
	LASERTYPEDAMAGE m_laserbomb;
	float m_fAngle;
	int m_nCountGraze;//カウント用
	static CEnemy *m_pEnemy;
	static int m_nLifeCount;
	int m_nBomb;
	D3DXVECTOR3 m_han;
	float m_omega;
	D3DXVECTOR3 m_vec;
	float m_fHight;
	float m_fWidth;
	float m_CountSircle;
	D3DXVECTOR3 m_Rot;
	static bool m_Laserplayerattack;
};
#endif

