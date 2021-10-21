//*****************************************************************************
//
//life処理[life.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CHp;

//*****************************************************************************
//LIFEクラスの定義
//*****************************************************************************
class CLife : public CScene
{
public:
	CLife(int nPriority = OBJTYPE_LIFE);
	~CLife();
	static CLife *Create(void);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
	static void SetRelease(int nIndex);

private:
	static CHp *m_apHp[4];
	float m_fx, m_fy;
};
#endif
