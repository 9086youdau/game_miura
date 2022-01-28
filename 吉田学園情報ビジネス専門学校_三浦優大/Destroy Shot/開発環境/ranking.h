//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
//�O���錾
//=============================================================================
class CNumber;

//=============================================================================
// �����L���O�N���X�̒�`
//=============================================================================
class CRanking : public CScene
{
public:
	CRanking();								//�R���X�g���N�^
	~CRanking();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	//�l�ύX
	static void RandRanking(void);

	//���Z����
	static void SetRanking(void);
	//�ő吔�\��
	static void MaxScoreRanking(void);

	void LoadData(void);
	void SaveData(void);

	//��������
	static CRanking *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	static int GetRanking(void);

	void Ranking(int nScore);

private:
	//�����L���O�\���ϐ�
	static CNumber *m_apRankingNumber[5][8];	//����
	static int m_nRankingScore[5][8];			//�X�R�A�̒l
	static int m_nNumber[5][8];
	int RankingCount;
	static int m_nRankScore[5];					//�X�R�A�̒l

												//�X�R�A�̍ő吔�\���p�ϐ�
	static CNumber *m_apMaxScoreNumber[8];		//����
	static int m_nMaxScore[8];					//�X�R�A�̒l
	static int m_nMaxScoreNumber[8];
	static int m_nMaxRankScore;					//�X�R�A�̒l
};

#endif