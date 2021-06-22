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
#include "object.h"
#include <stdio.h>
#include "Billboard.h"
#include "gamepad.h"
#include "wall.h"
#include "item.h"
#include "light.h"

//==============================================================================
// マクロ定義
//==============================================================================
//#define MODEL_MOVE	(1.0f)
//#define MODEL_TURN	(0.1f)
#define PLAYER_MOVE		(4.65f)
#define PLAYER_TURN		(0.1f)
#define VTX_MINP		(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// 仮頂点最小値
#define	VTX_MAXP		(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// 仮頂点最大値
#define TEXT_LENGTH		(128)	// テキスト読み込みの最大数
#define DATA_LENGTH		(64)	// データ読み込みの最大数
#define MOVE_MIN		(0.5f)	// 移動モーション時の最小移動力
#define MOVECNT_MAX		(20)	// 移動モーション以降カウント最大値
#define SPHERE_SLICE	(10)	// 球体スライス数
#define SPHERE_STACK	(7)		// 球体スタック数

//==============================================================================
// グローバル変数
//==============================================================================
Player g_player;							// プレイヤーの構造体

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitModel(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();// デバイスの取得

	LoadMotion();

	// Xファイルの読み込み
	for (int nCnt = 0; nCnt < (g_player.nNumModel); nCnt++)
	{
		D3DXLoadMeshFromX(
			g_player.aModel[nCnt].aFileName,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[nCnt].pBuffMatModel,
			NULL,
			&g_player.aModel[nCnt].nNumMatModel,
			&g_player.aModel[nCnt].pMeshModel);

		g_player.aModel[nCnt].bUse = false;
	}

	g_player.nNumModel = MODEL_PARTS;

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

	// 変数の初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	g_player.as.vtxMinSphere = VTX_MINP;
	g_player.as.vtxMaxSphere = VTX_MAXP;
	g_player.bMotionChange = false;
	g_player.nBrendFrame = 5;
	g_player.weapon = WEAPON_NONE;

	// ローカル変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_player.aModel[0].pMeshModel->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].pMeshModel->GetFVF());

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

		else if (g_player.aModel[nCnt].pBuffMatModel != NULL)
		{// バッファの破棄
			g_player.aModel[nCnt].pBuffMatModel->Release();
			g_player.aModel[nCnt].pBuffMatModel = NULL;
		}
	}

	if (g_player.as.pMeshSphere != NULL)
	{
		g_player.as.pMeshSphere->Release();
		g_player.as.pMeshSphere = NULL;
	}

	else if (g_player.as.pBuffMatSphere != NULL)
	{
		g_player.as.pBuffMatSphere->Release();
		g_player.as.pBuffMatSphere = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateModel(void)
{
	// カメラの取得
	Camera camera = GetCamera();

	g_player.posOld = g_player.pos;
	g_player.as.posold = g_player.as.pos;

	// モーションの更新
	UpdateMotion(g_player.motionType);

	// 移動量の加算
	g_player.pos += g_player.move;
	
	

	// モデルの移動
	if (IsButtonPush(KEYINFO_UP) == true)
	{
		if (g_player.nMoveCnt < 20)
		{
			g_player.nMoveCnt++;
		}

		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MODELMOVE;
		}

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
		if (g_player.nMoveCnt < MOVECNT_MAX)
		{
			g_player.nMoveCnt++;
		}
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MODELMOVE;
		}
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
		if (g_player.nMoveCnt < MOVECNT_MAX)
		{
			g_player.nMoveCnt++;
		}
		g_player.move.x -= cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z += sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / 2);
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MODELMOVE;
		}
	}
	else if (IsButtonPush(KEYINFO_RIGHT) == true)
	{// 右方向
		if (g_player.nMoveCnt < MOVECNT_MAX)
		{
			g_player.nMoveCnt++;
		}

		g_player.move.x += cosf(camera.rot.y) * PLAYER_MOVE;
		g_player.move.z -= sinf(camera.rot.y) * PLAYER_MOVE;
		g_player.rotDest.y = camera.rot.y + (D3DX_PI / -2);
		if (g_player.motionType != MOTIONTYPE_JUMP)
		{
			g_player.motionType = MOTIONTYPE_MODELMOVE;
		}
	}
	else
	{
		if (g_player.nMoveCnt > 0)
		{
			g_player.nMoveCnt--;
		}
	}

	// 移動モーションから待機モーションへ
	if (g_player.motionType == MOTIONTYPE_MODELMOVE && g_player.nMoveCnt == 0)
	{
		g_player.nCounterMotion = 0;
		g_player.nKey = 0;
		g_player.motionType = MOTIONTYPE_MODELNEUTRAL;
	}

	if (g_player.weapon == WEAPON_NONE)
	{
		if (IsButtonDown(KEYINFO_ATTACK) == true)
		{
			if (g_player.motionType == MOTIONTYPE_MODELNEUTRAL)
			{
				g_player.nKey = 0;
				g_player.nCounterMotion = 0;
				g_player.motionType = MOTIONTYPE_MODELACTION;
			}
		}
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


	CollisionVecObject(&g_player.pos, &g_player.posOld, &g_player.move, g_player.g_vtxMaxPlayer.x, g_player.g_vtxMinPlayer.x, g_player.g_vtxMaxPlayer.z, g_player.g_vtxMinPlayer.z, g_player.g_vtxMaxPlayer.y, g_player.g_vtxMinPlayer.y);
	CollisionVecItem(&g_player.pos, &g_player.posOld, &g_player.move ,g_player.g_vtxMaxPlayer.x, g_player.g_vtxMinPlayer.x, g_player.g_vtxMaxPlayer.z, g_player.g_vtxMinPlayer.z, g_player.g_vtxMaxPlayer.y, g_player.g_vtxMinPlayer.y);
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

		//マテリアルデータへのポインタを取得
		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		{
			shadowDef.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

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
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&shadowDef);

	AttackParts();
}

