//=============================================================================
//
// ���C������ [object3D.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "player3D.h"
#include "scene3D.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
D3DXVECTOR3		CObject3D::m_rotStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3		CObject3D::m_moveStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXMATRIX      CObject3D::m_mtxWorld;

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
int g_nTypeObject = -1;
int CObject3D::m_nType;
D3DXVECTOR3 g_posObjectVector[256][4];

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject3D::CObject3D(int nPriority, int nType)
{
	m_fUseScene3D = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	//if (m_nType == 0)
	{
		//D3DXLoadMeshFromX("data/MODEL/ob.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
		D3DXLoadMeshFromX("data/MODEL/city001.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}

	//�ʒu
	m_pos = pos;

	//����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ffRot = m_rot.y;
	m_move = m_moveStatic;

	BYTE *pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y

	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		m_vtx = *(D3DXVECTOR3*)pVtxBuff;//���_���W�̑��

		if (m_vtx.x > m_MaxStone.x)
		{
			m_MaxStone.x = m_vtx.x;
		}

		if (m_vtx.x < m_MinStone.x)
		{
			m_MinStone.x = m_vtx.x;
		}

		if (m_vtx.y > m_MaxStone.y)
		{
			m_MaxStone.y = m_vtx.y;
		}

		if (m_vtx.y < m_MinStone.y)
		{
			m_MinStone.y = m_vtx.y;
		}

		if (m_vtx.z > m_MaxStone.z)
		{
			m_MaxStone.z = m_vtx.z;
		}

		if (m_vtx.z < m_MinStone.z)
		{
			m_MinStone.z = m_vtx.z;
		}

		pVtxBuff += sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}
	if (m_MaxStone.x < 0.0f)
	{
		m_MaxStone.x *= -1.0f;
	}
	if (m_MaxStone.y < 0.0f)
	{
		m_MaxStone.y *= -1.0f;
	}
	if (m_MaxStone.z < 0.0f)
	{
		m_MaxStone.z *= -1.0f;
	}

	if (m_MinStone.x < 0.0f)
	{
		m_MinStone.x *= -1.0f;
	}
	if (m_MinStone.y < 0.0f)
	{
		m_MinStone.y *= -1.0f;
	}
	if (m_MinStone.z < 0.0f)
	{
		m_MinStone.z *= -1.0f;
	}

	m_posMaxBase = m_MaxStone;
	m_posMinBase = m_MinStone;

	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject3D::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject3D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject3D::Draw(void)
{
	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans, revmat;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ�
	D3DXMATERIAL *pMat;				//�}�e���A���ւ̃|�C���^

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����̔��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//m_posMaxBase�ɋt�s���������
	//D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);
	//D3DXVec3TransformCoord(&m_MaxStone, &m_posMaxBase, &mtxRot);
	//D3DXVec3TransformCoord(&m_MinStone, &m_posMinBase, &mtxRot);

	//�ʒu�̔��]
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_MaxStone.x < 0.0f)
	{
		m_MaxStone.x *= -1.0f;
	}
	if (m_MaxStone.y < 0.0f)
	{
		m_MaxStone.y *= -1.0f;
	}
	if (m_MaxStone.z < 0.0f)
	{
		m_MaxStone.z *= -1.0f;
	}

	if (m_MinStone.x < 0.0f)
	{
		m_MinStone.x *= -1.0f;
	}
	if (m_MinStone.y < 0.0f)
	{
		m_MinStone.y *= -1.0f;
	}
	if (m_MinStone.z < 0.0f)
	{
		m_MinStone.z *= -1.0f;
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A���̎擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ��������
//=============================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nType)
{
	//���f���𐶐�
	CObject3D *pObject3D = NULL;
	pObject3D = new CObject3D;

	//NULL�`�F�b�N
	if (pObject3D != NULL)
	{
		m_nType = nType;
		m_moveStatic = move;
		m_rotStatic = rot;
		pObject3D->Init(pos, 0.0f, 0.0f);
	}

	return pObject3D;
}

//==============================================================================
// �O�ς𗘗p���������蔻��
//==============================================================================
bool CObject3D::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// �J�������̎擾
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetRot();

	bool bLand = false;

	D3DXVECTOR3 g_Vec[18][4];
	D3DXVECTOR3 g_oldVec[18][4];
	float PlayerVec[18][4];
	float oldPlayerVec[18][4];
	D3DXVECTOR3 posold;

	D3DXVECTOR3 pos = *pPos;			// ����Ώۂ̈ʒu
	posold = *pPosOld;					// ����Ώۂ̈ʒu

	//�ǂݍ��񂾍��W�����Ƃɓ����蔻������
	for (int nCnt = 0; nCnt < g_FailCntDateObjectVector; nCnt++)
	{
		m_aPos[nCnt][0] = g_posObjectVector[nCnt][0];
		m_aPos[nCnt][1] = g_posObjectVector[nCnt][1];
		m_aPos[nCnt][2] = g_posObjectVector[nCnt][2];
		m_aPos[nCnt][3] = g_posObjectVector[nCnt][3];

		//�O�ς̎��@�x�N�g��
		m_Vec[nCnt][0] = m_aPos[nCnt][1] - m_aPos[nCnt][0];
		m_Vec[nCnt][1] = m_aPos[nCnt][2] - m_aPos[nCnt][1];
		m_Vec[nCnt][2] = m_aPos[nCnt][3] - m_aPos[nCnt][2];
		m_Vec[nCnt][3] = m_aPos[nCnt][0] - m_aPos[nCnt][3];

		//�l���_��
		for (int nCount = 0; nCount < 4; nCount++)
		{
			//�O�ς̎�2
			if (nCount == 0)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 1)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 2)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 3)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
			}

			//�O�όv�Z
			PlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_Vec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_Vec[nCnt][nCount].z);
		}
		
		if (pPos->y <= m_pos.y + m_MaxStone.y - fHeightMin && pPos->y > m_pos.y - m_MinStone.y - fHeightMax)
		{
			if (PlayerVec[nCnt][0] > 0.0f && PlayerVec[nCnt][1] > 0.0f && PlayerVec[nCnt][2] > 0.0f && PlayerVec[nCnt][3] > 0.0f)
			{
				for (int nCount = 0; nCount < 4; nCount++)
				{
					//�O�ς̎�2
					if (nCount == 0)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 1)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 2)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 3)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
					}

					oldPlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_oldVec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_oldVec[nCnt][nCount].z);
				}

				//���E�̓����蔻��
				if (PlayerVec[nCnt][1] > 0 && PlayerVec[nCnt][3] > 0)
				{
					//��
					if (oldPlayerVec[nCnt][0] <= 0 && PlayerVec[nCnt][0] >= 0)
					{
						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][1];

						//�p�x���v�Z
						float nAtan2 = (float)atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//�Εӂ̏ꍇ
						{
							pPos->z = pPosOld->z;				//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;					//�ړ��ʂ��[���ɂ���
							pMove->x = sinf(nAtan2) * 3.0f;
						}
						else if (nAtan2 == 0.0f)				//�����̏ꍇ
						{
							pPos->x = pPosOld->x;
							pMove->x = sinf(nAtan2 - rot.y);
						}
					}

					//�E
					if (oldPlayerVec[nCnt][2] <= 0 && PlayerVec[nCnt][2] >= 0)
					{
						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][3] - m_aPos[nCnt][2];

						//�p�x���v�Z
						float nAtan2 = (float)atan2(a.z, a.x);			//�Εӂ̏ꍇ

						if (nAtan2 > 0.0f)						//1�t���[���O�̈ʒu�ɖ߂�
						{
							pPos->z = pPosOld->z;				//�ړ��ʂ��[���ɂ���
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;
							pMove->x = sinf(-nAtan2) * 5.0f;	//�����̏ꍇ
						}
						else if (nAtan2 == 0.0f)
						{
							pPos->x = pPosOld->x;
							pMove->x = sinf(nAtan2 - rot.y);
						}
					}
				}

				//�O��̓����蔻��
				if (PlayerVec[nCnt][0] > 0 && PlayerVec[nCnt][2] > 0)
				{
					//��
					if (oldPlayerVec[nCnt][1] <= 0 && PlayerVec[nCnt][1] >= 0)
					{
						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][1] - m_aPos[nCnt][2];

						//�p�x���v�Z
						float nAtan2 = (float)atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//�Εӂ̏ꍇ
						{
							pPos->z = pPosOld->z;				//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;					//�ړ��ʂ��[���ɂ���
							pMove->z = sinf(-nAtan2) * 10.0f;
						}
						//�����̏ꍇ
						else if (nAtan2 == 0.0f)
						{
							pPos->z = pPosOld->z;
							pMove->z = sinf(nAtan2 - rot.y);
						}
					}

					//�O
					if (oldPlayerVec[nCnt][3] <= 0 && PlayerVec[nCnt][3] >= 0)
					{
						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][3];

						//�p�x���v�Z
						float nAtan2 = (float)atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//�Εӂ̏ꍇ
						{
							pPos->z = pPosOld->z;				//1�t���[���O�̈ʒu�ɖ߂�
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;					//�ړ��ʂ��[���ɂ���
							pMove->z = cosf(nAtan2) * 5.0f;
						}
						else if (nAtan2 == 0.0f)				//�����̏ꍇ
						{
							pPos->z = pPosOld->z;
							pMove->z = sinf(nAtan2 - rot.y);
						}
					}
				}
			}
		}
	}
	//����
	if (m_fUseScene3D == false)
	{
		//�ǂݍ��񂾍��W�����Ƃɓ����蔻������
		for (int nCnt = 0; nCnt < g_FailCntDateObjectVector; nCnt++)
		{
			CScene3D::Create(D3DXVECTOR3(0, 0, 0), g_posObjectVector[nCnt][0], g_posObjectVector[nCnt][1], g_posObjectVector[nCnt][3], g_posObjectVector[nCnt][2]);

			//if (nCnt == 18)
			{
				m_fUseScene3D = true;
			}
		}
	}
	
	return bLand;
}

