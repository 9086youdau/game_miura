//=============================================================================
//
// �`���[�g���A����ʏ��� [tutorial.cpp]
// Author : Miura Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "tutorial.h"
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"
#include "ranking.h"
#include "number.h"
#include "ui.h"
#include "gamepad.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//ui����
	CUi::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 18);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//��ʑJ��	����{�^�����������ꍇ
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
	{
		//���[�h�ݒ�
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);		// �V�[���J��
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CTutorial * CTutorial::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ���U���g��ʂ̐���
	CTutorial *pTutorial;
	pTutorial = new CTutorial;

	// NULL�`�F�b�N
	if (pTutorial != NULL)
	{
		//�`���[�g���A���̏���������
		pTutorial->Init(pos, fSizeX, fSizeY);
	}

	return pTutorial;
}
