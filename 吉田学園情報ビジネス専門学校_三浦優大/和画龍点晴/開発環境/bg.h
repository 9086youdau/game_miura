//*****************************************************************************
//
//背景処理[bg.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"
#include "Scene2D.h"

//*****************************************************************************
//バレットクラスの定義
//*****************************************************************************
class CBg : public CScene2D
{
public:
	CBg(int nPriority = OBJTYPE_BG);
	~CBg();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth); //初期化処理
	void Uninit(void);  //終了処理
	void Update(void); //更新処理
	void Draw(void);  //描画処理 
	static HRESULT Load(void);
	static void Unload(void);
	static CBg *Create(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }
private:
	static LPDIRECT3DTEXTURE9 m_apTexture;
	CScene2D *m_apScene;
	float m_nCounterAnim;									 //カウンター
	bool m_Pause;
	float m_fx, m_fy;
};	
#endif

                               