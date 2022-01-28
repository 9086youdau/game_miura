//=============================================================================
//
// �v���C���[3D���� [player3D.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//==============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "player3D.h"
#include "manager.h"
#include "model.h"
#include "object3D.h"
#include "camera.h"
#include "shadowS.h"
#include "bullet.h"
#include "passerby.h"
#include "animation.h"
#include "hp.h"
#include "fade.h"
#include "Purpose.h"
#include "score.h"
#include "effect.h"
#include "gamepad.h"
#include "input.h"
#include "sound.h"
#include "game.h"

//==============================================================================
//�}�N����`
//==============================================================================
#define PLAYER_MOVE (1.0f) //�v���C���[�̈ړ����x
#define BULLET_COUNT (25) //�e�̔��ˑ��x

//==============================================================================
//�ÓI�����o�ϐ��錾
//==============================================================================
D3DXVECTOR3	CPlayer3D::m_getpos;
D3DXVECTOR3 CPlayer3D::m_rot;
CModel *CPlayer3D::m_apModel[MODEL_PARTS] = {};
CBullet *CPlayer3D::m_pBullet = NULL;
bool CPlayer3D::m_bReset = false; 

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CPlayer3D::CPlayer3D(int nPriority, int nType)
{
	//�����o�ϐ�������
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	m_MotionChange = 0;
	m_nNextKey = false;
	m_nHpCount = 3;
	m_bReset = false;
	m_nParticlecount = 0;
	m_bParticle = false;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CPlayer3D::~CPlayer3D()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CPlayer3D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//���[�V�����ǂݍ���
	LoadMotion(); 

	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		//���f���𐶐�
		m_apModel[nCntModel] = CModel::Create(D3DXVECTOR3(m_aOffset[nCntModel][0], m_aOffset[nCntModel][1], m_aOffset[nCntModel][2]),
			D3DXVECTOR3(m_aOffset[nCntModel][3], m_aOffset[nCntModel][4], m_aOffset[nCntModel][5]), &m_pModelName[nCntModel][0]);

		m_apModel[nCntModel]->SetParent(m_apModel[m_aIndexParent[nCntModel]]);
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���[�J���ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATERIAL *pMat;		// �}�e���A���ւ̃|�C���^

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_apModel[0]->GetBuff()->GetBufferPointer();

	// �t�@�C�������g�p���ăe�N�X�`����ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/TX_L_William01_Body_D.png",
		&m_pTexture);

	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		//���f���̃p�[�c���e�N�X�`���𔽉f������
		m_apModel[nCntModel]->SetTexture(m_pTexture);
	}

	// ���_�����擾
	nNumVtx = m_apModel[0]->GetMash()->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	sizeFVF = D3DXGetFVFVertexSize(m_apModel[0]->GetMash()->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_apModel[0]->GetMash()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_���W�̔�r�ƍX�V
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// �e���W�̍ő�l�̔�r
		if (m_MaxPlayer.x < vtx.x)
		{
			m_MaxPlayer.x = vtx.x;
		}
		if (m_MaxPlayer.y < vtx.y)
		{
			m_MaxPlayer.y = vtx.y;
		}
		if (m_MaxPlayer.z < vtx.z)
		{
			m_MaxPlayer.z = vtx.z;
		}

		// �e���W�̍ŏ��l�̔�r
		if (m_MinPlayer.x > vtx.x)
		{
			m_MinPlayer.x = vtx.x;
		}
		if (m_MinPlayer.y > vtx.y)
		{
			m_MinPlayer.y = vtx.y;
		}
		if (m_MinPlayer.z > vtx.z)
		{
			m_MinPlayer.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_apModel[0]->GetMash()->UnlockVertexBuffer();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pHp[nCnt] = CGame::GetHp(nCnt);
	}

	//�ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�����̃��[�V����
	UpdateMotion(MOTIONTYPE_NEUTRAL,true);

	//�e���N���G�C�g
	CShadowS::Create();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CPlayer3D::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < 13; nCntModel++)
	{
		//NULL�`�F�b�N
		if (m_apModel[nCntModel] != NULL)
		{
			//���f����j��
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CPlayer3D::Update(void)
{
	//�ʒu�X�V
	m_getpos = m_pos;

	//�G�t�F�N�g����
	CEffect::Create(m_pos, D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f), 45, 20.0f, 20.0f);

	if (m_bParticle == false)
	{
		//�v���C���[�̈ړ�����
		PlayerMove();
	}

	//�e�p�[�c�̍X�V����
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}

	//�v���C���[�̓����蔻��
	//CollisionPlayer();
}

