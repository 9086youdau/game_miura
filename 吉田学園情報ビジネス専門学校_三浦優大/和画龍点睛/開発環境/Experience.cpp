//================================================
//
//�o���l����[Experience.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "Experience.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"
#include "gamepad.h"
#include "bullet.h"
#include "player.h"
#include "score.h"
#include "game.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
LPDIRECT3DTEXTURE9 CExperience::m_pTexture = NULL;

//=================================================
//�R���X�g���N�^
//=================================================
CExperience::CExperience()
{
}

//=================================================
//�f�X�g���N�^
//=================================================
CExperience::~CExperience()
{
}

//=================================================
//�o���l�̃N���G�C�g
//=================================================
CExperience * CExperience::Create(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�C���X�^���X����
	CExperience *pExperience = NULL;
	pExperience = new CExperience;

	//�������Ăяo��
	pExperience->Init(pos, fHight, fWidth);

	//�e�N�X�`���Ăяo��
	pExperience->BindTexture(m_pTexture);

	return pExperience;
}

//=================================================
//�o���l�̏���������
//=================================================
HRESULT CExperience::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//����������
	m_pos = pos;

	//�����o�ϐ�������
	m_bHoming = false;

	//�Ԃ�l
	return S_OK;
}

//=================================================
//�o���l�̏I������
//=================================================
void CExperience::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}

//=================================================
//�o���l�̍X�V����
//=================================================
void CExperience::Update(void)
{
	//�|�[�Y�̑��
	bool pause = false;
	pause = CGame::GetPause();

	//���[�J���ϐ��錾
	D3DXVECTOR3 posplayer;
	D3DXVECTOR3 vec;
	float fAngle;

	if (pause == false)
	{
		//�v���C���[�̍��W���Q�b�g
		posplayer = CPlayer::GetPlayer();

		vec = posplayer - m_pos;//���ݒn����ړI�n�ւ̃x�N�g��
		fAngle = atan2f(vec.x, vec.y);//�x�N�g���̊p�x�����߂�

		if (m_bHoming == false)
		{
			//�ړ�������
			m_move.x = sinf(D3DX_PI) * -1.5f;//�ړ���
			m_move.y = cosf(D3DX_PI) * -1.5f;//�ړ���
		}
		else
		{
			//�v���C���[�̏�Ԃ�NOMAL��������
			if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
			{
				//�ړ�������
				m_move.x = sinf(fAngle) * 12.0f;//�ړ���
				m_move.y = cosf(fAngle) * 12.0f;//�ړ���
			}
			else
			{
				//�ړ�������
				m_move.x = sinf(D3DX_PI) * -1.5f;//�ړ���
				m_move.y = cosf(D3DX_PI) * -1.5f;//�ړ���
			}
		}

		//�ʒu�X�V
		m_pos += m_move;

		//�e�̈ʒu�X�V
		SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//�|�C���^��NULL�ɂ���
		CScene *pScene = NULL;

		//�v���C���[�̒e�ƓG�̓����蔻��
		if (Collision(m_pos, 70.0f, 70.0f, CScene::OBJTYPE_PLAYER, &pScene) == true)
		{
			//�z�[�~���O�̔����true�ɂ���
			m_bHoming = true;

			if (Collision(m_pos, 40.0f, 40.0f, CScene::OBJTYPE_PLAYER, &pScene) == true)
			{
				//�X�R�A���Z
				CScore::AddScore(100);

				//�o���l������
				Uninit();
			}
		}

		if (m_pos.y >= 1200)
		{
			//�o���l������
			Uninit();
		}
	}
}

//=================================================
//�o���l�̕`�揈��
//=================================================
void CExperience::Draw(void)
{
	//��{�N���X�̕`�揈��
	CScene2D::Draw();
}

//=================================================
//�o���l�̃e�N�X�`�����[�h����
//=================================================
HRESULT CExperience::Load()
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Expre.png", &m_pTexture);

	return S_OK;
}

//=================================================
//�o���l�̃e�N�X�`���A�����[�h����
//=================================================
void CExperience::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

