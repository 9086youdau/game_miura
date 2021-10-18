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
	CEnemy(int nPriority = OBJTYPE_ENEMY);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType);       //����������
	void Uninit(void);                       //�I������
	void Update(void);						 //�X�V����
	void Draw(void);                         //�`�揈��
	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h
	static CEnemy *Create(D3DXVECTOR3 pos,float fHight, float fWidth,ENEMYTYPE enemyType);
	void EnemyMove2(void);//�G�̓���2
	void EnemyMove3(void);//�G�̓���3
	void EnemyMove4(void);//�G�̓���4
	void EnemyMove5(void);//�G�̓���4
	void EnemyBossMove(void);
	void SetEnemyState(ENEMY_DAMAGE_STATE damage,int nLifeDamage);
	void SetEnemyBossState(ENEMY_DAMAGE_STATE damage);
	int GetEnemy(void) { return m_nLife; }
private:
	static D3DXVECTOR3 m_pos;//�ʒu���
	D3DXVECTOR3 m_move;//�ړ���
	D3DXVECTOR3 m_enemymove;
	static LPDIRECT3DTEXTURE9 m_apTexture[ENEMYTYPE_MAX];				 //���L�e�N�X�`���̃|�C���^
	int m_nCounterAnim;									 //�J�E���^�[
	int m_nPatternAnim;									 //�p�^�[��No	
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
