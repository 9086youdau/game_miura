//================================================
//
//�i���o�[����[number.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "number.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
//�i���o�[�̏���������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu���̑��
	m_pos = D3DXVECTOR3(pos);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHight, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//�i���o�[�̏I������
//=============================================================================
void CNumber::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//�i���o�[�̍X�V����
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
//�i���o�[�`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//�i���o�[�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CNumber::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number005.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�i���o�[�̃e�N�X�`���j������
//=============================================================================
void CNumber::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�i���o�[�̐ݒ�E�쐬����
//=============================================================================
CNumber * CNumber::Create(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�C���X�^���X����
	CNumber *pNumber = NULL;
	pNumber = new CNumber;

	//�������Ăяo��
	pNumber->Init(pos, fHight, fWidth);

	return pNumber;
}

//=============================================================================
//�i���o�[�̃e�N�X�`������
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * (nNumber + 1), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * (nNumber + 1), 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
