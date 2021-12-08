//================================================
//
//���[�U�[����[laser.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "laser.h"
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
#include "bullet.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CLaser::m_pTexture = NULL;
CLaser::LASERTYPE CLaser::m_laserType = CLaser::LASERTYPE_NONE;
CEnemy *CLaser::m_pEnemy = NULL;
int CLaser::m_nLifeCount = 3;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CLaser::CLaser()
{

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CLaser::~CLaser()
{

}
//=============================================================================
//���[�U�[�̐ݒ�E�X�V����
//=============================================================================
CLaser * CLaser::Create(D3DXVECTOR3 pos, LASERTYPE laserType, LASERTYPEDAMAGE laserdamage, float omega, float fHight, float fWidth, float fAngle,D3DXVECTOR3 rot,int nLife)
{
	//�C���X�^���X����
	CLaser *pLaser = NULL;
	pLaser = new CLaser;

	//�������Ăяo��
	pLaser->Init(pos, laserType, fHight, fWidth, laserdamage, omega, fAngle,rot,nLife);

	//�e�N�X�`���Ăяo��
	pLaser->BindTexture(m_pTexture);

	return pLaser;
}
//=============================================================================
//���[�U�[�̏���������
//=============================================================================
HRESULT CLaser::Init(D3DXVECTOR3 pos, LASERTYPE laserType, float fHight, float fWidth, LASERTYPEDAMAGE laserdamage, float omega, float fAngle,D3DXVECTOR3 rot,int nLife)
{
	//����������
	CScene2D::Init(pos, fHight, fWidth);

	if (laserdamage == LASERTYPEDAMAGE_ENEMYNOMALSHOT)
	{
		//�J���[�ύX�֐��Ăяo��
		SetCol(D3DXCOLOR(0, 0, 255, 255));
	}
	else if (laserdamage == LASERTYPEDAMAGE_ENEMYNOTICE)
	{
		//�J���[�ύX�֐��Ăяo��
		SetCol(D3DXCOLOR(255, 255, 255, 255));
	}

	//�ړ��ʂ̏�����
	m_nCountGraze = 0;
	m_han.y = 0.0f;

	//�l����
	m_fAngle = fAngle;

	//�֐��Ăяo��
	SetLaser(pos, laserType, laserdamage, omega, fHight, fWidth, rot, nLife);

	return S_OK;
}
//=============================================================================
//���[�U�[�̏I������
//=============================================================================
void CLaser::Uninit(void)
{
	//��{�N���X�̏I������
	CScene2D::Uninit();
}
//=============================================================================
//���[�U�[�̍X�V����
//=============================================================================
void CLaser::Update(void)
{
	//�o���b�g�̃^�C�v
	switch (m_laserbomb)
	{
	case LASERTYPEDAMAGE_ENEMYNOMALSHOT:
		MoveEnemyNomalLaser(m_posLaser.x, m_posLaser.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 0.0f, m_omega, 0.0f);
		break;
	case LASERTYPEDAMAGE_ENEMYNOTICE:
		MoveEnemyNomalLaser(m_posLaser.x, m_posLaser.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 0.0f, m_omega, 0.0f);
		break;
	}

	//�����蔻��̏����֐��Ăяo��
	CollisionLaser();

	//���C�t�����炷
	m_nLife--;

	//���C�t��0�ɂȂ�����
	if (m_nLife <= 0)
	{
		//�I������
		Uninit();
	}
}
//=============================================================================
//���[�U�[�̕`�揈��
//=============================================================================
void CLaser::Draw(void)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�����_�[�X�e�[�g�̐ݒ�(���Z�����j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//��{�N���X�̕`�揈��
	CScene2D::Draw();

	//�����_�[�X�e�[�g�̐ݒ�(���ɖ߂��j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//���[�U�[�̃e�N�X�`������
//=============================================================================
HRESULT CLaser::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
//���[�U�[�̃e�N�X�`���̔p������
//=============================================================================
void CLaser::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//���[�U�[�̈ړ�����
//=============================================================================
void CLaser::SetLaser(D3DXVECTOR3 pos, LASERTYPE laserType, LASERTYPEDAMAGE laserdamage, float omega, float fHight, float fWidth,D3DXVECTOR3 rot,int nLife)
{
	if (laserType == LASERTYPE_ENEMY)
	{
		//�ʒu�̈���������
		m_pos = pos;

		//��������
		m_fWidth = fWidth;

		//�c������
		m_fHight = fHight;

		//�^�C�v����
		m_laserType = laserType;

		//�e�̎�ޕ���
		m_laserbomb = laserdamage;

		//�e�̊p�x�ύX
		m_omega = omega;

		//���[�U�[�p�x�ύX
		m_Rot = rot;

		//���[�U�[�̃��C�t
		m_nLife = nLife;
	}
}

//=============================================================================
//���[�U�[�̓����蔻�菈��
//=============================================================================
void CLaser::CollisionLaser(void)
{
	//���[�J���ϐ��錾
	D3DXVECTOR3 playerpos;//�v���C���[�̈ʒu���
	playerpos = CPlayer::GetPlayer();

	// ��`�̒��S�����_�Ƃ������΍��W���쐬����
	D3DXVECTOR2 relateve_position = D3DXVECTOR2(playerpos.x - m_posLaser.x, playerpos.y - m_posLaser.y);

	// ���΍��W�ɑ΂��ċ�`�̉�]��ł������t�s����|����
	D3DXVECTOR2 transform_pos = D3DXVECTOR2(cosf(-m_Rot.z) * relateve_position.x + sinf(-m_Rot.z) * relateve_position.y,
	-sinf(-m_Rot.z) * relateve_position.x + cosf(-m_Rot.z) * relateve_position.y);

	//�ϐ��ϊ�
	D3DXVECTOR3 laserpos = transform_pos;

	//�|�C���^��NULL�ɂ���
	CScene *pScene = NULL;

	//�G�ƃ{���̓����蔻��
	if (Collision(laserpos, m_fWidth, m_fHight, CScene::OBJTYPE_PLAYER, &pScene) == true)
	{
		pScene->Uninit();
	}
}

//=============================================================================
//���[�U�[�̓���������
//=============================================================================
void CLaser::MoveEnemyNomalLaser(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
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
	SetPosition(D3DXVECTOR3(x, y, 0.0f), m_Rot);

	//����������
	m_omega = theta;
}