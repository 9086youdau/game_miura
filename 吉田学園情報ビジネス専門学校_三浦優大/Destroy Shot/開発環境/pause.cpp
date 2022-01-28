//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : Miura Yudai
//
//=============================================================================

//==============================================================================
// �C���N���[�h�t�@�C���ǂ݂���
//==============================================================================
#include "pause.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"
#include "ui.h"
#include "gamepad.h"
#include "sound.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
bool CPause::m_bNotMove = false;

//==============================================================================
// �}�N����`
//==============================================================================
#define MENU_PAUSE_MAX (2)				// �I���ł��郂�[�h�̐�
#define SCREEN_TRANSITION_SPEED (30)	// ��ʑJ�ڂ̑��x

#define LOGO_RESTART_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f))		// ���X�^�[�g�̃��S�̈ʒu
#define LOGO_MENU_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f))			// ���j���[�̂̃��S�̈ʒu
#define LOGO_EXIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f))			// �I���̃��S�̈ʒu	

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause(int nPriority, int nType) : CScene(nPriority, nType)
{
	// ������
	m_nFadeCntPause = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ui����
	CUiPause::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 19);

	//ui����
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, 0);

	//ui����
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.05f, SCREEN_HEIGHT / 2.1f, 0.0f), 500.0f, 100.0f, 23);

	//ui����
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f, SCREEN_HEIGHT / 8, 0.0f), 800.0f, 150.0f, 20);

	//ui����
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f, SCREEN_HEIGHT / 1.65f, 0.0f), 800.0f, 150.0f, 21);

	//ui����
	CUiPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.5f, SCREEN_HEIGHT / 2.45f, 0.0f), 800.0f, 150.0f, 22);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
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
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true)
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
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CPause * CPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CPause *pPause;
	pPause = new CPause;

	// NULL�`�F�b�N
	if (pPause != NULL)
	{
		pPause->Init(pos, fSizeX, fSizeY);
	}

	return pPause;
}