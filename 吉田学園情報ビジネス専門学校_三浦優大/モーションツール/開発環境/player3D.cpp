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

//==============================================================================
//�ÓI�����o�ϐ��錾
//==============================================================================
D3DXVECTOR3	CPlayer3D::m_pos;
D3DXVECTOR3 CPlayer3D::m_rot;
MOTIONTYPE CPlayer3D::m_MotionTypeMax = MOTIONTYPE_MAX;
int CPlayer3D::m_MotionType = MOTIONTYPE_NEUTRAL;
int CPlayer3D::m_KeySet[MOTIONTYPE_MAX] = {};
int CPlayer3D::m_nNumKeyKeep = 0;
int CPlayer3D::m_GetKey = 0;
int CPlayer3D::m_MotionChange = 0;
int CPlayer3D::m_nFrame[MOTIONTYPE_MAX][10] = {};
int CPlayer3D::m_nFrameTotal[MOTIONTYPE_MAX] = {};
int CPlayer3D::m_nCounterMotion = 0;
int CPlayer3D::m_nNumModel = 0;
CModel *CPlayer3D::m_apModel[MODEL_PARTS] = {};
char *CPlayer3D::m_SelectData[MODEL_PARTS + 1][256] = {};
int CPlayer3D::m_DataCount = 0;
float CPlayer3D::m_aOffset[MODEL_PARTS][6] = {};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CPlayer3D::CPlayer3D(int nPriority, int nType)
{
	//�����o�ϐ�������
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	m_bUse = false;
	m_MotionChange = 0;
	m_AddKey = 0;
	m_nCntMat = 0;
	m_nCntMatOld = 1;
	m_bData = false;
	m_SelectData[0][256] = "��";
	for (int nCntData = 1; nCntData < m_nNumModel; nCntData++)
	{
		//�I�����Ă��郂�f���\�����O�ݒ�
		m_SelectData[nCntData][256] = "u";
	}
	m_OldDataCount = 1;
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
	////�����o�ϐ�������
	//for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	//{
	//	for (int nCnt = 0; nCnt < 10; nCnt++)
	//	{
	//		m_nFrame[nCntMotion][nCnt] = 40;
	//	}
	//}

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

	//�ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�����̃��[�V����
	UpdateMotion(MOTIONTYPE_NEUTRAL,false);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_apModel[m_nCntMat]->GetBuff()->GetBufferPointer();

	//�}�e���A���̐F�ύX
	pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);

	 //���_�o�b�t�@���A�����b�N
	m_apModel[m_nCntMat]->GetMash()->UnlockVertexBuffer();

	//�e���N���G�C�g
	CShadowS::Create();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CPlayer3D::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CPlayer3D::Update(void)
{
	//�֐��Ăяo��
	MotionTool();

	//�e�p�[�c�̍X�V����
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
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

	//�I�u�W�F�N�g�ւ̃|�C���^����
	//CObject3D *pObj;
	//pObj = CGame::GetObject();

	//�I�u�W�F�N�g�̓����蔻��
	//pObj->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x, m_MinPlayer.x, m_MaxPlayer.z, m_MinPlayer.z, m_MaxPlayer.y, m_MinPlayer.y);
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

							//�l��������
							//�p�[�c����
							for (int nCnt = 0; nCnt < 10; nCnt++)
							{
								for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
								{
									//�I�t�Z�b�g�̏�����
									m_Player3D[nMotion][nCnt][nCntKey][0] = m_aOffset[nCntKey][0];
									m_Player3D[nMotion][nCnt][nCntKey][1] = m_aOffset[nCntKey][1];
									m_Player3D[nMotion][nCnt][nCntKey][2] = m_aOffset[nCntKey][2];
								}
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
													for (int nCnt = 0; nCnt < 10; nCnt++)
													{
														for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
														{
															m_Player3D[nMotion][nCnt][nCntKey][0] = m_aOffset[nCntKey][0];
															m_Player3D[nMotion][nCnt][nCntKey][1] = m_aOffset[nCntKey][1];
															m_Player3D[nMotion][nCnt][nCntKey][2] = m_aOffset[nCntKey][2];
														}
													}
												}
												else if (strcmp("ROT", pKey) == 0)
												{// �p�x���̓ǂݍ���
													fscanf(pFile, "%s", &aEqual[0]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][3]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][4]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][5]);
												}
											}
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
	m_TotalKey += m_AddKey;
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

		if (m_bMotionChange == false)
		{// ���[�V�����ω��Ȃ�
			if (m_nCounterMotion == m_nFrame[MOTIONTYPE][m_Key])
			{// �L�[�t���[���I����
				//if (m_MotionType != MOTIONTYPE_JUMP || m_Key != m_nNumKeyKeep - 2)
				{// �W�����v�ȊO�ŃL�[�I�������Z�b�g
					m_nCounterMotion = 0;
				}

				if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
				{// ���[�v����Ƃ�
					m_Key = 0;
					m_GetKey = m_Key;
				}
				else if (m_Key == m_TotalKey - 1 && m_bLoopKeep == false)
				{// ���[�v�����I������Ƃ�
					//if (m_MotionTypeOld != MOTIONTYPE_JUMP)
					//{// ���O���W�����v�ȊO�̎�
						//m_MotionType = MOTIONTYPE_NEUTRAL;	// �j���[�g�����ֈڍs
						m_nCounterMotion = 40;
						//break;
					//}
				}

				else if (m_Key < m_TotalKey)
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
					m_nBrendFrame = 100;
				}
				if (m_MotionTypeOld == MOTIONTYPE_LEFTMOVE && m_MotionType == MOTIONTYPE_RIGHTMOVE)
				{// �j���[�g��������ړ���
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_RIGHTMOVE && m_MotionType == MOTIONTYPE_LEFTMOVE)
				{// �j���[�g��������ړ���
					m_nBrendFrame = 10;
				}
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
// ���[�V�����c�[���p�֐�
//==============================================================================
void CPlayer3D::MotionTool(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (m_bData == false)
	{
		//�I�����Ă��郂�f���\�����O�ݒ�
		m_SelectData[m_OldDataCount][256] = "u";
	}
	else
	{
		//���̃J�E���g��ۑ�
		m_OldDataCount = m_DataCount;

		//�J�E���g�A�b�v
		m_DataCount++;

		if (m_DataCount >= m_nNumModel)
		{
			//�J�E���g��0�ɖ߂�
			m_DataCount = 0;
		}

		//�I�����Ă��郂�f���\�����O�ݒ�
		m_SelectData[m_DataCount][256] = "��";

		//�f�[�^�t���O��߂��Ă�����
		m_bData = false;
	}

	if (m_bData2 == false)
	{
		//�I�����Ă��郂�f���\�����O�ݒ�
		m_SelectData[m_OldDataCount][256] = "u";
	}
	else
	{
		//���̃J�E���g��ۑ�
		m_OldDataCount = m_DataCount;

		//�J�E���g�A�b�v
		m_DataCount--;

		if (m_DataCount <= -1)
		{
			//�J�E���g��0�ɖ߂�
			m_DataCount = m_nNumModel - 1;
		}

		//�I�����Ă��郂�f���\�����O�ݒ�
		m_SelectData[m_DataCount][256] = "��";

		//�f�[�^�t���O��߂��Ă�����
		m_bData2 = false;
	}

	//F2�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{
		//true����false�ɂ���
		m_bUse = m_bUse ? false : true;

		//true����false�ɂ���
		m_bMove = m_bMove ? false : true;
	}

	//F9�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		//���[�V����������������
		WriteMotion();
	}

	if (m_bUse == true)
	{
		//�J�E���g��1�ȏ�̎�
		if (m_nCnt >= 1)
		{
			//���[�V�����J�E���g���Z�b�g
			m_nCounterMotion = 0;
			m_Key = 0;
			m_GetKey = m_Key;
			m_nCnt = 0;
		}
		//���[�V�����̍X�V
		UpdateMotion(m_MotionChange, false);
	}
	else
	{
		//�J�E���g�A�b�v
		m_nCnt++;
	}

	//RIGHT�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		//���[�V�����J�E���g���Z�b�g
		m_nCounterMotion = 0;
		m_Key = 0;
		m_GetKey = m_Key;

		//���[�V�����̍ő吔�܂ł�������
		if (m_MotionChange == MOTIONTYPE_MAX - 1)
		{
			//����������
			m_MotionChange = 0;
		}
		else
		{
			//�J�E���g�A�b�v
			m_MotionChange++;
		}

		//���[�V�����̍X�V
		UpdateMotion(m_MotionChange, false);
	}
	//LEFT�L�[���������Ƃ�
	else if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		//���[�V�����J�E���g���Z�b�g
		m_nCounterMotion = 0;
		m_Key = 0;
		m_GetKey = m_Key;

		//���[�V�����L�[��0�̏ꍇ
		if (m_MotionChange == 0)
		{
			//����������
			m_MotionChange = MOTIONTYPE_MAX - 1;
		}
		else
		{
			//�J�E���g�A�b�v
			m_MotionChange--;
		}

		//���[�V�����̍X�V
		UpdateMotion(m_MotionChange, false);
	}

	//I�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_I) == true)
	{
		//�L�[�𑝂₷
		m_nNumKeyKeep++;
		m_NumKey[m_MotionChange] = m_nNumKeyKeep;
	}
	//K�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_K) == true)
	{
		//�L�[�𑝂₷
		m_nNumKeyKeep--;
		m_NumKey[m_MotionChange] = m_nNumKeyKeep;
	}
	//2�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		//�L�[��i�߂�
		if (m_GetKey >= m_TotalKey - 1)
		{
			//�L�[�̑�������
			m_GetKey = 0;
			//���[�V�����J�E���g���Z�b�g
			m_nCounterMotion = 0;
			//���[�V�����̍X�V
			UpdateMotion(m_MotionChange, true);
		}
		//�L�[��i�߂�
		else if (m_GetKey < m_TotalKey - 1)
		{
			//�J�E���g�A�b�v
			m_GetKey++;
			//���[�V�����J�E���g���Z�b�g
			m_nCounterMotion = 0;
			//���[�V�����̍X�V
			UpdateMotion(m_MotionChange, true);
		}
	}
	//2�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		//�L�[��i�߂�
		if (m_GetKey <= 0)
		{
			//�L�[�̑�������
			m_GetKey = m_TotalKey - 1;
			//���[�V�����J�E���g���Z�b�g
			m_nCounterMotion = 0;
			//���[�V�����̍X�V
			UpdateMotion(m_MotionChange, true);
		}
		else if (m_GetKey <= m_TotalKey)
		{
			//�J�E���g�_�E��
			m_GetKey--;
			//���[�V�����J�E���g���Z�b�g
			m_nCounterMotion = 0;
			//���[�V�����̍X�V
			UpdateMotion(m_MotionChange, true);
		}
	}
	//2�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_N) == true)
	{
		m_nFrame[m_MotionChange][m_GetKey]++;
	}
	//2�L�[���������Ƃ�
	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		m_nFrame[m_MotionChange][m_GetKey]--;
	}

	//�t���[����1�܂ł�������1�ȉ��܂ł����Ȃ��悤�ɂ���
	if (m_nFrame[m_MotionChange][m_GetKey] <= 1)
	{
		m_nFrame[m_MotionChange][m_GetKey] = 1;
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATERIAL *pMat;		// �}�e���A���ւ̃|�C���^

	//���f����Rot��ύX���鎞�p�[�c�̐F��ύX
	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		//�I�����Ă��܂��p
		m_bData = true;

		//�J�E���g�����p�[�c����萔�l������������
		if (m_nCntMat >= m_nNumModel - 1)
		{
			//�J�E���g��-1�ɖ߂��Ă�����
			m_nCntMat = -1;
		}

		//�l��ۑ�
		m_nCntMatOld = m_nCntMat;

		//�J�E���g�A�b�v
		m_nCntMat++;

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)m_apModel[m_nCntMat]->GetBuff()->GetBufferPointer();

		//�}�e���A���̐F�ύX
		pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		m_apModel[m_nCntMat]->GetMash()->UnlockVertexBuffer();

		if (m_nCntMat == 0)
		{
			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)m_apModel[m_nNumModel - 1]->GetBuff()->GetBufferPointer();

			//�}�e���A���̐F�ύX
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�o�b�t�@���A�����b�N
			m_apModel[m_nNumModel - 1]->GetMash()->UnlockVertexBuffer();
		}

		//-1�̒l�������ꍇ
		if (m_nCntMatOld == -1)
		{
			//1�ɖ߂��Ă��グ��
			m_nCntMatOld = 1;
		}

		//�ۑ��p�Ƃ̔�r
		if (m_nCntMatOld <= m_nCntMat)
		{
			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)m_apModel[m_nCntMatOld]->GetBuff()->GetBufferPointer();

			//�}�e���A���̐F�ύX
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�o�b�t�@���A�����b�N
			m_apModel[m_nCntMatOld]->GetMash()->UnlockVertexBuffer();
		}
	}

	//���f����Rot��ύX���鎞�p�[�c�̐F��ύX
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		//�I�����Ă��܂��p
		m_bData2 = true;

		if (m_nCntMat <= 0)
		{
			m_nCntMat = m_nNumModel;
		}

		//�l��ۑ�
		m_nCntMatOld = m_nCntMat;

		if (m_nCntMat >= m_nNumModel)
		{
			m_nCntMatOld = 0;
		}

		//�J�E���g�A�b�v
		m_nCntMat--;

		// �}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)m_apModel[m_nCntMat]->GetBuff()->GetBufferPointer();

		//�}�e���A���̐F�ύX
		pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N
		m_apModel[m_nCntMat]->GetMash()->UnlockVertexBuffer();

		//�ۑ��p�Ƃ̔�r
		if (m_nCntMatOld >= m_nCntMat)
		{
			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)m_apModel[m_nCntMatOld]->GetBuff()->GetBufferPointer();

			//�}�e���A���̐F�ύX
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�o�b�t�@���A�����b�N
			m_apModel[m_nCntMatOld]->GetMash()->UnlockVertexBuffer();
		}
		//�ۑ��p�Ƃ̔�r
		if (m_nCntMatOld <= m_nCntMat)
		{
			// �}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)m_apModel[m_nCntMatOld]->GetBuff()->GetBufferPointer();

			//�}�e���A���̐F�ύX
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���_�o�b�t�@���A�����b�N
			m_apModel[m_nCntMatOld]->GetMash()->UnlockVertexBuffer();
		}
	}

	//��]�������p�[�c��ۑ�����
	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{
		//�p�[�c�ۑ�
		for (int nPart = 0; nPart < m_nNumModel; nPart++)
		{
			for (int nOffset = 0; nOffset < 6; nOffset++)
			{
				//��]�������p�[�c��ۑ�������
				m_Player3D[m_MotionChange][m_GetKey][nPart][nOffset] = m_aOffset[nPart][nOffset];
			}
		}
	}

	//���[�V�������R�s�[����
	if (pInputKeyboard->GetTrigger(DIK_F8) == true)
	{
		//�p�[�c�ۑ�
		for (int nPart = 0; nPart < m_nNumModel; nPart++)
		{
			for (int nOffset = 0; nOffset < 6; nOffset++)
			{
				//�ۑ��p�ϐ��ɒl��������
				m_aOldOffset[nPart][nOffset] = m_aOffset[nPart][nOffset];
			}
		}
	}

	//���[�V�����R�s�[��������y�[�X�g����
	if (pInputKeyboard->GetTrigger(DIK_F7) == true)
	{
		//�p�[�c�ۑ�
		for (int nPart = 0; nPart < m_nNumModel; nPart++)
		{
			for (int nOffset = 0; nOffset < 6; nOffset++)
			{
				//�ۑ����Ă������l��������
				m_aOffset[nPart][nOffset] = m_aOldOffset[nPart][nOffset];
			}
		}
	}

	//���f���̃p�[�cX���ύX
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{
		//X������]
		m_aOffset[m_DataCount][3] -= D3DX_PI / 100.0f;
	}
	//���f���̃p�[�cX���ύX
	else if (pInputKeyboard->GetPress(DIK_G) == true)
	{
		//X������]
		m_aOffset[m_DataCount][3] += D3DX_PI / 100.0f;
	}
	//���f���̃p�[�cX���ύX
	if (pInputKeyboard->GetPress(DIK_B) == true)
	{
		//X�������Z�b�g
		m_aOffset[m_DataCount][3] = m_Player3D[m_MotionChange][m_GetKey][m_DataCount][3];
	}

	//���f���̃p�[�cY���ύX
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{
		//Y������]
		m_aOffset[m_DataCount][4] -= D3DX_PI / 100.0f;
	}
	//���f���̃p�[�cY���ύX
	else if (pInputKeyboard->GetPress(DIK_H) == true)
	{
		//Y������]
		m_aOffset[m_DataCount][4] += D3DX_PI / 100.0f;
	}
	//���f���̃p�[�cY���ύX
	if (pInputKeyboard->GetPress(DIK_V) == true)
	{
		//Y�������Z�b�g
		m_aOffset[m_DataCount][4] = m_Player3D[m_MotionChange][m_GetKey][m_DataCount][4];
	}

	//���f���̃p�[�cZ���ύX
	if (pInputKeyboard->GetPress(DIK_U) == true)
	{
		//Z������]
		m_aOffset[m_DataCount][5] -= D3DX_PI / 100.0f;
	}
	//���f���̃p�[�cZ���ύX
	else if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		//Z������]
		m_aOffset[m_DataCount][5] += D3DX_PI / 100.0f;
	}
	//���f���̃p�[�cZ���ύX
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{
		//Z�������Z�b�g
		m_aOffset[m_DataCount][5] = m_Player3D[m_MotionChange][m_GetKey][m_DataCount][5];
	}

	if (m_bMove == false)
	{
		//�e�p�[�c��POS��ύX
		if (pInputKeyboard->GetPress(DIK_Q) == true)
		{
			//X�����ړ�
			m_aOffset[0][0] += D3DX_PI / 100.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			//X�����ړ�
			m_aOffset[0][0] -= D3DX_PI / 100.0f;
		}
		//�e�p�[�c��POS��ύX
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			//Y�����ړ�
			m_aOffset[0][1] += D3DX_PI / 100.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			//Y�����ړ�
			m_aOffset[0][1] -= D3DX_PI / 100.0f;
		}
		//�e�p�[�c��POS��ύX
		if (pInputKeyboard->GetPress(DIK_E) == true)
		{
			//Z�����ړ�
			m_aOffset[0][2] += D3DX_PI / 100.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			//Z�����ړ�
			m_aOffset[0][2] -= D3DX_PI / 100.0f;
		}

		//�e�p�[�c�̑S�Ă����Z�b�g
		if (pInputKeyboard->GetPress(DIK_RETURN) == true)
		{
			//�������Z�b�g
			for (int nCntPos = 0; nCntPos < 3; nCntPos++)
			{
				m_aOffset[0][nCntPos] = m_Player3D[m_MotionChange][m_GetKey][0][nCntPos];
			}
		}
	}
	else
	{
		//�v���C���[���ړ��ł���悤�ɂ���
		PlayerMove();
	}
}

