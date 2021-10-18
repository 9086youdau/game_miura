//*****************************************************************************
//
//�i���o�[����[number.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�i���o�[�N���X�̒�`
//*****************************************************************************
class CNumber
{
public:
	//�񋓌^�i�e�N�X�`���̎�ށj
	typedef enum
	{
		NUMBERTYPE_GAME = 0,
		NUMBERTYPE_RESULT,
		NUMBERTYPE_MAX
	}NUMBERTYPE;
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,NUMBERTYPE numbertype);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CNumber *Create(D3DXVECTOR3 pos, float fHight, float fWidth,NUMBERTYPE numbertype);
	void SetNumber(int nNumber);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXVECTOR3 m_pos;//�ʒu���
	NUMBERTYPE m_numbertype;
};
#endif

