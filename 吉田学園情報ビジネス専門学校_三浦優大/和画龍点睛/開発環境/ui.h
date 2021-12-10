//*****************************************************************************
//
//ui処理[ui.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//uiクラスの定義
//*****************************************************************************
class CUi : public CScene2D
{
public:
	//列挙型（タイトルの種類）
	typedef enum
	{
		UITYPE_MAXSCORE = 0,
		UITYPE_SCORE,
		UITYPE_GRAZE,
		UITYPE_LIFE,
		UITYPE_BOMB,
		UITYPE_LINE,
		UITYPE_LOGO,
		UITYPE_MAX
	}UITYPE;

	CUi(int nPriority = OBJTYPE_UI);
	~CUi();
	static CUi *Create(D3DXVECTOR3 pos, float fHight, float fWidth, UITYPE uitype);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static void Unload(void);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[UITYPE_MAX];//テクスチャ情報
	float m_fx, m_fy;
};
#endif
