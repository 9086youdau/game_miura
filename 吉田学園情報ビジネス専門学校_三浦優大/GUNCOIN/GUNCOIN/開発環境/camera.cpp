//================================================
//
//カメラ処理[Camera.h]
// Author; miura yuudai
//
//================================================
#include "camera.h"
#include "input.h"
#include "model.h"
#include "gamepad.h"

//==================
//グローバル変数
//==================
Camera g_camera;//カメラの情報
float g_Camera;

//=====================================
//初期化処理
//=====================================
void InitCamera(void)
{
	g_camera.PosV = D3DXVECTOR3(100.0f, 150.0f, -200.0f);
	g_camera.PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.moveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.rot.y = 0.0f;
	g_camera.fLength = 400.0f;
}
//=====================================
//終了処理
//=====================================
void UninitCamera(void)
{

}
//=====================================
//更新処理
//=====================================
void UpdateCamera(void)
{
	Player *pModel;
	pModel = GetModel();

	D3DXVECTOR3 vec;

	//注視点移動
	if (IsButtonPush(KEYINFO_CAMERA_FRONT) == true)
	{
		g_camera.rot.y -= 0.05f;
		g_camera.PosV.x = g_camera.PosR.x - sinf(D3DX_PI + g_camera.rot.y) * 200;
		g_camera.PosV.z = g_camera.PosR.z + cosf(D3DX_PI + g_camera.rot.y) * 200;
	}

	if (IsButtonPush(KEYINFO_WEAPON_DROP) == true)
	{
		g_camera.rot.y += 0.05f;
		g_camera.PosV.x = g_camera.PosR.x - sinf(D3DX_PI + g_camera.rot.y) * 200;
		g_camera.PosV.z = g_camera.PosR.z + cosf(D3DX_PI + g_camera.rot.y) * 200;
	}

	// モデルの位置へ注視点を置く
	g_camera.PosV.x = pModel->pos.x + sinf(g_camera.rot.y) * -g_camera.fLength;
	g_camera.PosV.z = pModel->pos.z + cosf(g_camera.rot.y) * -g_camera.fLength;

	// 目的の注視点
	g_camera.PosRDest.x = pModel->pos.x - 5.0f * sinf(pModel->rotDest.y);
	g_camera.PosRDest.z = pModel->pos.z - 5.0f * cosf(pModel->rotDest.y);
	g_camera.PosRDest.y = pModel->pos.y + 70.0f;

	// 目的の視点
	g_camera.PosVDest.x = pModel->pos.x - sinf(g_camera.rot.x) * g_camera.fLength;
	g_camera.PosVDest.z = pModel->pos.z - cosf(g_camera.rot.z) * g_camera.fLength;
	g_camera.PosVDest.y = pModel->pos.y + 150.0f;

	// 注視点の位置更新
	g_camera.PosR.x += (g_camera.PosRDest.x - g_camera.PosR.x) * 0.1f;
	g_camera.PosR.z += (g_camera.PosRDest.z - g_camera.PosR.z) * 0.1f;
	g_camera.PosR.y += (g_camera.PosRDest.y - g_camera.PosR.y) * 0.1f;

	// 視点の位置更新
	g_camera.PosV.x += (g_camera.PosVDest.x - g_camera.PosV.x) * 0.1f;
	g_camera.PosV.z += (g_camera.PosVDest.z - g_camera.PosV.z) * 0.1f;
	g_camera.PosV.y += (g_camera.PosVDest.y - g_camera.PosV.y) * 0.1f;

	if (GetKeyboardPress(DIK_T) == true)
	{
		g_camera.PosR.y += 1.5f*cosf(D3DX_PI + g_camera.rot.y);
	}

	if (GetKeyboardPress(DIK_B) == true)
	{
		g_camera.PosR.y -= 8.5f*cosf(D3DX_PI + g_camera.rot.y);
	}
	if (GetKeyboardPress(DIK_N) == true)
	{
		g_camera.PosV.y += 1.5f*cosf(D3DX_PI + g_camera.rot.y);
	}

	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_camera.PosV.y -= 1.5f*cosf(D3DX_PI + g_camera.rot.y);
	}

	//回転角度リセット
	if (g_camera.rot.y >= D3DX_PI)
	{
		g_camera.rot.y = -D3DX_PI;
	}

	else if (g_camera.rot.y <= -D3DX_PI)
	{
		g_camera.rot.y = D3DX_PI;
	}

	if (GetKeyboardPress(DIK_SPACE) == true)//プレイヤーの回転
	{
		InitCamera();
	}


	//位置更新
	g_camera.PosV.x += g_camera.moveV.x;
	g_camera.PosV.z += g_camera.moveV.z;

	//位置更新
	g_camera.PosR.x += g_camera.moveR.x;
	g_camera.PosR.z += g_camera.moveR.z;

	// 移動量の減衰
	g_camera.moveR.x += (0.0f - g_camera.moveR.x) *0.5f;
	g_camera.moveR.z += (0.0f - g_camera.moveR.z) *0.5f;
	
	// 移動量の減衰
	g_camera.moveV.x += (0.0f - g_camera.moveV.x) *0.5f;
	g_camera.moveV.z += (0.0f - g_camera.moveV.z) *0.5f;

}
//=====================================
//設定処理
//=====================================
void SetCamera(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,//手前
		1000.0f);//奥側

	//プロジェクションマッピングの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.PosV,
		&g_camera.PosR,
		&g_camera.VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}
Camera GetCamera(void)
{
	return g_camera;
}