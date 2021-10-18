//*****************************************************************************
//
//�|�[�Y����[pause.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�|�[�Y�N���X�̒�`
//*****************************************************************************
class CPause
{
public:
	//�񋓌^�i�^�C�g���̎�ށj
	typedef enum
	{
		PAUSETYPE_BG = 0,
		PAUSETYPE_PAUSE,
		PAUSETYPE_RETURN_GAME,
		PAUSETYPE_RETURN_TITLE,
		PAUSETYPE_MAX
	}PAUSETYPE;
	CPause();
	~CPause();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	static void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	static void SelectPause(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSETYPE_MAX];//�e�N�X�`�����
	D3DXVECTOR3 m_pos;//�ʒu
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static PAUSETYPE m_pauseType;
	static D3DXCOLOR m_pauseOneColor;
	static bool m_pause;
	static float m_nCnt;
};
#endif

