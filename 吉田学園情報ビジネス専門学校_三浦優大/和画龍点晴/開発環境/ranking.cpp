//================================================
//
//�����L���O����[ranking.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "ranking.h"
#include "number.h"
#include "score.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "gamepad.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "result.h"
#include "Graze.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CRanking::m_pTexture[4] = {};
CScore *CRanking::m_pScore = NULL;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
//�����L���O�̏���������
//=============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{ 
	//�����L���O�e�N�X�`�����[�h
	Load();

	//�X�R�A�̃e�N�X�`�����[�h
	CNumber::Load();

	//�֐��Ăяo��
	SetVertex();

	//�����L���O�X�R�A�\��
	m_pScore = CScore::Create(CScore::SCORETYPE_RANKING_SCORE);

	//�O���C�Y�̏�����
	CGraze::CalculatioGraze(0);

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//�����L���O�̏I������
//=============================================================================
void CRanking::Uninit(void)
{
	//�e�N�X�`���I������
	Unload();

	//�X�R�A�̃e�N�X�`�����[�h
	CNumber::Unload();

	//�I������
	m_pScore->Uninit();
	m_pScore = NULL;

	//�j���֐�
	Release();
}

//=============================================================================
//�����L���O�̍X�V����
//=============================================================================
void CRanking::Update(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		//�Q�[����ʂ֑J��
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//=============================================================================
//�����L���O�̕`�揈��
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
//�����L���O�̐ݒ�E�쐬����
//=============================================================================
CRanking * CRanking::Create(void)
{
	//�C���X�^���X����
	CRanking *pRanking = NULL;
	pRanking = new CRanking;

	//�������Ăяo��
	pRanking->Init(D3DXVECTOR3(0.0f,0.0f,0.0f), MAX_SCORE_SIZE_Y, MAX_SCORE_SIZE_X);

	return pRanking;
}

//==============================================================================
//���ʐݒ菈��
//==============================================================================
void CRanking::SetVertex(void)
{
	float m_Rank = 0;

	//�����L���O�̏��ʃC���X�^���X����
	m_pScene2D[0] = new CScene2D;
	m_pScene2D[0]->Init(D3DXVECTOR3(0.0f, 0.0f, 1.0f), SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScene2D[0]->BindTexture(m_pTexture[0]);

	for (int nCnt = 1; nCnt < 6; nCnt++)
	{
		//�����L���O�̏��ʃC���X�^���X����
		m_pScene2D[nCnt] = new CScene2D;
		m_pScene2D[nCnt]->Init(D3DXVECTOR3((nCnt * 55.0f) + 100.0f,(nCnt * 155.0f) + 120.0f, 1.0f), 120.0f, 250.0f);
		m_pScene2D[nCnt]->BindTexture(m_pTexture[1]);
		m_pScene2D[nCnt]->SetRanking(m_Rank);

		m_Rank += 0.2f;
	}

	//�����L���O�̏��ʃC���X�^���X����
	m_pScene2D[6] = new CScene2D;
	m_pScene2D[6]->Init(D3DXVECTOR3(350.0f, 0.0f, 1.0f), 300.0f, 1200.0f);
	m_pScene2D[6]->BindTexture(m_pTexture[2]);

	int nCntY = 0;

	for (int nCnt = 7; nCnt < 12; nCnt++)
	{
		//�����L���O�̏��ʃC���X�^���X����
		m_pScene2D[nCnt] = new CScene2D;
		m_pScene2D[nCnt]->Init(D3DXVECTOR3((nCnt * 55.0f) + 210.0f, (nCntY * 155.0f) + 270.0f, 1.0f), 120.0f, 900.0f);
		m_pScene2D[nCnt]->BindTexture(m_pTexture[3]);

		//�J�E���g�A�b�v
		nCntY += 1;
	}
}
//=============================================================================
//�����L���O�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CRanking::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlebg3.jpg", &m_pTexture[0]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Ranking_Rank2.png", &m_pTexture[1]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RankingMenu.png", &m_pTexture[2]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TITLECOVER.png", &m_pTexture[3]);

	return S_OK;
}
//=============================================================================
//�����L���O�̃e�N�X�`���A�����[�h����
//=============================================================================
void CRanking::Unload(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
