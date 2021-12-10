//=============================================================================
//
//�`���[�g���A������[tutorial.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "tutorial.h"
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
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[2] = {};

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{
	//�����o�ϐ�������
	m_nCount = 255;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
//�^�C�g���̏���������
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�e�L�X�`�����[�h
	Load();

	//�^�C�g���̃C���X�^���X����
	m_pScene2D[0] = new CScene2D;
	m_pScene2D[0]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[0]->BindTexture(m_pTexture[0]);
	m_pScene2D[0]->SetTex(1.0f, 0);

	//�^�C�g���̃C���X�^���X����
	m_pScene2D[1] = new CScene2D;
	m_pScene2D[1]->Init(D3DXVECTOR3(1250.0f, 50.0f, 0.0f), 80.0f, 500.0f);
	m_pScene2D[1]->BindTexture(m_pTexture[1]);
	m_pScene2D[1]->SetTex(1.0f, 0);

	return S_OK;
}

//=============================================================================
//�^�C�g���̏I������
//=============================================================================
void CTutorial::Uninit(void)
{
	//�e�N�X�`���A�����[�h
	Unload();

	//�j���֐�
	Release();
}

//=============================================================================
//�^�C�g���̍X�V����
//=============================================================================
void CTutorial::Update(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//���[�h�������Ă���
	CManager::MODE mode;
	mode = CManager::GetMode();

	//�J�E���g�A�b�v
	m_nCount++;

	if (m_nCount % 30 == 0)//�Ԋu
	{
		//�J���[�ύX
		m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}

	else if (m_nCount % 15 == 0 && m_nCount % 30)//�Ԋu
	{
		//�J���[�ύX
		m_pScene2D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//���[�h���^�C�g���̎�
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			//�����L���O��ʂ֑J��
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
		}
	}
}

//=============================================================================
//�^�C�g���̕`�揈��
//=============================================================================
void CTutorial::Draw(void)
{
}

//=============================================================================
//�^�C�g���̍쐬�E�ݒ菈��
//=============================================================================
CTutorial * CTutorial::Create(void)
{
	//�C���X�^���X����
	CTutorial *pTutorial = NULL;
	pTutorial = new CTutorial;

	//�������Ăяo��
	pTutorial->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pTutorial;
}

//=============================================================================
//�^�C�g���̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CTutorial::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture[0]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pressabutton.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
//�^�C�g���̃e�N�X�`���A�����[�h����
//=============================================================================
void CTutorial::Unload(void)
{
	//���[�v
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}