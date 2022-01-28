//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : Miura Yudai
//
//=============================================================================

//==============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"
#include "gamepad.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
bool CTitle::m_bExit = false;
bool CTitle::m_bNotMove = false;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTitle::CTitle()
{
	//�����o�ϐ�������
	m_bNotMove = false;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTitle::~CTitle()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�T�E���hBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM000);

	//ui�̃e�N�X�`�����[�h
	CUi::Load();

	//ui����
	CUi::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 1);

	for (int nCnt = 2; nCnt < 5; nCnt++)
	{
		//ui����
		CUi::Create(D3DXVECTOR3(760.0f, 450.0f + (nCnt * 120.0f), 0.0f), 100.0f, 450.0f, nCnt);
	}

	//ui����
	CUi::Create(D3DXVECTOR3(650.0f, 730.0f, 0.0f), 50.0f, 250.0f, 5);

	//ui����
	CUi::Create(D3DXVECTOR3(725.0f, 550.0f, 0.0f), 100.0f, 500.0f, 6);

	//ui����
	CUi::Create(D3DXVECTOR3(725.0f, 50.0f, 0.0f), 500.0f, 500.0f, 17);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CTitle::Uninit(void)
{
	//BGM�̔j��
	m_pSound->Stop(CSound::SOUND_LABEL_BGM000);

	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CTitle::Update(void)
{
	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//ui�̃|�C���^���Q�[���X�^�[�g�̏���������
	if (CUi::GetPointCount() == 0)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true  || pGamepad->IsButtonDown(CInput::A) == true)
		{
			//��~�t���O
			m_bNotMove = true;
			//�T�E���hBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
			//�Q�[���Ɉړ�����
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
		}
	}

	//ui�̃|�C���^���`���[�g���A���̏���������
	if (CUi::GetPointCount() == 1)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true)
		{
			//��~�t���O
			m_bNotMove = true;
			//�T�E���hBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
			//�`���[�g���A���Ɉړ�����
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);
		}
	}

	//ui�̃|�C���^���I���̏���������
	if (CUi::GetPointCount() == 2)
	{
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true)
		{
			//��~�t���O
			m_bNotMove = true;
			//�T�E���hBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
			// �E�B���h�E��j������悤�w������
			m_bExit = true;
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CTitle::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CTitle * CTitle::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CTitle *pTitle;
	pTitle = new CTitle;

	// NULL�`�F�b�N
	if (pTitle != NULL)
	{
		pTitle->Init(pos, fSizeX, fSizeY);
	}

	return pTitle;
}
