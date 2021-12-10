//*****************************************************************************
//
// �Q�[���w�i����[bg.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�Q�[���w�i�N���X�̒�`
//*****************************************************************************
class CBg : public CScene2D
{
public:
	CBg(int nPriority = OBJTYPE_BG);						   //�w�i�̃R���X�g���N�^(Priority�̗D�揇�ʕt��)
	~CBg();                                                    //�w�i�̃f�X�g���N�^
	static CBg *Create(D3DXVECTOR3 pos);                       //�w�i�̍쐬����

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth); //�w�i�̏���������
	void Uninit(void);                                         //�w�i�̏I������
	void Update(void);                                         //�w�i�̍X�V����
	void Draw(void);                                           //�w�i�`�揈�� 

	static HRESULT Load(void);                                 //�w�i�̃e�N�X�`�����[�h
	static void Unload(void);                                  //�w�i�̃e�N�X�`���A�����[�h

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }    //�w�i�̈ʒu����
	float GetSizeX(void) { return m_fx; }                      //�T�C�YX
	float GetSizeY(void) { return m_fy; }                      //�T�C�YY

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;                     //�w�i�̃e�N�X�`�������o�ϐ��@�@�@�@�@�@�@
	float m_fx, m_fy;                                          //�w�i�T�C�YX,Y
};	
#endif

                               