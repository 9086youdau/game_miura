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

	CBullet(int nPriority = OBJTYPE_ENEMY_BULLET);//�e�̃R���X�g���N�^
	~CBullet();                                   //�e�̃f�X�g���N�^
	static CBullet *Create(D3DXVECTOR3 pos,BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,BULLETTYPEDAMAGE bulletbomb,float omega,float fHight, float fWidth, float fAngle);//�e�̐�������

	HRESULT Init(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb,float omega,float fAngle);//����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��

	void SetBullet(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType, BULLETTYPEDAMAGE bulletbomb, float omega,float fHight, float fWidth);//�e�̃Z�b�g����
	void CollisionBullet(void);//�����蔻�菈��

	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h

	void MoveLoopingBullet2(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//�{���̓���
	void MoveNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//�v���C���[�̒e�̓���
	void MoveEnemyNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//�G�̒e�̓���
	void MoveEnemySircleBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//�~�̒e�̓���
	void MoveEnemyLoopingBullet2(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);//�~�ŉ�]�̓���
	void MoveEnemyCountBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float & R, float VR,float angle);//�Q���^�̓���
	void MoveHomingBullet(float& x, float& y, float& vx, float& vy, float& rx, float& ry, float ex, float ey, float fAngle);//�ǔ��e�̓���


	static int GetLife(void) { return m_nLifeCount; }//���C�t�J�E���g�p
	static void SetLife(int Damage);                 //���C�t�J�E���g�Z�b�g
	static void SetLifeRecovery(int Recovery);       //���C�t����
	static bool GetPlayerAttack(void) { return m_playerattack; }//�A�^�b�N���鏊�̃t���O�Ǘ�

private:
	D3DXVECTOR3 m_move;//�e�̈ړ���
	D3DXVECTOR3 m_pos;//�e�̈ʒu
	D3DXVECTOR3 m_posBullet;//�e�̈ʒu
	D3DXVECTOR3 m_posplayerBullet;//�v���C���[�̒e�̈ʒu
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//���L�e�N�X�`���̃|�C���^
	int m_nLife;//�e�̗̑�
	static BULLETTYPE m_bulletplayerType;//�e�̎��
	static BULLETTYPE m_bulletenemyType;//�e�̃^�C�v
	BULLETTYPEDAMAGE m_bulletbomb;//�e�̃^�C�v
	float m_fAngle;//�p�x�i�[�ϐ�
	int m_nCountGraze;//�J�E���g�p
	static int m_nLifeCount;//���C�t�J�E���g
	D3DXVECTOR3 m_han;//�e�̔��a�ω�
	D3DXVECTOR3 m_han2;//�e�̔��a�ω�2
	float m_omega;//�J�E���g�p
	D3DXVECTOR3 m_vec;//�x�N�g���ۑ��p
	float m_fHight;//�����蔻��̍���
	float m_fWidth;//�����蔻��̉�
	D3DXVECTOR3 m_BombBullet;//�{���̈ʒu
	static bool m_playerattack;//�e�̑łƂ���̃t���O�Ǘ�
};
#endif


