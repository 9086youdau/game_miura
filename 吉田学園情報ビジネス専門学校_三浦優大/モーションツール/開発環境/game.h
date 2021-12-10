//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CObject3D;

//=============================================================================
// ゲーム画面クラスの定義
//=============================================================================
//継承先描画クラス
class CGame : public CScene
{
public:
	CGame();									//コンストラクタ
	~CGame();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
	static CObject3D *GetObject(void) { return m_pObject3D; }

	//生成処理
	static CGame *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	int FailObject();

private:
	LPD3DXFONT	m_pTimeFontGame;					// 表示用フォント
	static CObject3D *m_pObject3D;					//時間Get
};

#endif