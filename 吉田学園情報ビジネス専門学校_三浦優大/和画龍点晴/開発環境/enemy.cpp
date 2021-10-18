//================================================
//
//�G�l�~�[����[enemy.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include <time.h>
#include "enemy.h"
#include "Renderer.h"
#include "manager.h"
#include "bullet.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "pause.h"
#include "CircleHp.h"
#include "laser.h"
#include "Experience.h"
#include "effect.h"
#include "particle.h"

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
HRESULT CEnemy::Init(D3DXVECTOR3 pos, float fHight, float fWidth, ENEMYTYPE enemyType)
{
	//��{�N���X�̏���������
	CScene2D::Init(pos, fHight, fWidth);

	//��{�N���X�̐F����������
	SetEnemyCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�֐��Ăяo��
	SetTex(MAX_ENEMY_TEXTURE, m_nPatternAnim);

	//�����o�ϐ��̏�����
	m_nPatternAnim = 0;
	m_nCounterAnim = 0;
	m_nEnemyCnt = 0;
	m_nLife = 100;

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

	//�J�E���g�A�b�v
	m_nBulletCount++;

	//��Ԃ��_���[�W��Ԃ�������
	if (m_EnemyState == ENEMY_DAMAGE_STATE_DAMAGE)
	{
		//�J�E���g�����炷
		m_nCountEnemy--;
		if (m_nCountEnemy <= 0)
		{
			//�G�l�~�[�̃��[�h��ʏ��ԂɕύX����
			ENEMY_DAMAGE_STATE_NONE;
			SetCol(D3DXCOLOR(255, 255, 255, 255));
		}
	}

	if (pause == false)
	{
		//�G�l�~�[�̃^�C�v����
		switch (m_enemyType)
		{
		case ENEMYTYPE_ONE:
			//�֐��Ăяo��
			SetTex(1.0f, m_nPatternAnim);
			//move�̊֐��Ăяo��
			EnemyMove2();
			break;
		case ENEMYTYPE_TWO:
			//�֐��Ăяo��
			SetTex(1.0f, m_nPatternAnim);
			//move�̊֐��Ăяo��
			EnemyMove3();
			break;
		case ENEMYTYPE_THREE:
			//�֐��Ăяo��
			SetTex(1.0f, m_nPatternAnim);
			//move�̊֐��Ăяo��
			EnemyMove4();
			break;
		case ENEMYTYPE_FOR:
			//�֐��Ăяo��
			SetTex(1.0f, m_nPatternAnim);
			//move�̊֐��Ăяo��
			EnemyMove5();
			break;
		case ENEMYTYPE_BOSS:
			//�֐��Ăяo��
			SetTex(1.0f, m_nPatternAnim);
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
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType)
{
	//�C���X�^���X����
	CEnemy *pEnemy;
	pEnemy = new CEnemy;

	//����������
	pEnemy->Init(pos, fHight, fWidth,enemyType);

	//�e�N�X�`���ݒ�
	pEnemy->BindTexture(m_apTexture[enemyType]);

	return pEnemy;
}
//=============================================================================
//�G�l�~�[�̓�������2
//=============================================================================
void CEnemy::EnemyMove2(void)
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

	if (m_nEnemy < 300)
	{
		//�ړ���
		m_move.y += 0.5f;
	}
	else if (m_nEnemy >= 300)
	{
		//�ړ���0.5�ɂ���
		m_move.y -= 0.3f;
	}

	if (m_nBulletCount % 125 == 0)
	{
		//�e�̃N���G�C�g
		for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++, rad += fAngle)
		{
			//�e�̃N���G�C�g
			CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULEETTYPEDAMAGE_SIRCLESHOT, rad, 20.0f, 20.0f, 0.0f);
		}
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.3f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 85.0f, 85.0f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.6f), CEffect::EFFECTTYPE_NONE);

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
//�G�l�~�[�̓�������3
//=============================================================================
void CEnemy::EnemyMove3(void)
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

	if (m_nBulletCount % 105 == 0)
	{
		//�e�̃N���G�C�g
		CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYNOMALSHOT, m_fRad, 25.0f, 25.0f, 0.0f);
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 85.0f, 85.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.6f), CEffect::EFFECTTYPE_NONE);

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
//�G�l�~�[�̓�������4
//=============================================================================
void CEnemy::EnemyMove4(void)
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

	if (m_nBulletCount % 105 == 0)
	{
		//�e�̃N���G�C�g
		CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYNOMALSHOT, m_fRad, 25.0f, 25.0f,0.0f);
	}

	//���[�J���ϐ��錾
	float fAngle;

	fAngle = (m_nCountParticle) / 100.0f + D3DX_PI * 2.0f;
	m_move.x = sinf(fAngle) * 2.5f;
	m_move.y = cosf(fAngle) * 2.5f;

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 85.0f, 85.0f,D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.6f), CEffect::EFFECTTYPE_NONE);

	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//��ʊO����
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//�I������
		Uninit();
	}
}
//=============================================================================
//�G�l�~�[�̓�������5
//=============================================================================
void CEnemy::EnemyMove5(void)
{
	//�l����
	m_pos = GetPosition();

	//�G�l�~�[�X�s�[�h
	m_move.y += 0.3f;

	//���̃��C���܂ōs������
	if (m_pos.y >= 300 && m_pos.y <= 600)
	{
		m_vec = CPlayer::GetOldPlayer() - m_pos;//���ݒn����ړI�n�ւ̃x�N�g��
		m_homingAngle = atan2f(m_vec.x, m_vec.y);//�x�N�g���̊p�x�����߂�

		if (m_nBulletCount % 85 == 0)
		{
			//�e�̃N���G�C�g
			CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYNOMALSHOT, m_fRad, MAX_SIZE_PLAYER_BULLET, MAX_SIZE_PLAYER_BULLET, 0.0f);
		}
	}

	if (m_pos.y >= 400)
	{
		m_move.x = sinf(m_homingAngle) * 1.0f;//�ړ��ʂ����߂�
		m_move.y = cosf(m_homingAngle) * 1.0f;//�ړ��ʂ����߂�
	}

	//�ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//�ʒu�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 100.0f, 100.0f,D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), CEffect::EFFECTTYPE_NONE);

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
//�G�̃{�X�̓���
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
	m_move.y += sinf(D3DX_PI * 0.5f) * 0.5f;

	if (m_pos.y >= 300)
	{
		//�ړ���0.0�ɂ���
		m_move.y = 0.0f;
	}

	if (m_BossCount <= 700)
	{
		if (m_BossCount % 150 == 0)
		{
			//�e�̃N���G�C�g
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++, rad += fAngle)
			{
				//�e�̃N���G�C�g
				CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULEETTYPEDAMAGE_SIRCLESHOT, rad, 20.0f, 20.0f, 0.0f);
			}
		}
		if (m_BossCount % 350 == 0)
		{
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++, rad += fAngle)
			{
				//�e�̃N���G�C�g
				CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYMOVESIRCLE, rad, 20.0f, 20.0f, 0.0f);
			}
		}
	}

	if (m_BossCount >= 700)
	{
		if (m_SircleCnt % 5 == 0)
		{
			m_fAngle += 0.05f;

			//�e�̃N���G�C�g
			CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYCOUNT, rad, 20.0f, 20.0f, m_fAngle);
		}
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

