//=============================================================================
//
// チュートリアル画面処理 [tutorial.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//==============================================================================
//インクルード読み込み
//==============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// チュートリアル画面クラスの定義
//=============================================================================
class CTutorial : public CScene
{
public:
	CTutorial();									//コンストラクタ
	~CTutorial();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	static CTutorial *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理

private:
};

#endif