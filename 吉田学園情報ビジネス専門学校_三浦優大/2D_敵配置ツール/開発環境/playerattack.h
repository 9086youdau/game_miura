//*****************************************************************************
//
//�v���C���[�A�^�b�N����[playerattack.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PLAYERATTACK_H_
#define _PLAYERATTACK_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�v���C���[�A�^�b�N�N���X�̒�`
//*****************************************************************************
class CPlayerAttack : public CScene2D
{
public:
	//�񋓌^�i�v���C���[�A�^�b�N�̍��E�j
	typedef enum
	{
		PLAYERATTACK_LEFT = 0,
		PLAYERATTACK_RIGHT,
		PLAYERATTACK_MAX
	}PLAYERATTACK;

	CPlayerAttack(int nPriority = OBJTYPE_PLAYER_ATTACK); //�R���X�g���N�^
	~CPlayerAttack();                                     //�f�X�g���N�^
	static CPlayerAttack *Create(D3DXVECTOR3 pos,PLAYERATTACK playerattack);             //��������

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,PLAYERATTACK playerattack); //����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��

	void PlayerAttackMove(void);						 //�ړ�����
	void InitPlayerAttackMove(void);					 //�������̓���

	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h

private:
	static D3DXVECTOR3 m_pos[2];               //�ʒu���
	D3DXVECTOR3 m_rot[2];                      //�p�x���
	static LPDIRECT3DTEXTURE9 m_pTexture;      //�e�N�X�`�����   
	D3DXVECTOR3 m_move[2];                     //�ړ����
	D3DXVECTOR3 m_vec[2];                      //�x�N�g�����
	float m_homingAngle[2];                    //�p�x�v�Z���
	bool m_Pause;                              //�|�[�Y���
	int m_raise[2];                            //�p�x�v�Z
	PLAYERATTACK m_playerattack;               //�v���C���[�A�^�b�N�̏��       
	bool m_toggle;                             //�g���Ă��邩���Ȃ���
	int m_shotcount;                           //�e�̊Ԋu�J�E���g
};
#endif
