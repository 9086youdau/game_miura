//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "scene3D.h"
#include "renderer.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(int nPriority, int nType)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col, float fHight, float fWidth)
{
	//����������
	CScene3D::Init(pos, fHight, fWidth);

	//�����p�֐�
	SetEffect(pos, nLife, col);

	//�e�N�X�`������
	SetTex();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	//�ʒu�X�V�̊֐��Ăяo��
	SetPosition(m_pos);

	//�J���[�ύX
	SetCol(m_col);

	//�A���t�@�l��������
	m_col.a -= 0.15f;

	//���C�t����
	m_nLife--;
	if (m_nLife == 0)//������0�ɂȂ�����
	{
		//��������
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�����_�[�X�e�[�g�̐ݒ�(���Z�����j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�`�揈��
	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�����_�[�X�e�[�g�̐ݒ�(���ɖ߂��j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ��������ϐ��ɑ������
//=============================================================================
void CEffect::SetEffect(D3DXVECTOR3 pos,int nLife, D3DXCOLOR col)
{
	//�ʒu���
	m_pos = pos;

	//�������
	m_nLife = nLife;

	//�F�ύX
	m_col = col;
}

//=============================================================================
// ��������
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fHight, float fWidth)
{
	//���f���𐶐�
	CEffect *pEffect = NULL;
	pEffect = new CEffect;

	//NULL�`�F�b�N
	if (pEffect != NULL)
	{
		//�I�u�W�F�N�g�̏���������
		pEffect->Init(pos, nLife, col, fHight, fWidth);

		//�e�N�X�`������
		pEffect->SetTexture(m_pTexture);
	}

	return pEffect;
}
//=============================================================================
// �e�N�X�`�����[�h����
//=============================================================================
void CEffect::Load(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);
}
//=============================================================================
// �e�N�X�`���A�����[�h����
//=============================================================================
void CEffect::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
