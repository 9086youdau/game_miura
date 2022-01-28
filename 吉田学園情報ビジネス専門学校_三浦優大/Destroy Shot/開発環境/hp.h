//=============================================================================
//
// �̗͏��� [hp.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _HP_H_
#define _HP_H_

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �̗̓N���X�̒�`
//=============================================================================

//�p����`��N���X
class CHp : public CScene2D
{
public:
	CHp(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CHp();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos,float fHight, float fWidth);			//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static void Load(void);
	static void Unload(void);

	static CHp *Create(D3DXVECTOR3 pos, float fHight, float fWidth);//��������

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`�����
	D3DXVECTOR3 m_pos;                              //�ʒu���
};

#endif
