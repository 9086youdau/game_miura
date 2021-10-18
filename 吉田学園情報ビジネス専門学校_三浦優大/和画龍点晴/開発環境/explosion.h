//*****************************************************************************
//
//爆発処理[explosion.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//爆発クラスの定義
//*****************************************************************************
class CExplosion : public CScene2D
{
public:
	CExplosion();
	~CExplosion();
	static CExplosion *Create(D3DXVECTOR3 pos);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);	//初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理
	static HRESULT Load(void);							 //テクスチャの読み込み
	static void Unload(void);							 //テクスチャの破棄
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				 //共有テクスチャのポインタ
	int m_nCounterAnim;									 //カウンター
	int m_nPatternAnim;									 //パターンNo							
};
#endif

