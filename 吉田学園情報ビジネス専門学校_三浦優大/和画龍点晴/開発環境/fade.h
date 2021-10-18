//*****************************************************************************
//
//�t�F�[�h����[fade.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"
#include "manager.h"

//*****************************************************************************
//�t�F�[�h�N���X�̒�`
//*****************************************************************************
class CFade : public CScene
{
public:
	//�t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	}FADE;
	CFade(int nPriority = OBJTYPE_FADE); //�R���X�g���N�^
	~CFade();//�f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);  //����������
	void Uninit(void);                       //�I������
	void Update(void);						 //�X�V����
	void Draw(void);                         //�`�揈��
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
	static void SetFade(FADE fade, CManager::MODE modeNext);
	static CFade *Create(CManager::MODE modenext);
	FADE GetFade(void) { return m_fade; }
private:
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  //���_���
	static FADE m_fade;
	static CManager::MODE m_modenext;
	D3DXCOLOR m_col;
	D3DXVECTOR3 m_pos;
	float posZ;
	float m_fx, m_fy;
};
#endif
