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
#include "camera.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject3D::CObject3D(int nPriority, int nType)
{
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
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeY)
{
	//����������
	CSceneX::Init(pos, fSizeX, fSizeY);

	// ���[�J���ϐ��錾
	int nNumVtx;			// ���_��
	DWORD sizeFVF;			// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = CSceneX::GetMesh()->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	sizeFVF = D3DXGetFVFVertexSize(CSceneX::GetMesh()->GetFVF());

	// ���_�o�b�t�@�����b�N
	CSceneX::GetMesh()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_���W�̔�r�ƍX�V
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// �e���W�̍ő�l�̔�r
		if (m_MaxObject.x < vtx.x)
		{
			m_MaxObject.x = vtx.x;
		}
		if (m_MaxObject.y < vtx.y)
		{
			m_MaxObject.y = vtx.y;
		}
		if (m_MaxObject.z < vtx.z)
		{
			m_MaxObject.z = vtx.z;
		}

		// �e���W�̍ŏ��l�̔�r
		if (m_MinObject.x > vtx.x)
		{
			m_MinObject.x = vtx.x;
		}
		if (m_MinObject.y > vtx.y)
		{
			m_MinObject.y = vtx.y;
		}
		if (m_MinObject.z > vtx.z)
		{
			m_MinObject.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	CSceneX::GetMesh()->UnlockVertexBuffer();

	//����������
	m_pos = pos;
	m_rot = rot;

	//���f���̉�]�ݒ�
	SetRevolution(m_rot);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject3D::Uninit(void)
{
	//�I������
	CSceneX::Uninit();

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
	if (m_MaxObject.x < 0.0f)
	{
		m_MaxObject.x *= -1.0f;
	}
	if (m_MaxObject.y < 0.0f)
	{
		m_MaxObject.y *= -1.0f;
	}
	if (m_MaxObject.z < 0.0f)
	{
		m_MaxObject.z *= -1.0f;
	}

	if (m_MinObject.x < 0.0f)
	{
		m_MinObject.x *= -1.0f;
	}
	if (m_MinObject.y < 0.0f)
	{
		m_MinObject.y *= -1.0f;
	}
	if (m_MinObject.z < 0.0f)
	{
		m_MinObject.z *= -1.0f;
	}

	//�`�揈��
	CSceneX::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char * TextName,char * TextureName)
{
	//���f���𐶐�
	CObject3D *pObject3D = NULL;
	pObject3D = new CObject3D;

	//NULL�`�F�b�N
	if (pObject3D != NULL)
	{
		//X�t�@�C���ݒ�
		pObject3D->BindTextName(TextName);

		//�e�N�X�`���ݒ�
		pObject3D->BindTextureName(TextureName);

		//�I�u�W�F�N�g�̏���������
		pObject3D->Init(pos, rot, 0.0f, 0.0f);
	}

	return pObject3D;
}

//==============================================================================
// �O�ς𗘗p���������蔻��
//==============================================================================
bool CObject3D::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// ���[�J���ϐ�

	CCamera *pCamera = CManager::GetCamera();	// �J�������̎擾
	D3DXVECTOR3 rot = pCamera->GetRot();		// �J�����p�x
	D3DXVECTOR3 CameraPos = pCamera->GetposV();	// �J�����̈ʒu
	bool bLand = false;

	D3DXVECTOR3		g_Vec[4];			// �x�N�g��
	D3DXVECTOR3		g_oldVec[4];		// �O�̃x�N�g��
	D3DXVECTOR3		posold;				// �O�̈ʒu
	float			PlayerVec[4];		// �v���C���[�x�N�g��
	float			oldPlayerVec[4];	// �O�̃v���C���[�x�N�g��

	D3DXVECTOR3 pos = *pPos;			// ����Ώۂ̈ʒu
	posold = *pPosOld;					// ����Ώۂ̈ʒu

	//�ǂ���x�N�g���v�Z�p
	D3DXVECTOR3 VecP = *pPos - *pPosOld;						// �i�s�x�N�g��
	fabsf(VecP.x);												// ��Βl
	fabsf(VecP.z);												// ��Βl
	D3DXVec3Normalize(&VecP, &VecP);

	float fLength[4];
	fLength[0] = sqrtf(m_MinObject.x * m_MinObject.x + m_MinObject.z * m_MinObject.z);
	fLength[1] = sqrtf(m_MinObject.x * m_MinObject.x + m_MaxObject.z * m_MaxObject.z);
	fLength[2] = sqrtf(m_MaxObject.x * m_MaxObject.x + m_MaxObject.z * m_MaxObject.z);
	fLength[3] = sqrtf(m_MaxObject.x * m_MaxObject.x + m_MinObject.z * m_MinObject.z);

	// �Ίp���p�x
	float fAngle[4];
	fAngle[0] = atan2f(m_MinObject.z, m_MinObject.x);
	fAngle[1] = atan2f(m_MinObject.z, m_MaxObject.x);
	fAngle[2] = atan2f(m_MaxObject.z, m_MaxObject.x);
	fAngle[3] = atan2f(m_MaxObject.z, m_MinObject.x);

	//���_�̍��W
	m_aPosColision[0].x = (m_pos.x + cosf(fAngle[1] - m_rot.y) * fLength[2]);
	m_aPosColision[0].z = (m_pos.z + sinf(fAngle[1] - m_rot.y) * fLength[2]);

	m_aPosColision[1].x = (m_pos.x + cosf(fAngle[2] + m_rot.y) * fLength[3]);
	m_aPosColision[1].z = (m_pos.z - sinf(fAngle[2] + m_rot.y) * fLength[3]);

	m_aPosColision[2].x = (m_pos.x - cosf(fAngle[3] - m_rot.y) * fLength[0]);
	m_aPosColision[2].z = (m_pos.z - sinf(fAngle[3] - m_rot.y) * fLength[0]);

	m_aPosColision[3].x = (m_pos.x - cosf(fAngle[0] + m_rot.y) * fLength[1]);
	m_aPosColision[3].z = (m_pos.z + sinf(fAngle[0] + m_rot.y) * fLength[1]);

	//���ѕ���������
	//90����Ă���ꍇ
	if (m_aPosColision[0].x > m_aPosColision[2].x && m_aPosColision[0].z < m_aPosColision[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPosColision[0];

		m_aPosColision[0] = m_aPosColision[3];
		m_aPosColision[3] = m_aPosColision[2];
		m_aPosColision[2] = m_aPosColision[1];
		m_aPosColision[1] = vecold;
	}
	//180����Ă���ꍇ
	else if (m_aPosColision[0].x < m_aPosColision[2].x && m_aPosColision[0].z < m_aPosColision[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPosColision[0];

		D3DXVECTOR3 vecold2;
		vecold2 = m_aPosColision[1];

		m_aPosColision[0] = m_aPosColision[2];
		m_aPosColision[2] = vecold;
		m_aPosColision[1] = m_aPosColision[3];
		m_aPosColision[3] = vecold2;
	}
	//270����Ă���ꍇ
	else if (m_aPosColision[0].x < m_aPosColision[2].x && m_aPosColision[0].z > m_aPosColision[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPosColision[0];

		m_aPosColision[0] = m_aPosColision[1];
		m_aPosColision[1] = m_aPosColision[2];
		m_aPosColision[2] = m_aPosColision[3];
		m_aPosColision[3] = vecold;
	}

	//�O�ς̎�
	m_aVec[0] = m_aPosColision[1] - m_aPosColision[0];	// �E
	m_aVec[1] = m_aPosColision[2] - m_aPosColision[1];	// ��								 
	m_aVec[2] = m_aPosColision[3] - m_aPosColision[2];	// ��
	m_aVec[3] = m_aPosColision[0] - m_aPosColision[3];	// ��

	for (int nCount = 0; nCount < 4; nCount++)
	{
		// �v���C���[�x�N�g���ۑ�
		if (nCount == 0)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 1)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPosColision[nCount];
		}
		else if (nCount == 2)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 3)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPosColision[nCount];
		}

		// �v���C���[��m_Vec���E�ɂ��邩�ǂ����̔���p
		oldPlayerVec[nCount] = (m_aVec[nCount].z * g_oldVec[nCount].x) - (m_aVec[nCount].x * g_oldVec[nCount].z);
	}
	for (int nCount = 0; nCount < 4; nCount++)
	{
		//�O�ς̎�2		
		if (nCount == 0)
		{// �E�ォ��v���C���[�̃x�N�g��
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 1)
		{// �E���v���C���[�̃x�N�g��
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPosColision[nCount];
		}
		else if (nCount == 2)
		{// �����v���C���[�̃x�N�g��
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 3)
		{// ����v���C���[�̃x�N�g��
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPosColision[nCount];
		}

		// �v���C���[��m_Vec���E�ɂ��邩�ǂ����̔���p
		PlayerVec[nCount] = (m_aVec[nCount].z * g_Vec[nCount].x) - (m_aVec[nCount].x * g_Vec[nCount].z);
	}

	if (pPos->y <= m_pos.y + m_MaxObject.y - fHeightMin && pPos->y > m_pos.y - m_MinObject.y - fHeightMax)
	{
		if (PlayerVec[0] > 0.0f && PlayerVec[1] > 0.0f && PlayerVec[2] > 0.0f && PlayerVec[3] > 0.0f)
		{
			// �㉺�̔���@�i�Ԃɂ��邩�ǂ����j
			if (PlayerVec[3] > 0.0f || PlayerVec[1] > 0.0f)
			{// �O�̃x�N�g����0��艺�̎�
				if (oldPlayerVec[0] < 0.0f)
				{// �E�̓����蔻��			
					if (PlayerVec[0] > 0.0f)
					{
						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPosColision[0] - m_aPosColision[1];

						//�p�x���v�Z
						float fA = atan2f(a.z, a.x);

						//�ǂ���x�N�g���v�Z�p
						D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �@���x�N�g��
						D3DXVec3Cross(&norVec, &a, &m_rot);
						D3DXVec3Normalize(&norVec, &norVec);
						fabsf(norVec.x);											// ��Βl
						fabsf(norVec.z);											// ��Βl
																					//norVec.y = 0;

						D3DXVECTOR3 Wvec;											// �ǂ���x�N�g��
						Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);

						if (m_rot.y == 0.0f)
						{
							pPos->x = pPosOld->x;				// 1�t���[���O�̈ʒu�ɖ߂�																//pPos->z = pPosOld->z;
							pMove->x = 0.0f;
						}
						else
						{
							pPos->x = pPosOld->x;				// 1�t���[���O�̈ʒu�ɖ߂�
							pPos->z = pPosOld->z;
							pMove->x = (Wvec.x) * 5;
							pMove->z = (Wvec.z) * 5;
						}
						bLand = true;

					}
				}
				// ���̓����蔻��
				if (oldPlayerVec[2] < 0.0f)
				{
					if (PlayerVec[2] > 0.0f)
					{// ��

						//2���_�����v�Z
						D3DXVECTOR3 a = m_aPosColision[2] - m_aPosColision[3];

						//�p�x���v�Z
						float fAtan2 = atan2f(a.z, a.x);

						//�ǂ���x�N�g���v�Z�p
						D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �@���x�N�g��
						D3DXVec3Cross(&norVec, &a, &m_rot);
						D3DXVec3Normalize(&norVec, &norVec);
						fabsf(norVec.x);											// ��Βl
						fabsf(norVec.z);											// ��Βl
																					//norVec.y = 0;
						D3DXVECTOR3 Wvec;											// �ǂ���x�N�g��
						Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);

						if (m_rot.y == 0.0f)
						{
							pPos->x = pPosOld->x;				// 1�t���[���O�̈ʒu�ɖ߂�																//pPos->z = pPosOld->z;
							pMove->x = 0.0f;
						}
						else
						{
							pPos->x = pPosOld->x;				// 1�t���[���O�̈ʒu�ɖ߂�
							pPos->z = pPosOld->z;
							pMove->x = (Wvec.x) * 5;
							pMove->z = (Wvec.z) * 5;
						}

						bLand = true;
					}
				}

				// ���E�ɐG��Ă��Ȃ��Ƃ�
				if (PlayerVec[0] > 0.0f || PlayerVec[2] > 0.0f)
				{
					if (oldPlayerVec[3] < 0.0f)
					{// ��̓����蔻��			
						if (PlayerVec[3] > 0.0f)
						{

							//2���_�����v�Z
							D3DXVECTOR3 a = m_aPosColision[0] - m_aPosColision[3];

							//�p�x���v�Z
							float fAtan2 = (float)atan2(a.z, a.x);
							//�ǂ���x�N�g���v�Z�p
							D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �@���x�N�g��
							D3DXVec3Cross(&norVec, &a, &m_rot);
							D3DXVec3Normalize(&norVec, &norVec);
							fabsf(norVec.x);											// ��Βl
							fabsf(norVec.z);											// ��Βl
																						//norVec.y = 0;
							D3DXVECTOR3 Wvec;											// �ǂ���x�N�g��
							Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);

							if (m_rot.y == 0.0f)
							{
								pPos->z = pPosOld->z;				// 1�t���[���O�̈ʒu�ɖ߂�																//pPos->z = pPosOld->z;
								pMove->z = 0.0f;
							}
							else
							{

								pPos->x = pPosOld->x;				// 1�t���[���O�̈ʒu�ɖ߂�
								pPos->z = pPosOld->z;
								pMove->x = (Wvec.x) * 5;
								pMove->z = (Wvec.z) * 5;
							}
							bLand = true;
						}
					}
					if (oldPlayerVec[1] < 0.0f)
					{// ���̓����蔻��			
						if (PlayerVec[1] > 0.0f)
						{// 
							//2���_�����v�Z
							D3DXVECTOR3 a = m_aPosColision[2] - m_aPosColision[1];

							//�p�x���v�Z
							float fAtan2 = (float)atan2(a.z, a.x);

							//�ǂ���x�N�g���v�Z�p
							D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �@���x�N�g��
							D3DXVec3Cross(&norVec, &a, &m_rot);							// 2��3D�x�N�g���̐ϊԂ�����
							D3DXVec3Normalize(&norVec, &norVec);						// 3D�x�N�^�[�̐��K��
							fabsf(norVec.x);											// ��Βl
							fabsf(norVec.z);											// ��Βl

							D3DXVECTOR3 Wvec;											// �ǂ���x�N�g��
							Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);		// �ǂ���x�N�g���v�Z

							if (m_rot.y == 0.0f)
							{
								pPos->z = pPosOld->z;				// 1�t���[���O�̈ʒu�ɖ߂�																//pPos->z = pPosOld->z;
								pMove->z = 0.0f;
							}
							else
							{
								pPos->x = pPosOld->x;				// 1�t���[���O�̈ʒu�ɖ߂�
								pPos->z = pPosOld->z;
								pMove->x = (Wvec.x) * 5;
								pMove->z = (Wvec.z) * 5;
							}
							bLand = true;
						}
					}
				}
			}
		}
	}

	return bLand;
}
