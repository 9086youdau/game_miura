//================================================
//
//モデル処理[model.h]
// Author; miura yuudai
//
//================================================

#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "input.h"
#include "camera.h"
#include "bullet.h"
#include "math.h"
#include "stone.h"
#include <stdio.h>
#include "Billboard.h"
#include "enemy.h"
#include "gamepad.h"
#include "wall.h"
#include "item.h"
#include "light.h"

//==============================================================================
// マクロ定義
//==============================================================================
//#define MODEL_MOVE	(1.0f)
//#define MODEL_TURN	(0.1f)
#define PLAYER_MOVE		(5.65f)
#define PLAYER_TURN		(0.1f)
#define VTX_MINP		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// 仮頂点最小値
#define	VTX_MAXP		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// 仮頂点最大値
//#define MAT_MAX		(3)		// マテリアルの最大数
//#define TEX_MAX		(3)		// テクスチャの最大数

//==============================================================================
// グローバル変数
//==============================================================================

//LPD3DXMESH g_pMeshPlayer = NULL;						// メッシュ(頂点情報)へのポインタ
//LPD3DXBUFFER g_pBuffMatPlayer = NULL;					// マテリアル(材質情報)へのポインタ
//LPDIRECT3DTEXTURE9 g_apTextureModel[TEX_MAX] = {};	// テクスチャへのポインタ
//DWORD g_nNumMatPlayer = 0;							// マテリアルの数
Player g_player;							// プレイヤーの構造体

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitModel(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// デバイスの取得

	//LoadMotion();

	//for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	//{
	//	// Xファイルの読み込み
	//	D3DXLoadMeshFromX(
	//		g_player.aModel[nCnt].aFileName,
	//		D3DXMESH_SYSTEMMEM,
	//		pDevice,
	//		NULL,
	//		&g_player.aModel[nCnt].pBuffMatModel,
	//		NULL,
	//		&g_player.aModel[nCnt].nNumMatModel,
	//		&g_player.aModel[nCnt].pMeshModel);
	//}

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/bodymiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[0].pBuffMatModel, NULL, &g_player.aModel[0].nNumMatModel, &g_player.aModel[0].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/atamamiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[1].pBuffMatModel, NULL, &g_player.aModel[1].nNumMatModel, &g_player.aModel[1].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/armmiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[2].pBuffMatModel, NULL, &g_player.aModel[2].nNumMatModel, &g_player.aModel[2].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/arm2miura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[3].pBuffMatModel, NULL, &g_player.aModel[3].nNumMatModel, &g_player.aModel[3].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/handmiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[4].pBuffMatModel, NULL, &g_player.aModel[4].nNumMatModel, &g_player.aModel[4].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/udemiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[5].pBuffMatModel, NULL, &g_player.aModel[5].nNumMatModel, &g_player.aModel[5].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/arm3miura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[6].pBuffMatModel, NULL, &g_player.aModel[6].nNumMatModel, &g_player.aModel[6].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/handRmiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[7].pBuffMatModel, NULL, &g_player.aModel[7].nNumMatModel, &g_player.aModel[7].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/legRmiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[8].pBuffMatModel, NULL, &g_player.aModel[8].nNumMatModel, &g_player.aModel[8].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/footmiuraL.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[9].pBuffMatModel, NULL, &g_player.aModel[9].nNumMatModel, &g_player.aModel[9].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/leg2miuraR.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[10].pBuffMatModel, NULL, &g_player.aModel[10].nNumMatModel, &g_player.aModel[10].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/legmiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[11].pBuffMatModel, NULL, &g_player.aModel[11].nNumMatModel, &g_player.aModel[11].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/footmiura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[12].pBuffMatModel, NULL, &g_player.aModel[12].nNumMatModel, &g_player.aModel[12].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/leg2miura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[13].pBuffMatModel, NULL, &g_player.aModel[13].nNumMatModel, &g_player.aModel[13].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/body2miura.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[14].pBuffMatModel, NULL, &g_player.aModel[14].nNumMatModel, &g_player.aModel[14].pMeshModel);
	D3DXLoadMeshFromX("data/MODEL/gun.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_player.aModel[15].pBuffMatModel, NULL, &g_player.aModel[15].nNumMatModel, &g_player.aModel[15].pMeshModel);

	g_player.nNumModel = MODEL_PARTS;

	// ローカル変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_player.aModel[0].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_player.nNumModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// ファイル名を使用してテクスチャを読み込む
			D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/body_d.tga",&g_player.aModel[0].pTextureModel);
		}
	}

	// 頂点数を取得
	nNumVtx = g_player.aModel[0].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].pMeshModel->GetFVF());

	//各パーツの階層構造設定
	g_player.aModel[0].nIdxModelParent = -1;//親のインデックスを設定 //体
	g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 85.0f, 0.0f);//位置の設定
	g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[1].nIdxModelParent = 0;//親のインデックスを設定　//頭
	g_player.aModel[1].pos = D3DXVECTOR3(-1.0f, 57.0f, -4.0f);//位置の設定
	g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[2].nIdxModelParent = 0;//親のインデックスを設定 //右腕
	g_player.aModel[2].pos = D3DXVECTOR3(-10.5f, 50.5f, 0.0f);//位置の設定
	g_player.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[3].nIdxModelParent = 2;//親のインデックスを設定　//右手
	g_player.aModel[3].pos = D3DXVECTOR3(-8.0f, -18.0f, -2.5f);//向きの設定
	g_player.aModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[4].nIdxModelParent = 3;//親のインデックスを設定 //左腕
	g_player.aModel[4].pos = D3DXVECTOR3(-5.0f, -29.0f, 3.0f);//位置の設定
	g_player.aModel[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[5].nIdxModelParent = 0;//親のインデックスを設定　//左手
	g_player.aModel[5].pos = D3DXVECTOR3(8.5f, 50.5f, 0.0f);//位置の設定
	g_player.aModel[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定

	g_player.aModel[6].nIdxModelParent = 5;//親のインデックスを設定　//右腿
	g_player.aModel[6].pos = D3DXVECTOR3(8.0f, -18.0f, 0.0f);//位置の設定
	g_player.aModel[6].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[7].nIdxModelParent = 6;//親のインデックスを設定　//右足
	g_player.aModel[7].pos = D3DXVECTOR3(5.0f, -29.0f, 3.0f);//位置の設定
	g_player.aModel[7].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定

	g_player.aModel[8].nIdxModelParent = 14;//親のインデックスを設定 //左腿
	g_player.aModel[8].pos = D3DXVECTOR3(0.0f, 4.0f, 4.0f);//位置の設定
	g_player.aModel[8].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[9].nIdxModelParent = 11;//親のインデックスを設定　//左足
	g_player.aModel[9].pos = D3DXVECTOR3(3.0f, -32.0f, -4.0f);//位置の設定
	g_player.aModel[9].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[10].nIdxModelParent = 9;//親のインデックスを設定　//右肘
	g_player.aModel[10].pos = D3DXVECTOR3(6.0f, -36.0f, 1.0f);//位置の設定
	g_player.aModel[10].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[11].nIdxModelParent = 14;//親のインデックスを設定　//左肘
	g_player.aModel[11].pos = D3DXVECTOR3(-2.0f, 6.0f, 4.0f);//位置の設定
	g_player.aModel[11].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[12].nIdxModelParent = 8;//親のインデックスを設定　//右膝
	g_player.aModel[12].pos = D3DXVECTOR3(-4.0f, -30.0f, -5.0f);//位置の設定
	g_player.aModel[12].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定
	
	g_player.aModel[13].nIdxModelParent = 12;//親のインデックスを設定　//左膝
	g_player.aModel[13].pos = D3DXVECTOR3(-4.0f, -36.0f, 0.0f);//位置の設定
	g_player.aModel[13].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定

	g_player.aModel[14].nIdxModelParent = 0;//親のインデックスを設定　//腰
	g_player.aModel[14].pos = D3DXVECTOR3(0.0f, 2.0f, 0.0f);//位置の設定
	g_player.aModel[14].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きの設定

	g_player.aModel[15].nIdxModelParent = 4;//親のインデックスを設定　//剣
	g_player.aModel[15].pos = D3DXVECTOR3(0.0f, -15.0f, -3.0f);//位置の設定
	g_player.aModel[15].rot = D3DXVECTOR3(0.0f, 1.45f, -0.75f);//向きの設定


	// 変数の初期化
	g_player.pos = D3DXVECTOR3(150.0f, 0.0f, 100.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.motionType = MOTIONTYPE_MODELNEUTRAL;
	g_player.bLoopMotion = false;
	g_player.nNumKey = 0;
	g_player.nKey = 0;
	g_player.nCounterMotion = 0;
	g_player.g_vtxMinPlayer = VTX_MINP;
	g_player.g_vtxMaxPlayer = VTX_MAXP;

	// 頂点バッファをロック
	g_player.aModel[0].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点座標の比較と更新
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 各座標の最大値の比較
		if (g_player.g_vtxMaxPlayer.x < vtx.x)
		{
			g_player.g_vtxMaxPlayer.x = vtx.x;
		}
		if (g_player.g_vtxMaxPlayer.y < vtx.y)
		{
			g_player.g_vtxMaxPlayer.y = vtx.y;
		}
		if (g_player.g_vtxMaxPlayer.z < vtx.z)
		{
			g_player.g_vtxMaxPlayer.z = vtx.z;
		}

		// 各座標の最小値の比較
		if (g_player.g_vtxMinPlayer.x > vtx.x)
		{
			g_player.g_vtxMinPlayer.x = vtx.x;
		}
		if (g_player.g_vtxMinPlayer.y > vtx.y)
		{
			g_player.g_vtxMinPlayer.y = vtx.y;
		}
		if (g_player.g_vtxMinPlayer.z > vtx.z)
		{
			g_player.g_vtxMinPlayer.z = vtx.z;
		}

		// 剣の最大値の比較
		if (g_player.aModel[15].g_vtxMaxSord.x < vtx.x)
		{
			g_player.aModel[15].g_vtxMaxSord.x = vtx.x;
		}
		if (g_player.aModel[15].g_vtxMaxSord.y < vtx.y)
		{
			g_player.aModel[15].g_vtxMaxSord.y = vtx.y;
		}
		if (g_player.aModel[15].g_vtxMaxSord.z < vtx.z)
		{
			g_player.aModel[15].g_vtxMaxSord.z = vtx.z;
		}

		// 剣の最小値の比較
		if (g_player.aModel[15].g_vtxMinSord.x > vtx.x)
		{
			g_player.aModel[15].g_vtxMinSord.x = vtx.x;
		}
		if (g_player.aModel[15].g_vtxMinSord.y > vtx.y)
		{
			g_player.aModel[15].g_vtxMinSord.y = vtx.y;
		}
		if (g_player.aModel[15].g_vtxMinSord.z > vtx.z)
		{
			g_player.aModel[15].g_vtxMinSord.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	g_player.aModel[0].pMeshModel->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitModel(void)
{
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		if (g_player.aModel[nCnt].pMeshModel != NULL)
		{// メッシュの破棄
			g_player.aModel[nCnt].pMeshModel->Release();
			g_player.aModel[nCnt].pMeshModel = NULL;
		}

		if (g_player.aModel[nCnt].pBuffMatModel != NULL)
		{// バッファの破棄
			g_player.aModel[nCnt].pBuffMatModel->Release();
			g_player.aModel[nCnt].pBuffMatModel = NULL;
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateModel(void)
{
	// カメラの取得
	Camera camera = GetCamera();
	//double dSqrt = sqrt(2);		// 2の平方根

	g_player.posOld = g_player.pos;
	g_player.aModel[15].posOld = g_player.aModel[15].pos;

	// 位置のリセット
	/*if (GetKeyboardTrigger(KEYINFO_PRESET) == true)
	{
		g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}*/

	// モーションカウンターの加算
	g_player.nCounterMotion++;

	// モーションの更新
	UpdateMotion(g_player.motionType);

	// 移動量の加算
	g_player.pos += g_player.move;

	// モデルの移動
	if (IsButtonPush(KEYINFO_UP) == true)
	{
		g_player.motionType = MOTIONTYPE_MODELMOVE;
		if (IsButtonPush(KEYINFO_LEFT) == true)
		{// 左上方向
			g_player.move.x -= cosf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_player.move.z += sinf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + (D3DX_PI * 3 / 4);
		}
		else if (IsButtonPush(KEYINFO_RIGHT) == true)
		{// 右上方向
			g_player.move.x += cosf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_player.move.z -= sinf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y - (D3DX_PI * 3 / 4);
		}
		else
		{// 上方向
			g_player.move.x += sinf(camera.rot.y) * PLAYER_MOVE;
			g_player.move.z += cosf(camera.rot.y) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI;
		}
	}
	else if (IsButtonPush(KEYINFO_DOWN) == true)
	{
		g_player.motionType = MOTIONTYPE_MODELMOVE;
		if (IsButtonPush(KEYINFO_LEFT) == true)
		{// 左下方向
			g_player.move.x += cosf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.move.z -= sinf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI / 4;
		}
		else if (IsButtonPush(KEYINFO_RIGHT) == true)
		{// 右下方向
			g_player.move.x -= cosf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.move.z += sinf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y + D3DX_PI / -4;
		}
		else
		{// 下方向
			g_player.move.x += sinf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
			g_player.move.z += cosf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
			g_player.rotDest.y = camera.rot.y;
		}
	}
	else if (IsButtonPush(KEYINFO_LEFT) == true)
	{// 左方向
		g_player.move.x -= cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z += sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / 2);
		g_player.motionType = MOTIONTYPE_MODELMOVE;
	}
	else if (IsButtonPush(KEYINFO_RIGHT) == true)
	{// 右方向
		g_player.move.x += cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z -= sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / -2);
		g_player.motionType = MOTIONTYPE_MODELMOVE;
	}
	else
	{
		g_player.motionType = MOTIONTYPE_MODELNEUTRAL;
	}

	if (IsButtonPush(KEYINFO_A) == true)
	{
		g_player.motionType = MOTIONTYPE_MODELACTION;

		// 弾の発射
		if (IsButtonPush(KEYINFO_B) == true)
		{
			SetBullet(
				D3DXVECTOR3(g_player.pos.x + sinf(D3DX_PI - g_player.rot.y) * -10.0f, g_player.pos.y + 130, g_player.pos.z - 70 +cosf(D3DX_PI - g_player.rot.y) * 10.0f),
				D3DXVECTOR3(sinf(D3DX_PI - g_player.rot.y) * -5.0f, 0.0f, cosf(D3DX_PI - g_player.rot.y) * 5.0f),
				100);
		}
	}


	if (IsButtonUp(KEYINFO_UP) == true || IsButtonUp(KEYINFO_DOWN) == true || IsButtonUp(KEYINFO_RIGHT) == true || IsButtonUp(KEYINFO_LEFT) == true)
	{
		g_player.nCounterMotion = 0;
		g_player.aModel[0].pos.y = 75.0f;
	}


	if (IsButtonDown(KEYINFO_UP) == true || IsButtonDown(KEYINFO_DOWN) == true || IsButtonDown(KEYINFO_RIGHT) == true || IsButtonDown(KEYINFO_LEFT) == true)
	{
		g_player.nCounterMotion = 0;
		g_player.nKey = 0;
		g_player.aModel[0].pos.y = 75.0f;
		g_player.motionType = MOTIONTYPE_MODELNEUTRAL;
	}


	// モデルの上昇・下降
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_player.move.y = 2;
	}
	else if (GetKeyboardPress(DIK_X) == true)
	{
		g_player.move.y = -2;
	}
	else
	{
		g_player.move.y = 0;
	}

	// モデルの旋回
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_player.rotDest.y += D3DX_PI / 36;
	}
	if (GetKeyboardPress(DIK_L) == true)
	{
		g_player.rotDest.y -= D3DX_PI / 36;
	}

	// 目的の回転角の上限
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.1f;

	// 現在の回転角の上限
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	// 加速後の減速処理
	g_player.move.x += (0.0f - g_player.move.x) * 0.5f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.5f;

	CollisionVec(&g_player.pos, &g_player.posOld, &g_player.move, g_player.g_vtxMaxPlayer.x, g_player.g_vtxMinPlayer.x, g_player.g_vtxMaxPlayer.z, g_player.g_vtxMinPlayer.z, g_player.g_vtxMaxPlayer.y, g_player.g_vtxMinPlayer.y);
	CollisionVecItem(&g_player.pos, &g_player.posOld, &g_player.move ,g_player.g_vtxMaxPlayer.x, g_player.g_vtxMinPlayer.x, g_player.g_vtxMaxPlayer.z, g_player.g_vtxMinPlayer.z, g_player.g_vtxMaxPlayer.y, g_player.g_vtxMinPlayer.y);
	CollisionVecSord(&g_player.pos, &g_player.posOld, g_player.g_vtxMaxPlayer.x, g_player.g_vtxMinPlayer.x, g_player.g_vtxMaxPlayer.z, g_player.g_vtxMinPlayer.z, g_player.g_vtxMaxPlayer.y, g_player.g_vtxMinPlayer.y);
	CollisionVecWall(&g_player.pos, &g_player.posOld, &g_player.move, g_player.g_vtxMaxPlayer.x, g_player.g_vtxMinPlayer.x, g_player.g_vtxMaxPlayer.z, g_player.g_vtxMinPlayer.z, g_player.g_vtxMaxPlayer.y, g_player.g_vtxMinPlayer.y);
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawModel(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DMATERIAL9 shadowDef;						// 現在のシャドウマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ
	D3DXMATRIX ShadowMatrix;//シャドウマトリックス
	D3DXVECTOR4 veclight;//ライトのベクトル
	D3DXVECTOR3 pos, normal;//平面上の座標、ベクトル
	D3DXPLANE plane;//平面

	D3DLIGHT9 *plight = GetLight();

	veclight = D3DXVECTOR4(0.2f, 0.8f, 0.4f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&ShadowMatrix);

	// プレイヤーのワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// プレイヤーの向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// プレイヤーの位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// プレイヤーのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 現在のシャドウマテリアルを取得
	pDevice->GetMaterial(&shadowDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX mtxParent;					// 親のマトリックス

		// 各パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		// 各パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// 各パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		// 各パーツの親マトリックスを設定
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;	// プレイヤーのマトリックスを設定
		}

		// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

		// 各パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[0].pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_player.aModel[0].pTextureModel);

			// モデル(パーツ)の描画
			g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//マテリアルデータへのポインタを取得
		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		{
			shadowDef.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			//マテリアルの設定
			pDevice->SetMaterial(&shadowDef);

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//シャドウマトリックスの生成
			D3DXMatrixShadow(&ShadowMatrix, &veclight, &plane);

			D3DXMatrixMultiply(&ShadowMatrix, &g_player.aModel[nCntModel].mtxWorld, &ShadowMatrix);

			//各ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &ShadowMatrix);

			//モデルパーツの描画
			g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
		}

		//通常の合成に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&shadowDef);
}

//==============================================================================
// モデルの取得
//==============================================================================
Player *GetModel(void)
{
	return &g_player;
}

//==============================================================================
// モーションの設定
//==============================================================================
void SetMotion(MOTIONMODELTYPE motionType)
{
	//g_player.motionType = motionType;

	if (motionType == MOTIONTYPE_MODELNEUTRAL)
	{// 待機モーション
		g_player.bLoopMotion = true;	// ループする
		g_player.nNumKey = 2;	// キー数
		g_player.aMotionInfo[0].aKeySet[0].nFrame = 40;	// フレーム数

		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fPosY = -0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fPosZ = -0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[0].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[1].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[2].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fRotY = -0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[3].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[4].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[5].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[6].fRotZ = -0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[7].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[8].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[9].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[10].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[11].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[12].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[13].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[14].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[0].aKeySet[0].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[15].fPosY = -20.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[15].fPosZ = -3.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[15].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[15].fRotY = 1.45f;
		g_player.aMotionInfo[0].aKeySet[0].aKey[15].fRotZ = -0.75f;

		g_player.aMotionInfo[0].aKeySet[1].nFrame = 8;	// フレーム数

		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fPosY = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fPosZ = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[0].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[1].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[2].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fRotY = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[3].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[4].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[5].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[6].fRotZ = -0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[7].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[8].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[9].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[10].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[11].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fRotX = -0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[12].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[13].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fRotY = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[14].fRotZ = 0.0f;
										
		g_player.aMotionInfo[0].aKeySet[1].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[15].fPosY = -20.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[15].fPosZ = -3.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[15].fRotX = 0.0f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[15].fRotY = 1.45f;
		g_player.aMotionInfo[0].aKeySet[1].aKey[15].fRotZ = -0.75f;
	}
	else if (g_player.motionType == MOTIONTYPE_MODELMOVE)
	{// 移動モーション
		g_player.bLoopMotion = true;					// ループする
		g_player.nNumKey = 4;							// キー数
		g_player.aMotionInfo[1].aKeySet[0].nFrame = 8;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fPosY = -7.20f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fRotX = -0.22f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fRotY = 0.25f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[0].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fRotY = -0.19f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[1].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fRotX = -1.29f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[2].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fRotX = 1.29f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[3].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[4].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fRotX = 0.50f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[5].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fRotX = 1.01f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[6].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[7].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fRotX = 1.17f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[8].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fRotX = -0.85f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[9].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[10].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fRotX = -0.44f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[11].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fRotX = -0.44f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[12].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[13].fRotZ = 0.0f;
							 			
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fRotY = -0.31f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[14].fRotZ = 0.03f;

		g_player.aMotionInfo[1].aKeySet[0].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[15].fPosY = -20.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[15].fPosZ = -3.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[15].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[15].fRotY = 1.45f;
		g_player.aMotionInfo[1].aKeySet[0].aKey[15].fRotZ = -0.75f;

		g_player.aMotionInfo[1].aKeySet[1].nFrame = 10;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fPosY = -19.10f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fRotX = -0.22f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fRotY = 0.25f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fRotY = -0.19f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fRotX = -0.54f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fRotX = 1.29f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[3].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[4].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[5].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fRotX = 1.54f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[6].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[7].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fRotX = 1.13f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[8].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fRotX = -1.82f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[9].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[10].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fRotX = 0.41f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[11].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fRotX = -1.32f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[12].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fRotX = 0.41f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[13].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fRotY = -0.31f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[14].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[1].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[15].fPosY = -20.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[15].fPosZ = -3.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[15].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[15].fRotY = 1.45f;
		g_player.aMotionInfo[1].aKeySet[1].aKey[15].fRotZ = -0.75f;

		g_player.aMotionInfo[1].aKeySet[2].nFrame = 8;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fPosY = -10.90f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fRotX = -0.22f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fRotY = -0.25f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fRotX = 0.57f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fRotX = 0.88f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[3].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[4].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fRotX = -0.91f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[5].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fRotX = 1.01f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[6].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[7].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fRotX = -0.57f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[8].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fRotX = -0.47f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[9].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[10].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fRotX = 1.23f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[11].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fRotX = -0.50f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[12].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[13].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fRotY = 0.35f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[14].fRotZ = 0.06f;

		g_player.aMotionInfo[1].aKeySet[2].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[15].fPosY = -20.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[15].fPosZ = -3.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[15].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[15].fRotY = 1.45f;
		g_player.aMotionInfo[1].aKeySet[2].aKey[15].fRotZ = -0.75f;


		g_player.aMotionInfo[1].aKeySet[3].nFrame = 10;	// フレーム数

		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fPosY = -18.20f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fRotX = -0.22f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fRotY = -0.25f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[0].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[1].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fRotX = 0.03f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[2].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fRotX = 0.88f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[3].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[4].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fRotX = -0.44f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[5].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fRotX = 1.01f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[6].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[7].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fRotX = 0.16f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[8].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fRotX = -1.13f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[9].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fRotX = 0.38f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[10].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fRotX = 1.04f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[11].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fRotX = -1.73f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[12].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[13].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fRotY = 0.35f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[14].fRotZ = 0.0f;

		g_player.aMotionInfo[1].aKeySet[3].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[15].fPosY = -20.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[15].fPosZ = -3.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[15].fRotX = 0.0f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[15].fRotY = 1.45f;
		g_player.aMotionInfo[1].aKeySet[3].aKey[15].fRotZ = -0.75f;
	}

	else if (g_player.motionType == MOTIONTYPE_MODELACTION)
	{// 移動モーション
		g_player.bLoopMotion = false;					// ループする
		g_player.nNumKey = 2;							// キー数
		g_player.aMotionInfo[2].aKeySet[0].nFrame = 5;	// フレーム数

		g_player.aMotionInfo[2].aKeySet[0].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[0].fPosY = -0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[0].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[0].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[0].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[1].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[2].fRotX = -0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[2].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[2].fRotZ = -0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[3].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[3].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[4].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[5].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[5].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[5].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[6].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[6].fRotY = -0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[6].fRotZ = -0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[7].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[8].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[8].fRotZ = -0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[9].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[9].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[10].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[10].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[11].fRotX = -0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[11].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[12].fRotX = -0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[12].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[13].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[13].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[14].fRotX = -0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[14].fRotY = -0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[14].fRotZ = 0.0f;
							 
		g_player.aMotionInfo[2].aKeySet[0].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[15].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[15].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[15].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[15].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[0].aKey[15].fRotZ = 0.0f;

		g_player.aMotionInfo[2].aKeySet[1].nFrame = 25;	// フレーム数

		g_player.aMotionInfo[2].aKeySet[1].aKey[0].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[0].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[0].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[0].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[0].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[0].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[1].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[1].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[1].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[1].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[1].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[1].fRotZ = 0.22f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[2].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[2].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[2].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[2].fRotX = 1.54f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[2].fRotY = -0.47f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[2].fRotZ = -0.16f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[3].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[3].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[3].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[3].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[3].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[3].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[4].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[4].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[4].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[4].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[4].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[4].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[5].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[5].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[5].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[5].fRotX = 1.54f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[5].fRotY = 0.06f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[5].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[6].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[6].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[6].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[6].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[6].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[6].fRotZ = -0.74f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[7].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[7].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[7].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[7].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[7].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[7].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[8].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[8].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[8].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[8].fRotX = -0.35f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[8].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[8].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[9].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[9].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[9].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[9].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[9].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[9].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[10].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[10].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[10].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[10].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[10].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[10].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[11].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[11].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[11].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[11].fRotX = 0.22f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[11].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[11].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[12].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[12].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[12].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[12].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[12].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[12].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[13].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[13].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[13].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[13].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[13].fRotY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[13].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[14].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[14].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[14].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[14].fRotX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[14].fRotY = 0.09f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[14].fRotZ = 0.0f;
										
		g_player.aMotionInfo[2].aKeySet[1].aKey[15].fPosX = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[15].fPosY = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[15].fPosZ = 0.0f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[15].fRotX = 0.35f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[15].fRotY = 2.15f;
		g_player.aMotionInfo[2].aKeySet[1].aKey[15].fRotZ = -1.45f;

	}
}

//==============================================================================
// モーションの更新
//==============================================================================
void UpdateMotion(MOTIONMODELTYPE motionType)
{
	SetMotion(motionType);

	for (int nCnt = 0; nCnt < MODEL_PARTS; nCnt++)
	{
		if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
		{
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
		}
		else
		{
			g_player.aModel[nCnt].pos.x += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.y += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].pos.z += (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame;
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);


			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == false)
			{
				break;
			}
		}

		if (g_player.nCounterMotion == g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame)
		{
			g_player.nCounterMotion = 0;

			if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
			{
				g_player.nKey = 0;
			}
			else
			{
				g_player.nKey++;
			}
		}
	}
}