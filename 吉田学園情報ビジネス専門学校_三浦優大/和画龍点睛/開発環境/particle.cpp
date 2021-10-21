//================================================
//
//�p�[�e�B�N������[paticle.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "particle.h"
#include "effect.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CParticle::CParticle()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//�p�[�e�B�N���̏���������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, float fHight, float fWidth,D3DXCOLOR col,PARTICLE particle)
{
	//����������
	CEffect::Init(pos, fHight, fWidth, col,CEffect::EFFECTTYPE_NONE);

	//�ړ��ʂ̏�����
	m_move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nLife = 5 + rand() % 7;

	if (particle == PARTICLE_ENEMY)
	{
		//�J���[�ύX�֐��Ăяo��
		SetCol(col);

		//move�̌v�Z����
		m_fAngle = (float)(rand() % 100 - 50) / 100.0f;
		m_move.x = sinf(m_fAngle * D3DX_PI) * 10.5f;
		m_move.y = cosf(m_fAngle * D3DX_PI) * 10.5f;
	}
	else if(particle == PARTICLE_PLAYER)
	{
		//�J���[�ύX�֐��Ăяo��
		SetCol(col);

		//move�̌v�Z����
		m_fAngle = (float)(rand() % 628 + 314) / 100.0f;
		m_move.x = sinf(m_fAngle * D3DX_PI) * 10.5f;
		m_move.y = cosf(m_fAngle * D3DX_PI) * 10.5f;
	}

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//�p�[�e�B�N���̐ݒ�E�X�V����
//=============================================================================
CParticle * CParticle::Create(D3DXVECTOR3 pos,D3DXCOLOR col,PARTICLE particle)
{
	//�C���X�^���X����
	CParticle *pParticle = NULL;
	pParticle = new CParticle;

	//�������Ăяo��
	pParticle->Init(pos, MAX_PARTICLE_SIZE, MAX_PARTICLE_SIZE,col,particle);

	//�e�N�X�`���Ăяo��
	pParticle->BindTexture(m_pTexture);

	//�Ԃ�l
	return pParticle;
}
//=============================================================================
//�p�[�e�B�N���̏I������
//=============================================================================
void CParticle::Uninit(void)
{
	//��{�N���X�̏I������
	CEffect::Uninit();
}
//=============================================================================
//�G�t�F�N�g�̍X�V����
//=============================================================================
void CParticle::Update(void)
{
	//�e�̏����֐��Ăяo��
	SetParticleMove();
}
//=============================================================================
//�G�t�F�N�g�̕`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	//��{�N���X�̕`�揈��
	CEffect::Draw();
}
//=============================================================================
//�G�t�F�N�g�̃e�N�X�`������
//=============================================================================
HRESULT CParticle::Load(void)
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
void CParticle::Unload(void)
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
void CParticle::SetParticleMove(void)
{
	//�l����
	m_pos = GetPosition();

	//�ʒu�X�V
	m_pos += m_move;

	//�e�̈ʒu�X�V
	SetPosition(m_pos,D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���C�t�����炷
	m_nLife--;

	//���C�t��0�ɂȂ�����
	if (m_nLife == 0)
	{
		//�I������
		Uninit();
	}
}