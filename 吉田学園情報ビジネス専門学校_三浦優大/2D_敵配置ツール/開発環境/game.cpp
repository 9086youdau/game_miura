//===========================================================================
//
//�Q�[������[game.cpp]
// Author; miura yuudai
//
//===========================================================================

//===========================================================================
//�C���N���[�h�ǂݍ���
//===========================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "input.h"
#include "player.h"
#include "player.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "gamepad.h"
#include "playerattack.h"
#include "bullet.h"

//===========================================================================
//�ÓI�����o�ϐ��錾
//===========================================================================
CPlayer *CGame::m_pPlayer = NULL;
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
bool CGame::m_Pause = false;
int CGame::m_nCnt = 0;
int CGame::m_nSubCnt = 0;
int CGame::m_nStartCount[MAX_DATA] = {};
int CGame::m_nCountText = 0;

//===========================================================================
//�R���X�g���N�^
//===========================================================================
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
}

//===========================================================================
//�f�X�g���N�^
//===========================================================================
CGame::~CGame()
{
}

//===========================================================================
//�Q�[���̏���������
//===========================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�l��������
	m_Pause = false;

	//�G�z�u�̃e�L�X�g�̓ǂݍ���
	LoadText();

	//�w�i�̃e�N�X�`�����[�h
	CBg::Load();

	//�G�l�~�[�̃e�N�X�`�����[�h
	CEnemy::Load();

	//�v���C���[�̃e�N�X�`�����[�h
	CPlayer::Load();

	//�e�̃e�N�X�`�����[�h
	CBullet::Load();

	//�w�i�쐬�E�ݒ�֐�
	m_pBg = CBg::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�A�^�b�N����v���C���[�����̃e�N�X�`�����[�h
	CPlayerAttack::Load();

	//�A�^�b�N����v���C���[�����̃N���G�C�g
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_RIGHT);
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_LEFT);

	//�v���C���[�쐬�E�ݒ�֐�
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f));

	return S_OK;
}

//===========================================================================
//�Q�[���̏I������
//===========================================================================
void CGame::Uninit(void)
{
	//�w�i�̃A�����[�h
	CBg::Unload();

	//�v���C���[�̃A�����[�h
	CPlayer::Unload();

	//�G�l�~�[�̃A�����[�h
	CEnemy::Unload();

	//�e�̃e�N�X�`���A�����[�h
	CBullet::Unload();

	if (m_pBg != NULL)
	{
		//�I������
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	//�j���֐�
	Release();
}

//===========================================================================
//�Q�[���̍X�V����
//===========================================================================
void CGame::Update(void)
{
	//�G�̔z�u����p�֐��Ăяo��
	EnemyTool();
}

//===========================================================================
//�Q�[���̕`�揈��
//===========================================================================
void CGame::Draw(void)
{
}

//===========================================================================
//�G�l�~�[�̓������e�L�X�g����ǂݍ���
//===========================================================================
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
				if (strcmp("PLAYER_BULLET_DAMAGE", pText) == 0)
				{
					//�G�̃J�E���g
					fscanf(pFile, "%s", &aEqual[0]);
					fscanf(pFile, "%d", &m_BulletDamage);
				}

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
						if (strcmp("EnemyHp", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nLife[m_nCountText]);
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

//===========================================================================
// �c�[������p�֐�
//===========================================================================
void CGame::EnemyTool(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�|�[�Y���
	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_PAUSE) == true || pInputKeyboard->GetTrigger(DIK_P) == true)
	{
		m_Pause = m_Pause ? false : true;
	}

	//�|�[�Y��Ԃł͂Ȃ��ꍇ
	if (m_Pause == false)
	{
		//�J�E���g�A�b�v
		m_nCnt++;
	}

	//�w�肵���J�E���g�ōĐ�
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetTrigger(DIK_F8) == true)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(OBJTYPE_ENEMY, nCnt);

			if (pScene != NULL)
			{
				//�S�Ă̓G�����ł�����
				pScene->Uninit();
			}
		}

		//�G�l�~�[�J�E���g�ɕۑ������J�E���g����
		m_nCnt = m_nSubCnt;
	}
	//�J�E���g�𒲐�����
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetPress(DIK_F6) == true)
	{
		//�G�l�~�[�J�E���g�����Z�b�g����
		m_nSubCnt++;
	}

	//�J�E���g�𒲐�����
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetPress(DIK_F4) == true)
	{
		//�G�l�~�[�J�E���g�����Z�b�g����
		m_nSubCnt--;
	}

	//�J�E���g�����Z�b�g����
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetTrigger(DIK_F5) == true)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(OBJTYPE_ENEMY, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				//�S�Ă̓G�����ł�����
				pScene->Uninit();
			}
		}
		//�G�l�~�[�J�E���g�����Z�b�g����
		m_nCnt = 0;
	}

	//�J�E���g�ŊǗ�
	for (int nCnt = 0; nCnt < m_nCountText; nCnt++)
	{
		if (m_nCnt == m_nStartCount[nCnt])//�e�L�X�g�̃J�E���g����������
		{
			//�G�l�~�[�̃N���G�C�g
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(m_Pos[nCnt].x, m_Pos[nCnt].y, m_Pos[nCnt].z), m_Size[nCnt].x, m_Size[nCnt].y, (CEnemy::ENEMYTYPE)m_EnemyType[nCnt], m_fEnemySpeed[nCnt], m_nHomingCount[nCnt], m_nLife[nCnt], m_BulletDamage);
		}
	}

	//�J�E���g�̐���
	if (m_nSubCnt < 0)
	{
		//0��艺�ɂȂ����ꍇ�ő吔����
		m_nSubCnt = m_nStartCount[m_nCountText - 1];
	}
	else if (m_nSubCnt > m_nStartCount[m_nCountText - 1])
	{
		//�ő吔����ɂȂ�����l��0�ɖ߂�
		m_nSubCnt = 0;
	}
}

//===========================================================================
//�Q�[���̍쐬�E�ݒ菈��
//===========================================================================
CGame * CGame::Create(void)
{
	//�C���X�^���X����
	CGame *pGame= NULL;
	pGame = new CGame;

	//�������Ăяo��
	pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pGame;
}
