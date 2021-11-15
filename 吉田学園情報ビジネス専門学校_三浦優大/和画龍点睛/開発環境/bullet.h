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

	CBullet(int nPriority = OBJTYPE_ENEMY_BULLET);//弾のコンストラクタ
	~CBullet();                                   //弾のデストラクタ
	static CBullet *Create(D3DXVECTOR3 pos,BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,BULLETTYPEDAMAGE bulletbomb,float omega,float fHight, float fWidth, float fAngle);//弾の生成処理

	HRESULT Init(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb,float omega,float fAngle);//初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理

	void SetBullet(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType, BULLETTYPEDAMAGE bulletbomb, float omega,float fHight, float fWidth);//弾のセット処理
	void CollisionBullet(void);//当たり判定処理

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

	void MoveLoopingBullet2(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//ボムの動き
	void MoveNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//プレイヤーの弾の動き
	void MoveEnemyNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//敵の弾の動き
	void MoveEnemySircleBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//円の弾の動き
	void MoveEnemyLoopingBullet2(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//円で回転の動き
	void MoveEnemyCountBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float & R, float VR,float angle);//渦巻型の動き
	void MoveHomingBullet(float& x, float& y, float& vx, float& vy, float& rx, float& ry, float ex, float ey, float fAngle);//追尾弾の動き


	static int GetLife(void) { return m_nLifeCount; }//ライフカウント用
	static void SetLife(int Damage);                 //ライフカウントセット
	static void SetLifeRecovery(int Recovery);       //ライフ処理
	static bool GetPlayerAttack(void) { return m_playerattack; }//アタックする所のフラグ管理

private:
	D3DXVECTOR3 m_move;//弾の移動量
	D3DXVECTOR3 m_pos;//弾の位置
	D3DXVECTOR3 m_posBullet;//弾の位置
	D3DXVECTOR3 m_posplayerBullet;//プレイヤーの弾の位置
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//共有テクスチャのポインタ
	int m_nLife;//弾の体力
	static BULLETTYPE m_bulletplayerType;//弾の種類
	static BULLETTYPE m_bulletenemyType;//弾のタイプ
	BULLETTYPEDAMAGE m_bulletbomb;//弾のタイプ
	float m_fAngle;//角度格納変数
	int m_nCountGraze;//カウント用
	static int m_nLifeCount;//ライフカウント
	D3DXVECTOR3 m_han;//弾の半径変化
	D3DXVECTOR3 m_han2;//弾の半径変化2
	float m_omega;//カウント用
	D3DXVECTOR3 m_vec;//ベクトル保存用
	float m_fHight;//当たり判定の高さ
	float m_fWidth;//当たり判定の横
	D3DXVECTOR3 m_BombBullet;//ボムの位置
	static bool m_playerattack;//弾の打つところのフラグ管理
};
#endif


