//================================================
//
//�o���b�g����[bullet.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "bullet.h"
#include "input.h"
#include "manager.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "hp.h"
#include "score.h"
#include "number.h"
#include "bg.h"
#include "life.h"
#include "Graze.h"
#include "fade.h"
#include "CircleHp.h"
#include "game.h"
#include "sound.h"
#include "Experience.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture[2] = {};
CBullet::BULLETTYPE CBullet::m_bulletplayerType = CBullet::BULLETTYPE_NONE;
CBullet::BULLETTYPE CBullet::m_bulletenemyType = CBullet::BULLETTYPE_NONE;
CEnemy *CBullet::m_pEnemy = NULL;
int CBullet::m_nLifeCount = 3;
int g_debug;
bool CBullet::m_playerattack = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}
//=============================================================================
//�o���b�g�̐ݒ�E�X�V����
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType, BULLETTYPEDAMAGE bulletbomb, float omega, float fHight, float fWidth,float fAngle)
{
	//�C���X�^���X����
	CBullet *pBullet = NULL;
	pBullet = new CBullet;

	//�������Ăяo��
	pBullet->Init(pos, bulletplayerType, bulletenemyType,fHight, fWidth,bulletbomb,omega,fAngle);

	if (bulletplayerType == BULLETTYPE_PLAYER)
	{
		//�e�N�X�`���Ăяo��
		pBullet->BindTexture(m_pTexture[0]);
	}
	else if (bulletenemyType == BULLETTYPE_ENEMY)
	{
		//�e�N�X�`���Ăяo��
		pBullet->BindTexture(m_pTexture[1]);
	}

	return pBullet;
 }