//==============================================================================
// プレイヤーの攻撃位置反映
//==============================================================================
void AttackParts(void)
{
	//// ローカル変数宣言
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	//D3DXMATRIX mtxRotAttack, mtxTransAttack;	// 計算用マトリックス
	//D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	//D3DXMATRIX mtxParent;						// 親のマトリックス
	//D3DXMATERIAL *pMat;

	//// 現在のマテリアルを取得
	//pDevice->GetMaterial(&matDef);

	//// 攻撃判定マトリックスの初期化
	//D3DXMatrixIdentity(&g_player.as.mtxWorld);

	//// 攻撃判定の向きを反映
	//D3DXMatrixRotationYawPitchRoll(
	//	&mtxRotAttack,
	//	g_player.as.rot.y,
	//	g_player.as.rot.x,
	//	g_player.as.rot.z);
	//D3DXMatrixMultiply(
	//	&g_player.as.mtxWorld,
	//	&g_player.as.mtxWorld,
	//	&mtxRotAttack);

	//// 各パーツの位置を反映
	//D3DXMatrixTranslation(
	//	&mtxTransAttack,
	//	g_player.as.pos.x,
	//	g_player.as.pos.y,
	//	g_player.as.pos.z);
	//D3DXMatrixMultiply(
	//	&g_player.as.mtxWorld,
	//	&g_player.as.mtxWorld,
	//	&mtxTransAttack);

	////mtxParent = g_player.aModel[g_player.as.nParts].mtxWorld;

	//// 算出した各パーツのワールドマトリックスと判定発生パーツのマトリックスを掛け合わせる
	///*D3DXMatrixMultiply(
	//	&g_player.as.mtxWorld,
	//	&g_player.as.mtxWorld,
	//	&mtxParent);*/

	//// 判定のワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &g_player.as.mtxWorld);

	//// マテリアルデータへのポインタを取得
	//pMat = (D3DXMATERIAL*)g_player.as.pBuffMatSphere->GetBufferPointer();

	//pMat[0].MatD3D.Diffuse = g_player.as.col;

	//for (int nCntModel = 0; nCntModel < 1; nCntModel++)
	//{
	//	//マテリアルの設定
	//	pDevice->SetMaterial(&pMat[nCntModel].MatD3D);

	//	if (g_player.as.bUse == true)
	//	{
	//		// モデル(パーツ)の描画
	//		//g_player.as.pMeshSphere->DrawSubset(nCntModel);
	//	}
	//}
	//// 保存していたマテリアルを戻す
	//pDevice->SetMaterial(&matDef);
}


