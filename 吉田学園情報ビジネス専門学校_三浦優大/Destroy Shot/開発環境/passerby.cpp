//=============================================================================
//
// �ʍs�l���� [passerby.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//==============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "passerby.h"
#include "manager.h"
#include "model.h"
#include "object3D.h"
#include "camera.h"
#include "shadowS.h"
#include "scene3D.h"
#include "player3D.h"
#include "bullet.h"
#include "score.h"
#include "particle.h"
#include "animation.h"
#include "Purpose.h"
#include "fade.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "effect.h"
#include "hp.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
D3DXVECTOR3 CPasserby::m_posHoming;
bool CPasserby::m_bParticle2 = false;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CPasserby::CPasserby(int nPriority, int nType)
{
	//�����o�ϐ�������
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CPasserby::~CPasserby()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CPasserby::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//���[�V�����ǂݍ���
	LoadMotion(m_passerbytype);

	//�^�C�v��������
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);

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
	int nNumVtx;			    // ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

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
		if (m_MaxPasserby.x < vtx.x)
		{
			m_MaxPasserby.x = vtx.x;
		}
		if (m_MaxPasserby.y < vtx.y)
		{
			m_MaxPasserby.y = vtx.y;
		}
		if (m_MaxPasserby.z < vtx.z)
		{
			m_MaxPasserby.z = vtx.z;
		}

		// �e���W�̍ŏ��l�̔�r
		if (m_MinPasserby.x > vtx.x)
		{
			m_MinPasserby.x = vtx.x;
		}
		if (m_MinPasserby.y > vtx.y)
		{
			m_MinPasserby.y = vtx.y;
		}
		if (m_MinPasserby.z > vtx.z)
		{
			m_MinPasserby.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_apModel[0]->GetMash()->UnlockVertexBuffer();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pHp[nCnt] = CGame::GetHp(nCnt);
	}

	//�ʒu�X�V
	m_pos = pos;
	m_poskeep = pos;

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CPasserby::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			//���f���𐶐�
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
void CPasserby::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	if (m_bParticle == false && m_bParticle2 == false)
	{
		//�G�̓���
		PasserbyMove();
	}

	//bullet�̈ʒu�擾
	D3DXVECTOR3 pos = CBullet::GetPos();
	
	//�o���b�g�̃|�C���^�擾
	CBullet *pBullet;
	pBullet = CPlayer3D::GetBullet();

	if (m_bParticle == false)
	{
		//NULL�`�F�b�N
		if (pBullet != NULL)
		{
			//�e�Ƃ̓����蔻��
			if (m_pos.x + 40.0f >= pos.x && m_pos.x - 40.0f <= pos.x&&
				m_pos.z + 40.0f >= pos.z && m_pos.z - 40.0f <= pos.z)
			{
				//�X�R�A���Z
				CScore::AddScore(100);

				//�G�����炷
				CPurpose::ReducePurpose(1);

				//�p�[�e�B�N������
				CGame::GameParticle(m_pos, 1);

				////NULL�`�F�b�N
				//if (pBullet != NULL)
				//{
				//	//�e����������
				//	pBullet->Uninit();
				//}

				if (CPurpose::GetPurpose() == 0)
				{
					//���Z�b�g�̃t���O�Ǘ�
					m_bParticle = true;
				}

				if (m_bParticle == false)
				{
					//�T�E���hBGM
					m_pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

					//�G����������
					Uninit();
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
				//�p�[�e�B�N������(���񂾂Ƃ�)
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, 1720.0f), 5);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				//�p�[�e�B�N������(���񂾂Ƃ�)
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, -1720.0f), 5);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				//�p�[�e�B�N������(���񂾂Ƃ�)
				CGame::GameParticle(D3DXVECTOR3(-2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 5);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				//�p�[�e�B�N������(���񂾂Ƃ�)
				CGame::GameParticle(D3DXVECTOR3(2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 5);
			}
		}

		//�p�[�e�B�N�������J�E���g
		m_nParticlecount++;
	}

	//200�J�E���g��������
	if (m_nParticlecount == 200)
	{
		//�T�E���hBGM
		m_pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

		//���U���g�ɂ���
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
	}

	//�v���C���[�Ƃ̓����蔻��
	CollisionPlayer();
}

