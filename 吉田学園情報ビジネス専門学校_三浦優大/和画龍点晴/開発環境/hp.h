//*****************************************************************************
//
//体力処理[hp.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _HP_H_
#define _HP_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//hpクラスの定義
//*****************************************************************************
class CHp
{
public:
	CHp();
	~CHp();
	static CHp *Create(D3DXVECTOR3 pos, float fHight, float fWidth);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);     //初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理
	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード
	void SetCol(int R, int G, int B, int A);
private:
	D3DXVECTOR3 m_pos;//位置情報
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	static LPDIRECT3DTEXTURE9 m_pTexture;//テクスチャ情報   
};
#endif
