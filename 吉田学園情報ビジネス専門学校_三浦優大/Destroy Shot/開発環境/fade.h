//=============================================================================
//
//�t�F�[�h����[fade.h]
// Author; Miura Yudai
//
//=============================================================================

#ifndef _FADE_H_
#define _FADE_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "Scene.h"
#include "manager.h"

//=============================================================================
//�t�F�[�h�N���X�̒�`
//=============================================================================
class CFade : public CScene
{
public:
	//�t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN, //�t�F�[�h�C��
		FADE_OUT,//�t�F�[�h�A�E�g
		FADE_MAX
	}FADE;

	CFade(int nPriority = 4, int nType = 1);				//�R���X�g���N�^
	~CFade();												//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);  //����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��

	static CFade *Create(CManager::MODE modenext);              //��������

	//�󂯓n���p�֐�
	D3DXVECTOR3		GetAllPosition(void) { return D3DXVECTOR3(); }
	FADE			GetFade(void) { return m_fade; }
	static void		SetFade(FADE fade, CManager::MODE modeNext);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					//���_���
	static CManager::MODE m_modenext;					//���̃��[�h�����L��
	D3DXCOLOR	m_col;									//�J���[���
	static FADE			m_fade;							//���[�h���
	D3DXVECTOR3			m_pos;							//�ʒu���
};
#endif
