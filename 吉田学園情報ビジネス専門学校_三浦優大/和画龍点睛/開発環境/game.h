//*****************************************************************************
//
//�@�Q�[������[game.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CPlayer;
class CBg;
class CEnemy;
class CScore;
class CLife;
class CGraze;
class CPause;
class CUi;
class CBomb;
class CDirecting;

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define TEXT_LENGTH (128)
#define DATA_LENGTH (64)
#define MAX_DATA (224)

//*****************************************************************************
//�Q�[���N���X�̒�`
//*****************************************************************************
class CGame : public CScene
{
public:
	CGame();//�R���X�g���N�^
	~CGame();//�f�X�g���N�^
	static CGame *Create(void);//�Q�[���̃N���G�C�g����

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);//�Q�[���̏���������
	void Uninit(void);//�Q�[���̏I������
	void Update(void);//�Q�[���̍X�V����
	void Draw(void);//�Q�[���̕`�揈��
	void LoadText(void);//�e�L�X�g�t�@�C������G�̓�����ǂݍ���

	static CBg *GetBg(void) { return m_pBg; }//�w�i�̃Q�b�g����
	static CPlayer *GetPlayer(void) { return m_pPlayer; }//�v���C���[�̃Q�b�g����
	static bool GetPause(void) { return m_Pause; }//�|�[�Y�̃Q�b�g����
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }

private:
	//�|�C���^�̃����o�ϐ�
	static CBg *m_pBg;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CScore *m_pScore;
	static CLife *m_pLife;
	static CGraze *m_pGraze;
	static CPause *m_pPause;
	static CBomb *m_pBomb;
	static CBomb *m_pBomb2;
	static CBomb *m_pBomb3;
	static CUi *m_pUi;
	static CDirecting *m_Directing;

	//�J�E���g�����o�ϐ�
	int m_nCnt;
	int m_nInterval[MAX_DATA];
	int m_nNumberEnemy;
	int m_DirectingCnt;
	int m_bombCnt;
	int m_nBossCount;

	//�e�L�X�g�ǂݎ��p�ϐ�
	int m_nCountText;
	int m_nStartCount[MAX_DATA];
	int m_nEndCount[MAX_DATA];
	int m_IntervalCount[MAX_DATA];
	D3DXVECTOR3 m_Pos[MAX_DATA];
	int m_EnemyType[MAX_DATA];
	D3DXVECTOR3 m_Size[MAX_DATA];
	float m_fEnemySpeed[MAX_DATA];
	int   m_nHomingCount[MAX_DATA];

	//�|�[�Y��ʃ����o�ϐ�
	static bool m_Pause;

	float m_fx, m_fy;
};
#endif
