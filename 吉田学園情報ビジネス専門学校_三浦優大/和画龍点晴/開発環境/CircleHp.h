//*****************************************************************************
//
//円のHP処理[CircleHp.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _CIRCLEHP_H_
#define _CIRCLEHP_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//円のHPの定義
//*****************************************************************************
class CCircleHp : public CScene2D
{
public:
	//列挙型（敵の種類）
	typedef enum
	{
		CIRCLEHP_OUT = 0,
		CIRCLEHP_IN,
		ENEMYTYPE_MAX
	}CIRCLEHP;
	CCircleHp(int nPriority = OBJTYPE_SIRCLEHP);
	~CCircleHp();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CCircleHp *Create(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp);
	D3DXVECTOR3 GetAllPosition(void) { return D3DXVECTOR3(); }
	static void SetBossLife(int nDamage);
	static int GetBossLife(void) { return m_sirclelife; }
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXVECTOR3 m_pos;//位置情報
	static int m_sirclelife;
	D3DXVECTOR3 m_bosspos;
	CIRCLEHP m_CircleHp;
};
#endif
