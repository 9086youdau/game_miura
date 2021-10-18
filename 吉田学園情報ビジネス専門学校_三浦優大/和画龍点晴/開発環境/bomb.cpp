//================================================
//
//�{������[bomb.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "bomb.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"
#include "gamepad.h"
#include "bullet.h"
#include "player.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
LPDIRECT3DTEXTURE9 CBomb::m_pTexture = NULL;
bool CBomb::m_bBomb = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CBomb::CBomb(int nPriority) : CScene2D(nPriority)
{
	//�����o�ϐ�������
	m_bBomb = false;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CBomb::~CBomb()
{
}

//=============================================================================
//�{���̏���������
//=============================================================================
HRESULT CBomb::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�^�C�v�����֐�
	CScene::SetObjType(CScene::OBJTYPE_BOMB);

	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�Ԃ�l
	return S_OK;
}

//=============================================================================
//�{���̏I������
//=============================================================================
void CBomb::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}

//=============================================================================
//�{���̍X�V����
//=============================================================================
void CBomb::Update(void)
{
	//�L�[�{�[�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//���[�J���ϐ��錾
	float fAngle;
	float rad = 0.0f;
	D3DXVECTOR3 player;
	D3DXVECTOR3 move;

	//���[�J���ϐ��ɑ��
	player = CPlayer::GetPlayer();
	fAngle = D3DX_PI * 2 / 8;//�e�͈̔�
	rad = fAngle / 2;

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_ATTACK) == true || pInputKeyboard->GetTrigger(DIK_X) == true)
	{
		if (m_nCnt == 0)
		{
			for (int nCntAppear = 0; nCntAppear < 8; nCntAppear++, rad += fAngle)
			{
				//�e�̃N���G�C�g
				CBullet::Create(player, CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPEDAMAGE_BOMB, rad, MAX_SIZE_BOMB_BULLET, MAX_SIZE_BOMB_BULLET, 0.0f);
			}

			//�J�E���g�A�b�v
			m_nCnt++;

			//�J�E���g���Z�b�g
			m_nbombcool1 = 0;

			//�t���O�ƂȂ�bool
			m_bBomb = true;
		}
	}

	//�{���̃N�[���^�C������
	if (m_nCnt == 1)
	{
		m_nbombcool1++;
	}
	//�{���̃N�[���^�C������
	if (m_nbombcool1 >= 200)
	{
		m_nCnt = 0;
	}
}

//=============================================================================
//�{���`�揈��
//=============================================================================
void CBomb::Draw(void)
{
	//��{�N���X�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//�{���̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CBomb::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/star.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�{���̃e�N�X�`���j������
//=============================================================================
void CBomb::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//�{���̐ݒ�E�쐬����
//=============================================================================
CBomb * CBomb::Create(D3DXVECTOR3 pos)
{
	//�C���X�^���X����
	CBomb *pBomb = NULL;
	pBomb = new CBomb;

	//�������Ăяo��
	pBomb->Init(pos, 60.0f, 60.0f);
	pBomb->BindTexture(m_pTexture);

	return pBomb;
}

//=============================================================================
//�{���̃Z�b�g����
//=============================================================================
void CBomb::SetBomb(bool bBomb)
{
	//��������bool���Q��
	m_bBomb = bBomb;
}