//=============================================================================
//�o���b�g�̏���������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos,BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb, float omega,float fAngle)
{
	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�ړ��ʂ̏�����
	m_nLife = 700;
	m_nCountGraze = 0;
	m_han.y = 0.0f;
	m_posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bulletplayerType = BULLETTYPE_NONE;
	m_bulletenemyType = BULLETTYPE_NONE;
	m_bulletbomb = BULLETTYPEDAMAGE_NONE;
	m_posplayerBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BombBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_playerattack = false;
		
	//�l����
	m_fAngle = fAngle;

	//�֐��Ăяo��
	SetBullet(pos, bulletplayerType, bulletenemyType,bulletbomb,omega,fHight,fWidth);

	return S_OK;
}
//=============================================================================
//�o���b�g�̏I������
//=============================================================================
void CBullet::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}
//=============================================================================
//�o���b�g�̍X�V����
//=============================================================================
void CBullet::Update(void)
{
	//�|�[�Y�̑��
	bool pause = false;
	pause = CGame::GetPause();

	//���[�J���ϐ��錾
	D3DXVECTOR3 playerpos;//�v���C���[�̈ʒu���
	playerpos = CPlayer::GetPlayer();

	if (pause == false)
	{
		//�o���b�g�̃^�C�v
		switch (m_bulletbomb)
		{
		case BULLETTYPEDAMAGE_BOMB:
			MoveLoopingBullet2(m_BombBullet.x, m_BombBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 1.0f, m_omega, 0.05f);
			break;
		case BULLETTYPEDAMAGE_NOMALSHOT:
			//�^�C�v��������
			SetObjType(CScene::OBJTYPE_PLAYER_BULLET);
			MoveNomalBullet(m_posplayerBullet.x, m_posplayerBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 20.0f, m_omega, 10.0f);
			break;
		case BULLETTYPEDAMAGE_ENEMYNOMALSHOT:
			//�^�C�v��������
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			//�J���[�ύX�֐��Ăяo��
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemyNomalBullet(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 10.0f, m_omega, 10.0f);
			break;
		case BULEETTYPEDAMAGE_SIRCLESHOT:
			//�^�C�v��������
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemySircleBullet(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 2.0f, m_omega, 10.0f);
			break;
		case BULLETTYPEDAMAGE_ENEMYMOVESIRCLE:
			//�^�C�v��������
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemyLoopingBullet2(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 2.0f, m_omega, 0.008f);
			break;
		case BULLETTYPEDAMAGE_ENEMYCOUNT:
			//�^�C�v��������
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemyCountBullet2(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 1.5f, m_han2.y, 1.0f, m_fAngle);
			break;
		case BULLETTYPEDAMAGE_ENEMYHOMING:
			//MoveHomingBullet(m_posBullet.x, m_posBullet.y, m_move.x, m_move.y, m_vec.x, m_vec.y, playerpos.x, playerpos.y, m_vecangle);
			break;
		}

		//�|�C���^��NULL�ɂ���
		CScene *pScene = NULL;

		//�v���C���[�̒e�ƓG�̓����蔻��
		if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY, &pScene) == true)
		{
			//�G�l�~�[�̓G�̈ʒu����
			D3DXVECTOR3 EnemyPos = pScene->GetPosition();

			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
			{
				//�p�[�e�B�N���𐶐�
				CParticle::Create(EnemyPos, D3DXCOLOR(15, 15, 255, 255), CParticle::PARTICLE_ENEMY);
			}

			//�|�C���^���G�l�~�[�ɐݒ肷��
			CEnemy *pEnemy = (CEnemy*)pScene;
			pEnemy->SetEnemyState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE, 5);

			//�e�̍폜
			Uninit();
		}

		//�O���C�Y�̓����蔻�菈��
		if (playerpos.x - 50 <= m_posBullet.x && m_posBullet.x <= playerpos.x + 50 &&
			playerpos.y - 50 <= m_posBullet.y && m_posBullet.y <= playerpos.y + 50)
		{
			if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
			{
				//�J�E���g�A�b�v
				m_nCountGraze++;

				//�J�E���g��1�̎�
				if (m_nCountGraze == 1)
				{
					//�����蔻��
					CGraze::AddGraze(1);
				}
			}
		}

		//�G�̒e�ƃv���C���[�̓����蔻��
		if (Collision(m_posBullet, m_fWidth, m_fHight, CScene::OBJTYPE_PLAYER, &pScene) == true)
		{
			if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
			{
				for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
				{
					//�p�[�e�B�N���𐶐�
					CParticle::Create(playerpos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_PLAYER);
				}

				//���C�t�J�E���g��-1�̎�
				if (m_nLifeCount == -1)
				{
					m_nLifeCount = 3;
				}

				//HP�����炷����
				CLife::SetRelease(m_nLifeCount);

				//�J�E���g�A�b�v
				m_nLifeCount--;

				if (m_nLifeCount > -1)
				{
					//�v���C���[�̏I������
					pScene->Uninit();

					//�������烊�Z�b�g
					CPlayer::Create(D3DXVECTOR3(550.0f, 1200.0f, 0.0f));

					//��ԕω��֐�
					CPlayer::SetStatePlayer(CPlayer::PLAYERSTATE_DAMAGE);
				}
				else if (m_nLifeCount == -1)
				{
					//�v���C���[�̏I������
					pScene->Uninit();

					//�v���C���[�A�^�b�N��true�ɕύX
					m_playerattack = true;

					//���C�t���Z�b�g
					m_nLifeCount = 3;

					//�Q�[����ʂ֑J��
					CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
				}

				//�e�̍폜
				Uninit();
			}
		}

		//�G�̒e�ƃ{���̓����蔻��
		else if (Collision(m_BombBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BULLET, &pScene) == true)
		{
			//�e�̍폜
			pScene->Uninit();
		}

		//�G�ƃ{���̓����蔻��
		else  if (Collision(m_BombBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY, &pScene) == true)
		{
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
			{
				//�p�[�e�B�N���𐶐�
				CParticle::Create(pScene->GetPosition(), D3DXCOLOR(25, 255, 25, 255), CParticle::PARTICLE_PLAYER);
			}

			for (int nCntExperience = 0; nCntExperience < 10; nCntExperience++)
			{
				//�o���l������
				CExperience::Create(D3DXVECTOR3(pScene->GetPosition().x + rand() % 100 + 1, pScene->GetPosition().y + rand() % 100 + 1, 0.0f), 25.0f, 25.0f);
			}

			//�G�l�~�[�̓G�̈ʒu����
			D3DXVECTOR3 EnemyPos = pScene->GetPosition();

			//��������
			CExplosion::Create(EnemyPos);

			//�e�̍폜
			pScene->Uninit();
		}

		//�G�ƃ{���̓����蔻��
		if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BOSS, &pScene) == true)
		{
			//�e�̏I������
			Uninit();

			//���C�t�����炷����
			CCircleHp::SetBossLife(1);

			//�|�C���^���G�l�~�[�ɐݒ肷��
			CEnemy *pEnemy = (CEnemy*)pScene;
			pEnemy->SetEnemyBossState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

			if (CCircleHp::GetBossLife() == 50)
			{
				//�{�X�̏I������
				pScene->Uninit();

				//�Q�[����ʂ֑J��
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}

		//�G�ƃ{���̓����蔻��
		if (Collision(m_BombBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BOSS, &pScene) == true)
		{
			//�e�̏I������
			Uninit();

			//���C�t�����炷����
			CCircleHp::SetBossLife(1);

			//�|�C���^���G�l�~�[�ɐݒ肷��
			CEnemy *pEnemy = (CEnemy*)pScene;
			pEnemy->SetEnemyBossState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

			if (CCircleHp::GetBossLife() == 50)
			{
				//�{�X�̏I������
				pScene->Uninit();

				//�Q�[����ʂ֑J��
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}

		//���C�t�����炷
		m_nLife--;

		//���C�t��0�ɂȂ�����
		if (m_nLife == 0)
		{
			//�I������
			Uninit();
		}
	}
}
//=============================================================================
//�o���b�g�̕`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	//��{�N���X�̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//�o���b�g�̃e�N�X�`������
//=============================================================================
HRESULT CBullet::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/cardbullet.png", &m_pTexture[0]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
//�o���b�g�̃e�N�X�`���̔p������
//=============================================================================
void CBullet::Unload(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
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
//�o���b�g�̈ړ�����
//=============================================================================
void CBullet::SetBullet(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,BULLETTYPEDAMAGE bulletbomb,float omega,float fHight, float fWidth)
{
	if (bulletplayerType == BULLETTYPE_PLAYER)
	{
		//�ʒu�̈���������
		m_pos = pos;

		//��������
		m_fWidth = fWidth;

		//�c������
		m_fHight = fHight;

		//�^�C�v����
		m_bulletplayerType = bulletplayerType;

		//�e�̎�ޕ���
		m_bulletbomb = bulletbomb;

		//�e�̊p�x�ύX
		m_omega = omega;
	}
	else if (bulletenemyType == BULLETTYPE_ENEMY)
	{
		//�ʒu�̈���������
		m_pos = pos;

		//��������
		m_fWidth = fWidth;

		//�c������
		m_fHight = fHight;

		//�^�C�v����
		m_bulletenemyType = bulletenemyType;

		//�e�̎�ޕ���
		m_bulletbomb = bulletbomb;

		//�e�̊p�x�ύX
		m_omega = omega;
	}
}

void CBullet::MoveLoopingBullet2(float& x, float& y,float cx, float cy, float& vx, float& vy, float& r, float vr, float theta,float omega)
{
	// �p�x��ω�������
	theta += omega;

	// ���a��ω�������
	r += vr;

	// �ʒu���v�Z����
	x = cx + r * cosf(theta);
	y = cy + r * sinf(theta);

	// �e�̑��x�i�K�v�ȏꍇ�����j
	vx = -r * omega * sinf(theta);
	vy = r  * omega * cosf(theta);

	//�e�̈ʒu�X�V
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�G�t�F�N�g����
	//CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 120.0f, 120.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

	//����������
	m_omega = theta;
}

void CBullet::MoveNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
{ 
	// ���a��ω�������
	r += vr;

	// �ʒu���v�Z����
	x = cx + r * cosf(D3DX_PI * 1.5f);
	y = cy + r * sinf(D3DX_PI * 1.5f);

	//����������
	m_omega = theta;

	//�e�̈ʒu�X�V
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}
}

