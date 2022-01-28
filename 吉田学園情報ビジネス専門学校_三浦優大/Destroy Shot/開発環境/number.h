//*****************************************************************************
//
//ナンバー処理[number.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//ナンバークラスの定義
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CNumber *Create(D3DXVECTOR3 pos, float fHight, float fWidth);
	void SetNumber(int nNumber);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXVECTOR3 m_pos;//位置情報
};
#endif

