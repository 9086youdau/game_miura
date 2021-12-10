//=============================================================================
//
//�^�C�g������[title.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "title.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "gamepad.h"
#include "sound.h"
#include "SceneX.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture[TITLETYPE_MAX] = {};
bool CTitle::m_bExit = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	//�����o�ϐ�������
	m_nCount = 255;
	m_titleType = TITLETYPE_GAME;
	m_nCounterAnim = 0.0f;
	m_pos = D3DXVECTOR3(950.0f, 0.0f, 0.0f);
	m_posui = D3DXVECTOR3(-120.0f, 950.0f, 0.0f);
	m_moveui = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//�^�C�g���̏���������
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�e�L�X�`�����[�h
	Load();

	//�֐��Ăяo��
	SetVertex();

	//�T�E���h�̃|�C���^�錾
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGM�ݒ�
	pSound->Stop(pSound->SOUND_LABEL_BGM002);

	//BGM�ݒ�
	pSound->Play(pSound->SOUND_LABEL_BGM000);

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//�^�C�g���̏I������
//=============================================================================
void CTitle::Uninit(void)
{
	//�e�N�X�`���A�����[�h
	Unload();

	//�j���֐�
	Release();
}

//=============================================================================
//�^�C�g���̍X�V����
//=============================================================================
void CTitle::Update(void)
{
	//���d�w�i����
	MoveBg();

	//�Z���N�g����
	TitleSelect();

	//�^�C�g�����S�̈ړ�
	m_pos.y += 3.0f;

	//�^�C�g����ui�̈ړ�
	m_moveui.x = cosf(-35) * -5.5f;
	m_moveui.y = sinf(-35) * -5.5f;

	//����ʒu�܂ł�����X�g�b�v
	if (m_pos.y >= 250)
	{
		m_pos.y = 250.0f;
	}

	//����ʒu�܂ł�����X�g�b�v
	if (m_posui.y <= 750)
	{
		m_moveui.x = 0.0f;
		m_moveui.y = 0.0f;
	}

	//�ʒu�X�V
	m_posui += m_moveui;

	//�ʒu�X�V
	m_pScene2D[TITLETYPE_TITLELOGO]->SetPosition(m_pos,D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_pScene2D[TITLETYPE_UI]->SetPosition(m_posui,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//�^�C�g���̕`�揈��
//=============================================================================
void CTitle::Draw(void)
{
}

//=============================================================================
//�^�C�g���̍쐬�E�ݒ菈��
//=============================================================================
CTitle * CTitle::Create(void)
{
	//�C���X�^���X����
	CTitle *pTitle = NULL;
	pTitle = new CTitle;

	//�������Ăяo��
	pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pTitle;
}

//=============================================================================
//�^�C�g���̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CTitle::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlebg3.jpg", &m_pTexture[TITLETYPE_BG]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlemovebg.png", &m_pTexture[TITLETYPE_MOVEBG]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlemovebg2.png", &m_pTexture[TITLETYPE_MOVEBG2]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlesircle.png", &m_pTexture[TITLETYPE_UI]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlelogo.png", &m_pTexture[TITLETYPE_TITLELOGO]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleStartCover.png", &m_pTexture[TITLETYPE_GAME]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TitleExitCover.png", &m_pTexture[TITLETYPE_EXIT]);

	return S_OK;
}

//=============================================================================
//�^�C�g���̃e�N�X�`���A�����[�h����
//=============================================================================
void CTitle::Unload(void)
{
	//���[�v
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
//�^�C�g���̕`��ݒ菈��
//=============================================================================
void CTitle::SetVertex(void)
{
	for (int nCntTitle = 0; nCntTitle < TITLETYPE_MAX; nCntTitle++)
	{
		if (nCntTitle == TITLETYPE_BG)
		{
			//�^�C�g���̃C���X�^���X����
			m_pScene2D[TITLETYPE_BG] = new CScene2D;
			m_pScene2D[TITLETYPE_BG]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
			m_pScene2D[TITLETYPE_BG]->BindTexture(m_pTexture[TITLETYPE_BG]);
		}

		else if (nCntTitle == TITLETYPE_MOVEBG)
		{
			//�^�C�g���̃C���X�^���X����
			m_pScene2D[TITLETYPE_MOVEBG] = new CScene2D;
			m_pScene2D[TITLETYPE_MOVEBG]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
			m_pScene2D[TITLETYPE_MOVEBG]->BindTexture(m_pTexture[TITLETYPE_MOVEBG]);
		}

		else if (nCntTitle == TITLETYPE_MOVEBG2)
		{
			//�^�C�g���̃C���X�^���X����
			m_pScene2D[TITLETYPE_MOVEBG2] = new CScene2D;
			m_pScene2D[TITLETYPE_MOVEBG2]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
			m_pScene2D[TITLETYPE_MOVEBG2]->BindTexture(m_pTexture[TITLETYPE_MOVEBG2]);
		}

		else if (nCntTitle == TITLETYPE_UI)
		{
			//�^�C�g���̃C���X�^���X����
			m_pScene2D[TITLETYPE_UI] = new CScene2D;
			m_pScene2D[TITLETYPE_UI]->Init(m_posui, 300.0f, 300.0f);
			m_pScene2D[TITLETYPE_UI]->BindTexture(m_pTexture[TITLETYPE_UI]);
		}

		else if (nCntTitle == TITLETYPE_TITLELOGO)
		{
			//�^�C�g���̃C���X�^���X����
			m_pScene2D[TITLETYPE_TITLELOGO] = new CScene2D;
			m_pScene2D[TITLETYPE_TITLELOGO]->Init(m_pos, 300.0f, 800.0f);
			m_pScene2D[TITLETYPE_TITLELOGO]->BindTexture(m_pTexture[TITLETYPE_TITLELOGO]);
		}

		else if (nCntTitle == TITLETYPE_GAME)
		{
			//�^�C�g���̃C���X�^���X����
			m_pScene2D[TITLETYPE_GAME] = new CScene2D;
			m_pScene2D[TITLETYPE_GAME]->Init(D3DXVECTOR3(1300.0f, 750.0f, 0.0f), 120.0f, 600.0f);
			m_pScene2D[TITLETYPE_GAME]->BindTexture(m_pTexture[TITLETYPE_GAME]);
			m_pScene2D[TITLETYPE_GAME]->SetTex(1.0f, 0);
		}

		else if (nCntTitle == TITLETYPE_EXIT)
		{
			//�^�C�g���̃C���X�^���X����
			m_pScene2D[TITLETYPE_EXIT] = new CScene2D;
			m_pScene2D[TITLETYPE_EXIT]->Init(D3DXVECTOR3(1300.0f, 900.0f, 0.0f), 120.0f, 600.0f);
			m_pScene2D[TITLETYPE_EXIT]->BindTexture(m_pTexture[TITLETYPE_EXIT]);
			m_pScene2D[TITLETYPE_EXIT]->SetTex(1.0f, 0);
		}
	}
}
//=============================================================================
//�^�C�g���̑��d�w�i����
//=============================================================================
void CTitle::MoveBg(void)
{
	//�J�E���g��i�߂�
	m_nCounterAnim -= 0.0008f;

	for (int nCnt = 1; nCnt < 3; nCnt++)
	{
		//�w�i�̃e�N�X�`������
		m_pScene2D[nCnt]->SetBg(1.0f, m_nCounterAnim * (nCnt + 1));
	}
}

//=============================================================================
//�^�C�g���̑I������
//=============================================================================
void CTitle::TitleSelect(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//���[�h���
	CManager::MODE mode;
	mode = CManager::GetMode();

	//�^�C�g���^�C�v��startgame�̎�
	if (m_titleType == TITLETYPE_GAME)
	{
		//�J�E���g�A�b�v
		m_nCount++;

		if (m_nCount % 30 == 0)//�Ԋu
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 0, 0, 0.5f));
		}

		else if (m_nCount % 15 == 0 && m_nCount % 30)//�Ԋu
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 0, 0, 1.0f));
		}

		//���[�h���^�C�g���̎�
		if (mode == CManager::MODE_TITLE)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				//�Q�[����ʂ֑J��
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);
			}
		}
	}
	//�^�C�g���^�C�v��exit�̎�
	else if (m_titleType == TITLETYPE_EXIT)
	{
		//�J�E���g�A�b�v����
		m_nCount++;

		if (m_nCount % 30 == 0)//�Ԋu
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 0, 0, 0.5f));
		}

		else if (m_nCount % 15 == 0 && m_nCount % 30)//�Ԋu
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 0, 0, 1.0f));
		}

		//���[�h���^�C�g���̎�
		if (mode == CManager::MODE_TITLE)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				// �E�B���h�E��j������悤�w������
				m_bExit = true;
			}
		}
	}

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_UP) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		if (m_titleType == TITLETYPE_GAME)
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_EXIT;
		}
		else if (m_titleType == TITLETYPE_EXIT)
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_GAME;
		}
	}

	else if (pInputGamePad->IsButtonDown(CInput::KEYINFO_DOWN) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		if (m_titleType == TITLETYPE_GAME)
		{
			m_pScene2D[TITLETYPE_GAME]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_EXIT;
		}
		else if (m_titleType == TITLETYPE_EXIT)
		{
			m_pScene2D[TITLETYPE_EXIT]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_titleType = TITLETYPE_GAME;
		}
	}
}
