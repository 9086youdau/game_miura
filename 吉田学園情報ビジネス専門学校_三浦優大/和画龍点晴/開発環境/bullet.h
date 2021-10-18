//*****************************************************************************
//
//�o���b�g����[bullet.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CEnemy;

//*****************************************************************************
//�o���b�g�N���X�̒�`
//*****************************************************************************
class CBullet : public CScene2D
{
public:
	//�񋓌^�i�e�̎�ށj
	typedef enum
	{
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX
	}BULLETTYPE;
	//�񋓌^�i�e�̎�ށj
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
	HRESULT Init(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb,float omega,float fAngle);//����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��
	void SetBullet(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType, BULLETTYPEDAMAGE bulletbomb, float omega,float fHight, float fWidth);//�e�̈ړ�����
	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h
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
	D3DXVECTOR3 m_move;//�e�̈ړ���
	D3DXVECTOR3 m_pos;//�e�̈ʒu
	D3DXVECTOR3 m_posBullet;//�e�̈ʒu
	D3DXVECTOR3 m_posplayerBullet;
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//���L�e�N�X�`���̃|�C���^
	int m_nLife;//�e�̗̑�
	static BULLETTYPE m_bulletplayerType;//�e�̎��
	static BULLETTYPE m_bulletenemyType;
	BULLETTYPEDAMAGE m_bulletbomb;
	float m_fAngle;
	int m_nCountGraze;//�J�E���g�p
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


