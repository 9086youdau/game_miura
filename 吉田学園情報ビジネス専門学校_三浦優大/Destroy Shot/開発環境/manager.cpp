//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "sound.h"
#include "gamepad.h"
#include "fade.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "pause.h"
#include "camera.h"
#include "texture.h"
#include "light.h"
#include "game.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CRenderer		*CManager::m_pRenderer = NULL;
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;
CInput			*CManager::m_pInput = NULL;
CSound			*CManager::m_pSound = NULL;
CScene			*CManager::m_pScene = NULL;
CInputGamePad	*CManager::m_pGamepad = NULL;
CGame			*CManager::m_pGame = NULL;
CCamera			*CManager::m_pCamera = NULL;
CLight			*CManager::m_pLight = NULL;
CSceneX			*CManager::m_pSceneX = NULL;
CTexture		*CManager::m_pTexture = NULL;
CTitle			*CManager::m_pTitle = NULL;
CResult			*CManager::m_pResult = NULL;
CTutorial		*CManager::m_pTutorial = NULL;
CPause			*CManager::m_pPause = NULL;

//�����̉��
CManager::MODE	CManager::m_mode = MODE_TITLE;
//�o�ߎ��Ԃ�\��������
bool	CManager::m_bEndGame = false;
bool	g_bPause = false;			//�|�[�Y���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	//������
	m_pTimeFont = NULL;
	m_dwGameStartTime = 0;
	m_dwGameTime = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//�����_���[�쐬
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, bWindow);
	}

	//�L�[�{�[�h����
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	//�Q�[���p�b�h����
	if (m_pGamepad == NULL)
	{
		m_pGamepad = new CInputGamePad;
		m_pGamepad->Init(hInstance, hWnd);
	}

	//�T�E���h����
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		m_pSound->Init(hWnd);
	}

	//�J��������
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
		m_pCamera->Init();
	}

	//���C�g����
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
		m_pLight->Init();
	}

	//�e�N�X�`���N���X�𐶐�
	if (m_pTexture == NULL)
	{
		m_pTexture = new CTexture;
		m_pTexture->Load();
	}

	//���[�h�ݒ�
	SetMode(m_mode);

	// �^�C�}�[�̏�����
	InitTime();

	return S_OK;
}

//==============================================================================
// ���[�h�I��
//==============================================================================
void CManager::SetMode(MODE mode)
{
	//�I������
	CScene::ReleaseAll();

	//�t�F�[�h���N���G�C�g
	CFade::Create(m_mode);

	//��ʑJ�ڎ��ɏ�����
	g_bPause = false;

	switch (mode)
	{
	case MODE_TITLE:
		//�^�C�g����ʂ̏���������
		m_pTitle->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_TUTORIAL:
		//�`���[�g���A����ʂ̏���������
		m_pTutorial->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_GAME:
		//�Q�[����ʂ̏���������
		m_pGame->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		//�|�[�Y��ʂ̐���
		m_pPause->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;

	case MODE_RESULT:
		//���U���g��ʂ̏���������
		m_pResult->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
		break;
	}
	
	//���[�h�ύX
	m_mode = mode;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	//�I�u�W�F�N�g�N���X�̔j��
	CScene::ReleaseAll();

	//�e�N�X�`���N���X�̔j��
	m_pTexture->Unload();
	m_pTexture = NULL;

	//���͏����̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = NULL;
	}

	//�T�E���h�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//�����_�����O�N���X�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//�Q�[���p�b�h�N���X�̔j��
	if (m_pRenderer != NULL)
	{
		m_pGamepad->Uninit();
		delete m_pGamepad;
		m_pGamepad = NULL;
	}

	// �^�C�}�[�t�H���g�̔j��
	UninitTime();
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�J�����̍X�V
	m_pCamera->Update();

	//�L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//�Q�[���p�b�h�̍X�V����
	if (m_pGamepad != NULL)
	{
		m_pGamepad->Update();
	}

	//�����_���[�̍X�V
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//�Q�[����ʎ��̂݃|�[�Y
	if (CManager::m_mode == MODE_GAME)
	{
		//�|�[�Y�̐؂�ւ��I��
		if (g_bPause == false)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true || pGamepad->IsButtonDown(CInput::START))
			{
				g_bPause = true;
			}
		}

		//�|�[�Y�̐؂�ւ��I�t
		else if (g_bPause == true)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true || pGamepad->IsButtonDown(CInput::START))
			{
				g_bPause = false;
			}
		}
	}

	// �^�C�}�[�X�V
	UpdateTime();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	//�S�Ẵ����_���[�̕`��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//==============================================================================
// �^�C�}�[������
//==============================================================================
HRESULT CManager::InitTime(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetRenderer()->GetDevice();

	// �t�H���g�I�u�W�F�N�g�̐���
	D3DXCreateFont(
		pDevice, 36, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&m_pTimeFont);

	// �J�n�����擾
	m_dwGameStartTime = timeGetTime();

	return S_OK;
}

//==============================================================================
// �^�C�}�[�X�V
//==============================================================================
void CManager::UninitTime(void)
{
	if (m_pTimeFont != NULL)
	{
		m_pTimeFont->Release();
		m_pTimeFont = NULL;
	}
}

//==============================================================================
// �^�C�}�[�X�V
//==============================================================================
void CManager::UpdateTime(void)
{
	if (m_bEndGame == false)
	{
		// �o�ߎ��ԍX�V
		m_dwGameTime = timeGetTime() - m_dwGameStartTime;
	}
}

//==============================================================================
// �^�C�}�[�`��
//==============================================================================
void CManager::DrawTime(void)
{
#ifdef _DEBUG
	// ���[�J���ϐ��錾
	RECT rect = { 0,30,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	if (m_bEndGame == false)
	{
		//�o�ߎ��Ԃ������o�֐��ɓ����
		m_fTime = (float)m_dwGameTime / 1000;
		
		//��ʂɕ\��������
		sprintf(&aStr[0], "�o�ߎ��� : %.3f\n", m_fTime);

		// �e�L�X�g�̕`��
		m_pTimeFont->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
#endif
}

//=============================================================================
// �L�[�{�[�h��Get�֐�
//=============================================================================
CInputKeyboard * CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// �����_���[��Get�֐�
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// �T�E���h��Get�֐�
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// �J������Get�֐�
//=============================================================================
CCamera * CManager::GetCamera(void)
{
	return m_pCamera;
}
//=============================================================================
// �e�N�X�`����Get�֐�
//=============================================================================
CTexture * CManager::GetTexture(void)
{
	return m_pTexture;
}
//=============================================================================
// �}�l�[�W���[��Get�֐�
//=============================================================================
bool CManager::GetPause()
{
	return g_bPause;
}
