//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _TECTURE_H_
#define _TECTURE_H_

//=============================================================================
//インクルードファイルの読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
//テクスチャクラスの定義
//=============================================================================
//継承先描画クラス
class CTexture
{
public:

	//テキスチャの種類（列挙型）
	typedef enum
	{
		TEX_PLAYER = 0,
		TEX_MAX,
	}TEX;

	CTexture(int nPriority = 0);				//コンストラクタ
	~CTexture();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	HRESULT Load(void);												//テクスチャ読み込み
	void Unload(void);												//テクスチャ破棄

	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);						//アドレス情報の受け渡し

private:
	LPDIRECT3DTEXTURE9 m_pTexture;						// ポインタ配列格納ポインタ　先頭アドレス
};

#endif