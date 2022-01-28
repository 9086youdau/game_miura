//=============================================================================
//
// �A�j���[�V�������� [animation.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "animation.h"
#include "manager.h"
#include "scene3D.h"
#include "effect.h"
#include "renderer.h"
#include "passerby.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CAnimation::m_pTexture = NULL; //�e�N�X�`�������o�ϐ�
bool CAnimation::m_bPasserbyColision = false;     //�G�̓����蔻��t���O
CPasserby *CAnimation::m_pPasserby[256] = {};     //�G�̕ۑ��p�|�C���^�ϐ�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CAnimation::CAnimation(int nPriority, int nType)
{
	//�����o�ϐ�������
	m_nPatternAnim = 0;
	m_nCounterAnim = 0; 
	m_nCnt = 0;
	m_bPasserbyColision = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAnimation::~CAnimation()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CAnimation::Init(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt)
{
	//����������
	CScene3D::Init(pos, fHight, fWidth);

	//�ʒu���̎󂯓n��
	m_pos = pos;
	
	//�G���̎󂯓n��
	m_nCnt = nCnt;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CAnimation::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CAnimation::Update(void)
{
	//�J�E���g�A�b�v
	m_nCounterAnim++;

	if ((m_nCounterAnim % 4) == 0)
	{
		//�A�j�[���[�V����������
		m_nPatternAnim++;

		//�A�j���[�V�����p�^�[�������������
		if (m_nPatternAnim <= 5)
		{
			//�֐��Ăяo��
			SetAnim(0.2f, m_nPatternAnim, 0.0f, 0.25f + 0.02f);
		}
		//�A�j���[�V�����p�^�[�������������
		if (m_nPatternAnim >= 5 && m_nPatternAnim <= 10)
		{
			//�֐��Ăяo��
			SetAnim(0.2f, m_nPatternAnim, 0.25f, 0.5f + 0.02f);
		}
		//�A�j���[�V�����p�^�[�������������
		if (m_nPatternAnim >= 10 && m_nPatternAnim <= 15)
		{
			//�֐��Ăяo��
			SetAnim(0.2f, m_nPatternAnim, 0.50f, 0.75f + 0.02f);
		}
		//�A�j���[�V�����p�^�[�������������
		if (m_nPatternAnim >= 15 && m_nPatternAnim <= 20)
		{
			//�֐��Ăяo��
			SetAnim(0.2f, m_nPatternAnim, 0.75f, 1.0f + 0.02f);
		}
		if (m_nPatternAnim == 13)
		{
			//�G�𐶐�(�A�j���[�V�����̃p�^�[����������)
			m_pPasserby[m_nCnt] = CPasserby::Create(m_pos, PASSERBY_TYPE_MEN, 0);
			//�����蔻���L���ɂ���
			m_bPasserbyColision = true;
		}
		//�A�j���[�V�����p�^�[�������������
		if (m_nPatternAnim == 20)
		{
			//��������
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CAnimation::Draw(void)
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
// ��������
//=============================================================================
CAnimation * CAnimation::Create(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt)
{
	//���f���𐶐�
	CAnimation *pAnimation = NULL;
	pAnimation = new CAnimation;

	//NULL�`�F�b�N
	if (pAnimation != NULL)
	{
		//�A�j���[�V�����̏���������
		pAnimation->Init(pos, fHight, fWidth, nCnt);

		//�e�N�X�`������
		pAnimation->SetTexture(m_pTexture);
	}

	return pAnimation;
}
//=============================================================================
// �e�N�X�`�����[�h����
//=============================================================================
void CAnimation::Load(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipo-btleffect213_480.png", &m_pTexture);
}
//=============================================================================
// �e�N�X�`���A�����[�h����
//=============================================================================
void CAnimation::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
