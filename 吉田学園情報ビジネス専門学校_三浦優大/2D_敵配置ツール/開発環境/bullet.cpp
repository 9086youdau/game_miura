//=============================================================================
//
// �o���b�g����[bullet.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include <time.h>
#include "bullet.h"
#include "input.h"
#include "manager.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "enemy.h"
#include "player.h"
#include "bg.h"
#include "fade.h"
#include "game.h"
#include "sound.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture[2] = {};
CBullet::BULLETTYPE CBullet::m_bulletplayerType = CBullet::BULLETTYPE_NONE;
CBullet::BULLETTYPE CBullet::m_bulletenemyType = CBullet::BULLETTYPE_NONE;
int CBullet::m_nLifeCount = 3;
bool CBullet::m_playerattack = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	//�����o�ϐ�������
	m_nLife = 700;                                //�e�̗̑�
	m_nCountGraze = 0;                            //�O���C�Y�p�̃J�E���g�ϐ��@�@�@�@�@�@�@�@�@�@�@
	m_han = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //�f�[�^�ۑ��p
	m_han2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //�f�[�^�ۑ��p
	m_posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //�G�̑łʂ̈ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //�ړ���
	m_bulletplayerType = BULLETTYPE_NONE;         //�o���b�g�^�C�v
	m_bulletenemyType = BULLETTYPE_NONE;          //�o���b�g�^�C�v
	m_bulletbomb = BULLETTYPEDAMAGE_NONE;         //�o���b�g�^�C�v
	m_posplayerBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�v���C���[�̒e
	m_BombBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�{���̒e�̈ʒu
	m_playerattack = false;                       //�U�����鏊�t���O
	m_fAngle = 0.0f;                              //�p�x�ۑ��p�ϐ�
	m_omega = 0.0f;                               //�e�̃J�E���g�p�ϐ�
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
	pBullet->Init(pos, bulletplayerType, bulletenemyType, fHight, fWidth, bulletbomb, omega, fAngle);

	//�^�C�v�ʂɃe�N�X�`���؂�ւ�
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

	//�|�C���^�Ԃ�l
	return pBullet;
 }
//=============================================================================
//�o���b�g�̏���������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos,BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb, float omega,float fAngle)
{
	//����������
	CScene2D::Init(pos, fHight, fWidth);
		
	//�l����
	m_fAngle = fAngle;

	//�֐��Ăяo��
	SetBullet(pos, bulletplayerType, bulletenemyType, bulletbomb, omega, fHight, fWidth);

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

	//�|�[�Y��Ԃł͖����ꍇ
	if (pause == false)
	{
		//�o���b�g�̃^�C�v��������
		switch (m_bulletbomb)
		{
		case BULLETTYPEDAMAGE_NOMALSHOT:
			//�^�C�v��������
			SetObjType(CScene::OBJTYPE_PLAYER_BULLET);
			MoveNomalBullet(m_posplayerBullet.x, m_posplayerBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 20.0f, m_omega, 10.0f);
			break;
		}

		//�����蔻��֐��Ăяo��
		CollisionBullet();

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
//�o���b�g�̃Z�b�g����
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

//=============================================================================
//�o���b�g�̓����蔻�菈��
//=============================================================================
void CBullet::CollisionBullet(void)
{
	//���[�J���ϐ��錾
	//�v���C���[�̈ʒu���
	D3DXVECTOR3 playerpos;
	playerpos = CPlayer::GetPlayer();

	//�|�C���^��NULL�ɂ���
	CScene *pScene = NULL;

	//�v���C���[�̒e�ƓG�̓����蔻��
	if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY, &pScene) == true)
	{
		//�G�l�~�[�̓G�̈ʒu����
		D3DXVECTOR3 EnemyPos = pScene->GetPosition();

		//�|�C���^���G�l�~�[�ɐݒ肷��
		CEnemy *pEnemy = (CEnemy*)pScene;
		pEnemy->SetEnemyState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

		//�e�̍폜
		Uninit();
	}

	//�v���C���[�̒e�ƓG�̓����蔻��
	else if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BOSS, &pScene) == true)
	{
		//�G�l�~�[�̓G�̈ʒu����
		D3DXVECTOR3 EnemyPos = pScene->GetPosition();

		//�|�C���^���G�l�~�[�ɐݒ肷��
		CEnemy *pEnemy = (CEnemy*)pScene;
		pEnemy->SetEnemyState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

		//�e�̍폜
		Uninit();
	}
}
//=============================================================================
//�o���b�g�̈ړ���������
//=============================================================================
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

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

	//����������
	m_omega = theta;
}

//=============================================================================
//�o���b�g�̈ړ���������
//=============================================================================
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

//=============================================================================
//�o���b�g�̈ړ���������
//=============================================================================
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

//=============================================================================
//�o���b�g�̈ړ���������
//=============================================================================
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

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

	//����������
	m_omega = theta;
}
//=============================================================================
//�o���b�g�̈ړ���������
//=============================================================================
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

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

	//����������
	m_omega = theta;
}

//=============================================================================
//�o���b�g�̈ړ���������
//=============================================================================
void CBullet::MoveEnemyCountBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float & R, float VR,float angle)
{
	// ���a��ω�������
	r += vr;

	// �ʒu���v�Z����
	x = cx + r * cosf(D3DX_PI * angle);
	y = cy + r * sinf(D3DX_PI * angle);

	//�e�̈ʒu�X�V
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//��ʊO����
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//�I������
		Uninit();
	}

}
//=============================================================================
//�o���b�g�̈ړ���������
//=============================================================================
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

//=============================================================================
//�o���b�g�̃��C�t�J�E���g����
//=============================================================================
void CBullet::SetLife(int Damage)
{
	//����������
	m_nLifeCount -= Damage;
}

//=============================================================================
//�o���b�g�̃��C�t����������
//=============================================================================
void CBullet::SetLifeRecovery(int Recovery)
{
	//����������
	m_nLifeCount = Recovery;
}

	
