//================================================
//
// カメラ処理[camera.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "camera.h"
#include "manager.h"
#include "Renderer.h"
#include "fade.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Init(void)
{
	m_PosV[0] = D3DXVECTOR3(0.0f, 140.0f, -250.0f);
	m_PosR[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VecU[0] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_viewport[0].X = (DWORD)40.0f;				// 描画する画面左上のＸ座標
	m_viewport[0].Y = (DWORD)40.0f;				// 描画する画面左上のＹ座標
	m_viewport[0].Width = (DWORD)1000.0f;;			// 描画する画面の幅
	m_viewport[0].Height = (DWORD)1000.0f;			// 描画する画面の高さ
	m_viewport[0].MinZ = 0.0f;
	m_viewport[0].MaxZ = 1.0f;

	m_PosV[1] = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
	m_PosR[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VecU[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	m_viewport[1].X = (DWORD)0.0f;				// 描画する画面左上のＸ座標
	m_viewport[1].Y = (DWORD)0.0f;				// 描画する画面左上のＹ座標
	m_viewport[1].Width = (DWORD)SCREEN_WIDTH;			// 描画する画面の幅
	m_viewport[1].Height = (DWORD)SCREEN_HEIGHT;			// 描画する画面の高さ
	m_viewport[1].MinZ = 0.0f;
	m_viewport[1].MaxZ = 1.0f;
}

void CCamera::Uninit(void)
{
}

void CCamera::Update(void)
{
}

void CCamera::SetCamera(int nIdx)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection[nIdx]);

	//ビューポートの設定
	pDevice->SetViewport(&m_viewport[nIdx]);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection[nIdx],
		D3DXToRadian(45.0f),
		((float)SCREEN_WIDTH / 2.0f) / (float)150.0f,
		10.0f,//手前
		3000.0f);//奥側

	//プロジェクションマッピングの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection[nIdx]);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView[nIdx]);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView[nIdx],
		&m_PosV[nIdx],
		&m_PosR[nIdx],
		&m_VecU[nIdx]);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView[nIdx]);
}
