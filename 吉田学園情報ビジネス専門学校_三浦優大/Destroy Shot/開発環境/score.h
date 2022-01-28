//*****************************************************************************
//
//スコア処理[score.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
//スコアクラスの定義
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore();
	~CScore();

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *Create(D3DXVECTOR3 pos);

	static void SetScore(int nScore);
	static void AddScore(int nValue);
	static int GetScore(void) { return m_nScore; }

	static void CalculatioScore(void);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
private:

	static CNumber *m_apNumber[8];
	static int m_nScore;
	static int m_nNumber[8];
};
#endif
