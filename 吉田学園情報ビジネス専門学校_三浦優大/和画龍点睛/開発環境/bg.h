//*****************************************************************************
//
// ゲーム背景処理[bg.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//ゲーム背景クラスの定義
//*****************************************************************************
class CBg : public CScene2D
{
public:
	CBg(int nPriority = OBJTYPE_BG);						   //背景のコンストラクタ(Priorityの優先順位付き)
	~CBg();                                                    //背景のデストラクタ
	static CBg *Create(D3DXVECTOR3 pos);                       //背景の作成処理

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth); //背景の初期化処理
	void Uninit(void);                                         //背景の終了処理
	void Update(void);                                         //背景の更新処理
	void Draw(void);                                           //背景描画処理 

	static HRESULT Load(void);                                 //背景のテクスチャロード
	static void Unload(void);                                  //背景のテクスチャアンロード

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }    //背景の位置処理
	float GetSizeX(void) { return m_fx; }                      //サイズX
	float GetSizeY(void) { return m_fy; }                      //サイズY

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;                     //背景のテクスチャメンバ変数　　　　　　　
	float m_fx, m_fy;                                          //背景サイズX,Y
};	
#endif

                               