//==============================================================================
// �`�揈��
//==============================================================================
void CPlayer3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����̔��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�̔��]
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}

//==============================================================================
// ��������
//==============================================================================
CPlayer3D * CPlayer3D::Create()
{
	//�v���C���[�𐶐�
	CPlayer3D *pPlayer3D = NULL;
	pPlayer3D = new CPlayer3D;

	//NULL�`�F�b�N
	if (pPlayer3D != NULL)
	{
		//����������
		pPlayer3D->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	//�Ԃ�l
	return pPlayer3D;
}

//==============================================================================
// .txt�t�H���_�̓ǂݍ���
//==============================================================================
void CPlayer3D::LoadMotion(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[TEXT_LENGTH] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aCharaSet[DATA_LENGTH] = { NULL };		// �L�����f�[�^�ǂݎ��p�ϐ�
	char aPartsSet[DATA_LENGTH] = { NULL };		// �p�[�c�f�[�^�ǂݎ��p�ϐ�
	char aMotionSet[DATA_LENGTH] = { NULL };	// ���[�V�����ǂݎ��p�ϐ�
	char aKeySet[DATA_LENGTH] = { NULL };		// �L�[�ݒ���ǂݎ��p�ϐ�
	char aKey[DATA_LENGTH] = { NULL };			// �L�[���ǂݎ��p�ϐ�

	// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/motion_player00.txt", "r");

	if (pFile != NULL)
	{ //�t�@�C���W�J�\
		aText[0] = { NULL };
		aCharaSet[0] = { NULL };
		aPartsSet[0] = { NULL };
		aMotionSet[0] = { NULL };
		aKeySet[0] = { NULL };
		aKey[0] = { NULL };

		//���[�J���ϐ��錾
		int nIdxParts = 0;			// �p�[�c��
		int nMotion = 0;			// ���[�V�����̐�
		int nCntFile = 0;			// X�f�[�^�̐�
		int nAs = 0;

		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// �R�����g�A�E�g���Ă��Ȃ�
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
					bComment = true;
				}
				else
				{// �ʏ펞
					if (strcmp("NUM_MODEL", pText) == 0)
					{// ���f���̐�
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &m_nNumModel);
					}
					else if (strcmp("MODEL_FILENAME", pText) == 0)
					{// X�t�@�C�����̓ǂݍ���
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_pModelName[nCntFile][0]);
						nCntFile++;
					}
					else if (strcmp("CHARACTERSET", pText) == 0)
					{// �L�����̏��
						while (strcmp("END_CHARACTERSET", pCharaSet) != 0)
						{// �L�������̓ǂݎ��
							fscanf(pFile, "%s", &aCharaSet[0]);
							if (strcmp("NUM_PARTS", pCharaSet) == 0)
							{

							}
							else if (strcmp("PARTSSET", pCharaSet) == 0)
							{// �p�[�c���
								aPartsSet[0] = { NULL };

								while (strcmp("END_PARTSSET", pPartsSet) != 0)
								{
									//��x���[�V�����̑S�Ă�������
									for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
									{
										for (int nCntKey = 0; nCntKey < 10; nCntKey++)
										{
											m_nFrame[nCntMotion][nCntKey] = 40;

											for (int nCntPart = 0; nCntPart < 30; nCntPart++)
											{
												for (int nCntPosRot = 0; nCntPosRot < 6; nCntPosRot++)
												{
													//���[�V�����̏�����
													m_Player3D[nCntMotion][nCntKey][nCntPart][nCntPosRot] = m_aOffset[nCntPart][nCntPosRot];
												}
											}
										}
									}

									fscanf(pFile, "%s", &aPartsSet[0]);
									if (strcmp("INDEX", pPartsSet) == 0)
									{// �p�[�c���̓ǂݎ��
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &nIdxParts);
									}
									else if (strcmp("PARENT", pPartsSet) == 0)
									{// �p�[�c�̐e���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &m_aIndexParent[nIdxParts]);
									}
									else if (strcmp("POS", pPartsSet) == 0)
									{// �p�[�c�̈ʒu���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][1]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][2]);
									}
									else if (strcmp("ROT", pPartsSet) == 0)
									{// �p�[�c�̊p�x���
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][3]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][4]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][5]);
									}
								}
							}
						}
					}
					else if (strcmp("MOTIONSET", pText) == 0)
					{
						// �ϐ��̏�����
						aMotionSet[0] = { NULL };

						// ���[�J���ϐ��錾
						int nLoop = 0;
						int nKeySet = 0;

						while (strcmp("END_MOTIONSET", pMotionSet) != 0)
						{// ���[�V�����̓ǂݍ���
						 //�l��������
							fscanf(pFile, "%s", &aMotionSet[0]);
							if (strcmp("LOOP", pMotionSet) == 0)
							{// ���[�v���邩
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &nLoop);
								if (nLoop == 1)
								{// �P�̏ꍇ���[�v����
									m_bLoopKeep = true;
									m_Loop[nMotion] = true;
								}
							}

							else if (strcmp("NUM_KEY", pMotionSet) == 0)
							{// �L�[���̓ǂݍ���
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nNumKeyKeep);
							}

								if (strcmp("KEYSET", pMotionSet) == 0)
								{// �L�[�̓ǂݍ���
								 // ���[�J���ϐ��錾
									int nKey = 0;
									// �ϐ��̏�����
									aKeySet[0] = { NULL };

									while (strcmp("END_KEYSET", pKeySet) != 0)
									{// �L�[��񍀖ڂ̓ǂݍ���
										fscanf(pFile, "%s", &aKeySet[0]);

										if (aKeySet[0] == '#')
										{// �R�����g�A�E�g
											// ���[�J���ϐ��錾
											char c = NULL;
											char d = NULL;
											fscanf(pFile, "%c", &c);
											while (c != '\n' && d != '\n')
											{
												fscanf(pFile, "%s", &aKey[0]);
												fscanf(pFile, "%c", &d);
											}
										}

										if (strcmp("KEY", pKeySet) == 0)
										{// �L�[���̓ǂݍ���
											aKey[0] = { NULL };
											while (strcmp("END_KEY", pKey) != 0)
											{// �ʒu�E�p�x�̓ǂݍ���
												fscanf(pFile, "%s", &aKey[0]);
												if (strcmp("POS", pKey) == 0)
												{// �ʒu���̓ǂݍ���
													fscanf(pFile, "%s", &aEqual[0]);

													// �ʒu���̓ǂݍ���
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][0]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][1]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][2]);
												}
												else if (strcmp("ROT", pKey) == 0)
												{// �p�x���̓ǂݍ���
													fscanf(pFile, "%s", &aEqual[0]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][3]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][4]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][5]);
												}
											}
											//���̃p�[�c�ʒu�Ɉړ�����POS�̒l�𑫂�
											m_Player3D[nMotion][nKeySet][nKey][0] += m_aOffset[nKey][0];
											m_Player3D[nMotion][nKeySet][nKey][1] += m_aOffset[nKey][1];
											m_Player3D[nMotion][nKeySet][nKey][2] += m_aOffset[nKey][2];

											nKey++;
										}
										else if (strcmp("FRAME", pKeySet) == 0)
										{// �L�[�t���[�����̓ǂݍ���
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%d", &m_nFrame[nMotion][nKeySet]);
											m_nFrameTotal[nMotion] += m_nFrame[nMotion][nKeySet];
											if (m_Loop[nMotion] == false && nKeySet == (m_NumKey[nMotion] - 1))
											{
												m_nFrameTotal[nMotion] -= m_nFrame[nMotion][nKeySet];
											}
										}
									}

									//�L�[���v���X
									nKeySet++;
								}
							}

						// �L�[���̐ݒ�
						m_NumKey[nMotion] = m_nNumKeyKeep;

						//���[�V�����̃J�E���g�A�b�v
						nMotion++;
						m_MotionTypeMax++;
					}
				}
			}
			else if (bComment == true)
			{// �R�����g�A�E�g����
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
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}

