//=============================================================================
//
// �G�l�~�[����[enemy.cpp]
// Author; miura yuudai
//
//=============================================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include <time.h>
#include "enemy.h"
#include "Renderer.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "game.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_SIZE_PLAYER_BULLET (30)

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[ENEMYTYPE_MAX] = {};
D3DXVECTOR3 CEnemy::m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
CEnemy::ENEMY_DAMAGE_STATE CEnemy::m_EnemyState = ENEMY_DAMAGE_STATE_NONE;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	//�����o�ϐ��̏�����
	m_nPatternAnim = 0;
	m_nLife = 100;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}
//=============================================================================
//�G�l�~�[�̏���������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, float fHight, float fWidth, ENEMYTYPE enemyType,float fMove, int nHoming)
{
	//��{�N���X�̏���������
	CScene2D::Init(pos, fHight, fWidth);

	//��{�N���X�̐F����������
	SetEnemyCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�֐��Ăяo��
	SetTex(1.0f, m_nPatternAnim);

	//�l����
	m_enemyType = enemyType;

	//�G�l�~�[�̃^�C�v����
	switch (m_enemyType)
	{
	case ENEMYTYPE_ONE:
		//�^�C�v��������
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_TWO:
		//�^�C�v��������
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_THREE:
		//�^�C�v��������
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_FOR:
		//�^�C�v��������
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_BOSS:
		//�^�C�v��������
		CScene::SetObjType(CScene::OBJTYPE_ENEMY_BOSS);
		break;
	}

	//��������l����
	m_fMove = fMove;
	m_HomingText = nHoming;

	//�Ԃ�l
	return S_OK;
}
//=============================================================================
//�G�l�~�[�̏I������
//=============================================================================
void CEnemy::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}
//=============================================================================
//�G�l�~�[�̍X�V����
//=============================================================================
void CEnemy::Update(void)
{
	//�|�[�Y�̑��
	bool pause = false;
	pause = CGame::GetPause();

	//�|�[�Y��Ԃ���Ȃ�������
	if (pause == false)
	{
		//�G�l�~�[�̃^�C�v����
		switch (m_enemyType)
		{
		case ENEMYTYPE_ONE:
			//�֐��Ăяo��
			//move�̊֐��Ăяo��
			EnemyMoveOne();
			break;
		case ENEMYTYPE_TWO:
			//move�̊֐��Ăяo��
			EnemyMoveTwo();
			break;
		case ENEMYTYPE_THREE:
			//move�̊֐��Ăяo��
			EnemyMoveThree();
			break;
		case ENEMYTYPE_FOR:
			//move�̊֐��Ăяo��
			EnemyMoveFor();
			break;
		case ENEMYTYPE_BOSS:
			//move�̊֐��Ăяo��
			EnemyBossMove();
			break;
		}
	}
}
//=============================================================================
//�G�l�~�[�̕`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	//��{�N���X�̕`��
	CScene2D::Draw();
}

//=============================================================================
//�o���b�g�̃e�N�X�`������
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[0]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[1]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[2]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[3]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[4]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[5]);

	return S_OK;
}

//=============================================================================
//�o���b�g�̃e�N�X�`���̔p������
//=============================================================================
void CEnemy::Unload(void)
{
	//�z�񕪃��[�v����
	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
//�G�l�~�[�̐ݒ�E�X�V����
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType,float fMove, int nHoming)
{
	//�C���X�^���X����
	CEnemy *pEnemy;
	pEnemy = new CEnemy;

	//����������
	pEnemy->Init(pos, fHight, fWidth, enemyType, fMove, nHoming);

	//�e�N�X�`���ݒ�
	pEnemy->BindTexture(m_apTexture[enemyType]);

	return pEnemy;
}
//=============================================================================
//�G�l�~�[�̓�������
//=============================================================================
void CEnemy::EnemyMoveOne(void)
{
	//���[�J���ϐ��錾
	float fAngle;
	float rad = 0.0f;

	//�ϐ����
	fAngle = D3DX_PI * 2 / 10;//�e�͈̔�
	rad = fAngle / 2;

	//�l����
	m_pos = GetPosition();

	//�J�E���g���Z
	m_nEnemy++;

	//�J�E���g��300�܂ł�������
	if (m_nEnemy < 300)
	{
		//�ړ���
		m_move.y += 0.5f + m_fMove;
	}
	else if (m_nEnemy >= 300)
	{
		//�ړ���0.5�ɂ���
		m_move.y -= 0.3f + m_fMove;
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.3f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�|���S���̈ʒu�֐�
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//��ʊO����
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//�I������
		Uninit();
	}
}

