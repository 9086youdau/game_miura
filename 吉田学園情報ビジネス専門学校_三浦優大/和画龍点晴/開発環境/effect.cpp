//================================================
//
//�G�t�F�N�g����[effect.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "effect.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
//�G�t�F�N�g�̐ݒ�E�X�V����
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos,float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect)
{
	//�C���X�^���X����
	CEffect *pEffect = NULL;
	pEffect = new CEffect;

	//�������Ăяo��
	pEffect->Init(pos, fHight, fWidth,col,effect);

	//�e�N�X�`���Ăяo��
	pEffect->BindTexture(m_pTexture);

	return pEffect;
}

//=============================================================================
//�G�t�F�N�g�̏���������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect)
{
	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�^�C�v�����֐�
	CScene::SetObjType(CScene::OBJTYPE_EFFECT);

	//�ړ��ʂ̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_effecttype = effect;

	switch (m_effecttype)
	{
	case EFFECTTYPE_NONE:
		m_nLife = 2;
		break;
	case EFFECTTYPE_PLAYER_MOVE:
		m_nLife = 15;
		break;
	}

	//�J���[�ύX�֐��Ăяo��
	SetCol(col);

	return S_OK;
}
//=============================================================================
//�G�t�F�N�g�̏I������
//=============================================================================
void CEffect::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}
//=============================================================================
//�G�t�F�N�g�̍X�V����
//=============================================================================
void CEffect::Update(void)
{
	//�e�̏����֐��Ăяo��
	SetEffectMove();
}
//=============================================================================
//�G�t�F�N�g�̕`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�����_�[�X�e�[�g�̐ݒ�(���Z�����j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//��{�N���X�̕`�揈��
	CScene2D::Draw();

	//�����_�[�X�e�[�g�̐ݒ�(���ɖ߂��j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============================================================================
//�G�t�F�N�g�̃e�N�X�`������
//=============================================================================
HRESULT CEffect::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}
//=============================================================================
//�G�t�F�N�g�̃e�N�X�`���̔p������
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
//=============================================================================
//�G�t�F�N�g�̈ړ�����
//=============================================================================
void CEffect::SetEffectMove(void)
{
	if (m_effecttype == EFFECTTYPE_NONE)
	{
		//�l����
		m_pos = GetPosition();

		//�ʒu�X�V
		m_pos += m_move;

		//�e�̈ʒu�X�V
		SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//���C�t�����炷
		m_nLife--;

		//���C�t��0�ɂȂ�����
		if (m_nLife == 0)
		{
			//�I������
			Uninit();
		}
	}
	if (m_effecttype == EFFECTTYPE_PLAYER_MOVE)
	{
		//�l����
		m_pos = GetPosition();

		//���C�t�����炷
		m_nLife--;

		//move�̌v�Z����
		m_fAngle = (float)(rand() % 100 - 50) / 100.0f;
		m_move.x = sinf(m_fAngle * D3DX_PI) * 10.5f;
		m_move.y = cosf(m_fAngle * D3DX_PI) * 10.5f;

		//�ʒu�X�V
		m_pos += m_move;

		//�e�̈ʒu�X�V
		SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//���C�t��0�ɂȂ�����
		if (m_nLife == 0)
		{
			//�I������
			Uninit();
		}
	}
}