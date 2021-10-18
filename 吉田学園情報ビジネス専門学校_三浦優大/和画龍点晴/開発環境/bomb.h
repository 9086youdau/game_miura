//*****************************************************************************
//
//�{������[bomb.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BOMB_H_
#define _BOMB_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_SIZE_BOMB_BULLET (120)//�o���b�g�̃T�C�Y

//*****************************************************************************
//�{���N���X�̒�`
//*****************************************************************************
class CBomb : public CScene2D
{
public:
	CBomb(int nPriority = OBJTYPE_BOMB);
	~CBomb();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CBomb *Create(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
	static bool GetBomb(void) { return m_bBomb; }
	static void SetBomb(bool bBomb);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nCnt;
	int m_nBomb;
	float m_fx, m_fy;
	int m_nbombcool1;
	int m_nbombcool2;
	static bool m_bBomb;
};
#endif
