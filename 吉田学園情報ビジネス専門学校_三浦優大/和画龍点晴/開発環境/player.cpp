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
#include "bullet.h"
#include "Renderer.h"
#include "explosion.h"
#include "sound.h"
#include "Score.h"
#include "effect.h"
#include "gamepad.h"
#include "game.h"
#include "particle.h"
#include "life.h"
#include "fade.h"
#include "Experience.h"
#include "playerattack.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
D3DXVECTOR3 CPlayer::m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 CPlayer::m_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CPlayer::PLAYERSTATE CPlayer::m_playerstate = PLAYERSTATE_NOMAL;
int CPlayer::m_statecount = 0;
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
	//�^�C�v�����֐�
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�����o�ϐ�������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_playerstate = PLAYERSTATE_NOMAL;
	m_statecount = 0;

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
	PlayerCol();

	//�֐��Ăяo��
	InitPlayerMove();

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 40.0f, 40.0f, D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.6f), CEffect::EFFECTTYPE_PLAYER_MOVE);
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 100.0f, 100.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), CEffect::EFFECTTYPE_NONE);

	//�|�C���^��NULL�ɂ���
	CScene *pScene = NULL;

	//�G�ƃv���C���[�̓����蔻��
	if (Collision(m_pos, 50, 50, CScene::OBJTYPE_ENEMY, &pScene) == true)
	{
		if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
		{
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
			{
				//�p�[�e�B�N���𐶐�
				CParticle::Create(m_pos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_PLAYER);
			}

			//���C�t�v�Z
			m_nLifeCount = CBullet::GetLife();

			//HP�����炷����
			CLife::SetRelease(m_nLifeCount);

			//�J�E���g�A�b�v
			m_nLifeCount--;

			//�Z�b�g�̃��C�t����
			CBullet::SetLife(1);

			if (m_nLifeCount > -1)
			{
				//�v���C���[�̍폜
				Uninit();

				//�������烊�Z�b�g
				CPlayer::Create(D3DXVECTOR3(550.0f, 1200.0f, 0.0f));

				//��ԕω��֐�
				CPlayer::SetStatePlayer(CPlayer::PLAYERSTATE_DAMAGE);
			}
			else if (m_nLifeCount == -1)
			{
				//�v���C���[�̍폜
				Uninit();

				//�v���C���[�A�^�b�N��true�ɕύX
				m_playerattack = true;

				//���C�t�����Z�b�g����
				m_nLifeCount = 3;

				//�Q�[����ʂ֑J��
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}
	}
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

	//�e�̏����̃J�E���g
	m_shotcount++;

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
//�v���C���[�̏�ԕω�����
//=============================================================================
void CPlayer::SetStatePlayer(PLAYERSTATE playerstate)
{
	//����������
	switch (playerstate)
	{
	case PLAYERSTATE_NOMAL:
		m_playerstate = PLAYERSTATE_NOMAL;
		break;
	case PLAYERSTATE_DAMAGE:
		m_playerstate = PLAYERSTATE_DAMAGE;
		m_statecount = 150;
		break;
	}
}
//=============================================================================
//�v���C���[�̐F�ύX����
//=============================================================================
void CPlayer::PlayerCol(void)
{
	switch (m_playerstate)
	{
	case PLAYERSTATE_NOMAL:
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case PLAYERSTATE_DAMAGE:
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		m_statecount--;
		if (m_statecount <= 0)
		{
			m_playerstate = PLAYERSTATE_NOMAL;
		}
		break;
	}
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
