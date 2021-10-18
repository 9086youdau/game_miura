//*****************************************************************************
//
//グレイズ処理[Graze.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _GRAZE_H_
#define _GRAZE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
//グレイズクラスの定義
//*****************************************************************************
class CGraze : public CScene
{
public:
	CGraze(int nPriority = OBJTYPE_GRAZE);
	~CGraze();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGraze *Create(float posX,float posY,float fHight, float fWidth);
	static void SetGraze(int nScore);
	static void AddGraze(int nValue);
	static void CalculatioGraze(int nValue);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }
	static void CalculatioGraze(void);
	static int GetGraze(void) { return m_nGraze; }
private:
	static CNumber *m_apNumber[3];
	static int m_nGraze;
	static int m_nNumber[3];
	float m_fx, m_fy;
};
#endif