//==============================================================================
// モデルの取得
//==============================================================================
Player *GetModel(void)
{
	return &g_player;
}

//==============================================================================
// モーションの更新
//==============================================================================
void UpdateMotion(MOTIONMODELTYPE motionType)
{
	// キー数とループ情報の代入
	g_player.nNumKey = g_player.aMotionInfo[motionType].nNumKey;
	g_player.bLoopMotion = g_player.aMotionInfo[motionType].bLoop;

	// モーションカウンターの加算
	if (g_player.bMotionChange == true)
	{// モーションブレンド実行時
		if (g_player.nCounterMotion < g_player.nBrendFrame)
		{// カウント上限
			g_player.nCounterMotion++;
		}
	}
	else
	{// 通常時
		if (g_player.nCounterMotion < g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame)
		{// カウント上限
			g_player.nCounterMotion++;
		}
	}

	if (g_player.as.nCntFrameTotal < g_player.nFrameTotal[motionType])
	{
		g_player.as.nCntFrameTotal++;
	}

	// モデルパーツごとのモーションアニメーション
	for (int nCnt = 0; nCnt < MODEL_PARTS; nCnt++)
	{
		if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
		{// ループして最初に戻るとき
			g_player.aModel[nCnt].pos.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].pos.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].pos.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].posMemo = g_player.aModel[nCnt].pos;
			g_player.aModel[nCnt].rotMemo = g_player.aModel[nCnt].rot;
		}
		else if (g_player.bMotionChange == true)
		{// モーションブレンド
			g_player.aModel[nCnt].pos.x = g_player.aModel[nCnt].posMemo.x + (g_player.aMotionInfo[g_player.motionType].aKeySet[0].aKey[nCnt].fPosX - g_player.aModel[nCnt].posMemo.x) * ((float)g_player.nCounterMotion / (float)g_player.nBrendFrame);
			g_player.aModel[nCnt].pos.y = g_player.aModel[nCnt].posMemo.y + (g_player.aMotionInfo[g_player.motionType].aKeySet[0].aKey[nCnt].fPosY - g_player.aModel[nCnt].posMemo.y) * ((float)g_player.nCounterMotion / (float)g_player.nBrendFrame);
			g_player.aModel[nCnt].pos.z = g_player.aModel[nCnt].posMemo.z + (g_player.aMotionInfo[g_player.motionType].aKeySet[0].aKey[nCnt].fPosZ - g_player.aModel[nCnt].posMemo.z) * ((float)g_player.nCounterMotion / (float)g_player.nBrendFrame);
			g_player.aModel[nCnt].rot.x = g_player.aModel[nCnt].rotMemo.x + (g_player.aMotionInfo[g_player.motionType].aKeySet[0].aKey[nCnt].fRotX - g_player.aModel[nCnt].rotMemo.x) * ((float)g_player.nCounterMotion / (float)g_player.nBrendFrame);
			g_player.aModel[nCnt].rot.y = g_player.aModel[nCnt].rotMemo.y + (g_player.aMotionInfo[g_player.motionType].aKeySet[0].aKey[nCnt].fRotY - g_player.aModel[nCnt].rotMemo.y) * ((float)g_player.nCounterMotion / (float)g_player.nBrendFrame);
			g_player.aModel[nCnt].rot.z = g_player.aModel[nCnt].rotMemo.z + (g_player.aMotionInfo[g_player.motionType].aKeySet[0].aKey[nCnt].fRotZ - g_player.aModel[nCnt].rotMemo.z) * ((float)g_player.nCounterMotion / (float)g_player.nBrendFrame);
			g_player.aModel[nCnt].posMemo = g_player.aModel[nCnt].pos;
			g_player.aModel[nCnt].rotMemo = g_player.aModel[nCnt].rot;
		}
		else
		{// 通常時
			g_player.aModel[nCnt].pos.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].pos.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].pos.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fPosZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fPosZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.x = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotX - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotX) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.y = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotY - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotY) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].rot.z = g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ + (g_player.aMotionInfo[motionType].aKeySet[g_player.nKey + 1].aKey[nCnt].fRotZ - g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].aKey[nCnt].fRotZ) * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame);
			g_player.aModel[nCnt].posMemo = g_player.aModel[nCnt].pos;
			g_player.aModel[nCnt].rotMemo = g_player.aModel[nCnt].rot;
		}

		if (g_player.bMotionChange == false)
		{// モーション変化なし
			if (g_player.nCounterMotion == g_player.aMotionInfo[motionType].aKeySet[g_player.nKey].nFrame)
			{// キーフレーム終了時
				if (g_player.motionType != MOTIONTYPE_JUMP || g_player.nKey != g_player.nNumKey - 2)
				{// ジャンプ以外でキー終了時リセット
					g_player.nCounterMotion = 0;
				}

				if (g_player.nKey == g_player.nNumKey - 1 && g_player.bLoopMotion == true)
				{// ループするとき
					g_player.nKey = 0;
					g_player.as.nCntFrameTotal = 0;
				}
				else if (g_player.nKey == g_player.nNumKey - 2 && g_player.bLoopMotion == false)
				{// ループせず終了するとき
					if (g_player.motionTypeOld != MOTIONTYPE_JUMP)
					{// 直前がジャンプ以外の時
						g_player.motionType = MOTIONTYPE_MODELNEUTRAL;	// ニュートラルへ移行
						g_player.as.nCntFrameTotal = 0;
					}
				}
				else if (g_player.nKey < g_player.nNumKey)
				{// キーのカウント
					g_player.nKey++;
				}
			}
			else if (g_player.motionTypeOld != g_player.motionType)
			{// モーション変化時	
				g_player.nKey = 0;
				g_player.nCounterMotion = 0;
				g_player.attack.nCntFrameTotal = 0;

				if (g_player.motionTypeOld == MOTIONTYPE_MODELNEUTRAL && g_player.motionType == MOTIONTYPE_MODELMOVE)
				{// ニュートラルから移動時
					g_player.nBrendFrame = MBF_NEU_MOVE;
				}
				else if (g_player.motionTypeOld == MOTIONTYPE_MODELMOVE && g_player.motionType == MOTIONTYPE_MODELNEUTRAL)
				{// 移動からニュートラル時
					g_player.nBrendFrame = MBF_MOVE_NEU;
				}
				else if (g_player.motionTypeOld == MOTIONTYPE_MODELNEUTRAL && g_player.motionType == MOTIONTYPE_MODELACTION)
				{// ニュートラルからアクション時
					g_player.nBrendFrame = g_player.aMotionInfo[MOTIONTYPE_MODELACTION].aKeySet[0].nFrame;
				}
				else if (g_player.motionTypeOld == MOTIONTYPE_MODELACTION && g_player.motionType == MOTIONTYPE_MODELNEUTRAL)
				{// アクションからニュートラル時
					g_player.nBrendFrame = MBF_ACT_NEU;
				}
				break;
			}
		}
		else if (g_player.bMotionChange == true)
		{// モーションブレンド時のカウントリセット
			if (g_player.nCounterMotion == g_player.nBrendFrame || g_player.motionTypeOld != g_player.motionType)
			{
				g_player.nKey = 0;
				g_player.nCounterMotion = 0;
				g_player.as.nCntFrameTotal = 0;
				g_player.bMotionChange = false;
			}
		}

		// 打撃の当たり判定の有効化
		if (g_player.motionType == MOTIONTYPE_MODELACTION)
		{
			if (g_player.as.nCntFrameTotal >= g_player.as.nStartFrame &&
				g_player.as.nCntFrameTotal <= g_player.as.nEndFrame)
			{// カウント範囲内で有効
				g_player.as.bUse = true;
				if (g_player.motionType == MOTIONTYPE_MODELACTION)
				{
					if (g_player.as.nCntFrameTotal == g_player.as.nStartFrame)
					{
						//PlaySound(SOUND_LABEL_SE_SMASH);
					}
				}
				else
				{
					if (g_player.as.nCntFrameTotal == g_player.as.nStartFrame)
					{
						//PlaySound(SOUND_LABEL_SE_SLASH);
					}
				}
			}
		}
		else
		{// それ以外
			g_player.as.bUse = false;
		}

		// 当たり判定の表示
		if (g_player.as.bUse == true)
		{
			g_player.as.col.a = 1.0f;
		}
		else
		{
			g_player.as.col.a = 0.0f;
		}
	}

	// 直前のモーション状態の保存
	g_player.motionTypeOld = g_player.motionType;
}
//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void LoadMotion(void)
{
	// ローカル変数宣言
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aCharaSet[DATA_LENGTH] = { NULL };		// キャラデータ読み取り用変数
	char aPartsSet[DATA_LENGTH] = { NULL };		// パーツデータ読み取り用変数
	char aMotionSet[DATA_LENGTH] = { NULL };	// モーション読み取り用変数
	char aKeySet[DATA_LENGTH] = { NULL };		// キー設定情報読み取り用変数
	char aKey[DATA_LENGTH] = { NULL };			// キー情報読み取り用変数

	//g_player[0].modelType = (ModelType)pSelect->chara;

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	// データの読み込み
	pFile = fopen("data/TEXT/motion_player00.txt", "r");

		if (pFile != NULL)
		{ //ファイル展開可能
			aText[0] = { NULL };
			aCharaSet[0] = { NULL };
			aPartsSet[0] = { NULL };
			aMotionSet[0] = { NULL };
			aKeySet[0] = { NULL };
			aKey[0] = { NULL };
			int nIdxParts = 0;			// パーツ数
			int nMotion = 0;			// モーションの数
			int nCntFile = 0;			// Xデータの数
			int nAs = 0;
			while (strcmp("END_SCRIPT", pText) != 0)
			{
				aText[0] = { NULL };
				if (bComment == false)
				{// コメントアウトしていない
					fscanf(pFile, "%s", &aText[0]);

					if (aText[0] == '#')
					{// 文字列の先頭が「#」ならばコメントアウトへ
						bComment = true;
					}
					else
					{// 通常時
						if (strcmp("NUM_MODEL", pText) == 0)
						{// モデルの数
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &g_player.nNumModel);
						}
						else if (strcmp("MODEL_FILENAME", pText) == 0)
						{// Xファイル名の読み込み
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%s", &g_player.aModel[nCntFile].aFileName[0]);
							nCntFile++;
						}
						else if (strcmp("CHARACTERSET", pText) == 0)
						{// キャラの情報
							while (strcmp("END_CHARACTERSET", pCharaSet) != 0)
							{// キャラ情報の読み取り
								fscanf(pFile, "%s", &aCharaSet[0]);
								if (strcmp("NUM_PARTS", pCharaSet) == 0)
								{

								}
								else if (strcmp("RADIUS", pCharaSet) == 0)
								{// 当たり判定
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%f", &g_player.fOutRadius);
								}
								else if (strcmp("HEIGHT", pCharaSet) == 0)
								{// 当たり判定
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%f", &g_player.fHeight);
								}
								else if (strcmp("PARTSSET", pCharaSet) == 0)
								{// パーツ情報
									aPartsSet[0] = { NULL };

									while (strcmp("END_PARTSSET", pPartsSet) != 0)
									{
										fscanf(pFile, "%s", &aPartsSet[0]);
										if (strcmp("INDEX", pPartsSet) == 0)
										{// パーツ情報の読み取り
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%d", &nIdxParts);
										}
										else if (strcmp("PARENT", pPartsSet) == 0)
										{// パーツの親情報
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%d", &g_player.aModel[nIdxParts].nIdxModelParent);
										}
										else if (strcmp("POS", pPartsSet) == 0)
										{// パーツの位置情報
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.x);
											fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.y);
											fscanf(pFile, "%f", &g_player.aModel[nIdxParts].pos.z);
											g_player.aModel[nIdxParts].posReset = g_player.aModel[nIdxParts].pos;
										}
										else if (strcmp("ROT", pPartsSet) == 0)
										{// パーツの角度情報
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.x);
											fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.y);
											fscanf(pFile, "%f", &g_player.aModel[nIdxParts].rot.z);
										}
									}
								}
							}
						}
						else if (strcmp("MOTIONSET", pText) == 0)
						{
							// 変数の初期化
							aMotionSet[0] = { NULL };

							// ローカル変数宣言
							int nLoop = 0;
							int nKeySet = 0;

							while (strcmp("END_MOTIONSET", pMotionSet) != 0)
							{// モーションの読み込み
								fscanf(pFile, "%s", &aMotionSet[0]);
								if (strcmp("LOOP", pMotionSet) == 0)
								{// ループするか
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%d", &nLoop);
									if (nLoop == 1)
									{// １の場合ループする
										g_player.bLoopMotion = true;
										g_player.aMotionInfo[nMotion].bLoop = true;
									}
								}
								else if (strcmp("NUM_KEY", pMotionSet) == 0)
								{// キー数の読み込み
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%d", &g_player.nNumKey);
								}
								else if (strcmp("COLLISION", pMotionSet) == 0)
								{
									fscanf(pFile, "%s", &aEqual[0]);
									fscanf(pFile, "%d", &g_player.as.nParts);
									fscanf(pFile, "%f", &g_player.as.pos.x);
									fscanf(pFile, "%f", &g_player.as.pos.y);
									fscanf(pFile, "%f", &g_player.as.pos.z);
									fscanf(pFile, "%f", &g_player.as.fRadius);
									fscanf(pFile, "%d", &g_player.as.nStartFrame);
									fscanf(pFile, "%d", &g_player.as.nEndFrame);
									nAs++;
								}
								else if (strcmp("KEYSET", pMotionSet) == 0)
								{// キーの読み込み
								 // ローカル変数宣言
									int nKey = 0;
									// 変数の初期化
									aKeySet[0] = { NULL };

									while (strcmp("END_KEYSET", pKeySet) != 0)
									{// キー情報項目の読み込み
										fscanf(pFile, "%s", &aKeySet[0]);

										if (aKeySet[0] == '#')
										{// コメントアウト
										 // ローカル変数宣言
											char c = NULL;
											char d = NULL;
											fscanf(pFile, "%c", &c);
											while (c != '\n' && d != '\n')
											{
												fscanf(pFile, "%s", &aKey[0]);
												fscanf(pFile, "%c", &d);
											}
										}

										if (strcmp("KEY", pKeySet) == 0)
										{// キー情報の読み込み
											aKey[0] = { NULL };
											while (strcmp("END_KEY", pKey) != 0)
											{// 位置・角度の読み込み
												fscanf(pFile, "%s", &aKey[0]);
												if (strcmp("POS", pKey) == 0)
												{// 位置情報の読み込み
													fscanf(pFile, "%s", &aEqual[0]);
													fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosX);
													fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosY);
													fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosZ);
													g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosX += g_player.aModel[nKey].pos.x;
													g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosY += g_player.aModel[nKey].pos.y;
													g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fPosZ += g_player.aModel[nKey].pos.z;
												}
												else if (strcmp("ROT", pKey) == 0)
												{// 角度情報の読み込み
													fscanf(pFile, "%s", &aEqual[0]);
													fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotX);
													fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotY);
													fscanf(pFile, "%f", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].aKey[nKey].fRotZ);
												}
											}
											nKey++;
										}
										else if (strcmp("FRAME", pKeySet) == 0)
										{// キーフレーム数の読み込み
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%d", &g_player.aMotionInfo[nMotion].aKeySet[nKeySet].nFrame);
											g_player.nFrameTotal[nMotion] += g_player.aMotionInfo[nMotion].aKeySet[nKeySet].nFrame;
											if (g_player.aMotionInfo[nMotion].bLoop == false && nKeySet == (g_player.nNumKey - 1))
											{
												g_player.nFrameTotal[nMotion] -= g_player.aMotionInfo[nMotion].aKeySet[nKeySet].nFrame;
											}
										}
									}
									nKeySet++;
								}
							}
							g_player.aMotionInfo[nMotion].nNumKey = nKeySet;	// キー数の設定
							nMotion++;
						}
					}
				}
				else if (bComment == true)
				{// コメントアウト処理
				 // ローカル変数宣言
					char a = NULL;
					char b = NULL;
					fscanf(pFile, "%c", &a);
					while (a != '\n' && b != '\n')
					{
						fscanf(pFile, "%s", &aText[0]);
						fscanf(pFile, "%c", &b);
					}
					bComment = false;
				}
			}
			fclose(pFile);
		}
		else
		{ // ファイル展開不可
		}
}
