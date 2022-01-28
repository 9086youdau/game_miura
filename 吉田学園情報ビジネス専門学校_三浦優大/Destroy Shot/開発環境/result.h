//=============================================================================
//
// リザルト画面処理 [result.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_

//==============================================================================
//インクルード
//==============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//==============================================================================
//前方宣言
//==============================================================================
class CScore;
class CSound;
class CNumber;

//=============================================================================
// リザルト画面クラスの定義
//=============================================================================
class CResult : public CScene
{
public:
	CResult();									//コンストラクタ
	~CResult();									//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	static CResult *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//生成処理

private:
	LPD3DXFONT					m_pTimeFontGame;		// 表示用フォント
	int							m_nRankintData[5];		//ランキングデータ保管

	CNumber *m_apNumber[5][8];			//桁数
	static CScore		*m_pScore[5];					//アドレス保存用ポインタ
	CSound		*m_pSound;								//サウンドポインタ
};

#endif