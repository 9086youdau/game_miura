//================================================
//
//スコア処理[score.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "score.h"
#include "number.h"
#include "ranking.h"
#include "result.h"
#include "Graze.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
int CScore::m_nScore[5] = {};
int CScore::m_nNumber[7][8] = {};
int CScore::m_nResultNumber[2][8] = {};
int CScore::m_nResultScore[2] = {};
int CScore::m_nGameNumber[2][8] = {};
int CScore::m_nGameScore[2] = {};
CNumber *CScore::m_apNumber[7][8] = {};
CScore::SCORETYPE CScore::m_scoreType = SCORETYPE_GAME_SCORE;

//=============================================================================
//コンストラクタ
//=============================================================================
CScore::CScore(int nPriority) : CScene2D(nPriority)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
//スコアの初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fHight, float fWidth,SCORETYPE scoretype)
{
	//データの読み込み
	LoadData(SCORETYPE_RANKING_SCORE);

	//変数初期化
	m_nGameScore[0] = m_nScore[0];
	m_nResultScore[1] = m_nGameScore[1] * CGraze::GetGraze();

	switch (scoretype)
	{
	case SCORETYPE_GAME_SCORE:
		//メンバ変数を代入
		m_scoreType = scoretype;
		//ナンバーのクリエイト生成
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_apNumber[nCntNumber][nCnt] = CNumber::Create(D3DXVECTOR3(1500.0f + (nCnt * 35.0f), 125.0f + (nCntNumber * 100.0f), 0.0f), 20.0f, 20.0f,CNumber::NUMBERTYPE_GAME);
			}
		}
		break;
	case SCORETYPE_RESULT_SCORE:
		//メンバ変数を代入
		m_scoreType = scoretype;
		//変数初期化
		m_nResultScore[0] = m_nGameScore[1];
		//ナンバーのクリエイト生成
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			m_apNumber[0][nCnt] = CNumber::Create(D3DXVECTOR3(270.0f + (nCnt * 70.0f), 550.0f, 0.0f), 40.0f, 30.0f,CNumber::NUMBERTYPE_RESULT);
		}
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			m_apNumber[1][nCnt] = CNumber::Create(D3DXVECTOR3(730.0f + (nCnt * 70.0f), 900.0f, 0.0f), 40.0f, 30.0f,CNumber::NUMBERTYPE_RESULT);
		}
		break;
	case SCORETYPE_RANKING_SCORE:
		m_nGameScore[1] = 0;
		//ランキング計算
		SetRanking(m_nResultScore[1]);
		//メンバ変数を代入
		m_scoreType = scoretype;
		//ナンバーのクリエイト生成
		for (int nCntNumber = 0; nCntNumber < 5; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_apNumber[nCntNumber][nCnt] = CNumber::Create(D3DXVECTOR3((nCnt * 75.0f) + 800.0f + (nCntNumber * 55.0f), 335.0f + (nCntNumber * 155.0f), 0.0f), 40.0f, 30.0f, CNumber::NUMBERTYPE_RESULT);
			}
		}
		break;
	}

	//返り値
	return S_OK;
}

//=============================================================================
//スコアの終了処理
//=============================================================================
void CScore::Uninit(void)
{
	switch (m_scoreType)
	{
	case SCORETYPE_GAME_SCORE:
		//スコア廃棄
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//終了処理
				if (m_apNumber[nCntNumber][nCnt] != NULL)
				{
					m_apNumber[nCntNumber][nCnt]->Uninit();
					delete m_apNumber[nCntNumber][nCnt];
				}
			}
		}
		break;
	case SCORETYPE_RESULT_SCORE:
		//スコア廃棄
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//終了処理
				if (m_apNumber[nCntNumber][nCnt] != NULL)
				{
					m_apNumber[nCntNumber][nCnt]->Uninit();
					delete m_apNumber[nCntNumber][nCnt];
				}
			}
		}
		break;
	case SCORETYPE_RANKING_SCORE:
		//スコア廃棄
		for (int nCntNumber = 0; nCntNumber < 5; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//終了処理
				if (m_apNumber[nCntNumber][nCnt] != NULL)
				{
					m_apNumber[nCntNumber][nCnt]->Uninit();
					delete m_apNumber[nCntNumber][nCnt];
				}
			}
		}
		break;
	}

	//廃棄処理
	Release();
}

