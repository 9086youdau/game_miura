//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
//前方宣言
//=============================================================================
class CNumber;

//=============================================================================
// ランキングクラスの定義
//=============================================================================
class CRanking : public CScene
{
public:
	CRanking();								//コンストラクタ
	~CRanking();								//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	//値変更
	static void RandRanking(void);

	//加算処理
	static void SetRanking(void);
	//最大数表示
	static void MaxScoreRanking(void);

	void LoadData(void);
	void SaveData(void);

	//生成処理
	static CRanking *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	static int GetRanking(void);

	void Ranking(int nScore);

private:
	//ランキング表示変数
	static CNumber *m_apRankingNumber[5][8];	//桁数
	static int m_nRankingScore[5][8];			//スコアの値
	static int m_nNumber[5][8];
	int RankingCount;
	static int m_nRankScore[5];					//スコアの値

												//スコアの最大数表示用変数
	static CNumber *m_apMaxScoreNumber[8];		//桁数
	static int m_nMaxScore[8];					//スコアの値
	static int m_nMaxScoreNumber[8];
	static int m_nMaxRankScore;					//スコアの値
};

#endif