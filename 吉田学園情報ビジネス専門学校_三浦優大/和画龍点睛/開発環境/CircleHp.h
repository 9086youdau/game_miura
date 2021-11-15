//*****************************************************************************
//
//�~��HP����[CircleHp.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _CIRCLEHP_H_
#define _CIRCLEHP_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�~��HP�̒�`
//*****************************************************************************
class CCircleHp : public CScene2D
{
public:
	//�񋓌^�i�G�̎�ށj
	typedef enum
	{
		CIRCLEHP_OUT = 0,
		CIRCLEHP_IN,
		ENEMYTYPE_MAX
	}CIRCLEHP;

	CCircleHp(int nPriority = OBJTYPE_SIRCLEHP);//�R���X�g���N�^
	~CCircleHp();//�f�X�g���N�^
	static CCircleHp *Create(D3DXVECTOR3 pos, float fHight, float fWidth, CIRCLEHP CircleHp);//�T�[�N��HP�̍쐬����

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp);//�T�[�N��HP�̏���������
	void Uninit(void);//�T�[�N��HP�̏I������
	void Update(void);//�T�[�N��HP�̍X�V����
	void Draw(void);//�T�[�N��HP�̕`�揈��

	static HRESULT Load(void);//�e�N�X�`�����[�h
	static void Unload(void);//�e�N�X�`���A�����[�h

	D3DXVECTOR3 GetAllPosition(void) { return D3DXVECTOR3(); }
	static void SetBossLife(int nDamage);//�{�X�̃��C�t�̃Z�b�g�֐�
	static int GetBossLife(void) { return m_sirclelife; }//�{�X�̃��C�t�̃Q�b�g�֐�

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//�e�N�X�`���̃����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//���_���̃����o�ϐ�
	D3DXVECTOR3 m_pos;//�ʒu���
	static int m_sirclelife;//HP�̃��C�t����
	D3DXVECTOR3 m_bosspos;//�{�X�̈ʒu����
	CIRCLEHP m_CircleHp;//HP�̃^�C�v�ϐ�
};
#endif
