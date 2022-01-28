//=============================================================================
//
// �V�[��X���� [sceneX.cpp]
// Author : Miura Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "player3D.h"
#include "renderer.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
char *CSceneX::m_pTextName;
char *CSceneX::m_pTextureName;
LPD3DXMESH CSceneX::m_pGetMesh;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(int nPriority, int nType) : CScene(nPriority, nType)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//model�ǂݍ���
	D3DXLoadMeshFromX(m_pTextName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	// �}�e���A���ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �t�@�C�������g�p���ăe�N�X�`����ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		m_pTextureName,
		&m_pTexture);

	//�ʒu
	m_pos = pos;
	m_pGetMesh = m_pMesh;

	//����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneX::Uninit(void)
{
	//���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneX::Draw()
{
	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ�
	D3DXMATERIAL *pMat;				//�}�e���A���ւ̃|�C���^
	D3DMATERIAL9 shadowDef;			// ���݂̃V���h�E�}�e���A���ۑ��p
	D3DXMATRIX ShadowMatrix;        //�V���h�E�}�g���b�N�X
	D3DXVECTOR4 veclight;           //���C�g�̃x�N�g��
	D3DXVECTOR3 pos, normal;        //���ʏ�̍��W�A�x�N�g��
	D3DXPLANE plane;                //����

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	veclight = D3DXVECTOR4(0.2f, 0.8f, 0.4f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&ShadowMatrix);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�V���h�E�̈ʒu�ݒ�
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	//�����̔��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�̔��]
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A���̎擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// ���݂̃V���h�E�}�e���A�����擾
	pDevice->GetMaterial(&shadowDef);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		shadowDef.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&shadowDef);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�V���h�E�}�g���b�N�X�̐���
		D3DXMatrixShadow(&ShadowMatrix, &veclight, &plane);

		D3DXMatrixMultiply(&ShadowMatrix, &m_mtxWorld, &ShadowMatrix);

		//�e���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &ShadowMatrix);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&shadowDef);
}

//=============================================================================
// ���f���̓ǂݍ���
//=============================================================================
void CSceneX::BindTextName(char *TextName)
{
	//���f���̓ǂݍ��݂���������
	m_pTextName = TextName;
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CSceneX::BindTextureName(char * TextureName)
{
	//�e�N�X�`���̓ǂݍ��݂���������
	m_pTextureName = TextureName;
}

//=============================================================================
// Set�ʒu
//=============================================================================
void CSceneX::SetPosition(D3DXVECTOR3 pos)
{
	//�ʒu������������ǂݍ���
	m_pos = pos;
}

//=============================================================================
// Set����
//=============================================================================
void CSceneX::SetRevolution(D3DXVECTOR3 rot)
{
	//��]������������ǂݍ���
	m_rot = rot;
}

//=============================================================================
// ��������
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, char *TextName)
{
	//�V�[��X�𐶐�
	CSceneX *pSceneX = NULL;
	pSceneX = new CSceneX;

	m_pTextName = TextName;

	//NULL�`�F�b�N
	if (pSceneX != NULL)
	{
		pSceneX->Init(pos, 0.0f, 0.0f);
	}

	return pSceneX;
}
