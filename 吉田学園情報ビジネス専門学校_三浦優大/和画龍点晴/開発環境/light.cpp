//================================================
//
// ���C�g����[light.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "light.h"
#include "manager.h"
#include "Renderer.h"

CLight::CLight()
{
}

CLight::~CLight()
{
}

void CLight::Init(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�ݒ�p�����x�N�g��
	D3DXVECTOR3 VecDir;
	D3DXVECTOR3 VecDir1;
	D3DXVECTOR3 VecDir2;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���C�g���N���A�ɂ���
	ZeroMemory(&m_light[0], sizeof(D3DLIGHT9));
	ZeroMemory(&m_light[1], sizeof(D3DLIGHT9));
	ZeroMemory(&m_light[2], sizeof(D3DLIGHT9));

	//���C�g�̎��
	m_light[0].Type = D3DLIGHT_DIRECTIONAL;
	m_light[1].Type = D3DLIGHT_DIRECTIONAL;
	m_light[2].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	m_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�̕�����ݒ�
	VecDir = D3DXVECTOR3(0.4f, 0.8f, -0.8f);
	VecDir1 = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
	VecDir2 = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	D3DXVec3Normalize(&VecDir, &VecDir);//���K��
	D3DXVec3Normalize(&VecDir1, &VecDir1);//���K��
	D3DXVec3Normalize(&VecDir2, &VecDir2);//���K��

	m_light[0].Direction = VecDir;
	m_light[1].Direction = VecDir1;
	m_light[2].Direction = VecDir2;

	//���C�g�̐ݒ�
	pDevice->SetLight(0, &m_light[0]);
	pDevice->SetLight(1, &m_light[1]);
	pDevice->SetLight(2, &m_light[2]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);
}

void CLight::Uninit(void)
{
}

void CLight::Update(void)
{
}
