//=============================================================================
//
//���U���g����[result.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "result.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "Renderer.h"
#include "fade.h"
#include "number.h"
#include "score.h"
#include "gamepad.h"
#include "Graze.h"
#include "sound.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CResult::m_pTexture[8] = {};
CNumber *CResult::m_apNumber[2][8] = {};
int CResult::m_nResultScore = 0;
int CResult::m_nResultAllScore = 0;
int CResult::m_nNumber[8] = {};
int CResult::m_nAllNumber[8] = {};
CScore *CResult::m_pScore = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	//�����o�ϐ�������
	m_nCounterAnim = 0.0f;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//���U���g�̏���������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�T�E���h�̃|�C���^�錾
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGM�ݒ�
	pSound->Stop(pSound->SOUND_LABEL_BGM001);

	//BGM�ݒ�
	pSound->Play(pSound->SOUND_LABEL_BGM002);

	//�e�N�X�`�����[�h
	Load();

	//�X�R�A�̃e�N�X�`�����[�h
	CNumber::Load();

	//�Q�[���̃X�R�A����
	m_nResultScore = CScore::GetScore();

	//�֐��Ăяo��
	SetVertex();

	//�O���C�Y�\��
	m_pGraze = CGraze::Create(1300.0f, 550.0f, 40.0f, 30.0f);

	//�X�R�A�̍쐬�E�ݒ�֐�
	m_pScore = CScore::Create(CScore::SCORETYPE_RESULT_SCORE);

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//���U���g�̏I������
//=============================================================================
void CResult::Uninit(void)
{
	//�e�N�X�`���A�����[�h
	Unload();

	//�X�R�A�̃e�N�X�`�����[�h
	CNumber::Unload();

	//�I������
	m_pGraze->Uninit();
	m_pGraze = NULL;

	//�I������
	m_pScore->Uninit();
	m_pScore = NULL;

	//�j���֐�
	Release();
}

//=============================================================================
//���U���g�̍X�V����
//=============================================================================
void CResult::Update(void)
{
	//�֐��Ăяo��
	TimesGraze();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//���[�h���^�C�g���̎�
	if (CManager::GetMode() == CManager::MODE_RESULT)
	{
		if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			//�����L���O��ʂ֑J��
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RANKING);
		}
	}

	//�J�E���g��i�߂�
	m_nCounterAnim -= 0.0008f;

	for (int nCnt = 1; nCnt < 3; nCnt++)
	{
		//�w�i�̃e�N�X�`������
		m_pScene2D[nCnt]->SetBg(1.5f, m_nCounterAnim * (nCnt + 1));
	}
}

//=============================================================================
//���U���g�̕`�揈��
//=============================================================================
void CResult::Draw(void)
{

}
//=============================================================================
//���U���g�̃e�N�X�`������
//=============================================================================
HRESULT CResult::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlebg3.jpg", &m_pTexture[0]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultMoveBg.png", &m_pTexture[1]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultMoveBg2.png", &m_pTexture[2]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultLogo.png", &m_pTexture[3]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MaxScore.png", &m_pTexture[4]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/MaxGraze.png", &m_pTexture[5]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Times.png", &m_pTexture[6]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultScore.png", &m_pTexture[7]);

	return S_OK;
}
//=============================================================================
//���U���g�̃e�N�X�`���A�����[�h����
//=============================================================================
void CResult::Unload(void)
{
	for (int nCnt = 0; nCnt < 8; nCnt++)
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
//���U���g�̍쐬�E�ݒ菈��
//=============================================================================
CResult * CResult::Create(void)
{
	//�C���X�^���X����
	CResult *pResult = NULL;
	pResult = new CResult;

	//�������Ăяo��
	pResult->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pResult;
}

//=============================================================================
//�O���C�Y�̌v�Z����
//=============================================================================
void CResult::TimesGraze(void)
{
	//�v�Z����
	m_nResultAllScore = m_nResultScore * CGraze::GetGraze();
}

//=============================================================================
//���U���g�̐ݒ菈��
//=============================================================================
void CResult::SetVertex(void)
{
	//���U���g�̃C���X�^���X����
	m_pScene2D[0] = new CScene2D;
	m_pScene2D[0]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[0]->BindTexture(m_pTexture[0]);

	//���U���g�̃C���X�^���X����
	m_pScene2D[1] = new CScene2D;
	m_pScene2D[1]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[1]->BindTexture(m_pTexture[1]);

	//���U���g�̃C���X�^���X����
	m_pScene2D[2] = new CScene2D;
	m_pScene2D[2]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[2]->BindTexture(m_pTexture[2]);

	//���U���g�̃C���X�^���X����
	m_pScene2D[3] = new CScene2D;
	m_pScene2D[3]->Init(D3DXVECTOR3(350.0f, 0.0f, 1.0f), 300.0f, 1200.0f);
	m_pScene2D[3]->BindTexture(m_pTexture[3]);

	//���U���g�̃C���X�^���X����
	m_pScene2D[4] = new CScene2D;
	m_pScene2D[4]->Init(D3DXVECTOR3(200.0f, 350.0f, 1.0f), 150.0f, 600.0f);
	m_pScene2D[4]->BindTexture(m_pTexture[4]);

	//���U���g�̃C���X�^���X����
	m_pScene2D[5] = new CScene2D;
	m_pScene2D[5]->Init(D3DXVECTOR3(1050.0f, 350.0f, 1.0f), 150.0f, 600.0f);
	m_pScene2D[5]->BindTexture(m_pTexture[5]);

	//���U���g�̃C���X�^���X����
	m_pScene2D[6] = new CScene2D;
	m_pScene2D[6]->Init(D3DXVECTOR3(550.0f, 450.0f, 1.0f), 200.0f, 900.0f);
	m_pScene2D[6]->BindTexture(m_pTexture[6]);

	//���U���g�̃C���X�^���X����
	m_pScene2D[7] = new CScene2D;
	m_pScene2D[7]->Init(D3DXVECTOR3(550.0f, 650.0f, 1.0f), 200.0f, 800.0f);
	m_pScene2D[7]->BindTexture(m_pTexture[7]);
}
