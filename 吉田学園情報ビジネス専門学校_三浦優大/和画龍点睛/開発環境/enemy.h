//*****************************************************************************
//
//�G�l�~�[����[enemy.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�G�l�~�[�N���X�̒�`
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	//�񋓌^�i�G�̎�ށj
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

	//�񋓌^�i�G�_���[�W�̎�ށj
	typedef enum
	{
		ENEMY_DAMAGE_STATE_NONE = 0,
		ENEMY_DAMAGE_STATE_DAMAGE,
		ENEMY_DAMAGE_STATE_MAX
	}ENEMY_DAMAGE_STATE;

	CEnemy(int nPriority = OBJTYPE_ENEMY);//�R���X�g���N�^
	~CEnemy();//�f�X�g���N�^
	static CEnemy *Create(D3DXVECTOR3 pos, float fHight, float fWidth, ENEMYTYPE enemyType);//�G�̐�������

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType);       //����������
	void Uninit(void);                       //�I������
	void Update(void);						 //�X�V����
	void Draw(void);                         //�`�揈��

	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h

	void EnemyMove2(void);//�G�̓���
	void EnemyMove3(void);//�G�̓���
	void EnemyMove4(void);//�G�̓���
	void EnemyMove5(void);//�G�̓���
	void EnemyBossMove(void);//�G�̃{�X�̓���

	void SetEnemyState(ENEMY_DAMAGE_STATE damage,int nLifeDamage);//�G�̏�ԏ���
	void SetEnemyBossState(ENEMY_DAMAGE_STATE damage);//�{�X�̏�ԊǗ�
	int GetEnemy(void) { return m_nLife; }//���C�t�Ǘ�

private:
	static D3DXVECTOR3 m_pos;//�ʒu���
	D3DXVECTOR3 m_move;//�ړ���
	static LPDIRECT3DTEXTURE9 m_apTexture[ENEMYTYPE_MAX];				 //���L�e�N�X�`���̃|�C���^
	int m_nPatternAnim;									 //�p�^�[��No	
	float m_fAngle;//�p�x�ۑ��p
	float m_nCountParticle;//�G�̓����J�E���g
	float m_nEnemy;//�J�E���g�p
	ENEMYTYPE m_enemyType;//�G�l�~�[�̃^�C�v
	int m_nBulletCount;//�J�E���g�p
	float m_fRad;//�e�̃J�E���g�p
	D3DXVECTOR3 m_vec;//�x�N�g���p
	float m_homingAngle;//�z�[�~���O�p
	int m_BossCount;//�{�X�̃J�E���g�p
	int m_SircleCnt;//�{�X�̃J�E���g�p
	static ENEMY_DAMAGE_STATE m_EnemyState;//�G�l�~�[�̏�ԗp
	int m_nCountState;//��ԗp�J�E���g
	int m_nCountEnemy;//��ԗp�J�E���g
	int m_nLife;//�G�l�~�[�̗̑�
};
#endif
