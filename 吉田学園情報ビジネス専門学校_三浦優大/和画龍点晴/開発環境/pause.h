//*****************************************************************************
//
//ポーズ処理[pause.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//ポーズクラスの定義
//*****************************************************************************
class CPause
{
public:
	//列挙型（タイトルの種類）
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
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSETYPE_MAX];//テクスチャ情報
	D3DXVECTOR3 m_pos;//位置
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static PAUSETYPE m_pauseType;
	static D3DXCOLOR m_pauseOneColor;
	static bool m_pause;
	static float m_nCnt;
};
#endif

