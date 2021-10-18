//*****************************************************************************
//
//�v���C���[����[player.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PLAYER_H_
#define _PLAYER_H_

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
class CPlayer : public CScene2D
{
public:
	//�񋓌^�i�G�̎�ށj
	typedef enum
	{
		PLAYERSTATE_NOMAL = 0,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_MAX
	}PLAYERSTATE;
	CPlayer(int nPriority = OBJTYPE_PLAYER);
	~CPlayer();
	static CPlayer *Create(D3DXVECTOR3 pos);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);     //����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��
	void PlayerMove(void);								 //�ړ�����
	void InitPlayerMove(void);							 //�������̓���
	static void SetStatePlayer(PLAYERSTATE playerstate); //�v���C���[�̏�ԕω�
	void PlayerCol(void);                                //�F�ύX����
	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h
	static D3DXVECTOR3 GetPlayer(void) { return m_pos; }
	static D3DXVECTOR3 GetOldPlayer(void) { return m_oldpos; }
	static PLAYERSTATE GetState(void) { return m_playerstate; }
	static bool GetPlayerAttack(void) { return m_playerattack; }
private:
	static D3DXVECTOR3 m_pos;//�ʒu���
	static D3DXVECTOR3 m_oldpos;//�̂̈ʒu���
	static LPDIRECT3DTEXTURE9 m_pTexture;       //�e�N�X�`�����   
	D3DXVECTOR3 m_move;                   //�ړ����
	float  m_DiagonalMove;                //�΂߈ړ��p
	float m_Counter;
	bool m_Pause;
	bool m_bInit;
	static PLAYERSTATE m_playerstate;
	static int m_statecount;
	int m_shotcount;
	int m_nLifeCount;
	static bool m_playerattack;
};
#endif