//==============================================================================
//X�t�@�C���̓ǂݍ���
//==============================================================================
void CObject3D::LoadObject(void)
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

	pFile = fopen("data/TEXT/VectorGet.txt", "r");

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
				if (strcmp("VECTORSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_VECTORSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Vec1POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][0].x);
						}

						if (strcmp("Vec1POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][0].y);
						}

						if (strcmp("Vec1POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][0].z);
						}

						if (strcmp("Vec2POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][1].x);
						}

						if (strcmp("Vec2POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][1].y);
						}

						if (strcmp("Vec2POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][1].z);
						}

						if (strcmp("Vec3POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][2].x);
						}

						if (strcmp("Vec3POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][2].y);
						}

						if (strcmp("Vec3POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][2].z);
						}

						if (strcmp("Vec4POSX", pSetObjectEND) == 0)
						{
							// ���f��posX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][3].x);
						}

						if (strcmp("Vec4POSY", pSetObjectEND) == 0)
						{
							// ���f��posY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][3].y);
						}

						if (strcmp("Vec4POSZ", pSetObjectEND) == 0)
						{
							// ���f��posZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][3].z);
						}
					}
					g_FailCntDateObjectVector++;
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
	fclose(pFile);
}

D3DXMATRIX CObject3D::GetMtxWorld(void)
{
	return m_mtxWorld;
}
