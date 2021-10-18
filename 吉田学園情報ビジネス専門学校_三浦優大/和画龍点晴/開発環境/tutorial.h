//*****************************************************************************
//
//�`���[�g���A������[tutorial.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
//�`���[�g���A���N���X�̒�`
//*****************************************************************************
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTutorial *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//�e�N�X�`�����
	CScene2D *m_pScene2D[2];
	int m_nCount;//�J�E���g�p
	float m_fx, m_fy;
};
#endif
