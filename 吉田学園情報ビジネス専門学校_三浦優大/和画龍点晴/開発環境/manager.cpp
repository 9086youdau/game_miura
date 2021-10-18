//================================================
//
//�}�l�[�W���[����[manager.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "manager.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "ranking.h"
#include "number.h"
#include "gamepad.h"
#include "tutorial.h"
#include "camera.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputGamePad *CManager::m_pInputGamePad = NULL;
CSound *CManager::m_pSound = NULL;
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CRanking *CManager::m_pRanking = NULL;
CManager::MODE CManager::m_mode = MODE_TITLE;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//�}�l�[�W���[�̏���������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow)
{
	//�����_���[�̐���
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//�L�[�{�[�h�̐���
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	//�Q�[���p�b�h�̐���
	m_pInputGamePad = new CInputGamePad;
	m_pInputGamePad->Init(hInstance, hWnd);

	//�T�E���h�̐���
	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	//���[�h�ݒ�
	SetMode(m_mode);

	return S_OK;
}

//=============================================================================
//�}�l�[�W���[�̏I������
//=============================================================================
void CManager::Uninit(void)
{
	//�Q�[���p�b�h�̔j��
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}

	//�L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//�����_���[�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//�T�E���h�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//�����[�X�֐�
	CScene::ReleaseAll();
}

//=============================================================================
//�}�l�[�W���[�̍X�V����
//=============================================================================
void CManager::Update(void)
{
	//�L�[�{�[�h�̍X�V����
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad->Update();
	}

	//�L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//�����_���[�̍X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
//�}�l�[�W���[�̕`�揈��
//=============================================================================
void CManager::Draw(void)
{
	//�S�Ă̕`��ݒ�
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
//��ʑJ�ڂ̏���
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//�T�E���h�̃|�C���^�錾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�S�Ă̗v�f��j��
	CScene::ReleaseAll();

	//�t�F�[�h���N���G�C�g
	CFade::Create(m_mode);

	//���[�h�I��
	switch (mode)
	{
	case MODE_TITLE:
		//�^�C�g���̃N���G�C�g
		m_pTitle->Create();
		break;
	case MODE_TUTORIAL:
		//�`���[�g���A���̃N���G�C�g
		m_pTutorial->Create();
		break;
	case MODE_GAME:
		//�Q�[���̃N���G�C�g
		m_pGame->Create();
		break;
	case MODE_RESULT:
		//���U���g�̃N���G�C�g
		m_pResult->Create();
		break;
	case MODE_RANKING:
		//�����L���O�̃N���G�C�g
		m_pRanking->Create();
		break;
	}

	//�l����
	m_mode = mode;
}


