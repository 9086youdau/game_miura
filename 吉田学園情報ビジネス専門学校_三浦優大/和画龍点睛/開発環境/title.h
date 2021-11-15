//*****************************************************************************
//
//�^�C�g������[title.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _TITLE_H_
#define _TITLE_H_

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
//�^�C�g���N���X�̒�`
//*****************************************************************************
class CTitle : public CScene
{
public:
	//�񋓌^�i�^�C�g���̎�ށj
	typedef enum
	{
		TITLETYPE_BG = 0,
		TITLETYPE_MOVEBG,
		TITLETYPE_MOVEBG2,
		TITLETYPE_UI,
		TITLETYPE_TITLELOGO,
		TITLETYPE_GAME,
		TITLETYPE_EXIT,
		TITLETYPE_MAX
	}TITLETYPE;

	CTitle();
	~CTitle();
	static CTitle *Create(void);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetVertex(void);
	void MoveBg(void);
	void TitleSelect(void);

	static bool GetExit(void) { return m_bExit; }

	static HRESULT Load(void);
	static void Unload(void);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TITLETYPE_MAX];//�e�N�X�`�����
	CScene2D *m_pScene2D[TITLETYPE_MAX];
	TITLETYPE m_titleType;
	int m_nCount;//�J�E���g�p
	float m_nCounterAnim;//�w�i�ړ�
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posui;
	D3DXVECTOR3 m_moveui;
	static bool m_bExit;
	float m_fx, m_fy;
};
#endif