//=============================================================================
//スコアの更新処理
//=============================================================================
void CScore::Update(void)
{
	//関数呼び出し
	CalculatioScore();
}

//=============================================================================
//スコアの描画処理
//=============================================================================
void CScore::Draw(void)
{
	if (m_scoreType == SCORETYPE_GAME_SCORE)
	{
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//描画処理
				m_apNumber[nCntNumber][nCnt]->Draw();
			}
		}
	}
	else if (m_scoreType == SCORETYPE_RESULT_SCORE)
	{
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//描画処理
				m_apNumber[nCntNumber][nCnt]->Draw();
			}
		}
	}
	else if (m_scoreType == SCORETYPE_RANKING_SCORE)
	{
		for (int nCntNumber = 0; nCntNumber < 5; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//描画処理
				m_apNumber[nCntNumber][nCnt]->Draw();
			}
		}
	}
}

//=============================================================================
//スコアの設定・作成処理
//=============================================================================
CScore * CScore::Create(SCORETYPE scoretype)
{
	//インスタンス生成
	CScore *pScore = NULL;
	pScore = new CScore;

	//初期化呼び出し
	pScore->Init(D3DXVECTOR3(0.0f,0.0f,0.0f),MAX_SCORE_SIZE_Y, MAX_SCORE_SIZE_X,scoretype);

	return pScore;
}

//=============================================================================
//スコアのセット処理
//=============================================================================
void CScore::SetScore(int nScore)
{
	for (int nCntNumber = 0; nCntNumber < 3; nCntNumber++)
	{
		//引数から代入
		m_nScore[nCntNumber] = nScore;
	}

	//関数呼び出し
	CalculatioScore();
}

//=============================================================================
//スコアの足す処理
//=============================================================================
void CScore::AddScore(int nValue)
{
	//引数からプラスする
	m_nGameScore[1] += nValue;

	//関数呼び出し
	CalculatioScore();
}

//==============================================================================
// ファイルの読み込み
//==============================================================================
void CScore::LoadData(SCORETYPE scoretype)
{
	// ローカル変数宣言
	FILE *pFile;

	switch (scoretype)
	{
	case SCORETYPE_GAME_SCORE:
		// データの読み込み
		pFile = fopen("ranking.txt", "r");
		if (pFile != NULL)
		{ //ファイル展開可能
			fscanf(pFile, "%d", &m_nScore[0]);
			fclose(pFile);
		}
		else
		{ // ファイル展開不可
		}
		break;
	case SCORETYPE_RANKING_SCORE:
		// データの読み込み
		pFile = fopen("ranking.txt", "r");
		if (pFile != NULL)
		{ //ファイル展開可能
			for (int nCount = 0; nCount < 5; nCount++)
			{
				fscanf(pFile, "%d", &m_nScore[nCount]);
			}
			fclose(pFile);
		}
		else
		{ // ファイル展開不可
		}
		break;
	}
}

