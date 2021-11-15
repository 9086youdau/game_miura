//*****************************************************************************
//
// ボム処理[bomb.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _BOMB_H_
#define _BOMB_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//ボムクラスの定義
//*****************************************************************************
class CBomb : public CScene2D
{
public:
	CBomb(int nPriority = OBJTYPE_BOMB);                      //ボムのコンストラクタ(Priorityの優先順位付き)
	~CBomb();                                                 //ボムのデストラクタ
	static CBomb *Create(D3DXVECTOR3 pos);                    //ボムの生成処理

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);//ボムの初期化処理
	void Uninit(void);                                        //ボムの終了処理
	void Update(void);                                        //ボムの更新処理
	void Draw(void);                                          //ボム描画処理

	static HRESULT Load(void);                                //ボムのテクスチャロード
	static void Unload(void);                                 //ボムのテクスチャアンロード

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };  //ボムの位置獲得処理
	float GetSizeX(void) { return m_fx; };                    //サイズX
	float GetSizeY(void) { return m_fy; };					  //サイズY
	static bool GetBomb(void) { return m_bBomb; }
	static void SetBomb(bool bBomb);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;//背景のテクスチャメンバ変数
	int m_nCnt;                          //ボムのカウント変数
	float m_fx, m_fy;
	int m_nbombcool1;                    //ボムのクールタイム
	static bool m_bBomb;                 //ボムの使用フラグ
};
#endif
