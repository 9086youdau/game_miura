//*****************************************************************************
//
//���[�U�[����[laser.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _LASER_H_
#define _LASER_H_

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
class CLaser : public CScene2D
{
public:
	//�񋓌^�i�e�̎�ށj
	typedef enum
	{
		LASERTYPE_NONE = 0,
		LASERTYPE_ENEMY,
		LASERTYPE_MAX
	}LASERTYPE;

	//�񋓌^�i�e�̎�ށj
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

	HRESULT Init(D3DXVECTOR3 pos, LASERTYPE laserType, float fHight, float fWidth, LASERTYPEDAMAGE laserdamage, float omega, float fAngle, D3DXVECTOR3 rot,int nLife);//����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��

	void SetLaser(D3DXVECTOR3 pos, LASERTYPE laserType, LASERTYPEDAMAGE laserdamage, float omega, float fHight, float fWidth,D3DXVECTOR3 rot,int nLife);//�e�̈ړ�����
	void CollisionLaser(void);							 //�e�̈ړ�����

	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h

	void MoveEnemyNomalLaser(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega);

	static bool GetPlayerAttack(void) { return m_Laserplayerattack; }//�A�^�b�N���鏊�̃t���O�Ǘ�
	static void SetPlayerAttack(bool bUse);

private:
	D3DXVECTOR3 m_move;//�e�̈ړ���
	D3DXVECTOR3 m_pos;//�e�̈ʒu
	D3DXVECTOR3 m_posLaser;//�e�̈ʒu
	static LPDIRECT3DTEXTURE9 m_pTexture;//���L�e�N�X�`���̃|�C���^
	int m_nLife;//�e�̗̑�
	static LASERTYPE m_laserType;//�e�̎��
	LASERTYPEDAMAGE m_laserbomb;
	float m_fAngle;
	int m_nCountGraze;//�J�E���g�p
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

