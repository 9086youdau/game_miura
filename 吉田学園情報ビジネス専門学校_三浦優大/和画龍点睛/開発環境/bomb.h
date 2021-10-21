//*****************************************************************************
//
// �{������[bomb.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BOMB_H_
#define _BOMB_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�{���N���X�̒�`
//*****************************************************************************
class CBomb : public CScene2D
{
public:
	CBomb(int nPriority = OBJTYPE_BOMB);                      //�{���̃R���X�g���N�^(Priority�̗D�揇�ʕt��)
	~CBomb();                                                 //�{���̃f�X�g���N�^
	static CBomb *Create(D3DXVECTOR3 pos);                    //�{���̐�������

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);//�{���̏���������
	void Uninit(void);                                        //�{���̏I������
	void Update(void);                                        //�{���̍X�V����
	void Draw(void);                                          //�{���`�揈��

	static HRESULT Load(void);                                //�{���̃e�N�X�`�����[�h
	static void Unload(void);                                 //�{���̃e�N�X�`���A�����[�h

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };  //�{���̈ʒu�l������
	float GetSizeX(void) { return m_fx; };                    //�T�C�YX
	float GetSizeY(void) { return m_fy; };					  //�T�C�YY
	static bool GetBomb(void) { return m_bBomb; }
	static void SetBomb(bool bBomb);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;//�w�i�̃e�N�X�`�������o�ϐ�
	int m_nCnt;                          //�{���̃J�E���g�ϐ�
	float m_fx, m_fy;
	int m_nbombcool1;                    //�{���̃N�[���^�C��
	static bool m_bBomb;                 //�{���̎g�p�t���O
};
#endif
