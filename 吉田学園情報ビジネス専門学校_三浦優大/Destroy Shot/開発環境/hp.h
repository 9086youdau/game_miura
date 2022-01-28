//=============================================================================
//
// 体力処理 [hp.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _HP_H_
#define _HP_H_

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 体力クラスの定義
//=============================================================================

//継承先描画クラス
class CHp : public CScene2D
{
public:
	CHp(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CHp();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos,float fHight, float fWidth);			//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static void Load(void);
	static void Unload(void);

	static CHp *Create(D3DXVECTOR3 pos, float fHight, float fWidth);//生成処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャ情報
	D3DXVECTOR3 m_pos;                              //位置情報
};

#endif
