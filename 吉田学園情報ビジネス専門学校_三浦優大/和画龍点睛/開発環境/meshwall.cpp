//================================================
//
// ���b�V���E�H�[������[meshwall.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�}�N����`
//=================================================
#define WALL_POINT_MAX	(65535)		// ���_���̍ő�
#define WALL_IDX_MAX	(65535)		// �C���f�b�N�X�o�b�t�@�̍ő�m�ې�

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "meshwall.h"
#include "manager.h"
#include "Renderer.h"

//=================================================
//�R���X�g���N�^
//=================================================
CMeshwall::CMeshwall()
{
}

//=================================================
//�f�X�g���N�^
//=================================================
CMeshwall::~CMeshwall()
{
}

//=================================================
//���b�V���E�H�[���̏���������
//=================================================
HRESULT CMeshwall::InitMeshwall(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	int nCount = 0;

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/walltex.jpg",
		&m_pTextureMeshwall);

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_meshwall[nCnt].m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_meshwall[nCnt].m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_meshwall[nCnt].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_meshwall[nCnt].m_fWidth = 0.0f;
		m_meshwall[nCnt].m_fHeight = 0.0f;
		m_meshwall[nCnt].m_fWidthMax = 0.0f;
		m_meshwall[nCnt].m_fHeightMax = 0.0f;
		m_meshwall[nCnt].m_nAllPoint = 0;
		m_meshwall[nCnt].m_nPolygon = 0;
		m_meshwall[nCnt].m_nIdxPoint = 0;
		m_meshwall[nCnt].m_nWidth = 0;
		m_meshwall[nCnt].m_nHeight = 0;
		m_meshwall[nCnt].m_bUse = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * WALL_IDX_MAX * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMeshwall,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);

	// ����������
	for (int nCntWall = 0; nCntWall < 2; nCntWall++)
	{
		for (int nCnt = 0; nCnt < m_meshwall[nCntWall].m_nHeight - 1; nCnt++)
		{
			for (int nCntA = 0; nCntA < m_meshwall[nCntWall].m_nWidth - 1; nCntA++, pVtx++)
			{
				// �|���S���̊e���_���W
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// �e���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

				// �e�N�X�`�����_���̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMeshwall->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * WALL_POINT_MAX * 2,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffMeshwall,
		NULL);

	return S_OK;
}

//=================================================
//���b�V���E�H�[���̏I������
//=================================================
void CMeshwall::UninitMeshwall(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuffMeshwall != NULL)
	{
		m_pVtxBuffMeshwall->Release();
		m_pVtxBuffMeshwall = NULL;
	}

	// �e�N�X�`���̊J��
	if (m_pTextureMeshwall != NULL)
	{
		m_pTextureMeshwall->Release();
		m_pTextureMeshwall = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuffMeshwall != NULL)
	{
		m_pIdxBuffMeshwall->Release();
		m_pIdxBuffMeshwall = NULL;
	}
}

