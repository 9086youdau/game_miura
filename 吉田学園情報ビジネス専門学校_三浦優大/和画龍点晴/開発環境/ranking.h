//*****************************************************************************
//
//�����L���O����[ranking.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CNumber;
class CAlphabet;
class CScene2D;
class CScore;

//*****************************************************************************
//�i���o�[�N���X�̒�`
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking();
	~CRanking();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRanking *Create(void);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
	
	void SetVertex(void);
	static HRESULT Load(void);
	static void Unload(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[4]; //�e�N�X�`�����
	CScene2D *m_pScene2D[12];
	static CScore *m_pScore;
	float m_fx, m_fy;
};
#endif
