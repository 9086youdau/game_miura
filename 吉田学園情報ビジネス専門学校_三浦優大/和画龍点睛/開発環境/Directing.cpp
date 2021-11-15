//=============================================================================
//
//���o����[Directing.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "Directing.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "gamepad.h"
#include "sound.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CDirecting::m_pTexture[DIRECTINGTYPE_MAX] = {};
CDirecting::FADETYPE CDirecting::m_fadetype = FADE_NONE;
CDirecting::FADETYPE CDirecting::m_fadetype2 = FADE_NONE;
CDirecting::DIRECTINGTYPE CDirecting::m_directingType = DIRECTINGTYPE_NONE;
bool CDirecting::m_benemycol = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CDirecting::CDirecting(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
//�f�X�g�g���N�^
//=============================================================================
CDirecting::~CDirecting()
{
}

//=============================================================================
//���o�����̏���������
//=============================================================================
HRESULT CDirecting::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�^�C�v�����֐�
	CScene::SetObjType(CScene::OBJTYPE_DIRECTING);

	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�����o�ϐ�������
	m_FadeCount = 300;
	m_pos = pos;
	m_colstart = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_bcol = false;
	m_nCnt = 0;
	m_benemycol = false;

	return S_OK;
}

//=============================================================================
//���o�����̏I������
//=============================================================================
void CDirecting::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}

//=============================================================================
//���o�����̍X�V����
//=============================================================================
void CDirecting::Update(void)
{
	//�l����
	m_pos = GetPosition();

	//�t�F�[�h�̃J�E���g�}�C�i�X
	m_FadeCount--;

	//�t�F�[�h�C���̎�
	if (m_fadetype == FADE_IN)
	{
		m_colstart.a -= 0.01f;
		if (m_colstart.a <= 0.0f)
		{
			m_colstart.a = 0.0f;
			m_fadetype = FADE_NONE;
		}
	}

	//�t�F�[�h�A�E�g�̎�
	if (m_fadetype == FADE_OUT)
	{
		m_colstart.a += 0.01f;
		if (m_colstart.a >= 1.0f)
		{
			m_colstart.a = 1.0f;
			if (m_FadeCount <= 0)
			{
				m_fadetype = FADE_IN;
			}
		}
	}

	SetCol(m_colstart);
}

//=============================================================================
//���o�����̕`�揈��
//=============================================================================
void CDirecting::Draw(void)
{
	//��{�N���X�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//���o�����̐ݒ菈��
//=============================================================================
CDirecting * CDirecting::Create(D3DXVECTOR3 pos,float fHight, float fWidth,FADETYPE fadetype, DIRECTINGTYPE directing)
{
	//�C���X�^���X����
	CDirecting *pDirecting = NULL;
	pDirecting = new CDirecting;

	//����������
	m_directingType = directing;
	switch (directing)
	{
	case DIRECTINGTYPE_START:
		m_fadetype = fadetype;
		pDirecting->BindTexture(m_pTexture[DIRECTINGTYPE_START]);
		break;
	}

	//�������Ăяo��
	pDirecting->Init(pos, fHight, fWidth);

	//�|�C���^�̕Ԃ�l
	return pDirecting;
}

//=============================================================================
//���o�����̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CDirecting::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlecole.png", &m_pTexture[DIRECTINGTYPE_START]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_pTexture[DIRECTINGTYPE_BOSS]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_pTexture[DIRECTINGTYPE_PLAYER]);

	return S_OK;
}

//=============================================================================
//���o�����̃e�N�X�`���A�����[�h����
//=============================================================================
void CDirecting::Unload(void)
{
	//���[�v
	for (int nCnt = 0; nCnt < DIRECTINGTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