//==============================================================================
// ���[�V�����̍X�V
//==============================================================================
void CPlayer3D::UpdateMotion(int MOTIONTYPE, bool bUse)
{
	// �L�[���ƃ��[�v���̑��
	m_Key = m_GetKey;
	m_TotalKey = (m_nNumKeyKeep = m_NumKey[MOTIONTYPE]);
	m_bLoopKeep = m_Loop[MOTIONTYPE];

	if (m_bMove == true)
	{
		m_MotionType = m_MotionChange;
	}

	// ���[�V�����J�E���^�[�̉��Z
	if (m_bMotionChange == true)
	{// ���[�V�����u�����h���s��
		if (m_nCounterMotion < m_nBrendFrame)
		{// �J�E���g���
			m_nCounterMotion++;
		}
	}
	else
	{// �ʏ펞
		if (m_nCounterMotion < m_nFrame[MOTIONTYPE][m_Key])
		{// �J�E���g���
			if (bUse == false)
			{
				//���[�V�����̃J�E���g�A�b�v
				m_nCounterMotion++;
			}
		}
	}

	// ���f���p�[�c���Ƃ̃��[�V�����A�j���[�V����
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
		{// ���[�v���čŏ��ɖ߂�Ƃ�
			m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][0][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][0][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][0][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][0][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][0][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][0][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else if (m_bMotionChange == true)
		{// ���[�V�����u�����h
			m_aOffset[nCnt][0] = m_aOffsetMemo[nCnt][0] + (m_Player3D[m_MotionType][0][nCnt][0] - m_aOffsetMemo[nCnt][0]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][1] = m_aOffsetMemo[nCnt][1] + (m_Player3D[m_MotionType][0][nCnt][1] - m_aOffsetMemo[nCnt][1]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][2] = m_aOffsetMemo[nCnt][2] + (m_Player3D[m_MotionType][0][nCnt][2] - m_aOffsetMemo[nCnt][2]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][3] = m_aOffsetMemo[nCnt][3] + (m_Player3D[m_MotionType][0][nCnt][3] - m_aOffsetMemo[nCnt][3]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][4] = m_aOffsetMemo[nCnt][4] + (m_Player3D[m_MotionType][0][nCnt][4] - m_aOffsetMemo[nCnt][4]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][5] = m_aOffsetMemo[nCnt][5] + (m_Player3D[m_MotionType][0][nCnt][5] - m_aOffsetMemo[nCnt][5]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else
		{// �ʏ펞
			if (m_nNextKey == false && (m_NumKey[MOTIONTYPE] - 1) != 0)
			{
				m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
				m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
				m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
				m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
				m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
				m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
			}
			if (m_nNextKey == false && (m_NumKey[MOTIONTYPE] - 1) == 0)
			{
				m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
				m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
				m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
				m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
				m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
				m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
			}
		}

		if (m_bMotionChange == false)
		{// ���[�V�����ω��Ȃ�
			if (m_nCounterMotion == m_nFrame[MOTIONTYPE][m_Key])
			{// �L�[�t���[���I����
				if (m_Key != m_TotalKey - 2 || m_bLoopKeep == true)
				{// �W�����v�ȊO�ŃL�[�I�������Z�b�g
					if (m_nNextKey == false && m_nNextKey == true || m_bLoopKeep == true)
					{
						m_nCounterMotion = 0;
					}
				}

				if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
				{// ���[�v����Ƃ�
					m_Key = 0;
					m_GetKey = m_Key;
				}
				else if (m_Key == m_TotalKey - 2 && m_bLoopKeep == false || m_Key == 0 && m_bLoopKeep == false)
				{// ���[�v�����I������Ƃ�
					if (m_Key != 0)
					{
						m_nCounterMotion = m_nFrame[MOTIONTYPE][m_Key];
						m_Key++;
						m_GetKey = m_Key;
					}
					m_nNextKey = true;
				}

				else if (m_Key < m_TotalKey && m_nNextKey != true)
				{// �L�[�̃J�E���g
					m_Key++;
					m_GetKey = m_Key;
				}
			}
			else if (m_MotionTypeOld != m_MotionType)
			{// ���[�V�����ω���	
				m_Key = 0;
				m_nCounterMotion = 0;
				m_bMotionChange = true;

				if (m_MotionTypeOld == MOTIONTYPE_NEUTRAL && m_MotionType == MOTIONTYPE_MOVE)
				{// �ړ�����j���[�g������
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_MOVE && m_MotionType == MOTIONTYPE_NEUTRAL)
				{// �j���[�g��������ړ���
					m_nBrendFrame = 30;
				}
				//if (m_MotionTypeOld == MOTIONTYPE_LEFTMOVE && m_MotionType == MOTIONTYPE_RIGHTMOVE)
				//{// �j���[�g��������ړ���
				//	m_nBrendFrame = 10;
				//}
				//if (m_MotionTypeOld == MOTIONTYPE_RIGHTMOVE && m_MotionType == MOTIONTYPE_LEFTMOVE)
				//{// �j���[�g��������ړ���
				//	m_nBrendFrame = 10;
				//}
				break;
			}
		}
		else if (m_bMotionChange == true)
		{// ���[�V�����u�����h���̃J�E���g���Z�b�g
			if (m_nCounterMotion == m_nBrendFrame || m_MotionTypeOld != m_MotionType)
			{
				m_Key = 0;
				m_nCounterMotion = 0;
				m_bMotionChange = false;
			}
		}
	}

	// ���O�̃��[�V������Ԃ̕ۑ�
	m_MotionTypeOld = m_MotionType;
}

//==============================================================================
// �v���C���[�̈ړ������֐�
//==============================================================================
void CPlayer3D::PlayerMove(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�J�����p�x�擾
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	// �p�x�C��
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	//�����̃��[�V����
	UpdateMotion(m_MotionChange, false);

	//�ʒu�X�V
	m_posold = m_pos;

	// �ړ��ʂ̉��Z
	m_pos += m_move;

	//�e���ˊԊu�J�E���g
	m_nBulletCount++;

	// W�L�[�̃��f���̈ړ�
	if (pInputKeyboard->GetPress(DIK_W) == true || pGamepad->IsButtonPush(CInput::UP) == true)
	{
		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		//�J�E���g�A�b�v
		m_nCounter++;

		//���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		// �������
		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
		{
			//�J�E���g�A�b�v
			m_nCounter++;

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_MOVE;

			//�ړ��ʌv�Z
			m_move.x -= cosf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z += sinf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY + (D3DX_PI * 3 / 4);
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
		{// �E�����

			//�J�E���g�A�b�v
			m_nCounter++;

		    //���[�V�����̕ω�
		    m_MotionChange = MOTIONTYPE_MOVE;
			
			m_move.x += cosf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z -= sinf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY - (D3DX_PI * 3 / 4);
		}
		else
		{// �����
			m_move.x += sinf(RotCameraY) * PLAYER_MOVE;
			m_move.z += cosf(RotCameraY) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY + D3DX_PI;
		}
	}

	// S�L�[�̃��f���̈ړ�
	else if (pInputKeyboard->GetPress(DIK_S) == true || pGamepad->IsButtonPush(CInput::DOWN) == true)
	{
		if (m_nMoveCnt < 20)
		{
			//�J�E���g�A�b�v
			m_nMoveCnt++;
		}

		//���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		//�J�E���g�A�b�v
		m_nCounter++;

		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
		{// ��������

		    //�J�E���g�A�b�v
			m_nCounter++;

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_MOVE;

			//�ړ��ʌv�Z
			m_move.x -= cosf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z += sinf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY - D3DX_PI / -4;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
		{// �E������

		    //�J�E���g�A�b�v
			m_nCounter++;

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_MOVE;

			//�ړ��ʌv�Z
			m_move.x += cosf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z -= sinf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY - D3DX_PI / 4;
		}
		else
		{// ������
			m_move.x -= sinf(RotCameraY) * PLAYER_MOVE;
			m_move.z -= cosf(RotCameraY) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY;
		}
	}

	else if (pGamepad->IsButtonPush(CInput::LEFT) == true)
	{// ������

	    //���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		//�J�E���g�A�b�v
		m_nCounter++;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		m_move.x -= cosf(RotCameraY) * PLAYER_MOVE;
		m_move.z += sinf(RotCameraY) * PLAYER_MOVE;
		m_rotDest.y = RotCameraY + (D3DX_PI / 2);
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
	{// �E����

	    //���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		//�J�E���g�A�b�v
		m_nCounter++;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}
		m_move.x += cosf(RotCameraY) * PLAYER_MOVE;
		m_move.z -= sinf(RotCameraY) * PLAYER_MOVE;
		m_rotDest.y = RotCameraY + (D3DX_PI / -2);
	}

	else
	{
		if (m_nMoveCnt > 10)
		{
			//�J�E���g�����炵�Ă���
			m_nMoveCnt--;
		}
	}

	if (pGamepad->IsButtonPush(CInput::LEFT_UP) == true)
	{
		// �������
		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_LEFT) == true)
		{
			m_rotDest.y = RotCameraY - (D3DX_PI * 3 / 4);
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		// �E�����
		else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_RIGHT) == true)
		{
			m_rotDest.y = RotCameraY + (D3DX_PI * 3 / 4);
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		else
		{
			m_rotDest.y = RotCameraY + D3DX_PI;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
	}
	else if (pGamepad->IsButtonPush(CInput::LEFT_DOWN) == true)
	{
		// �������
		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_LEFT) == true)
		{
			m_rotDest.y = RotCameraY - D3DX_PI / 4;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		// �E�����
		else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_RIGHT) == true)
		{
			m_rotDest.y = RotCameraY - D3DX_PI / -4;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		else
		{
			m_rotDest.y = RotCameraY;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_LEFT) == true)
	{
		m_rotDest.y = RotCameraY + (D3DX_PI / -2);
		if (m_nBulletCount % BULLET_COUNT == 0)
		{//�T�E���hBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
			m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
				D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_RIGHT) == true)
	{
		m_rotDest.y = RotCameraY + (D3DX_PI / 2);
		if (m_nBulletCount % BULLET_COUNT == 0)
		{
			//�T�E���hBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
			m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
				D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
		}
	}

	// �ړ����[�V��������ҋ@���[�V������
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nMoveCnt == 10)
	{
		//�����o�ϐ�������
		m_nCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
		m_Key = 0;//�t���[���L�[�����Z�b�g
		m_GetKey = 0;//�t���[���L�[�����Z�b�g
		m_nCounter = 0;
		m_MotionChange = MOTIONTYPE_NEUTRAL;//���[�V�������j���[�g�����ɂ���
	}
	// �ړ����[�V��������ҋ@���[�V������
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nCounter == 1)
	{
		//�����o�ϐ�������
		m_nCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
		m_Key = 0;//�t���[���L�[�����Z�b�g
		m_GetKey = 0;//�t���[���L�[�����Z�b�g
	}

	// �ړI�̉�]�p�̏��
	if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2.0f;
	}

	// �����̍X�V
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;

	// ���݂̉�]�p�̏��
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	//�e�p�[�c�̍X�V����
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		for (int nCntRot = 3; nCntRot < 6; nCntRot++)
		{
			// ���݂̉�]�p�̏��
			if (m_aOffset[nCnt][nCntRot] < -D3DX_PI)
			{
				m_aOffset[nCnt][nCntRot] += D3DX_PI * 2.0f;
			}
			else if (m_aOffset[nCnt][nCntRot] > D3DX_PI)
			{
				m_aOffset[nCnt][nCntRot] -= D3DX_PI * 2.0f;
			}
		}
	}

	// ������̌�������
	m_move.x += (0.0f - m_move.x) * 0.07f;
	m_move.z += (0.0f - m_move.z) * 0.07f;

	for (int nCnt = 0; nCnt < 256; nCnt++)
	{
		//�I�u�W�F�N�g�ւ̃|�C���^����
		CObject3D *pObj;
		pObj = CGame::GetObject(nCnt);

		if (pObj != NULL)
		{
			//�I�u�W�F�N�g3D�̓����蔻��
			pObj->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x - 0.0f, m_MinPlayer.x + 0.0f, m_MaxPlayer.z - 0.0f, m_MinPlayer.z + 0.0f, m_MaxPlayer.y, m_MinPlayer.y);
		}
	}
}