//==============================================================================
// �v���C���[�̈ړ������֐�
//==============================================================================
void CPlayer3D::PlayerMove(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�J�����p�x�擾
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	// �p�x�C��
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	//�ʒu�X�V
	m_posold = m_pos;

	// �ړ��ʂ̉��Z
	m_pos += m_move;

	// W�L�[�̃��f���̈ړ�
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		//���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		// �������
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			//�J�E���g�A�b�v
			m_nCounter++;

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_LEFTMOVE;

			//�ړ��ʌv�Z
			m_move.x -= cosf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_move.z += sinf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY + (D3DX_PI * 3 / 4);
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{// �E�����

			//�J�E���g�A�b�v
			m_nCounter++;

		    //���[�V�����̕ω�
		    m_MotionChange = MOTIONTYPE_RIGHTMOVE;
			
			m_move.x += cosf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_move.z -= sinf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY - (D3DX_PI * 3 / 4);
		}
		else
		{// �����
			m_move.x += sinf(RotCameraY) * 2.0f;
			m_move.z += cosf(RotCameraY) * 2.0f;
			m_rotDest.y = RotCameraY + D3DX_PI;
		}
	}

	// S�L�[�̃��f���̈ړ�
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		if (m_nMoveCnt < 20)
		{
			//�J�E���g�A�b�v
			m_nMoveCnt++;
		}

		//���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		if (pInputKeyboard->GetPress(DIK_A) == true)
		{// ��������

		    //�J�E���g�A�b�v
			m_nCounter++;

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_RIGHTMOVE;

			//�ړ��ʌv�Z
			m_move.x -= cosf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_move.z += sinf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY - D3DX_PI / -4;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{// �E������

		    //�J�E���g�A�b�v
			m_nCounter++;

			//���[�V�����̕ω�
			m_MotionChange = MOTIONTYPE_LEFTMOVE;

			//�ړ��ʌv�Z
			m_move.x += cosf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_move.z -= sinf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY - D3DX_PI / 4;
		}
		else
		{// ������
			m_move.x -= sinf(RotCameraY) * 2.0f;
			m_move.z -= cosf(RotCameraY) * 2.0f;
			m_rotDest.y = RotCameraY;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{// ������

	    //���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		m_move.x -= cosf(RotCameraY) * 2.0f;
		m_move.z += sinf(RotCameraY) * 2.0f;
		m_rotDest.y = RotCameraY + (D3DX_PI / 2);
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{// �E����

	    //���[�V�����̕ω�
		m_MotionChange = MOTIONTYPE_MOVE;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}
		m_move.x += cosf(RotCameraY) * 2.0f;
		m_move.z -= sinf(RotCameraY) * 2.0f;
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

	// �ړ����[�V��������ҋ@���[�V������
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nMoveCnt == 10)
	{
		//�����o�ϐ�������
		m_nCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
		m_Key = 0;//�t���[���L�[�����Z�b�g
		m_GetKey = 0;//�t���[���L�[�����Z�b�g
		m_MotionChange = MOTIONTYPE_NEUTRAL;//���[�V�������j���[�g�����ɂ���
	}

	//�E�ړ����[�V�����̎� && �J�E���g��1�̎�
	if (m_MotionChange == MOTIONTYPE_RIGHTMOVE && m_nCounter == 1)
	{
		//�����o�ϐ�������
		m_nCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
		m_Key = 0;//�t���[���L�[�����Z�b�g
		m_GetKey = 0;//�t���[���L�[�����Z�b�g
	}
	//���ړ����[�V�����̎� && �J�E���g��1�̎�
	if (m_MotionChange == MOTIONTYPE_LEFTMOVE && m_nCounter == 1)
	{
		//�����o�ϐ�������
		m_nCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
		m_Key = 0;//�t���[���L�[�����Z�b�g
		m_GetKey = 0;//�t���[���L�[�����Z�b�g
	}
	//�ړ����[�V������؂�ւ��鎞 && �J�E���g��1�̎�
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nCounter >= 1)
	{
		//�����o�ϐ�������
		m_nCounterMotion = 0;//�t���[���J�E���g�����Z�b�g
		m_Key = 0;//�t���[���L�[�����Z�b�g
		m_GetKey = 0;//�t���[���L�[�����Z�b�g
		m_nCounter = 0;//�J�E���g�����Z�b�g
	}

	//�e�p�[�c�̍X�V����
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
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
}

