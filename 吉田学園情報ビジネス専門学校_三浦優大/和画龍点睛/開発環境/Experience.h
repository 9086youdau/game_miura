//*****************************************************************************
//
// 経験値処理[Experience.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _EXPERIENCE_H_
#define _EXPERIENCE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//経験値クラスの定義
//*****************************************************************************
class CExperience : public CScene2D
{
public:
	CExperience();//コンストラクタ
	~CExperience();//デストラクタ
	static CExperience *Create(D3DXVECTOR3 pos, float fHight, float fWidth);//経験値の生成

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);//初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

private:
	D3DXVECTOR3 m_move;//弾の移動量
	D3DXVECTOR3 m_pos;//弾の位置
	D3DXVECTOR3 m_posExperience;//弾の位置
	static LPDIRECT3DTEXTURE9 m_pTexture;//共有テクスチャのポインタ
	int m_nLife;//弾の体力
	bool m_bHoming;//ホーミングのフラグ回収
};
#endif
