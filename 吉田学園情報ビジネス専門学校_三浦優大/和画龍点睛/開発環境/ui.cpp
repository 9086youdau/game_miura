//=============================================================================
//
//ui����[ui.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "ui.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "gamepad.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CUi::m_pTexture[UITYPE_MAX] = {};

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CUi::CUi(int nPriority) : CScene2D(nPriority)
{
	
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
//ui�̏���������
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�^�C�v�����֐�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//ui�̏I������
//=============================================================================
void CUi::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}

//=============================================================================
//ui�̍X�V����
//=============================================================================
void CUi::Update(void)
{
	
}

//=============================================================================
//ui�̕`�揈��
//=============================================================================
void CUi::Draw(void)
{
	//��{�N���X�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//ui�̍쐬�E�ݒ菈��
//=============================================================================
CUi * CUi::Create(D3DXVECTOR3 pos, float fHight, float fWidth,UITYPE uitype)
{
	//�C���X�^���X����
	CUi *pUi = NULL;
	pUi = new CUi;

	//�������Ăяo��
	pUi->Init(pos, fHight, fWidth);

	//�e�N�X�`���Ăяo��
	pUi->BindTexture(m_pTexture[uitype]);

	return pUi;
}

//=============================================================================
//ui�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CUi::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Max.png", &m_pTexture[UITYPE_MAXSCORE]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Score.png", &m_pTexture[UITYPE_SCORE]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/graze.png", &m_pTexture[UITYPE_GRAZE]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/life.png", &m_pTexture[UITYPE_LIFE]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bomb.png", &m_pTexture[UITYPE_BOMB]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lineui.png", &m_pTexture[UITYPE_LINE]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlelogo.png", &m_pTexture[UITYPE_LOGO]);

	return S_OK;
}

//=============================================================================
//ui�̃e�N�X�`���A�����[�h����
//=============================================================================
void CUi::Unload(void)
{
	//���[�v
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
