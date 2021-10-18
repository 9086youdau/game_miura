//*****************************************************************************
//
//�̗͏���[hp.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _HP_H_
#define _HP_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//hp�N���X�̒�`
//*****************************************************************************
class CHp
{
public:
	CHp();
	~CHp();
	static CHp *Create(D3DXVECTOR3 pos, float fHight, float fWidth);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);     //����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��
	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h
	void SetCol(int R, int G, int B, int A);
private:
	D3DXVECTOR3 m_pos;//�ʒu���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexture;//�e�N�X�`�����   
};
#endif
