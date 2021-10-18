//*****************************************************************************
//
//�o���l����[Experience.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _EXPERIENCE_H_
#define _EXPERIENCE_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�o���l�N���X�̒�`
//*****************************************************************************
class CExperience : public CScene2D
{
public:
	CExperience();
	~CExperience();
	static CExperience *Create(D3DXVECTOR3 pos, float fHight, float fWidth);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);//����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��
	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h
	void SetHoming(bool bHoming);
private:
	D3DXVECTOR3 m_move;//�e�̈ړ���
	D3DXVECTOR3 m_pos;//�e�̈ʒu
	D3DXVECTOR3 m_posExperience;//�e�̈ʒu
	static LPDIRECT3DTEXTURE9 m_pTexture;//���L�e�N�X�`���̃|�C���^
	int m_nLife;//�e�̗̑�
	D3DXVECTOR3 m_half;
	float m_addhalf;
	bool m_bHoming;
};
#endif
