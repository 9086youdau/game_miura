//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "game.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "object3D.h"
#include "player3D.h"
#include "meshfield.h"
#include "scene3D.h"
#include "passerby.h"
#include "effect.h"
#include "animation.h"
#include "hp.h"
#include "ui.h"
#include "score.h"
#include "number.h"
#include "particle.h"
#include "Purpose.h"
#include "bullet.h"
#include "sound.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
CObject3D *CGame::m_pObject3D[256] = {};                 //�I�u�W�F�N�g�̕ۑ��p�|�C���^�����o�ϐ�
int CGame::m_nGParicleMax[DATA_GAME_LENGTH] = {};             //�p�[�e�B�N���̔�����
float CGame::m_GSizeX[DATA_GAME_LENGTH] = {};                 //�p�[�e�B�N���̃T�C�YX
float CGame::m_GSizeY[DATA_GAME_LENGTH] = {};                 //�p�[�e�B�N���̃T�C�YY
float CGame::m_fGSpeed[DATA_GAME_LENGTH] = {};                //�p�[�e�B�N���̈ړ����x
D3DXCOLOR CGame::m_GParticlecol[DATA_GAME_LENGTH] = {};       //�p�[�e�B�N���̃J���[���
int CGame::m_nGLife[DATA_GAME_LENGTH] = {};                   //�p�[�e�B�N���̎���
float CGame::m_fGGravity[DATA_GAME_LENGTH] = {};              //�p�[�e�B�N���̏d��
CHp *CGame::m_pHp[4] = {};                               //�v���C���[�̃��C�tUI
int CGame::m_nCnt = 0;                                   //�G�̏o���J�E���g
int CGame::m_nHpCount = 0;                               //�v���C���[��HP�J�E���g
int CGame::m_nCountEnemyText = 0;                        //�G�̏o����

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CGame::CGame()
{
	//�����o�ϐ�������
	m_nHpCount = 3;
	m_nCnt = 0;
	m_nCountEnemyText = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CGame::~CGame()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�T�E���hBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM001);

	//�e�L�X�g�ǂݍ���
	LoadEnemy();    //�G�̏��
	LoadObject();   //�I�u�W�F�N�g���
	LoadParticle(); //�p�[�e�B�N�����

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �t�H���g�I�u�W�F�N�g�̐���
	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFontGame);	

	//���b�V���t�B�[���h�̃e�N�X�`�����[�h
	CMeshfield::Load();

	//���b�V���t�B�[���h
	CMeshfield::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	//ui�̃e�N�X�`�����[�h
	CUi::Load();
	
	//ui����
	CUi::Create(D3DXVECTOR3(50.0f, 30.0f, 0.0f), 110.0f, 400.0f, 0);
	CUi::Create(D3DXVECTOR3(1400.0f, 30.0f, 0.0f), 120.0f, 500.0f,0);
	CUi::Create(D3DXVECTOR3(700.0f, 30.0f, 0.0f), 120.0f, 500.0f, 0);
	CUi::Create(D3DXVECTOR3(700.0f, 37.0f, 0.0f), 100.0f, 300.0f, 8);

	//�̗͂̃e�N�X�`�����[�h
	CHp::Load();

	//�̗͐���
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pHp[nCnt] = CHp::Create(D3DXVECTOR3((nCnt * 70.0f) + 100.0f, 50.0f, 0.0f), 80.0f, 80.0f);
	}

	//3D�v���C���[�̐���
	CPlayer3D::Create();

	//�A�j���[�V�����e�N�X�`�����[�h
	CAnimation::Load();

	//�X�R�A�̃e�N�X�`�����[�h
	CNumber::Load();

	//�p�[�e�B�N���̃e�N�X�`�����[�h
	CParticle::Load();

	//�G�t�F�N�g�̃e�N�X�`�����[�h
	CEffect::Load();

	//�o���b�g�̃e�N�X�`�����[�h
	CBullet::Load();

	//�e�L�X�g����ǂݍ��܂ꂽ������
	for (int nCnt = 0; nCnt < m_nCountObjectText; nCnt++)
	{
		//�I�u�W�F�N�g����
		m_pObject3D[nCnt] = CObject3D::Create(m_ObjectPos[nCnt], m_ObjectRot[nCnt], &m_sObjectName[m_nModelType[nCnt]][0], &m_sTextureName[m_nTextureType[nCnt]][0]);
	}

	//�X�R�A����
	CScore::Create(D3DXVECTOR3(1480.0f, 87.0f, 0.0f));
	//�X�R�A������
	CScore::SetScore(0);

	//�ړI����
	CPurpose::Create();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CGame::Uninit(void)
{
	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�T�E���h�̔j��
	m_pSound->Stop(CSound::SOUND_LABEL_BGM001);

	//�X�R�A�̃e�N�X�`���A�����[�h
	CNumber::Unload();

	//ui�̃e�N�X�`���A�����[�h
	CUi::Unload();

	//�̗͂̃e�N�X�`���A�����[�h
	CHp::Unload();

	//�A�j���[�V�����e�N�X�`���A�����[�h
	CAnimation::Unload();

	//�p�[�e�B�N���̃e�N�X�`���A�����[�h
	CParticle::Unload();

	//�G�t�F�N�g�̃e�N�X�`���A�����[�h
	CEffect::Unload();

	//�o���b�g�̃e�N�X�`���A�����[�h
	CBullet::Unload();

	//���b�V���t�B�[���h�̃A���e�N�X�`�����[�h
	CMeshfield::Unload();

	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CGame::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//bullet�̈ʒu�擾
	D3DXVECTOR3 pos = CPlayer3D::GetPosition();

	//�G�̃J�E���g���A�b�v
	m_nCnt++;

	//�e�L�X�g����ǂݍ��܂ꂽ������
	for (int nCnt = 0; nCnt < m_nCountEnemyText; nCnt++)
	{
		if (m_nCnt == m_nStartEnemyText[nCnt])//�e�L�X�g�̃J�E���g����������
		{
			//�G�l�~�[�̃N���G�C�g
			CAnimation::Create(m_EnemyPos[nCnt], m_EnemySize[nCnt].x, m_EnemySize[nCnt].y, nCnt);
		}
	}

	// W�L�[�̃��f���̈ړ�
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
	}

	//�p�[�e�B�N������)
	GameParticle(D3DXVECTOR3(2000.0f, 0.0f, 1760.0f), 4);
	GameParticle(D3DXVECTOR3(2000.0f, 0.0f, -1760.0f), 4);
	GameParticle(D3DXVECTOR3(-2000.0f, 0.0f, 1760.0f), 4);
	GameParticle(D3DXVECTOR3(-2000.0f, 0.0f, -1760.0f), 4);
}