//==============================================================================
// �v���C���[3D��Pos��Get�֐�
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetPosition(void)
{
	//�ʒu���
	return m_pos;
}

//==============================================================================
// �v���C���[3D��Rot��Get�֐�
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetRotation(void)
{
	//��]���
	return m_rot;
}

//==============================================================================
//���[�V�����̏�������
//==============================================================================
void CPlayer3D::WriteMotion(void)
{
	//�t�@�C���|�C���^
	FILE *fp;

	//�e�L�X�g�ɏ�������
	fp = fopen("data/TEXT/key_info.txt", "w");

	//�������ݓ��e
	fprintf(fp, "#=======================================================\n");
	fprintf(fp, "#\n");
	fprintf(fp, "# ���[�V�������\n");
	fprintf(fp, "# [���[�V������� : %d]\n",m_MotionChange);
	fprintf(fp, "#\n");
	fprintf(fp, "#=======================================================\n\n");

	//�������ݓ��e
	fprintf(fp, "MOTIONSET\n");
	fprintf(fp, "     LOOP = %d    # ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1:���[�v����]\n", m_Loop[m_MotionChange]);
	fprintf(fp, "     NUM_KEY = %d # �L�[��\n\n", m_NumKey[m_MotionChange]);

	//�������ݓ��e
	for (int nCntMotion = 0; nCntMotion < m_NumKey[m_MotionChange]; nCntMotion++)
	{
		fprintf(fp, "     KEYSET       # --- << KEY : %d / %d >> ---\n", nCntMotion, m_NumKey[m_MotionChange]);
		fprintf(fp, "         FRAME = %d\n", m_nFrame[m_MotionChange][nCntMotion]);
		for (int nCntNum = 0; nCntNum < m_nNumModel; nCntNum++)
		{
			//�������ݓ��e
			fprintf(fp, "         KEY # ----- [ %d ] ----- \n", nCntNum);
			fprintf(fp, "             POS = %.2f %.2f %.2f\n", m_aOffset[nCntNum][0], m_aOffset[nCntNum][1], m_aOffset[nCntNum][2]);
			fprintf(fp, "             ROT = %.2f %.2f %.2f\n", m_aOffset[nCntNum][3], m_aOffset[nCntNum][4], m_aOffset[nCntNum][5]);
			if (nCntNum == m_nNumModel - 1)
			{
				fprintf(fp, "         END_KEY\n");
			}
			else
			{
				fprintf(fp, "         END_KEY\n\n");
			}
		}
		if (nCntMotion == m_NumKey[m_MotionChange] - 1)
		{
			fprintf(fp, "     END_KEYSET\n");
		}
		else
		{
			fprintf(fp, "     END_KEYSET\n\n");
		}
	}

	//�������ݓ��e(�I�����})
	fprintf(fp, " END_MOTIONSET\n\n");

	//�utxt�v�̃t�@�C�������
	fclose(fp);
}