//==============================================================================
// �v���C���[�̓����蔻�菈��
//==============================================================================
void CPlayer3D::CollisionPlayer(void)
{
	//�ړI�̓G�̐���0���ゾ������
	if (CPurpose::GetPurpose() > 0)
	{
		// �G�Ƃ̓����蔻��
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			for (int nCntPriority = 0; nCntPriority < 3; nCntPriority++)
			{
				// �I�u�W�F�N�g���̎擾
				CScene *pScene = NULL;
				pScene = GetScene(nCntScene, nCntPriority);

				// NULL�`�F�b�N
				if (pScene != NULL)
				{
					// �I�u�W�F�N�g�̎�ނ̎擾
					CScene::OBJTYPE objType = pScene->GetObjType();

					//�I�u�W�F�N�g�^�C�v���G�l�~�[��������
					if (objType == OBJTYPE_ENEMY)
					{
						//�G�l�~�[�̃|�C���^�擾
						for (int nCnt = 0; nCnt < 256; nCnt++)
						{
							//�G���쐬�����|�C���^���擾
							CPasserby *pPasserby = NULL;
							pPasserby = CAnimation::GetPasserby(nCnt);

							//NULL�`�F�b�N
							if (pPasserby != NULL)
							{
								//�e�Ƃ̓����蔻��
								if (m_pos.x + 40.0f >= pPasserby->GetPasserbyPos().x && m_pos.x - 40.0f <= pPasserby->GetPasserbyPos().x&&
									m_pos.z + 40.0f >= pPasserby->GetPasserbyPos().z && m_pos.z - 40.0f <= pPasserby->GetPasserbyPos().z)
								{
									//�G��S�ď���
									pScene->Uninit();

									//�X�R�A������
									CScore::SetScore(0);

									//�G�̖ړI�̐�������������
									CPurpose::SetPurpose(CGame::GetEnemyPurpose());

									//���Z�b�g�̃t���O�Ǘ�
									m_bParticle = true;
								}
							}
						}
					}
				}
			}
		}
	}
	if (m_bParticle == true)
	{
		if (m_nParticlecount < 200)
		{
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, 1720.0f), 3);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, -1720.0f), 3);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(-2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 3);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 3);
			}
		}

		//�p�[�e�B�N�������J�E���g
		m_nParticlecount++;

		//�G�l�~�[�J�E���g�����Z�b�g
		CGame::SetCount(0);
	}

	if (m_nParticlecount == 200)
	{
		//���Z�b�g�̃t���O�Ǘ�
		m_bReset = true;
	}

	//�t���O���L���ɂȂ�����
	if (m_bReset == true)
	{
		if (CGame::GetLife() > 0)
		{
			//�v���C���[�����ʒu�ɂ���
			m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//�̗͂����炷
			m_pHp[CGame::GetLife()]->Uninit();
			CGame::SetLife(1);

			//�G�l�~�[�J�E���g�����Z�b�g
			CGame::SetCount(0);

			//����������
			m_bReset = false;
			m_bParticle = false;
			m_nParticlecount = 0;
		}
		//���C�t��0�ɂȂ�����
		else if (CGame::GetLife() == 0)
		{
			//�̗͂����炷
			m_pHp[CGame::GetLife()]->Uninit();
			CGame::SetLife(1);

			//��������������
			Uninit();

			//���U���g�ɍs��
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
		}
	}
}

//==============================================================================
// �v���C���[3D��Pos��Get�֐�
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetPosition(void)
{
	//�ʒu���
	return m_getpos;
}

//==============================================================================
// �v���C���[3D��Rot��Get�֐�
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetRotation(void)
{
	//��]���
	return m_rot;
}