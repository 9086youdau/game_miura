//*****************************************************************************
//
//演出処理[Directing.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _DIRECTING_H_
#define _DIRECTING_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//演出クラスの定義
//*****************************************************************************
class CDirecting : public CScene2D
{
public:
	//列挙型（演出の種類）
	typedef enum
	{
		DIRECTINGTYPE_NONE = 0,
		DIRECTINGTYPE_START,
		DIRECTINGTYPE_BOSS,
		DIRECTINGTYPE_PLAYER,
		DIRECTINGTYPE_MAX
	}DIRECTINGTYPE;

	//列挙型（フェードの種類）
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	}FADETYPE;

	CDirecting(int nPriority = OBJTYPE_DIRECTING);//
	~CDirecting();
	static CDirecting *Create(D3DXVECTOR3 pos, float fHight, float fWidth, FADETYPE fadetype, DIRECTINGTYPE directing);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static void Unload(void);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
	static bool GetEnemy(void) { return m_benemycol; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[DIRECTINGTYPE_MAX];//テクスチャ情報
	static DIRECTINGTYPE m_directingType;
	int m_nCount;//カウント用
	float m_nCounterAnim;//背景移動
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_pos2;
	D3DXVECTOR3 m_move2;
	float m_fx, m_fy;
	int m_FadeCount;
	int m_FadeCount2;
	D3DXCOLOR m_colstart;
	D3DXCOLOR m_colplayer;
	D3DXCOLOR m_colboss;
	static FADETYPE m_fadetype;
	static FADETYPE m_fadetype2;
	bool m_bcol;
	static bool m_benemycol;
	int m_nCnt;
};
#endif