//==============================================================================
// �`�揈��
//==============================================================================
void CPasserby::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans,mtxScale;	//�v�Z�p�}�g���b�N�X

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
// �`�揈��
//==============================================================================
void CPasserby::PasserbyMove(void)
{
	//���[�J���ϐ��錾
	float fAngle;       //�ړI�n�܂ł̊p�x
	D3DXVECTOR3 vec;    //�ړI�n�܂ł̃x�N�g��

	D3DXVECTOR3 PlayerPos = CPlayer3D::GetPosition();

	// �ړ��ʂ̉��Z
	m_pos += m_move;
	m_posHoming = m_pos;

	//�G�t�F�N�g����
	CEffect::Create(m_pos, D3DXCOLOR(0.3f, 1.0f, 1.0f, 1.0f), 25, 20.0f, 20.0f);

	//���[�V�����̍X�V
	UpdateMotion(m_MotionType);

	//���[�V�����̕ω�
	m_MotionType = MOTIONTYPE_PASSERBY_MOVE;

	vec = PlayerPos - m_pos;//���ݒn����ړI�n�ւ̃x�N�g��
	fAngle = atan2f(vec.x, vec.z);//�x�N�g���̊p�x�����߂�
	m_move.x = sinf(fAngle) * 3.5f;//�ړ��ʂ����߂�
	m_move.z = cosf(fAngle) * 3.5f;//�ړ��ʂ����߂�
	m_rotDest.y = fAngle + D3DX_PI;//�ړI�n�ɍ��킹�Ċp�x��ς���
	
	//�e�p�[�c�̍X�V����
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}

	// ������̌�������
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.z += (0.0f - m_move.z) * 0.2f;

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
}

//==============================================================================
// ��������
//==============================================================================
CPasserby * CPasserby::Create(D3DXVECTOR3 pos,PASSERBY_TYPE passerbytype,int nType)
{
	//�v���C���[�𐶐�
	CPasserby *pPasserby = NULL;
	pPasserby = new CPasserby;

	//NULL�`�F�b�N
	if (pPasserby != NULL)
	{
		//��������l����
		pPasserby->m_passerbytype = passerbytype;
		pPasserby->m_nPasserbyCount = nType;

		//����������
		pPasserby->Init(pos, 0.0f, 0.0f);
	}

	return pPasserby;
}