//==============================================================================
// データ保存処理
//==============================================================================
void CScore::SaveData(void)
{
	// ローカル変数宣言
	FILE *pFile;

	// ファイルの展開
	pFile = fopen("ranking.txt", "w");
	if (pFile != NULL)
	{ // ファイル展開可能
		for (int nCount = 0; nCount < 5; nCount++)
		{
			fprintf(pFile, "%d\n", m_nScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//=============================================================================
//スコアの計算処理
//=============================================================================
void CScore::CalculatioScore(void)
{
	if (m_scoreType == SCORETYPE_GAME_SCORE)
	{
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			m_nGameNumber[nCntNumber][0] = m_nGameScore[nCntNumber] % 100000000 / 10000000;
			m_nGameNumber[nCntNumber][1] = m_nGameScore[nCntNumber] % 10000000 / 1000000;
			m_nGameNumber[nCntNumber][2] = m_nGameScore[nCntNumber] % 1000000 / 100000;
			m_nGameNumber[nCntNumber][3] = m_nGameScore[nCntNumber] % 100000 / 10000;
			m_nGameNumber[nCntNumber][4] = m_nGameScore[nCntNumber] % 10000 / 1000;
			m_nGameNumber[nCntNumber][5] = m_nGameScore[nCntNumber] % 1000 / 100;
			m_nGameNumber[nCntNumber][6] = m_nGameScore[nCntNumber] % 100 / 10;
			m_nGameNumber[nCntNumber][7] = m_nGameScore[nCntNumber] % 10 / 1;
		}

		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_apNumber[nCntNumber][nCnt]->SetNumber(m_nGameNumber[nCntNumber][nCnt]);
			}
		}
	}
	else if (m_scoreType == SCORETYPE_RESULT_SCORE)
	{
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			m_nResultNumber[nCntNumber][0] = m_nResultScore[nCntNumber] % 100000000 / 10000000;
			m_nResultNumber[nCntNumber][1] = m_nResultScore[nCntNumber] % 10000000 / 1000000;
			m_nResultNumber[nCntNumber][2] = m_nResultScore[nCntNumber] % 1000000 / 100000;
			m_nResultNumber[nCntNumber][3] = m_nResultScore[nCntNumber] % 100000 / 10000;
			m_nResultNumber[nCntNumber][4] = m_nResultScore[nCntNumber] % 10000 / 1000;
			m_nResultNumber[nCntNumber][5] = m_nResultScore[nCntNumber] % 1000 / 100;
			m_nResultNumber[nCntNumber][6] = m_nResultScore[nCntNumber] % 100 / 10;
			m_nResultNumber[nCntNumber][7] = m_nResultScore[nCntNumber] % 10 / 1;
		}

		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_apNumber[nCntNumber][nCnt]->SetNumber(m_nResultNumber[nCntNumber][nCnt]);
			}
		}
	}
	else if (m_scoreType == SCORETYPE_RANKING_SCORE)
	{
		for (int nCntNumber = 0; nCntNumber < 5; nCntNumber++)
		{
			m_nNumber[nCntNumber][0] = m_nScore[nCntNumber] % 100000000 / 10000000;
			m_nNumber[nCntNumber][1] = m_nScore[nCntNumber] % 10000000 / 1000000;
			m_nNumber[nCntNumber][2] = m_nScore[nCntNumber] % 1000000 / 100000;
			m_nNumber[nCntNumber][3] = m_nScore[nCntNumber] % 100000 / 10000;
			m_nNumber[nCntNumber][4] = m_nScore[nCntNumber] % 10000 / 1000;
			m_nNumber[nCntNumber][5] = m_nScore[nCntNumber] % 1000 / 100;
			m_nNumber[nCntNumber][6] = m_nScore[nCntNumber] % 100 / 10;
			m_nNumber[nCntNumber][7] = m_nScore[nCntNumber] % 10 / 1;
		}

		for (int nCntNumber = 0; nCntNumber < 5; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_apNumber[nCntNumber][nCnt]->SetNumber(m_nNumber[nCntNumber][nCnt]);
			}
		}
	}
}
//==============================================================================
// ランキングの並び替え
//==============================================================================
void CScore::SetRanking(int nScore)
{
	// ローカル変数宣言
	int nSubData;

	// ランキング外の除外
	if (m_nScore[5 - 1] <= nScore)
	{ // スコアがランキング内の場合
		m_nScore[5 - 1] = nScore;
	}

	// 計算処理
	for (int nCnt = 0; nCnt < 5 - 1; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < 5; nCnt2++)
		{
			if (m_nScore[nCnt] < m_nScore[nCnt2])
			{
				nSubData = m_nScore[nCnt];
				m_nScore[nCnt] = m_nScore[nCnt2];
				m_nScore[nCnt2] = nSubData;
			}
		}
	}

	// データの保存処理
	SaveData();
}




