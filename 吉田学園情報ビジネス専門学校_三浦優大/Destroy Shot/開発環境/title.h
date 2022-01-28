//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CUi;
class CSound;

//=============================================================================
// タイトルクラスの定義
//=============================================================================
//継承先描画クラス
class CTitle : public CScene
{
public:
	CTitle();								//コンストラクタ
	~CTitle();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CTitle *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//生成処理

	static bool GetExit(void) { return m_bExit; }
	static bool GetNotMove(void) { return m_bNotMove; }

private:
	D3DXVECTOR3 m_pos; //位置情報
	static bool m_bExit;//ウィンドウ破棄用
	static bool m_bNotMove;//動かなくする処理
	CSound		*m_pSound;								//サウンドポインタ
};
#endif
