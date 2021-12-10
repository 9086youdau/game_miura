//================================================
//
//�v���C���[����[player.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "Renderer.h"
#include "sound.h"
#include "gamepad.h"
#include "game.h"
#include "fade.h"
#include "playerattack.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
D3DXVECTOR3 CPlayer::m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 CPlayer::m_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
bool CPlayer::m_playerattack = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	//�����o�ϐ�������
	m_Pause = false;
	m_bInit = false;
	m_playerattack = false;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}
//=============================================================================
//�v���C���[�̐����E�ݒ�
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	//�C���X�^���X����
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;

	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, MAXSIZEPLAYER, MAXSIZEPLAYER);
	}

	//�e�N�X�`���ݒ�֐�
	pPlayer->BindTexture(m_pTexture);

	//�Ԃ�l
	return pPlayer;
}
//=============================================================================
//�v���C���[�̏���������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�^�C�v�����֐�
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�����o�ϐ�������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�Ԃ�l
	return S_OK;
}
//=============================================================================
//�v���C���[�̏I������
//=============================================================================
void CPlayer::Uninit(void)
{
	//�I������
	CScene2D::Uninit();
}
//=============================================================================
//�v���C���[�̍X�V����
//=============================================================================
void CPlayer::Update(void)
{
	//�֐��Ăяo��
	InitPlayerMove();
}
//=============================================================================
//�v���C���[�̕`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	//�`�揈��
	CScene2D::Draw();
}
//=============================================================================
//�v���C���[�̈ړ�����
//=============================================================================
void CPlayer::PlayerMove(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�ʒu���擾
	m_pos = GetPosition();

	//�̂̈ʒu��ݒ�
	m_oldpos = m_pos;

	//�΂ߌv�Z����
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true
		|| pInputKeyboard->GetPress(DIK_RIGHT) == true || pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_UP) == true || pInputGamePad->IsButtonPush(CInput::KEYINFO_DOWN) == true
			|| pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
		{
			//�΂߈ړ��̃����o�ϐ��ɒl����
			m_DiagonalMove = 0.71f;
		}
		else
		{
			//�΂߈ړ��̃����o�ϐ��ɒl����
			m_DiagonalMove = 1.0f;
		}
	}
	else if (pInputGamePad->IsButtonPush(CInput::KEYINFO_UP) == true || pInputGamePad->IsButtonPush(CInput::KEYINFO_DOWN) == true
		|| pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
	{
		//�΂߈ړ��̃����o�ϐ��ɒl����
		m_DiagonalMove = 1.0f;
	}

	//�E�ړ�
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		//�ړ���
		m_move.x += sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;

		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//�ړ���
			m_move.x += sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//���ړ�
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		//�ړ���
		m_move.x -= sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;

		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//�ړ���
			m_move.x -= sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//��ړ�
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_UP) == true || pInputKeyboard->GetPress(DIK_UP) == true)
	{
		//�ړ���
		m_move.y -= sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//�ړ���
			m_move.y -= sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//���ړ�
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_DOWN) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
	{
		//�ړ���
		m_move.y += sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//�ړ���
			m_move.y += sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//��ʊO����
	if (m_pos.x >= 1000)
	{
		m_pos.x = 1000;
	}
	else if (m_pos.x <= 80)
	{
		m_pos.x = 80;
	}
	//��ʊO����
	if (m_pos.y <= 80)
	{
		m_pos.y = 80;
	}
	else if (m_pos.y >= 980)
	{
		m_pos.y = 980;
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.3f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�|���S���̈ʒu�֐�
	SetPosition(m_pos,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//�v���C���[�̏����ړ�����
//=============================================================================
void CPlayer::InitPlayerMove(void)
{
	//�ʒu���擾
	m_pos = GetPosition();

	//bool�̒l��Ԃ�
	m_Pause = CGame::GetPause();

	//�|�[�Y���
	if (m_Pause == false)
	{
		if (m_bInit == false)
		{
			//�ړ���
			m_move.y -= sinf(D3DX_PI * 0.5f) * 2.0f;

			if (m_pos.y <= 900)
			{
				m_bInit = true;
			}
		}
		else
		{
			PlayerMove();
		}
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.3f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�|���S���̈ʒu�֐�
	SetPosition(m_pos,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//�o���b�g�̃e�N�X�`������
//=============================================================================
HRESULT CPlayer::Load()
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ddd.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�o���b�g�̃e�N�X�`���̔p������
//=============================================================================
void CPlayer::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
