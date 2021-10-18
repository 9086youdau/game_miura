//================================================
//
// ���b�V���t�B�[���h����[meshfield.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "meshfield.h"
#include "manager.h"
#include "Renderer.h"

CMeshfield::CMeshfield()
{
}

CMeshfield::~CMeshfield()
{
}

HRESULT CMeshfield::InitMeshfield(void)
{
	// ���[�J���ϐ��錾
	VERTEX_3D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/mesh.jpg",
		&m_pTextureMeshfield);

	// �ϐ��̏�����
	m_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nAllPoint = DEPTHNUM * 2 * (WIDTHNUM + 2) - 2;				// �����_��
	m_nPolygon = WIDTHNUM * DEPTHNUM * 2 + (4 * (DEPTHNUM - 1));	// ���|���S����
	m_nIdxPoint = m_nWidth * m_nDepth;								// �C���f�b�N�X�o�b�t�@�ł̒��_��


	m_fWidthMax = m_fWidth * (float)WIDTHNUM;
	m_fDepthMax = m_fDepth * (float)DEPTHNUM;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nIdxPoint,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMeshfield,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepth; nCnt++)
	{// ���s��
		for (int nCntA = 0; nCntA < m_nWidth; nCntA++)
		{// ����
		 // �|���S���̊e���_���W
			pVtx[0].pos = D3DXVECTOR3(-m_fWidthMax / 2.0f + (float)nCntA * m_fWidth, m_posMeshfield.y, m_fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �e���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����_���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * nCntA, (0.1f * nCnt) + m_meshcount);

			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMeshfield->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nAllPoint,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffMeshfield,
		NULL);

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntA = 0; nCntA < DEPTHNUM; nCntA++)
	{
		for (int nCnt = 0; nCnt < m_nWidth + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == m_nWidth && nCntA != DEPTHNUM - 1)
			{// �E�[����܂�Ԃ���
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == DEPTHNUM - 1 && nCnt == m_nWidth)
			{// �I�����ɖ�������
				break;
			}
			else
			{// �ʏ�z�u
				pIdx[0] = m_nWidth + (m_nWidth * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - m_nWidth;
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuffMeshfield->Unlock();

	return S_OK;
}

void CMeshfield::UninitMeshfield(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuffMeshfield != NULL)
	{
		m_pVtxBuffMeshfield->Release();
		m_pVtxBuffMeshfield = NULL;
	}

	// �e�N�X�`���̊J��
	if (m_pTextureMeshfield != NULL)
	{
		m_pTextureMeshfield->Release();
		m_pTextureMeshfield = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuffMeshfield != NULL)
	{
		m_pIdxBuffMeshfield->Release();
		m_pIdxBuffMeshfield = NULL;
	}
}

void CMeshfield::UpdateMeshfield(void)
{
	VERTEX_3D *pVtx;

	m_meshcount -= 0.003f;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepth; nCnt++)
	{// ���s��
		for (int nCntA = 0; nCntA < m_nWidth; nCntA++)
		{// ����
		 // �|���S���̊e���_���W
			pVtx[0].pos = D3DXVECTOR3(-m_fWidthMax / 2.0f + (float)nCntA * m_fWidth, m_posMeshfield.y, m_fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �e���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

			// �e�N�X�`�����_���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.1f * nCntA, (0.1f * nCnt) + m_meshcount);

			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMeshfield->Unlock();
}

void CMeshfield::DrawMeshfield(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;// �v�Z�p�}�g���b�N�X

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldMeshfield);

	// �����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMeshfield.y, m_rotMeshfield.x, m_rotMeshfield.z);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_posMeshfield.x, m_posMeshfield.y, m_posMeshfield.z);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldMeshfield);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffMeshfield);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTextureMeshfield);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nIdxPoint,			// ���_��
		0,
		m_nPolygon);			// �v���~�e�B�u��
}
