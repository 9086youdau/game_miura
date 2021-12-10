//*****************************************************************************
//
//�X�R�A����[score.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
//�X�R�A�N���X�̒�`
//*****************************************************************************
class CScore : public CScene2D
{
public:
	//�񋓌^�i�X�R�A�̎�ށj
	typedef enum
	{
		SCORETYPE_GAME_SCORE = 0,
		SCORETYPE_RESULT_SCORE,
		SCORETYPE_RANKING_SCORE,
		SCORETYPE_MAX
	}SCORETYPE;

	CScore(int nPriority = OBJTYPE_SCORE);
	~CScore();
	static CScore *Create(SCORETYPE scoretype);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,SCORETYPE scoretype);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void AddScore(int nValue);
	static void CalculatioScore(void);

	void SetScore(int nScore);
	static int GetScore(void) { return m_nScore[1]; }
	void SetRanking(int nScore);
	D3DXVECTOR3 GetAllPosition(void) { return D3DXVECTOR3(); }

	static void LoadData(SCORETYPE scoretype);
	void SaveData(void);

private:
	static CNumber *m_apNumber[7][8];
	static int m_nScore[5];
	static int m_nNumber[7][8];
	static int m_nResultNumber[2][8];
	static int m_nResultScore[2];
	static int m_nGameNumber[2][8];
	static int m_nGameScore[2];
	static SCORETYPE m_scoreType;
};
#endif