void CBullet::MoveEnemyNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
{
	// ���a��ω�������
	r += vr;

	// �ʒu���v�Z����
	x = cx + r * cosf(D3DX_PI * -1.5f);
	y = cy + r * sinf(D3DX_PI * -1.5f);

	// �e�̑��x�i�K�v�ȏꍇ�����j
	vx = -r * omega * sinf(D3DX_PI * -1.5f);
	vy = r  * omega * cosf(D3DX_PI * -1.5f);

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 50.0f, 50.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//�e�̈ʒu�X�V
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

	//����������
	m_omega = theta;
}

void CBullet::MoveEnemySircleBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
{
	// ���a��ω�������
	r += vr;

	// �ʒu���v�Z����
	x = cx + r * cosf(theta);
	y = cy + r * sinf(theta);

	// �e�̑��x�i�K�v�ȏꍇ�����j
	vx = -r * omega * sinf(theta);
	vy = r  * omega * cosf(theta);

	//�e�̈ʒu�X�V
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 40.0f, 40.0f, D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

	//����������
	m_omega = theta;
}

void CBullet::MoveEnemyLoopingBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float theta, float omega)
{
	// �p�x��ω�������
	theta += omega;

	// ���a��ω�������
	r += vr;

	// �ʒu���v�Z����
	x = cx + r * cosf(theta);
	y = cy + r * sinf(theta);

	// �e�̑��x�i�K�v�ȏꍇ�����j
	vx = -r * omega * sinf(theta);
	vy = r  * omega * cosf(theta);

	//�e�̈ʒu�X�V
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 50.0f, 50.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

	//����������
	m_omega = theta;
}

void CBullet::MoveEnemyCountBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float & R, float VR,float angle)
{
	// ���a��ω�������
	r += vr;

	// �ʒu���v�Z����
	x = cx + r * cosf(D3DX_PI * angle);
	y = cy + r * sinf(D3DX_PI * angle);

	//�G�t�F�N�g����
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 50.0f, 50.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),CEffect::EFFECTTYPE_NONE);

	//�e�̈ʒu�X�V
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

}
void CBullet::MoveHomingBullet(float& x, float& y, float& vx, float& vy, float& rx,float& ry ,float ex, float ey,float fAngle)
{
	////�G�̍��W�ɒǔ�
	//rx = ex - x;
	//ry = ey - y;

	//fAngle = atan2f(rx, ry);//�x�N�g���̊p�x�����߂�

	//// �e�̑��x�i�K�v�ȏꍇ�����j
	//vx = sinf(fAngle) * 2.0f;
	//vy = cosf(fAngle) * 2.0f;

	////�ʒu�X�V�v�Z
	//x += vx;
	//y += vy;

	////�e�̈ʒu�X�V
	//SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	////�G�t�F�N�g����
	////CEffect::Create(D3DXVECTOR3(x, y, 0.0f));
}

void CBullet::SetLife(int Damage)
{
	//����������
	m_nLifeCount -= Damage;
}

void CBullet::SetLifeRecovery(int Recovery)
{
	//����������
	m_nLifeCount = Recovery;
}

	