//=============================================================================
//�G�l�~�[�̓�������
//=============================================================================
void CEnemy::EnemyMoveTwo(void)
{
	//�l����
	m_pos = GetPosition();

	//���̃��C���܂ōs������
	if (m_pos.y >= 20)
	{
		//�J�E���g�A�b�v
		m_nCountParticle -= 0.6f;
	}

	//���[�J���ϐ��錾
	float fAngle;

	fAngle = (m_nCountParticle) / 100.0f + D3DX_PI * 2.0f;
	m_move.x = sinf(fAngle) * 2.5f;
	m_move.y = cosf(fAngle) * 2.5f;

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.2f + m_fMove;
	m_move.y += (0.0f - m_move.y) * 0.2f + m_fMove;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�|���S���̈ʒu�֐�
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//��ʊO����
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//�I������
		Uninit();
	}
}

//=============================================================================
//�G�l�~�[�̓�������
//=============================================================================
void CEnemy::EnemyMoveThree(void)
{
	//�l����
	m_pos = GetPosition();

	//���̃��C���܂ōs������
	if (m_pos.y >= 20)
	{
		//�J�E���g�A�b�v
		m_nCountParticle += 0.6f;
	}

	float rad = 0.0f;

	//���[�J���ϐ��錾
	float fAngle;

	fAngle = (m_nCountParticle) / 100.0f + D3DX_PI * 2.0f;
	m_move.x = sinf(fAngle) * 2.5f + m_fMove;
	m_move.y = cosf(fAngle) * 2.5f + m_fMove;

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//��ʊO����
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//�I������
		Uninit();
	}
}
//=============================================================================
//�G�l�~�[�̓�������
//=============================================================================
void CEnemy::EnemyMoveFor(void)
{
	//�l����
	m_pos = GetPosition();

	//�J�E���g����
	m_HomingCount++;

	if (m_HomingCount <= m_HomingText)
	{
		m_vec = CPlayer::GetPlayer() - m_pos;//���ݒn����ړI�n�ւ̃x�N�g��
		m_homingAngle = atan2f(m_vec.x, m_vec.y);//�x�N�g���̊p�x�����߂�

		m_move.x = sinf(m_homingAngle) * (1.0f + m_fMove);//�ړ��ʂ����߂�
		m_move.y = cosf(m_homingAngle) * (1.0f + m_fMove);//�ړ��ʂ����߂�
	}
	else
	{ 
		m_move.y += (m_fMove / 2 + 0.5f);
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�|���S���̈ʒu�֐�
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//��ʊO����
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//�I������
		Uninit();
	}
}
//=============================================================================
//�G�l�~�[�̓�������
//=============================================================================
void CEnemy::EnemyBossMove(void)
{
	//���[�J���ϐ��錾
	float fAngle;
	float rad = 0.0f;

	//�ϐ����
	fAngle = D3DX_PI * 2 / 40;//�e�͈̔�
	rad = fAngle / 2;

	//�l����
	m_pos = GetPosition();

	//�J�E���g�A�b�v�@
	m_BossCount++;
	m_SircleCnt++;

	//�ړ���
	m_move.y += sinf(D3DX_PI * 0.5f) * 0.5f + m_fMove;

	//�{�X��pos��300�ȏ�ɂȂ�����
	if (m_pos.y >= 300)
	{
		//�ړ���0.0�ɂ���
		m_move.y = 0.0f;
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�|���S���̈ʒu�֐�
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}



