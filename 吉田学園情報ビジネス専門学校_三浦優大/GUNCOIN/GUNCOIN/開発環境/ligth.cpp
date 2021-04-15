//================================================
//
// カメラ処理[light.h]
// Author; miura yuudai
//
//================================================

#include "light.h"

//==================
//グローバル変数
//==================
D3DLIGHT9 g_light[3];//ライトの情報

//=====================================
//初期化処理
//=====================================
void InitLight(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//設定用方向ベクトル
	D3DXVECTOR3 VecDir;
	D3DXVECTOR3 VecDir1;
	D3DXVECTOR3 VecDir2;

	//デバイスの取得
	pDevice = GetDevice();

	//ライトをクリアにする
	ZeroMemory(&g_light[0], sizeof(D3DLIGHT9));
	ZeroMemory(&g_light[1], sizeof(D3DLIGHT9));
	ZeroMemory(&g_light[2], sizeof(D3DLIGHT9));

	//ライトの種類
	g_light[0].Type = D3DLIGHT_DIRECTIONAL;
	g_light[1].Type = D3DLIGHT_DIRECTIONAL;
	g_light[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	g_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	VecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	VecDir1 = D3DXVECTOR3(0.6f, -0.8f, -0.6f);
	VecDir2 = D3DXVECTOR3(0.9f, -0.8f, -0.4f);

	D3DXVec3Normalize(&VecDir, &VecDir);//正規化
	D3DXVec3Normalize(&VecDir1, &VecDir1);//正規化
	D3DXVec3Normalize(&VecDir2, &VecDir2);//正規化

	g_light[0].Direction = VecDir;
	g_light[1].Direction = VecDir1;
	g_light[2].Direction = VecDir2;

	//ライトの設定
	pDevice->SetLight(0, &g_light[0]);
	pDevice->SetLight(0, &g_light[1]);
	pDevice->SetLight(0, &g_light[2]);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);


}

//=====================================
//終了処理
//=====================================
void UninitLight(void)
{

}

//=====================================
//更新処理
//=====================================
void UpdateLight(void)
{

}
D3DLIGHT9 *GetLight(void)
{
	return &g_light[0];
}