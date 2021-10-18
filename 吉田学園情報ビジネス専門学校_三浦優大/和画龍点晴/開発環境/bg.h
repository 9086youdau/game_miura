//*****************************************************************************
//
//�w�i����[bg.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"
#include "Scene2D.h"

//*****************************************************************************
//�o���b�g�N���X�̒�`
//*****************************************************************************
class CBg : public CScene2D
{
public:
	CBg(int nPriority = OBJTYPE_BG);
	~CBg();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth); //����������
	void Uninit(void);  //�I������
	void Update(void); //�X�V����
	void Draw(void);  //�`�揈�� 
	static HRESULT Load(void);
	static void Unload(void);
	static CBg *Create(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }
private:
	static LPDIRECT3DTEXTURE9 m_apTexture;
	CScene2D *m_apScene;
	float m_nCounterAnim;									 //�J�E���^�[
	bool m_Pause;
	float m_fx, m_fy;
};	
#endif

                               