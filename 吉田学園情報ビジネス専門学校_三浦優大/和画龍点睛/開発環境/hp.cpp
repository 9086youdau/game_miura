//================================================
//
//�̗͏���[hp.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "hp.h"
#include "Renderer.h"
#include "manager.h"
#include "bullet.h"
#include "input.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
LPDIRECT3DTEXTURE9 CHp::m_pTexture = NULL;

//=================================================
//�R���X�g���N�^
//=================================================
CHp::CHp()
{
}

//=================================================
//�f�X�g���N�^
//=================================================
CHp::~CHp()
{
}

//=================================================
//hp�̐ݒ�E�쐬����
//=================================================
CHp * CHp::Create(D3DXVECTOR3 pos,float fHight, float fWidth)
{
	//�C���X�^���X����
	CHp *pHp;
	pHp = new CHp;

	//�������Ăяo��
	pHp->Init(pos, fHight, fWidth);

	return pHp;
}

//=================================================
//hp�̏���������
//=================================================
HRESULT CHp::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
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

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//���_�J���[�̐ݒ�
	SetCol(255, 255, 255, 255);

	//�Ԃ�l
	return S_OK;
}

//=================================================
//hp�̏I������
//=================================================
void CHp::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=================================================
//hp�̍X�V����
//=================================================
void CHp::Update(void)
{

}

//=================================================
//hp�̕`�揈��
//=================================================
void CHp::Draw(void)
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

//=================================================
//hp�̃e�N�X�`���ݒ菈��
//=================================================
HRESULT CHp::Load()
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/HP.png", &m_pTexture);
	
	return S_OK;
}

//=================================================
//hp�̃e�N�X�`���A�����[�h�ݒ菈��
//=================================================
void CHp::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// �J���[�ϊ��֐�����
//=============================================================================
void CHp::SetCol(int R, int G, int B, int A)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(R, G, B, A);
	pVtx[1].col = D3DCOLOR_RGBA(R, G, B, A);
	pVtx[2].col = D3DCOLOR_RGBA(R, G, B, A);
	pVtx[3].col = D3DCOLOR_RGBA(R, G, B, A);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