//=================================================
//���b�V���E�H�[���̍X�V����
//=================================================
void CMeshwall::UpdateMeshwall(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;
	WORD *pIdx;

	anCnt -= 0.008f;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuffMeshwall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMax = 0; nCntMax < 2; nCntMax++)
	{
		if (m_meshwall[nCntMax].m_bUse == true)
		{
			for (int nCntWall = 0; nCntWall < m_meshwall[nCntMax].m_nHeightPoint; nCntWall++)
			{
				for (int nCnt = 0; nCnt < m_meshwall[nCntMax].m_nWidthPoint; nCnt++, pVtx++)
				{
					pVtx[0].pos = D3DXVECTOR3(
						-m_meshwall[nCntMax].m_fWidthMax / 2.0f + (float)nCnt * m_meshwall[nCntMax].m_fWidth,
						m_meshwall[nCntMax].m_fHeightMax / 2.0f - (float)nCntWall * m_meshwall[nCntMax].m_fHeight,
						0.0f);

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// �e���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					// �e�N�X�`�����_���̐ݒ�
					pVtx[0].tex = D3DXVECTOR2((0.5f * nCnt) + anCnt, 0.5f * nCntWall);
				}
			}
		}

		for (int nCntA = 0; nCntA < m_meshwall[nCntMax].m_nHeight; nCntA++)
		{
			for (int nCnt = 0; nCnt < (m_meshwall[nCntMax].m_nWidthPoint + 1); nCnt++)
			{
				if (nCnt != 0 && nCnt == m_meshwall[nCntMax].m_nWidthPoint && nCntA != m_meshwall[nCntMax].m_nHeight - 1)
				{// �E�[����܂�Ԃ���
					pIdx[0] = pIdx[-1];
					pIdx[1] = pIdx[-2] + 1;
				}
				else if (nCntA == m_meshwall[nCntMax].m_nHeight - 1 && nCnt == m_meshwall[nCntMax].m_nWidthPoint)
				{// �I�����ɖ�������
					break;
				}
				else
				{// �ʏ�z�u
					pIdx[0] = m_meshwall[nCntMax].m_nWidthPoint + (m_meshwall[nCntMax].m_nWidthPoint * nCntA) + nCnt;
					pIdx[1] = pIdx[0] - (m_meshwall[nCntMax].m_nWidthPoint);
				}
				pIdx += 2;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuffMeshwall->Unlock();

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMeshwall->Unlock();
}

//=================================================
//���b�V���E�H�[���̕`�揈��
//=================================================
void CMeshwall::DrawMeshwall(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�̃|�C���^
									
	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

		if (m_meshwall[nCnt].m_bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_meshwall[nCnt].m_mtxWorld);

			// �����̔��f
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				m_meshwall[nCnt].m_rot.y,
				m_meshwall[nCnt].m_rot.x,
				m_meshwall[nCnt].m_rot.z);

			D3DXMatrixMultiply(
				&m_meshwall[nCnt].m_mtxWorld,
				&m_meshwall[nCnt].m_mtxWorld,
				&mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(
				&mtxTrans,
				m_meshwall[nCnt].m_pos.x,
				m_meshwall[nCnt].m_pos.y,
				m_meshwall[nCnt].m_pos.z);

			D3DXMatrixMultiply(
				&m_meshwall[nCnt].m_mtxWorld,
				&m_meshwall[nCnt].m_mtxWorld,
				&mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &m_meshwall[nCnt].m_mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuffMeshwall, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(m_pIdxBuffMeshwall);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTextureMeshwall);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				0,
				0,
				m_meshwall[nCnt].m_nIdxPoint/*g_nIdxPointWall*/,				// ���_��
				0,
				m_meshwall[nCnt].m_nPolygon/*g_nPolygonWall*/);			// �v���~�e�B�u��
		}
	}
}

void CMeshwall::SetMeshwall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nWidth, int nHeight)
{
	// ���[�J���ϐ��錾
	Meshwall *pMeshwall;
	pMeshwall = &m_meshwall[0];

	// �ǂ̐ݒ�
	for (int nCntWall = 0; nCntWall < 2; nCntWall++, pMeshwall++)
	{
		if (pMeshwall->m_bUse == false)
		{
			pMeshwall->m_pos = pos;				// �ʒu

			pMeshwall->m_rot = rot;				// ����

			pMeshwall->m_fWidth = fWidth;			// ��

			pMeshwall->m_fHeight = fHeight;		// ����

			pMeshwall->m_nWidth = nWidth;			// ��̐�

			pMeshwall->m_nHeight = nHeight;		// �s�̐�

			pMeshwall->m_nWidthPoint = nWidth + 1;		// ���̒��_��

			pMeshwall->m_nHeightPoint = nHeight + 1;		// �����̒��_��

			pMeshwall->m_fWidthMax = pMeshwall->m_fWidth * (float)nWidth;

			pMeshwall->m_fHeightMax = pMeshwall->m_fHeight * (float)nHeight;

			pMeshwall->m_nAllPoint = nHeight * 2 * (nWidth + 2) - 2;
			pMeshwall->m_nPolygon = nWidth * nHeight * 2 + (4 * (nHeight - 1));
			pMeshwall->m_nIdxPoint = (nWidth + 1) * (nHeight + 1);

			pMeshwall->m_bUse = true;			// �g�p���
			break;
		}
	}
}
