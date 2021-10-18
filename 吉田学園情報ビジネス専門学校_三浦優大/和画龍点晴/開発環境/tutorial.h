//*****************************************************************************
//
//チュートリアル処理[tutorial.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
//チュートリアルクラスの定義
//*****************************************************************************
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTutorial *Create(void);
	static HRESULT Load(void);
	static void Unload(void);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//テクスチャ情報
	CScene2D *m_pScene2D[2];
	int m_nCount;//カウント用
	float m_fx, m_fy;
};
#endif
