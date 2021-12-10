//================================================
//
// �V�[��X����[SceneX.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "SceneX.h"
#include "manager.h"
#include "Renderer.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================


//=================================================
//�R���X�g���N�^
//=================================================
CSceneX::CSceneX()
{
}

//=================================================
//�f�X�g���N�^
//=================================================
CSceneX::~CSceneX()
{
}

//=================================================
//�V�[��X�̏���������
//=================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data/MODEL/prant2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//��]����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ʒu����
	m_pos = pos;

	return S_OK;
}

//=================================================
//�V�[��X�̏I������
//=================================================
void CSceneX::Uninit(void)
{
	if (m_pMesh != NULL)
	{// ���b�V���̔j��
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	else if (m_pBuffMat != NULL)
	{// �o�b�t�@�̔j��
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�j������
	Release();
}

//=================================================
//�V�[��X�̍X�V����
//=================================================
void CSceneX::Update(void)
{
	//�^�C�v����
	if (m_scenetype == SCENEXTYPE_LEFT)
	{
		//��]����
		m_rot.y -= 0.02f;
	}

	else if (m_scenetype == SCENEXTYPE_RIGHT)
	{
		//��]����
		m_rot.y += 0.02f;
	}
}

//=================================================
//�V�[��X�̕`�揈��
//=================================================
void CSceneX::Draw(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//�}�e���A���ւ̃|�C���^
	D3DXMATERIAL *pMat;

	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
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

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=================================================
//�V�[��X�̃N���G�C�g����
//=================================================
CSceneX * CSceneX::Create(D3DXVECTOR3 pos,SCENEXTYPE scenextype)
{
	//�C���X�^���X����
	CSceneX *pSceneX = NULL;
	pSceneX = new CSceneX;

	//�������Ăяo��
	pSceneX->Init(pos, 0.0f, 0.0f);

	//�^�C�v������������
	pSceneX->m_scenetype = scenextype;

	return pSceneX;
}
