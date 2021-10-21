//================================================
//
//�X�R�A����[score.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "score.h"
#include "number.h"
#include "ranking.h"
#include "result.h"
#include "Graze.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
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
//�R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority) : CScene2D(nPriority)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
//�X�R�A�̏���������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fHight, float fWidth,SCORETYPE scoretype)
{
	//�f�[�^�̓ǂݍ���
	LoadData(SCORETYPE_RANKING_SCORE);

	//�ϐ�������
	m_nGameScore[0] = m_nScore[0];
	m_nResultScore[1] = m_nGameScore[1] * CGraze::GetGraze();

	switch (scoretype)
	{
	case SCORETYPE_GAME_SCORE:
		//�����o�ϐ�����
		m_scoreType = scoretype;
		//�i���o�[�̃N���G�C�g����
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_apNumber[nCntNumber][nCnt] = CNumber::Create(D3DXVECTOR3(1500.0f + (nCnt * 35.0f), 125.0f + (nCntNumber * 100.0f), 0.0f), 20.0f, 20.0f,CNumber::NUMBERTYPE_GAME);
			}
		}
		break;
	case SCORETYPE_RESULT_SCORE:
		//�����o�ϐ�����
		m_scoreType = scoretype;
		//�ϐ�������
		m_nResultScore[0] = m_nGameScore[1];
		//�i���o�[�̃N���G�C�g����
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
		//�����L���O�v�Z
		SetRanking(m_nResultScore[1]);
		//�����o�ϐ�����
		m_scoreType = scoretype;
		//�i���o�[�̃N���G�C�g����
		for (int nCntNumber = 0; nCntNumber < 5; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_apNumber[nCntNumber][nCnt] = CNumber::Create(D3DXVECTOR3((nCnt * 75.0f) + 800.0f + (nCntNumber * 55.0f), 335.0f + (nCntNumber * 155.0f), 0.0f), 40.0f, 30.0f, CNumber::NUMBERTYPE_RESULT);
			}
		}
		break;
	}

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//�X�R�A�̏I������
//=============================================================================
void CScore::Uninit(void)
{
	switch (m_scoreType)
	{
	case SCORETYPE_GAME_SCORE:
		//�X�R�A�p��
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//�I������
				if (m_apNumber[nCntNumber][nCnt] != NULL)
				{
					m_apNumber[nCntNumber][nCnt]->Uninit();
					delete m_apNumber[nCntNumber][nCnt];
				}
			}
		}
		break;
	case SCORETYPE_RESULT_SCORE:
		//�X�R�A�p��
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//�I������
				if (m_apNumber[nCntNumber][nCnt] != NULL)
				{
					m_apNumber[nCntNumber][nCnt]->Uninit();
					delete m_apNumber[nCntNumber][nCnt];
				}
			}
		}
		break;
	case SCORETYPE_RANKING_SCORE:
		//�X�R�A�p��
		for (int nCntNumber = 0; nCntNumber < 5; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//�I������
				if (m_apNumber[nCntNumber][nCnt] != NULL)
				{
					m_apNumber[nCntNumber][nCnt]->Uninit();
					delete m_apNumber[nCntNumber][nCnt];
				}
			}
		}
		break;
	}

	//�p������
	Release();
}

//=============================================================================
//�X�R�A�̍X�V����
//=============================================================================
void CScore::Update(void)
{
	//�֐��Ăяo��
	CalculatioScore();
}

//=============================================================================
//�X�R�A�̕`�揈��
//=============================================================================
void CScore::Draw(void)
{
	if (m_scoreType == SCORETYPE_GAME_SCORE)
	{
		for (int nCntNumber = 0; nCntNumber < 2; nCntNumber++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//�`�揈��
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
				//�`�揈��
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
				//�`�揈��
				m_apNumber[nCntNumber][nCnt]->Draw();
			}
		}
	}
}

//=============================================================================
//�X�R�A�̐ݒ�E�쐬����
//=============================================================================
CScore * CScore::Create(SCORETYPE scoretype)
{
	//�C���X�^���X����
	CScore *pScore = NULL;
	pScore = new CScore;

	//�������Ăяo��
	pScore->Init(D3DXVECTOR3(0.0f,0.0f,0.0f),MAX_SCORE_SIZE_Y, MAX_SCORE_SIZE_X,scoretype);

	return pScore;
}

//=============================================================================
//�X�R�A�̃Z�b�g����
//=============================================================================
void CScore::SetScore(int nScore)
{
	for (int nCntNumber = 0; nCntNumber < 3; nCntNumber++)
	{
		//����������
		m_nScore[nCntNumber] = nScore;
	}

	//�֐��Ăяo��
	CalculatioScore();
}

//=============================================================================
//�X�R�A�̑�������
//=============================================================================
void CScore::AddScore(int nValue)
{
	//��������v���X����
	m_nGameScore[1] += nValue;

	//�֐��Ăяo��
	CalculatioScore();
}

//==============================================================================
// �t�@�C���̓ǂݍ���
//==============================================================================
void CScore::LoadData(SCORETYPE scoretype)
{
	// ���[�J���ϐ��錾
	FILE *pFile;

	switch (scoretype)
	{
	case SCORETYPE_GAME_SCORE:
		// �f�[�^�̓ǂݍ���
		pFile = fopen("ranking.txt", "r");
		if (pFile != NULL)
		{ //�t�@�C���W�J�\
			fscanf(pFile, "%d", &m_nScore[0]);
			fclose(pFile);
		}
		else
		{ // �t�@�C���W�J�s��
		}
		break;
	case SCORETYPE_RANKING_SCORE:
		// �f�[�^�̓ǂݍ���
		pFile = fopen("ranking.txt", "r");
		if (pFile != NULL)
		{ //�t�@�C���W�J�\
			for (int nCount = 0; nCount < 5; nCount++)
			{
				fscanf(pFile, "%d", &m_nScore[nCount]);
			}
			fclose(pFile);
		}
		else
		{ // �t�@�C���W�J�s��
		}
		break;
	}
}

//==============================================================================
// �f�[�^�ۑ�����
//==============================================================================
void CScore::SaveData(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;

	// �t�@�C���̓W�J
	pFile = fopen("ranking.txt", "w");
	if (pFile != NULL)
	{ // �t�@�C���W�J�\
		for (int nCount = 0; nCount < 5; nCount++)
		{
			fprintf(pFile, "%d\n", m_nScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}

//=============================================================================
//�X�R�A�̌v�Z����
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
// �����L���O�̕��ёւ�
//==============================================================================
void CScore::SetRanking(int nScore)
{
	// ���[�J���ϐ��錾
	int nSubData;

	// �����L���O�O�̏��O
	if (m_nScore[5 - 1] <= nScore)
	{ // �X�R�A�������L���O���̏ꍇ
		m_nScore[5 - 1] = nScore;
	}

	// �v�Z����
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

	// �f�[�^�̕ۑ�����
	SaveData();
}




