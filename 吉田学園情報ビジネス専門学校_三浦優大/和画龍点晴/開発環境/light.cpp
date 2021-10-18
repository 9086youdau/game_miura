//================================================
//
// ライト処理[light.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
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
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//設定用方向ベクトル
	D3DXVECTOR3 VecDir;
	D3DXVECTOR3 VecDir1;
	D3DXVECTOR3 VecDir2;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ライトをクリアにする
	ZeroMemory(&m_light[0], sizeof(D3DLIGHT9));
	ZeroMemory(&m_light[1], sizeof(D3DLIGHT9));
	ZeroMemory(&m_light[2], sizeof(D3DLIGHT9));

	//ライトの種類
	m_light[0].Type = D3DLIGHT_DIRECTIONAL;
	m_light[1].Type = D3DLIGHT_DIRECTIONAL;
	m_light[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	m_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	VecDir = D3DXVECTOR3(0.4f, 0.8f, -0.8f);
	VecDir1 = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
	VecDir2 = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	D3DXVec3Normalize(&VecDir, &VecDir);//正規化
	D3DXVec3Normalize(&VecDir1, &VecDir1);//正規化
	D3DXVec3Normalize(&VecDir2, &VecDir2);//正規化

	m_light[0].Direction = VecDir;
	m_light[1].Direction = VecDir1;
	m_light[2].Direction = VecDir2;

	//ライトの設定
	pDevice->SetLight(0, &m_light[0]);
	pDevice->SetLight(1, &m_light[1]);
	pDevice->SetLight(2, &m_light[2]);

	//ライトを有効にする
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