//=============================================================================
//�G�̏�Ԉ���
//=============================================================================
void CEnemy::SetEnemyState(ENEMY_DAMAGE_STATE damage,int nLifeDamage)
{
	//�l����
	m_pos = GetPosition();

	//����������
	m_EnemyState = damage;

	//��������̗͂��Ђ�
	m_nLife -= nLifeDamage;

	//�̗͂�0�ȏゾ������
	if (m_nLife >= 0)
	{
		//�_���[�W��Ԃɂ���
		if (damage == ENEMY_DAMAGE_STATE_DAMAGE)
		{
			//�J�E���g��ݒ�
			m_nCountEnemy = 10;
			SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f));
		}
	}
	else
	{
		for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
		{
			//�p�[�e�B�N���𐶐�
			CParticle::Create(m_pos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_PLAYER);
		}

		//�����n��̕ύX
		srand((unsigned)time(NULL));

		for (int nCntExperience = 0; nCntExperience < 10; nCntExperience++)
		{
			//�o���l������
			CExperience::Create(D3DXVECTOR3(m_pos.x + rand() % 100 + 10, m_pos.y + rand() % 100 + 10, 0.0f), 25.0f, 25.0f);
		}

		//�G������
		Uninit();
	}
}

//=============================================================================
//�{�X�̏�ԏ���
//=============================================================================
void CEnemy::SetEnemyBossState(ENEMY_DAMAGE_STATE damage)
{
	//�l����
	m_pos = GetPosition();

	//����������
	m_EnemyState = damage;

	//�_���[�W��Ԃɂ���
	if (damage == ENEMY_DAMAGE_STATE_DAMAGE)
	{
		//�J�E���g��ݒ�
		m_nCountEnemy = 10;
		SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f));

		for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
		{
			//�p�[�e�B�N���𐶐�
			CParticle::Create(m_pos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_ENEMY);
		}
	}
}



