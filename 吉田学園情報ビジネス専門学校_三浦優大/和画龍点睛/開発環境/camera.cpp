//================================================
//
// �J��������[camera.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "camera.h"
#include "manager.h"
#include "Renderer.h"
#include "fade.h"

//=================================================
//�R���X�g���N�^
//=================================================
CCamera::CCamera()
{
}

//=================================================
//�f�X�g���N�^
//=================================================
CCamera::~CCamera()
{
}
//=================================================
//�J�����̏���������
//=================================================
void CCamera::Init(void)
{
	//�J������ڂ̐ݒ�
	m_PosV[0] = D3DXVECTOR3(0.0f, 140.0f, -250.0f);//�J�����̎��_
	m_PosR[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�J�����̒����_
	m_VecU[0] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//�J�����̏�����̃x�N�g��

	m_viewport[0].X = (DWORD)40.0f;				// �`�悷���ʍ���̂w���W
	m_viewport[0].Y = (DWORD)40.0f;				// �`�悷���ʍ���̂x���W
	m_viewport[0].Width = (DWORD)1000.0f;;			// �`�悷���ʂ̕�
	m_viewport[0].Height = (DWORD)1000.0f;			// �`�悷���ʂ̍���
	m_viewport[0].MinZ = 0.0f;
	m_viewport[0].MaxZ = 1.0f;

	//�J������ڂ̐ݒ�
	m_PosV[1] = D3DXVECTOR3(0.0f, 100.0f, -200.0f);//�J�����̎��_
	m_PosR[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�J�����̒����_
	m_VecU[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//�J�����̏�����̃x�N�g��
	
	m_viewport[1].X = (DWORD)0.0f;				// �`�悷���ʍ���̂w���W
	m_viewport[1].Y = (DWORD)0.0f;				// �`�悷���ʍ���̂x���W
	m_viewport[1].Width = (DWORD)SCREEN_WIDTH;			// �`�悷���ʂ̕�
	m_viewport[1].Height = (DWORD)SCREEN_HEIGHT;			// �`�悷���ʂ̍���
	m_viewport[1].MinZ = 0.0f;
	m_viewport[1].MaxZ = 1.0f;
}

//=================================================
//�J�����̏I������
//=================================================
void CCamera::Uninit(void)
{
}

//=================================================
//�J�����̍X�V����
//=================================================
void CCamera::Update(void)
{
}

//=================================================
//�J�����̃Z�b�g����
//=================================================
void CCamera::SetCamera(int nIdx)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection[nIdx]);

	//�r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport[nIdx]);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection[nIdx],
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,//��O
		3000.0f);//����

	//�v���W�F�N�V�����}�b�s���O�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection[nIdx]);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView[nIdx]);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView[nIdx],
		&m_PosV[nIdx],
		&m_PosR[nIdx],
		&m_VecU[nIdx]);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView[nIdx]);
}
