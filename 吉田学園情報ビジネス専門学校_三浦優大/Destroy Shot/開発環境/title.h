//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CUi;
class CSound;

//=============================================================================
// �^�C�g���N���X�̒�`
//=============================================================================
//�p����`��N���X
class CTitle : public CScene
{
public:
	CTitle();								//�R���X�g���N�^
	~CTitle();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CTitle *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//��������

	static bool GetExit(void) { return m_bExit; }
	static bool GetNotMove(void) { return m_bNotMove; }

private:
	D3DXVECTOR3 m_pos; //�ʒu���
	static bool m_bExit;//�E�B���h�E�j���p
	static bool m_bNotMove;//�����Ȃ����鏈��
	CSound		*m_pSound;								//�T�E���h�|�C���^
};
#endif