//==============================================================================
// .txt�t�H���_�̓ǂݍ���
//==============================================================================
void CPasserby::LoadMotion(PASSERBY_TYPE passerbytype)
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

	switch (passerbytype)
	{
	case PASSERBY_TYPE_MEN:
		// �f�[�^�̓ǂݍ���
		pFile = fopen("data/TEXT/motion_passerby00.txt", "r");
		break;
	default:
		break;
	}

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
							else if (strcmp("KEYSET", pMotionSet) == 0)
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
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][0]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][1]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][2]);
												m_Passerby[nMotion][nKeySet][nKey][0] += m_aOffset[nKey][0];
												m_Passerby[nMotion][nKeySet][nKey][1] += m_aOffset[nKey][1];
												m_Passerby[nMotion][nKeySet][nKey][2] += m_aOffset[nKey][2];
											}
											else if (strcmp("ROT", pKey) == 0)
											{// �p�x���̓ǂݍ���
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][3]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][4]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][5]);
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
								nKeySet++;
							}
						}
						m_NumKey[nMotion] = nKeySet;	// �L�[���̐ݒ�
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
void CPasserby::UpdateMotion(MOTIONTYPE_PASSERBY passerby)
{
	// �L�[���ƃ��[�v���̑��
	m_nNumKeyKeep = m_NumKey[passerby];
	m_bLoopKeep = m_Loop[passerby];

	// ���[�V�����J�E���^�[�̉��Z
	if (m_bMotionChange == true)
	{// ���[�V�����u�����h���s��
		if (m_fCounterMotion < m_nBrendFrame)
		{// �J�E���g���
			m_fCounterMotion++;
		}
	}
	else
	{// �ʏ펞
		if (m_fCounterMotion < m_nFrame[passerby][m_Key])
		{// �J�E���g���
			m_fCounterMotion++;
		}
	}

	// ���f���p�[�c���Ƃ̃��[�V�����A�j���[�V����
	for (int nCnt = 0; nCnt < MAX_PASSERBY_PARTS; nCnt++)
	{
		if (m_Key == m_nNumKeyKeep - 1 && m_bLoopKeep == true)
		{// ���[�v���čŏ��ɖ߂�Ƃ�
			m_aOffset[nCnt][0] = m_Passerby[passerby][m_Key][nCnt][0] + (m_Passerby[passerby][0][nCnt][0] - m_Passerby[passerby][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][1] = m_Passerby[passerby][m_Key][nCnt][1] + (m_Passerby[passerby][0][nCnt][1] - m_Passerby[passerby][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][2] = m_Passerby[passerby][m_Key][nCnt][2] + (m_Passerby[passerby][0][nCnt][2] - m_Passerby[passerby][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][3] = m_Passerby[passerby][m_Key][nCnt][3] + (m_Passerby[passerby][0][nCnt][3] - m_Passerby[passerby][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][4] = m_Passerby[passerby][m_Key][nCnt][4] + (m_Passerby[passerby][0][nCnt][4] - m_Passerby[passerby][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][5] = m_Passerby[passerby][m_Key][nCnt][5] + (m_Passerby[passerby][0][nCnt][5] - m_Passerby[passerby][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else if (m_bMotionChange == true)
		{// ���[�V�����u�����h
			m_aOffset[nCnt][0] = m_aOffsetMemo[nCnt][0] + (m_Passerby[m_MotionType][0][nCnt][0] - m_aOffsetMemo[nCnt][0]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][1] = m_aOffsetMemo[nCnt][1] + (m_Passerby[m_MotionType][0][nCnt][1] - m_aOffsetMemo[nCnt][1]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][2] = m_aOffsetMemo[nCnt][2] + (m_Passerby[m_MotionType][0][nCnt][2] - m_aOffsetMemo[nCnt][2]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][3] = m_aOffsetMemo[nCnt][3] + (m_Passerby[m_MotionType][0][nCnt][3] - m_aOffsetMemo[nCnt][3]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][4] = m_aOffsetMemo[nCnt][4] + (m_Passerby[m_MotionType][0][nCnt][4] - m_aOffsetMemo[nCnt][4]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][5] = m_aOffsetMemo[nCnt][5] + (m_Passerby[m_MotionType][0][nCnt][5] - m_aOffsetMemo[nCnt][5]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else
		{// �ʏ펞
			m_aOffset[nCnt][0] = m_Passerby[passerby][m_Key][nCnt][0] + (m_Passerby[passerby][m_Key + 1][nCnt][0] - m_Passerby[passerby][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][1] = m_Passerby[passerby][m_Key][nCnt][1] + (m_Passerby[passerby][m_Key + 1][nCnt][1] - m_Passerby[passerby][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][2] = m_Passerby[passerby][m_Key][nCnt][2] + (m_Passerby[passerby][m_Key + 1][nCnt][2] - m_Passerby[passerby][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][3] = m_Passerby[passerby][m_Key][nCnt][3] + (m_Passerby[passerby][m_Key + 1][nCnt][3] - m_Passerby[passerby][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][4] = m_Passerby[passerby][m_Key][nCnt][4] + (m_Passerby[passerby][m_Key + 1][nCnt][4] - m_Passerby[passerby][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][5] = m_Passerby[passerby][m_Key][nCnt][5] + (m_Passerby[passerby][m_Key + 1][nCnt][5] - m_Passerby[passerby][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}

		if (m_bMotionChange == false)
		{// ���[�V�����ω��Ȃ�
			if (m_fCounterMotion == m_nFrame[passerby][m_Key])
			{// �L�[�t���[���I����
			 //if (m_MotionType != MOTIONTYPE_JUMP || m_Key != m_nNumKeyKeep - 2)
				{// �W�����v�ȊO�ŃL�[�I�������Z�b�g
					m_fCounterMotion = 0;
				}

				if (m_Key == m_nNumKeyKeep - 1 && m_bLoopKeep == true)
				{// ���[�v����Ƃ�
					m_Key = 0;
				}
				else if (m_Key == m_nNumKeyKeep - 2 && m_bLoopKeep == false)
				{// ���[�v�����I������Ƃ�
				 //if (m_MotionTypeOld != MOTIONTYPE_JUMP)
					{// ���O���W�����v�ȊO�̎�
						m_MotionType = MOTIONTYPE_PASSERBY_NEUTRAL;	// �j���[�g�����ֈڍs
						m_fCounterMotion = 0;
						break;
					}
				}
				else if (m_Key < m_nNumKeyKeep)
				{// �L�[�̃J�E���g
					m_Key++;
				}
			}
			else if (m_MotionTypeOld != m_MotionType)
			{// ���[�V�����ω���	
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = true;

				if (m_MotionTypeOld == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_NEUTRAL && m_MotionType == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_MOVE)
				{// �ړ�����j���[�g������
					m_nBrendFrame = 20;
				}
				else if (m_MotionTypeOld == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_MOVE && m_MotionType == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_NEUTRAL)
				{// �j���[�g��������ړ���
					m_nBrendFrame = 40;
				}
				break;
			}
		}
		else if (m_bMotionChange == true)
		{// ���[�V�����u�����h���̃J�E���g���Z�b�g
			if (m_fCounterMotion == m_nBrendFrame || m_MotionTypeOld != m_MotionType)
			{
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = false;
			}
		}
	}

	// ���O�̃��[�V������Ԃ̕ۑ�
	m_MotionTypeOld = m_MotionType;
}

//==============================================================================
// �O�ς𗘗p���������蔻��
//==============================================================================
bool CPasserby::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	bool bLand = false;

	D3DXVECTOR3 pos = *pPos;// ����Ώۂ̈ʒu

	//aPos�̒l�ݒ�
  	m_aPosColision[0] = D3DXVECTOR3(m_pos.x + m_MinPasserby.x, 0.0f, m_pos.z + m_MinPasserby.z);
	m_aPosColision[1] = D3DXVECTOR3(m_pos.x + m_MinPasserby.x, 0.0f, m_pos.z + m_MaxPasserby.z);
	m_aPosColision[2] = D3DXVECTOR3(m_pos.x + m_MaxPasserby.x, 0.0f, m_pos.z + m_MaxPasserby.z);
	m_aPosColision[3] = D3DXVECTOR3(m_pos.x + m_MaxPasserby.x, 0.0f, m_pos.z + m_MinPasserby.z);

	//�O�ς̎�
	m_aVec[0] = m_aPosColision[1] - m_aPosColision[0];
	m_aVec[1] = m_aPosColision[2] - m_aPosColision[1];
	m_aVec[2] = m_aPosColision[3] - m_aPosColision[2];
	m_aVec[3] = m_aPosColision[0] - m_aPosColision[3];

	//�l���_��
	for (int nCount = 0; nCount < 4; nCount++)
	{
		//�O�ς̎�2
		if (nCount == 0)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 1)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPosColision[nCount];
		}
		else if (nCount == 2)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 3)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPosColision[nCount];
		}

		m_fPlayerVec[nCount] = (m_aVec[nCount].z * m_bVec[nCount].x) - (m_aVec[nCount].x * m_bVec[nCount].z);

   		if (pPos->y <= m_pos.y + m_MaxPasserby.y - fHeightMin && pPos->y > m_pos.y + m_MinPasserby.y - fHeightMax)
		{
			if (m_fPlayerVec[0] > 0.0f && m_fPlayerVec[1] > 0.0f && m_fPlayerVec[2] > 0.0f && m_fPlayerVec[3] > 0.0f)
			{
				pMove->x = 0.0f;
			}
		}
	}

	return bLand;
}

//==============================================================================
// �v���C���[�Ƃ̓����蔻��
//==============================================================================
void CPasserby::CollisionPlayer(void)
{
	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 playerpos = CPlayer3D::GetPosition();

	if (m_bParticle2 == false)
	{
		//�e�Ƃ̓����蔻��
		if (m_pos.x + 40.0f >= playerpos.x && m_pos.x - 40.0f <= playerpos.x&&
			m_pos.z + 40.0f >= playerpos.z && m_pos.z - 40.0f <= playerpos.z)
		{
			//�X�R�A������
			CScore::SetScore(0);

			//�G�̖ړI�̐�������������
			CPurpose::SetPurpose(CGame::GetEnemyPurpose());

			//���Z�b�g�̃t���O�Ǘ�
			m_bParticle2 = true;
			m_bParticle3 = true;
		}
	}

	if (m_bParticle2 == true && m_bParticle3 == true)
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
			//�̗͂����炷
			m_pHp[CGame::GetLife()]->Uninit();
			CGame::SetLife(1);

			//�G�l�~�[�J�E���g�����Z�b�g
			CGame::SetCount(0);

			//����������
			m_bReset = false;
			m_bParticle2 = false;
			m_nParticlecount = 0;
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
							//�G�̔j��
							pScene->Uninit();
						}
					}
				}
			}
		}
		//���C�t��0�ɂȂ�����
		else if (CGame::GetLife() == 0)
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
							//�G�̔j��
							pScene->Uninit();
						}
					}
				}
			}

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
