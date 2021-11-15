//*****************************************************************************
//
//���U���g����[result.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScene2D;
class CNumber;
class CGraze;
class CScore;

//*****************************************************************************
//���U���g�N���X�̒�`
//*****************************************************************************
class CResult : public CScene
{
public:
	//�񋓌^�i���U���g�̎�ށj
	typedef enum
	{
		RESULTTYPE_BG = 0,
		RESULTTYPE_RESULTLOGO,
		RESULTTYPE_MOVEBG2,
		RESULTTYPE_UI,
		RESULTTYPE_TITLELOGO,
		RESULTTYPE_GAME,
		RESULTTYPE_EXIT,
		RESULTTYPE_MAX
	}RESULTTYPE;

	CResult();
	~CResult();
	static CResult *Create(void);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static void Unload(void);

	void TimesGraze(void);

	void SetVertex(void);
	static int GetScore(void) { return m_nResultAllScore; }
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }

private:
	static CNumber *m_apNumber[2][8];
	static LPDIRECT3DTEXTURE9 m_pTexture[8];		 //�e�N�X�`�����
	CScene2D *m_pScene2D[8];
	static int m_nResultScore;
	static int m_nResultAllScore;
	static int m_nAllNumber[8];
	static int m_nNumber[8];
	CGraze *m_pGraze;
	static CScore *m_pScore;
	bool m_bool;
	float m_nCounterAnim;
	float m_fx, m_fy;
};
#endif

