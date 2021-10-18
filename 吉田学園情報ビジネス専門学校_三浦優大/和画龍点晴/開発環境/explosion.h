//*****************************************************************************
//
//��������[explosion.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�����N���X�̒�`
//*****************************************************************************
class CExplosion : public CScene2D
{
public:
	CExplosion();
	~CExplosion();
	static CExplosion *Create(D3DXVECTOR3 pos);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);	//����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��
	static HRESULT Load(void);							 //�e�N�X�`���̓ǂݍ���
	static void Unload(void);							 //�e�N�X�`���̔j��
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				 //���L�e�N�X�`���̃|�C���^
	int m_nCounterAnim;									 //�J�E���^�[
	int m_nPatternAnim;									 //�p�^�[��No							
};
#endif

