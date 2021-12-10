//================================================
//
//�Q�[������[game.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "player.h"
#include "explosion.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "number.h"
#include "score.h"
#include "effect.h"
#include "particle.h"
#include "hp.h"
#include "life.h"
#include "title.h"
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "Graze.h"
#include "pause.h"
#include "gamepad.h"
#include "ui.h"
#include "bomb.h"
#include "CircleHp.h"
#include "camera.h"
#include "laser.h"
#include "playerattack.h"
#include "Directing.h"
#include "SceneX.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
CPlayer *CGame::m_pPlayer = NULL;
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CScore *CGame::m_pScore = NULL;
CLife *CGame::m_pLife = NULL;
CGraze *CGame::m_pGraze = NULL;
CPause *CGame::m_pPause = NULL;
CBomb *CGame::m_pBomb = NULL;
CBomb *CGame::m_pBomb2 = NULL;
CBomb *CGame::m_pBomb3 = NULL;
CUi *CGame::m_pUi = NULL;
CDirecting *CGame::m_Directing = NULL;
bool CGame::m_Pause = false;

//=================================================
//�R���X�g���N�^
//=================================================
CGame::CGame()
{
	//�J�E���g�����o�ϐ�������
	m_nCnt = 0;
	m_nCountText = 0;
	for (int nCnt = 0; nCnt < MAX_DATA; nCnt++)
	{
		m_nInterval[nCnt] = 0;
		m_EnemyType[nCnt] = 0;
		m_Size[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Pos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_nNumberEnemy = 0;
}

//=================================================
//�f�X�g���N�^
//=================================================
CGame::~CGame()
{
}

//=================================================
//�Q�[���̏���������
//=================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�O���C�Y�̏�����
	CGraze::CalculatioGraze(0);

	//�A�^�b�N�|�C���g�̏�����
	CLaser::SetPlayerAttack(false);

	//�l��������
	m_Pause = false;

	//���C�t�Ǘ�
	CBullet::SetLifeRecovery(3);

	//�T�E���h�̃|�C���^�錾
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGM�ݒ�
	pSound->Stop(pSound->SOUND_LABEL_BGM000);

	//BGM�ݒ�
	pSound->Play(pSound->SOUND_LABEL_BGM001);

	//�G�z�u�̃e�L�X�g�̓ǂݍ���
	LoadText();

	//�w�i�̃e�N�X�`�����[�h
	CBg::Load();

	//�e�̃e�N�X�`�����[�h
	CBullet::Load();

	//���[�U�[�̃e�N�X�`�����[�h
	CLaser::Load();

	//�����̃e�N�X�`�����[�h
	CExplosion::Load();

	//�G�l�~�[�̃e�N�X�`�����[�h
	CEnemy::Load();

	//�X�R�A�̃e�N�X�`�����[�h
	CNumber::Load();

	//���o�ʂ̃e�N�X�`�����[�h
	CDirecting::Load();

	//�G�t�F�N�g�̃e�N�X�`�����[�h
	CEffect::Load();

	//�p�[�e�B�N���̃e�N�X�`�����[�h
	CParticle::Load();

	//�{���̃e�N�X�`�����[�h
	CBomb::Load();

	//HP�̃e�N�X�`�����[�h
	CHp::Load();

	//�v���C���[�̃e�N�X�`�����[�h
	CPlayer::Load();

	//�w�i�쐬�E�ݒ�֐�
	m_pBg = CBg::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�X�R�A�̍쐬�E�ݒ�֐�
	m_pScore = CScore::Create(CScore::SCORETYPE_GAME_SCORE);

	//�O���C�Y�̍쐬�E�ݒ�֐�
	m_pGraze = CGraze::Create(1550.0f, 325.0f, 20.0f, 20.0f);

	//�̗͂̍쐬�E�ݒ菈��
	m_pLife = CLife::Create();

	//�A�^�b�N����v���C���[�����̃e�N�X�`�����[�h
	CPlayerAttack::Load();

	//�A�^�b�N����v���C���[�����̃N���G�C�g
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_RIGHT);
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_LEFT);

	//ui�̃e�N�X�`�����[�h
	CUi::Load();

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//ui���N���G�C�g
		CUi::Create(D3DXVECTOR3(1050.0f, (nCnt * 100.0f) + 40.0f, 0.0f), 300.0f, 850.0f, CUi::UITYPE_LINE);
	}

	//ui���N���G�C�g
	CUi::Create(D3DXVECTOR3(1120.0f, 80.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_MAXSCORE);
	CUi::Create(D3DXVECTOR3(1120.0f, 180.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_SCORE);
	CUi::Create(D3DXVECTOR3(1120.0f, 280.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_GRAZE);
	CUi::Create(D3DXVECTOR3(1120.0f, 380.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_LIFE);
	CUi::Create(D3DXVECTOR3(1120.0f, 480.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_BOMB);

	//ui���N���G�C�g
	CUi::Create(D3DXVECTOR3(950.0f, 650.0f, 0.0f), 400.0f, 1100.0f, CUi::UITYPE_LOGO);

	//�{���̃N���G�C�g����
	m_pBomb = CBomb::Create(D3DXVECTOR3(1535.0f, 500.0f, 0.0f));
	m_pBomb2 = CBomb::Create(D3DXVECTOR3(1568.0f, 500.0f, 0.0f));
	m_pBomb3 = CBomb::Create(D3DXVECTOR3(1601.0f, 500.0f, 0.0f));

	//�v���C���[�쐬�E�ݒ�֐�
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f));

	//�V�[��X�̃N���G�C�g
	CSceneX::Create(D3DXVECTOR3(-5.0f, -25.0f, 0.0f),CSceneX::SCENEXTYPE_LEFT);
	CSceneX::Create(D3DXVECTOR3(-30.0f, -25.0f, 100.0f),CSceneX::SCENEXTYPE_LEFT);
	CSceneX::Create(D3DXVECTOR3(-60.0f, -25.0f, 200.0f),CSceneX::SCENEXTYPE_LEFT);
	CSceneX::Create(D3DXVECTOR3(-90.0f, -25.0f, 300.0f),CSceneX::SCENEXTYPE_LEFT);

	//�V�[��X�̃N���G�C�g
	CSceneX::Create(D3DXVECTOR3(-135.0f, -25.0f, 0.0f),CSceneX::SCENEXTYPE_RIGHT);
	CSceneX::Create(D3DXVECTOR3(-175.0f, -25.0f, 100.0f),CSceneX::SCENEXTYPE_RIGHT);
	CSceneX::Create(D3DXVECTOR3(-205.0f, -25.0f, 200.0f),CSceneX::SCENEXTYPE_RIGHT);
	CSceneX::Create(D3DXVECTOR3(-235.0f, -25.0f, 300.0f),CSceneX::SCENEXTYPE_RIGHT);

	return S_OK;
}

//=================================================
//�Q�[���̏I������
//=================================================
void CGame::Uninit(void)
{
	//�w�i�̃A�����[�h
	CBg::Unload();

	//�v���C���[�̃A�����[�h
	CPlayer::Unload();

	//�e�̃A�����[�h
	CBullet::Unload();

	//���[�U�[�̃e�N�X�`���A�����[�h
	CLaser::Unload();

	//�����̃A�����[�h
	CExplosion::Unload();

	//�G�l�~�[�̃A�����[�h
	CEnemy::Unload();

	//�X�R�A�̃e�N�X�`���A�����[�h
	CNumber::Unload();

	//�G�t�F�N�g�̃e�N�X�`���A�����[�h
	CEffect::Unload();

	//�p�[�e�B�N���̃e�N�X�`���A�����[�h
	CParticle::Unload();

	//�̗͂̃e�N�X�`���A�����[�h
	CHp::Unload();

	//ui�̃e�N�X�`���A�����[�h
	CUi::Unload();

	//�{���̃e�N�X�`���A�����[�h
	CBomb::Unload();

	//���o�ʂ̃e�N�X�`���A�����[�h
	CDirecting::Unload();

	//�o���l�̃A���e�N�X�`�����[�h
	CExplosion::Unload();

	if (m_pBg != NULL)
	{
		//�I������
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	else if (m_pScore != NULL)
	{
		//�I������
		m_pScore->Uninit();
		m_pScore = NULL;
	}

	else if (m_pGraze != NULL)
	{
		//�I������
		m_pGraze->Uninit();
		m_pGraze = NULL;
	}

	else if (m_pLife != NULL)
	{
		//�I������
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	else if (m_Directing != NULL)
	{
		//�I������
		m_Directing->Uninit();
		m_Directing = NULL;
	}

	//�j���֐�
	Release();
}

//=================================================
//�Q�[���̍X�V����
//=================================================
void CGame::Update(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//���[�J���ϐ��錾
	bool benemy = CDirecting::GetEnemy();
	bool bbomb = CBomb::GetBomb();

	//�|�[�Y���
	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_PAUSE) == true || pInputKeyboard->GetTrigger(DIK_P) == true)
	{
		m_Pause = m_Pause ? false : true;
	}

	//�{���̔j������
	if (bbomb == true && m_bombCnt == 0)
	{
		//�{����Ui�j��
		if (m_pBomb != NULL)
		{
			m_pBomb->Uninit();
			m_pBomb = NULL;
		}

		//������
		CBomb::SetBomb(false);

		//�J�E���g�A�b�v
		m_bombCnt++;

		//�l���Q�b�g���Ă���
		bbomb = CBomb::GetBomb();
	}
	//�{���̔j������
	if (bbomb == true && m_bombCnt == 1)
	{
		//�{����Ui�j��
		if (m_pBomb2 != NULL)
		{
			m_pBomb2->Uninit();
			m_pBomb2 = NULL;
		}

		//������
		CBomb::SetBomb(false);

		//�J�E���g�A�b�v
		m_bombCnt++;

		//�l���Q�b�g���Ă���
		bbomb = CBomb::GetBomb();
	}
	//�{���̔j������
	if (bbomb == true && m_bombCnt == 2)
	{
		//�{����Ui�j��
		if (m_pBomb3 != NULL)
		{
			m_pBomb3->Uninit();
			m_pBomb3 = NULL;
		}
	}

	//�|�[�Y��Ԃł͂Ȃ��ꍇ
	if (m_Pause == false)
	{
		//�J�E���g�A�b�v
		m_nCnt++;

		//�J�E���g�ŊǗ�
		if (m_nCnt == 100)
		{
			//���o�̃N���G�C�g
			m_Directing = CDirecting::Create(D3DXVECTOR3(170.0f, 200.0f, 0.0f), 200, 800, CDirecting::FADE_OUT, CDirecting::DIRECTINGTYPE_START);
		}

		//�J�E���g�ŊǗ�
		for (int nCnt = 0; nCnt < m_nCountText; nCnt++)
		{
			if (m_nCnt == m_nStartCount[nCnt])//�e�L�X�g�̃J�E���g����������
			{
				//�G�l�~�[�̃N���G�C�g
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(m_Pos[nCnt].x, m_Pos[nCnt].y, m_Pos[nCnt].z), m_Size[nCnt].x, m_Size[nCnt].y, (CEnemy::ENEMYTYPE)m_EnemyType[nCnt],m_fEnemySpeed[nCnt], m_nHomingCount[nCnt]);
			}
		}
	}
}
//=================================================
//�Q�[���̕`�揈��
//=================================================
void CGame::Draw(void)
{
}

//=================================================
//�G�l�~�[�̓������e�L�X�g����ǂݍ���
//=================================================
void CGame::LoadText(void)
{
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObject[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�

	// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/EnemySet.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// �R�����g�A�E�g���Ă��Ȃ�
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
				bComment = true;
			}

			else
			{
				if (strcmp("ENEMYCOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_ENEMYCOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("StartCount", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nStartCount[m_nCountText]);
						}
						if (strcmp("Pos", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_Pos[m_nCountText].x);
							fscanf(pFile, "%f", &m_Pos[m_nCountText].y);
							fscanf(pFile, "%f", &m_Pos[m_nCountText].z);
						}
						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_Size[m_nCountText].x);
							fscanf(pFile, "%f", &m_Size[m_nCountText].y);
						}
						if (strcmp("EnemyType", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_EnemyType[m_nCountText]);
						}
						if (strcmp("EnemyMove", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fEnemySpeed[m_nCountText]);
						}
						if (strcmp("HomingCount", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nHomingCount[m_nCountText]);
						}
					}
					//�J�E���g�A�b�v
					m_nCountText++;
				}
			}
		}
		else if (bComment == true)
		{
			// �R�����g�A�E�g����
			// ���[�J���ϐ��錾
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}
	//�t�@�C�������
	fclose(pFile);
}

//=================================================
//�Q�[���̍쐬�E�ݒ菈��
//=================================================
CGame * CGame::Create(void)
{
	//�C���X�^���X����
	CGame *pGame= NULL;
	pGame = new CGame;

	//�������Ăяo��
	pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pGame;
}
