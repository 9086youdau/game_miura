//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : Miura Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "result.h"
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"
#include "ranking.h"
#include "number.h"
#include "ui.h"
#include "sound.h"
#include "gamepad.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CScore *CResult::m_pScore[5];

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�T�E���hBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM000);

	//ui�̃e�N�X�`�����[�h
	CUi::Load();

	//�X�R�A�̃e�N�X�`�����[�h
	CNumber::Load();

	//ui����
	CUi::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 1);

	//ui����
	CUi::Create(D3DXVECTOR3(600.0f, 50.0f, 0.0f), 150.0f, 750.0f, 7);

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//ui����
		CUi::Create(D3DXVECTOR3(750.0f, 450.0f + (nCnt * 120.0f), 0.0f), 100.0f, 500.0f, 0);
	}

	//ui����
	CUi::Create(D3DXVECTOR3(670.0f, 280.0f, 0.0f), 150.0f, 650.0f, 9);

	//ui����
	CUi::Create(D3DXVECTOR3(650.0f, 300.0f, 0.0f), 100.0f, 100.0f, 10);

	//ui����
	CUi::Create(D3DXVECTOR3(1230.0f, 300.0f, 0.0f), 100.0f, 100.0f, 11);

	// ���ʃ��S
	CUi::Create(D3DXVECTOR3(550.0f, 460.0f, 0.0f), 90, 120, 12);
	CUi::Create(D3DXVECTOR3(550.0f, 580.0f, 0.0f), 90, 120, 13);
	CUi::Create(D3DXVECTOR3(550.0f, 700.0f, 0.0f), 90, 120, 14);
	CUi::Create(D3DXVECTOR3(560.0f, 820.0f, 0.0f), 65, 90, 15);
	CUi::Create(D3DXVECTOR3(560.0f, 940.0f, 0.0f), 65, 90, 16);

	//�v���C���[�X�R�A
	CScore::Create(D3DXVECTOR3(150.0f, 350.0f, 0.0f));
	//�Q�[�����̃X�R�A���Ăяo��
	CScore::SetScore(CScore::GetScore());

	//�����L���O�f�[�^�̊Ǘ��@�ǂݍ��݁����ёւ�
	CRanking::Create(D3DXVECTOR3(1180.0f, 500.0f, 0.0f), 100.0f, 100.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	//�T�E���h�̔j��
	m_pSound->Stop(CSound::SOUND_LABEL_BGM000);

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
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
void CResult::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CResult * CResult::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ���U���g��ʂ̐���
	CResult *pResult;
	pResult = new CResult;

	// NULL�`�F�b�N
	if (pResult != NULL)
	{
		pResult->Init(pos, fSizeX, fSizeY);
	}

	return pResult;
}
