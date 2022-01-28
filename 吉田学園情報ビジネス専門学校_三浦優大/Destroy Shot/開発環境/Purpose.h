//*****************************************************************************
//
//�ړI����[purpose.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PURPOSE_H_
#define _PURPOSE_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
//�ړI�N���X�̒�`
//*****************************************************************************
class CPurpose : public CScene
{
public:
	CPurpose();
	~CPurpose();

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPurpose *Create(void);

	static void SetPurpose(int nPurpose);
	static void ReducePurpose(int nValue);
	static int GetPurpose(void) { return m_nScorePurpose; }

	static void CalculatioPurpose(void);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
private:

	static CNumber *m_apNumberPurpose[3];
	static int m_nScorePurpose;
	static int m_nPurpose[3];
};
#endif
