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
//�}�N����`
//*****************************************************************************
#define MAX_SIZE_PLAYER_BULLET (30)//�o���b�g�̃T�C�Y

//*****************************************************************************
//�}�l�[�W���[�N���X�̒�`
//*****************************************************************************
class CPlayerAttack : public CScene2D
{
public:
	//�񋓌^�i�G�̎�ށj
	typedef enum
	{
		PLAYERATTACK_LEFT = 0,
		PLAYERATTACK_RIGHT,
		PLAYERATTACK_MAX
	}PLAYERATTACK;
	CPlayerAttack(int nPriority = OBJTYPE_PLAYER_ATTACK);
	~CPlayerAttack();
	static CPlayerAttack *Create(D3DXVECTOR3 pos,PLAYERATTACK playerattack);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,PLAYERATTACK playerattack);     //����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��
	void PlayerAttackMove(void);								 //�ړ�����
	void InitPlayerAttackMove(void);							 //�������̓���
	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h
private:
	static D3DXVECTOR3 m_pos[2];//�ʒu���
	D3DXVECTOR3 m_rot[2];
	static LPDIRECT3DTEXTURE9 m_pTexture;       //�e�N�X�`�����   
	D3DXVECTOR3 m_move[2];                   //�ړ����
	D3DXVECTOR3 m_vec[2];
	float m_homingAngle[2];
	bool m_Pause;
	bool m_bInit;
	int m_shotcount;
	int m_raise[2];
	PLAYERATTACK m_playerattack;
	bool m_toggle;
};
#endif
