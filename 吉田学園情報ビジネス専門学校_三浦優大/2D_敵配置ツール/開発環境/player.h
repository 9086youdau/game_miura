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
//�v���C���[�N���X�̒�`
//*****************************************************************************
class CPlayer : public CScene2D
{
public:

	CPlayer(int nPriority = OBJTYPE_PLAYER);
	~CPlayer();
	static CPlayer *Create(D3DXVECTOR3 pos);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);     //����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��

	void PlayerMove(void);								 //�ړ�����
	void InitPlayerMove(void);							 //�������̓���

	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h

	static D3DXVECTOR3 GetPlayer(void) { return m_pos; }
	static D3DXVECTOR3 GetOldPlayer(void) { return m_oldpos; }
	static bool GetPlayerAttack(void) { return m_playerattack; }

private:
	static D3DXVECTOR3 m_pos;              //�ʒu���
	static D3DXVECTOR3 m_oldpos;           //�̂̈ʒu���
	static LPDIRECT3DTEXTURE9 m_pTexture;  //�e�N�X�`�����   
	D3DXVECTOR3 m_move;                    //�ړ����
	float  m_DiagonalMove;                 //�΂߈ړ��p
	float m_Counter;                       //�J�E���g�ۑ��p
	bool m_Pause;                          //�|�[�Y���
	bool m_bInit;                          //�Ǘ����
	static bool m_playerattack;            //�v���C���[�A�^�b�N�̏��
	D3DXVECTOR3 m_Rot;                     //��]���
	float  m_fLength;                      //���̒���
	float m_fAngle;                        //�p�x�ۑ��p
};
#endif
