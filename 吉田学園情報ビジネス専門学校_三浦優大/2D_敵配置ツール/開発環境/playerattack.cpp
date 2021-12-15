//================================================
//
//�v���C���[�A�^�b�N����[playerattack.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "playerattack.h"
#include "input.h"
#include "manager.h"
#include "Renderer.h"
#include "sound.h"
#include "gamepad.h"
#include "game.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_SIZE_PLAYER_BULLET (30)//�o���b�g�̃T�C�Y

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayerAttack::m_pTexture = NULL;
D3DXVECTOR3 CPlayerAttack::m_pos[2] = {};

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CPlayerAttack::CPlayerAttack(int nPriority) : CScene2D(nPriority)
{
	//�����o�ϐ�������
	m_raise[0] = 2;
	m_toggle = false;
}

//=============================================================================
//�f�X�g�g���N�^
//=============================================================================
CPlayerAttack::~CPlayerAttack()
{
}

//=============================================================================
//�v���C���[�A�^�b�N�̐����E�ݒ�
//=============================================================================
CPlayerAttack * CPlayerAttack::Create(D3DXVECTOR3 pos,PLAYERATTACK playerattack)
{
	//�C���X�^���X����
	CPlayerAttack *pPlayerAttack = NULL;
	pPlayerAttack = new CPlayerAttack;

	if (pPlayerAttack != NULL)
	{
		pPlayerAttack->Init(pos, 12.0f, 12.0f, playerattack);
	}

	//�e�N�X�`���ݒ�֐�
	pPlayerAttack->BindTexture(m_pTexture);

	//�Ԃ�l
	return pPlayerAttack;
}

//=============================================================================
//�v���C���[�A�^�b�N�̏���������
//=============================================================================
HRESULT CPlayerAttack::Init(D3DXVECTOR3 pos, float fHight, float fWidth,PLAYERATTACK playerattack)
{
	//�^�C�v�����֐�
	CScene::SetObjType(CScene::OBJTYPE_PLAYER_ATTACK);

	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�����o�ϐ�������
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_move[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_pos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//�����o�ϐ�������
	m_playerattack = playerattack;

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//�v���C���[�A�^�b�N�̏I������
//=============================================================================
void CPlayerAttack::Uninit(void)
{
	//�I������
	CScene2D::Uninit();
}

//=============================================================================
//�v���C���[�A�^�b�N�̍X�V����
//=============================================================================
void CPlayerAttack::Update(void)
{
	//�|�[�Y�̑��
	bool pause = false;
	pause = CGame::GetPause();

	if (pause == false)
	{
		//�֐��Ăяo��
		InitPlayerAttackMove();
	}
}

//=============================================================================
//�v���C���[�A�^�b�N�̕`�揈��
//=============================================================================
void CPlayerAttack::Draw(void)
{
	//�`�揈��
	CScene2D::Draw();
}

//=============================================================================
//�v���C���[�A�^�b�N�̈ړ�����
//=============================================================================
void CPlayerAttack::PlayerAttackMove(void)
{
}

//=============================================================================
//�v���C���[�A�^�b�N�̈ړ�����
//=============================================================================
void CPlayerAttack::InitPlayerAttackMove(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�T�E���h�̃|�C���^�錾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�J�E���g�A�b�v
	m_shotcount++;

	if (m_playerattack == PLAYERATTACK_RIGHT)
	{
		//�ʒu���擾
		m_pos[0] = GetPosition();

		//��]����
		m_rot[0].z += 0.1f;

		//�v���C���[�ɑ΂��Ă̏���
		m_homingAngle[0] += m_raise[0];

		m_pos[0].x = CPlayer::GetPlayer().x + 70.0f;//�ړ��ʂ����߂�
		m_pos[0].y = CPlayer::GetPlayer().y + sinf(m_homingAngle[0] * D3DX_PI / 180.0f) * 15.0f;

		if (m_homingAngle[0] == 90)
		{
			m_toggle = true;
		}
		else if (m_homingAngle[0] == -90)
		{
			m_toggle = false;
		}

		if (m_toggle)
		{
			m_raise[0] = -2;
		}
		else
		{
			m_raise[0] = 2;
		}

		//�e����
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_OK) == true || pInputKeyboard->GetPress(DIK_Z) == true)
		{
			float rad = 0.0f;

			if (m_shotcount % 7 == 0)
			{
				//�e�̃N���G�C�g
				CBullet::Create(m_pos[0], CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPEDAMAGE_NOMALSHOT, rad, MAX_SIZE_PLAYER_BULLET, MAX_SIZE_PLAYER_BULLET, 0.0f);
			}
		}

		//�|���S���̈ʒu�֐�
		SetPosition(m_pos[0], m_rot[0]);
	}
	else
	{
		//�ʒu���擾
		m_pos[1] = GetPosition();

		//��]����
		m_rot[1].z -= 0.1f;

		m_homingAngle[1] += m_raise[0];

		m_pos[1].x = CPlayer::GetPlayer().x - 70.0f;//�ړ��ʂ����߂�
		m_pos[1].y = CPlayer::GetPlayer().y + sinf(m_homingAngle[1] * D3DX_PI / 180.0f) * 15.0f;

		if (m_homingAngle[1] == 90)
		{
			m_toggle = true;
		}
		else if (m_homingAngle[1] == -90)
		{
			m_toggle = false;
		}

		if (m_toggle)
		{
			m_raise[1] = -2;
		}
		else
		{
			m_raise[1] = 2;
		}

		//�e����
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_OK) == true || pInputKeyboard->GetPress(DIK_Z) == true)
		{
			float rad = 0.0f;

			if (m_shotcount % 7 == 0)
			{
				//�e�̃N���G�C�g
				CBullet::Create(m_pos[1], CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPEDAMAGE_NOMALSHOT, rad, MAX_SIZE_PLAYER_BULLET, MAX_SIZE_PLAYER_BULLET, 0.0f);
			}
		}

		//�|���S���̈ʒu�֐�
		SetPosition(m_pos[1], m_rot[1]);
	}
}

//=============================================================================
//�v���C���[�A�^�b�N�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CPlayerAttack::Load()
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/AttackPoint.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�v���C���[�A�^�b�N�̃e�N�X�`���A�����[�h����
//=============================================================================
void CPlayerAttack::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