//==============================================================================
// �`�揈��
//==============================================================================
void CGame::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CGame * CGame::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CGame *pGame;
	pGame = new CGame;

	// NULL�`�F�b�N
	if (pGame != NULL)
	{
		pGame->Init(pos, fSizeX, fSizeY);
	}

	return pGame;
}

//===========================================================================
//�G�l�~�[�̓������e�L�X�g����ǂݍ���
//===========================================================================
void CGame::LoadEnemy(void)
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
							fscanf(pFile, "%d", &m_nStartEnemyText[m_nCountEnemyText]);
						}
						if (strcmp("Pos", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_EnemyPos[m_nCountEnemyText].x);
							fscanf(pFile, "%f", &m_EnemyPos[m_nCountEnemyText].y);
							fscanf(pFile, "%f", &m_EnemyPos[m_nCountEnemyText].z);
						}
						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_EnemySize[m_nCountEnemyText].x);
							fscanf(pFile, "%f", &m_EnemySize[m_nCountEnemyText].y);
						}
					}
					//�J�E���g�A�b�v
					m_nCountEnemyText++;
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
//�I�u�W�F�N�g�̈ʒu���e�L�X�g����ǂݍ���
//===========================================================================
void CGame::LoadObject(void)
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

	pFile = fopen("data/TEXT/ObjectSet.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	if (pFile != NULL)
	{ //�t�@�C���W�J�\

		int nModelCntFile = 0;			// X�f�[�^�̐�
		int nTextureCntFile = 0;		// �e�N�X�`���̐�

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
					if (strcmp("NUM_MODEL", pText) == 0)
					{// ���f���̐�
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &m_nObjectCount);
					}
					if (strcmp("TEXTURE_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_sTextureName[nTextureCntFile][0]);
						nTextureCntFile++;
					}
					if (strcmp("MODEL_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_sObjectName[nModelCntFile][0]);
						nModelCntFile++;
					}
					else if (strcmp("OBJECTCOUNTSET", aText) == 0)
					{
						pSetObjectEND[0] = { NULL };

						while (strcmp("END_OBJECTCOUNTSET", pSetObjectEND) != 0)
						{
							fscanf(pFile, "%s", &pSetObjectEND[0]);

							if (strcmp("Pos", pSetObjectEND) == 0)
							{
								//�G�̃J�E���g
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &m_ObjectPos[m_nCountObjectText].x);
								fscanf(pFile, "%f", &m_ObjectPos[m_nCountObjectText].y);
								fscanf(pFile, "%f", &m_ObjectPos[m_nCountObjectText].z);
							}
							if (strcmp("Rot", pSetObjectEND) == 0)
							{
								//�G�̃J�E���g
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &m_ObjectRot[m_nCountObjectText].x);
								fscanf(pFile, "%f", &m_ObjectRot[m_nCountObjectText].y);
								fscanf(pFile, "%f", &m_ObjectRot[m_nCountObjectText].z);
							}

							if (strcmp("ModelType", pSetObjectEND) == 0)
							{
								//�G�̃J�E���g
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nModelType[m_nCountObjectText]);
							}
							if (strcmp("TextureType", pSetObjectEND) == 0)
							{
								//�G�̃J�E���g
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nTextureType[m_nCountObjectText]);
							}
						}
						//�J�E���g�A�b�v
						m_nCountObjectText++;
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
}
void CGame::SetCount(int nCnt)
{
	//����������
	m_nCnt = nCnt;
}
//===========================================================================
//�p�[�e�B�N���̐ݒ���e�L�X�g����ǂݍ���
//===========================================================================
void CGame::LoadParticle(void)
{
	FILE *pFile = NULL;

	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[128] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObject[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�

	// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/ParticleSet.txt", "r");

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
				if (strcmp("PARTICLECOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_PARTICLECOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GSizeX[m_nGParicleText]);
							fscanf(pFile, "%f", &m_GSizeY[m_nGParicleText]);
						}
						if (strcmp("Speed", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGSpeed[m_nGParicleText]);
						}
						if (strcmp("Col", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].r);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].g);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].b);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].a);
						}
						if (strcmp("Life", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGLife[m_nGParicleText]);
						}
						if (strcmp("Gravity", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGGravity[m_nGParicleText]);
						}
						if (strcmp("ParticleMax", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGParicleMax[m_nGParicleText]);
						}
					}
					//�J�E���g�A�b�v
					m_nGParicleText++;
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
//�p�[�e�B�N���̐ݒ�
//===========================================================================
void CGame::GameParticle(D3DXVECTOR3 pos,int nType)
{
	for (int nCnt = 0; nCnt < m_nGParicleMax[nType]; nCnt++)
	{
		//�p�[�e�B�N������
		CParticle::Create(pos, m_GSizeX[nType], m_GSizeY[nType], m_fGSpeed[nType], m_GParticlecol[nType], m_nGLife[nType], m_fGGravity[nType]);
	}
}
//===========================================================================
//�v���C���[�̃��C�t�ݒ�
//===========================================================================
void CGame::SetLife(int nLife)
{
	//����������
	m_nHpCount -= nLife;
}