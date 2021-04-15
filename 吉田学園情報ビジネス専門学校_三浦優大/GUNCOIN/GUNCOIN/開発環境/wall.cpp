//================================================
//
//石の処理[stone.h]
// Author; miura yuudai
//
//================================================

#include "wall.h"
#include "input.h"
#include "camera.h"
#include "Billboard.h"
#include "bullet.h"
#include "model.h"

//=====================
//グローバル変数
//=====================
//メッシュ（頂点情報）へのポインタ
LPD3DXMESH g_pMeshWall = NULL;

//マテリアル（材質情報）へのポインタ
LPD3DXBUFFER g_pBuffMatWall = NULL;

DWORD g_nNumMatWall = 0;//マテリアルの数
WALL g_PosWall[MAXWALL];//xファイル
D3DXMATRIX g_mtxWolrdWall;//ワールドマトリックス

//====================
//モデルの初期化処理
//====================
void InitWall(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/sinndenn.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatWall, NULL, &g_nNumMatWall, &g_pMeshWall);


	for (int nCntMat = 0; nCntMat < MAXWALL; nCntMat++)
	{
			//変数初期化
			g_PosWall[nCntMat].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_PosWall[nCntMat].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_PosWall[nCntMat].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_PosWall[nCntMat].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_PosWall[nCntMat].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//g_PosWall[nCntMat].MaxStone = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
			//g_PosWall[nCntMat].MinStone = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
			g_PosWall[nCntMat].bUse = false;
			g_PosWall[nCntMat].nType = 0;

			//マテリアルへのポインタ
			D3DXMATERIAL *pMat;

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatWall->GetBufferPointer();

			if (pMat[0].pTextureFilename != NULL)
			{
				//テキスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ro-ma.jpg", &g_PosWall[nCntMat].g_apTextureWall);
			}


			g_pMeshWall->UnlockVertexBuffer();
		}
}
//=======================
//モデルの終了処理
//=======================
void UninitWall(void)
{
		//頂点の開放
		if (g_pMeshWall != NULL)
		{
			g_pMeshWall->Release();
			g_pMeshWall = NULL;
		}

		//材質の開放
		if (g_pBuffMatWall != NULL)
		{
			g_pBuffMatWall->Release();
			g_pBuffMatWall = NULL;
		}

		for (int nCntMat = 0; nCntMat < MAXWALL; nCntMat++)
		{
			//テクスチャの開放
			if (g_PosWall[nCntMat].g_apTextureWall != NULL)
			{
				g_PosWall[nCntMat].g_apTextureWall->Release();
				g_PosWall[nCntMat].g_apTextureWall = NULL;
			}
		}
}
//=======================
//モデルの更新処理
//=======================
void UpdateWall(void)
{
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE *pVtxBuff;//頂点バッファへのポインタ

	for (int nCntMat = 0; nCntMat < MAXWALL; nCntMat++)
	{
		nNumVtx = g_pMeshWall->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshWall->GetFVF());

		//頂点バッファのロック
		g_pMeshWall->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;//頂点座標の代入

			if (vtx.x > g_PosWall[nCntMat].MaxStone.x)
			{
				g_PosWall[nCntMat].MaxStone.x = vtx.x;
			}

			if (vtx.x < g_PosWall[nCntMat].MinStone.x)
			{
				g_PosWall[nCntMat].MinStone.x = vtx.x;
			}

			if (vtx.y > g_PosWall[nCntMat].MaxStone.y)
			{
				g_PosWall[nCntMat].MaxStone.y = vtx.y;
			}

			if (vtx.y < g_PosWall[nCntMat].MinStone.y)
			{
				g_PosWall[nCntMat].MinStone.y = vtx.y;
			}

			if (vtx.z > g_PosWall[nCntMat].MaxStone.z)
			{
				g_PosWall[nCntMat].MaxStone.z = vtx.z;
			}

			if (vtx.z < g_PosWall[nCntMat].MinStone.z)
			{
				g_PosWall[nCntMat].MinStone.z = vtx.z;
			}

			pVtxBuff += sizeFVF;//頂点フォーマットのサイズ分ポインタを進める
		}
	}

	int nCnt = 0;
}
//=======================
//モデルの描画処理
//=======================
void DrawWall(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//マテリアルへのポインタ
	D3DXMATERIAL *pMat;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	float VecRoll = 0;

	for (int nCntMat = 0; nCntMat < MAXWALL; nCntMat++)
	{
		if (g_PosWall[nCntMat].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWolrdWall);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PosWall[nCntMat].rot.y, g_PosWall[nCntMat].rot.x, g_PosWall[nCntMat].rot.z);

			D3DXMatrixMultiply(&g_mtxWolrdWall, &g_mtxWolrdWall, &mtxRot);

			D3DXVec3TransformCoord(&g_PosWall[nCntMat].MaxStone, &g_PosWall[nCntMat].MaxStone, &g_mtxWolrdWall);
			D3DXVec3TransformCoord(&g_PosWall[nCntMat].MinStone, &g_PosWall[nCntMat].MinStone, &g_mtxWolrdWall);

			if (g_PosWall[nCntMat].MaxStone.x < g_PosWall[nCntMat].MinStone.x)
			{
				VecRoll = g_PosWall[nCntMat].MaxStone.x;
				g_PosWall[nCntMat].MaxStone.x = g_PosWall[nCntMat].MinStone.x;
				g_PosWall[nCntMat].MinStone.x = VecRoll;
			}

			else if (g_PosWall[nCntMat].MaxStone.z < g_PosWall[nCntMat].MinStone.z)
			{
				VecRoll = g_PosWall[nCntMat].MaxStone.z;
				g_PosWall[nCntMat].MaxStone.z = g_PosWall[nCntMat].MinStone.z;
				g_PosWall[nCntMat].MinStone.z = VecRoll;
			}

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_PosWall[nCntMat].Pos.x, g_PosWall[nCntMat].Pos.y, g_PosWall[nCntMat].Pos.z);

			D3DXMatrixMultiply(&g_mtxWolrdWall, &g_mtxWolrdWall, &mtxTrans);


			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWolrdWall);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCntMatVer = 0; nCntMatVer < (int)g_nNumMatWall; nCntMatVer++)
			{
				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_pBuffMatWall->GetBufferPointer();

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMatVer].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデルパーツの描画
				g_pMeshWall->DrawSubset(nCntMatVer);;
			}
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}
WALL *GetWall(void)
{
	return &g_PosWall[0];
}
//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVecWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	bool bLand = false;

	D3DXVECTOR3 pos = *pPos;// 判定対象の位置

	int g_nCntSt;
	int nCntStone;
	int nCnt = 0;

	g_nCntSt = 0;
	nCntStone = 0;

	for (int nCount = 0; nCount < MAXWALL; nCount++)
	{
		if (g_PosWall[nCount].bUse == true)
		{
			//aPosの値設定
			g_PosWall[nCnt].aPos[0 + (nCount * 4)] = D3DXVECTOR3(g_PosWall[nCnt].Pos.x + g_PosWall[nCnt].MinStone.x, 0.0f, g_PosWall[nCnt].Pos.z + g_PosWall[nCnt].MinStone.z);
			g_PosWall[nCnt].aPos[1 + (nCount * 4)] = D3DXVECTOR3(g_PosWall[nCnt].Pos.x + g_PosWall[nCnt].MinStone.x, 0.0f, g_PosWall[nCnt].Pos.z + g_PosWall[nCnt].MaxStone.z);
			g_PosWall[nCnt].aPos[2 + (nCount * 4)] = D3DXVECTOR3(g_PosWall[nCnt].Pos.x + g_PosWall[nCnt].MaxStone.x, 0.0f, g_PosWall[nCnt].Pos.z + g_PosWall[nCnt].MaxStone.z);
			g_PosWall[nCnt].aPos[3 + (nCount * 4)] = D3DXVECTOR3(g_PosWall[nCnt].Pos.x + g_PosWall[nCnt].MaxStone.x, 0.0f, g_PosWall[nCnt].Pos.z + g_PosWall[nCnt].MinStone.z);

			//外積の式
			g_PosWall[nCnt].Vec[0 + (nCount * 4)] = g_PosWall[nCnt].aPos[1 + (nCount * 4)] - g_PosWall[nCnt].aPos[0 + (nCount * 4)];
			g_PosWall[nCnt].Vec[1 + (nCount * 4)] = g_PosWall[nCnt].aPos[2 + (nCount * 4)] - g_PosWall[nCnt].aPos[1 + (nCount * 4)];
			g_PosWall[nCnt].Vec[2 + (nCount * 4)] = g_PosWall[nCnt].aPos[3 + (nCount * 4)] - g_PosWall[nCnt].aPos[2 + (nCount * 4)];
			g_PosWall[nCnt].Vec[3 + (nCount * 4)] = g_PosWall[nCnt].aPos[0 + (nCount * 4)] - g_PosWall[nCnt].aPos[3 + (nCount * 4)];

			nCnt += 1;
		}
	}

	for (int nCount = 0; nCount < MAXWALLVER; nCount++)
	{
		if (g_PosWall[nCntStone].bUse == true)
		{
			//外積の式2
			if (nCount == 0 + g_nCntSt)
			{
				g_PosWall[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - g_PosWall[nCntStone].aPos[nCount];
			}
			else if (nCount == 1 + g_nCntSt)
			{
				g_PosWall[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - g_PosWall[nCntStone].aPos[nCount];
			}
			else if (nCount == 2 + g_nCntSt)
			{
				g_PosWall[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - g_PosWall[nCntStone].aPos[nCount];
			}
			else if (nCount == 3 + g_nCntSt)
			{
				g_PosWall[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - g_PosWall[nCntStone].aPos[nCount];
			}

			g_PosWall[nCntStone].PlayerVec[nCount] = (g_PosWall[nCntStone].Vec[nCount].z * g_PosWall[nCntStone].g_Vec[nCount].x) - (g_PosWall[nCntStone].Vec[nCount].x * g_PosWall[nCntStone].g_Vec[nCount].z);

			if (pPos->y <= g_PosWall[nCntStone].Pos.y + g_PosWall[nCntStone].MaxStone.y - fHeightMin && pPos->y > g_PosWall[nCntStone].Pos.y + g_PosWall[nCntStone].MinStone.y - fHeightMax)
			{
				if (g_PosWall[nCntStone].PlayerVec[0 + (nCntStone * 4)] > 0.0f && g_PosWall[nCntStone].PlayerVec[1 + (nCntStone * 4)] > 0.0f && g_PosWall[nCntStone].PlayerVec[2 + (nCntStone * 4)] > 0.0f && g_PosWall[nCntStone].PlayerVec[3 + (nCntStone * 4)] > 0.0f)
				{
					if (pPos->x >= pPosOld->x && pPosOld->x < g_PosWall[nCntStone].Pos.x + g_PosWall[nCntStone].MinStone.x)
					{// 左側	
						pPos->x = g_PosWall[nCntStone].Pos.x + g_PosWall[nCntStone].MinStone.x - fWidthMax;
						//pMove->x = 0.0f;
					}
					else if (pPos->x <= pPosOld->x && pPosOld->x > g_PosWall[nCntStone].Pos.x + g_PosWall[nCntStone].MaxStone.x)
					{// 右側	
						pPos->x = g_PosWall[nCntStone].Pos.x + g_PosWall[nCntStone].MaxStone.x - fWidthMin;
						//pMove->x = 0.0f;
					}
					else if (pPos->z < pPosOld->z &&  pPosOld->z > g_PosWall[nCntStone].Pos.z + g_PosWall[nCntStone].MaxStone.z)
					{// 奥側
						pPos->z = g_PosWall[nCntStone].Pos.z + g_PosWall[nCntStone].MaxStone.z - fDepthMin;
					}
					else if (pPos->z > pPosOld->z && pPosOld->z < g_PosWall[nCntStone].Pos.z + g_PosWall[nCntStone].MinStone.z)
					{// 手前
						pPos->z = g_PosWall[nCntStone].Pos.z + g_PosWall[nCntStone].MinStone.z - fDepthMax;
					}
				}
			}

			if (nCount == 3 + (4 * nCntStone))
			{
				g_nCntSt += 4;
				nCntStone += 1;
			}
		}
	}

	//for (int nCount = 0; nCount < MAXSTONEVER; nCount++)
	//{
	//	if (g_PosStone[nCntStone].bUse == true)
	//	{
	//		//外積の式2
	//		if (nCount == 0 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - g_PosStone[0].aPos[nCount];
	//		}
	//		else if (nCount == 1 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - g_PosStone[0].aPos[nCount];
	//		}
	//		else if (nCount == 2 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - g_PosStone[0].aPos[nCount];
	//		}
	//		else if (nCount == 3 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - g_PosStone[0].aPos[nCount];
	//		}

	//		g_PosStone[0].PlayerVec[nCount] = (g_PosStone[0].Vec[nCount].z * g_PosStone[0].g_Vec[nCount].x) - (g_PosStone[0].Vec[nCount].x * g_PosStone[0].g_Vec[nCount].z);

	//		if (pPos->y <= g_PosStone[0].Pos.y + g_PosStone[0].MaxStone.y - fHeightMin && pPos->y > g_PosStone[0].Pos.y + g_PosStone[0].MinStone.y - fHeightMax)
	//		{
	//			if (g_PosStone[0].PlayerVec[0 + (0 * 4)] > 0.0f && g_PosStone[0].PlayerVec[1 + (0 * 4)] > 0.0f && g_PosStone[0].PlayerVec[2 + (0 * 4)] > 0.0f && g_PosStone[0].PlayerVec[3 + (0 * 4)] > 0.0f)
	//			{
	//				if (pPos->x > pPosOld->x && pPosOld->x < g_PosStone[0].Pos.x + g_PosStone[0].MinStone.x)
	//				{// 左側	
	//					pPos->x = g_PosStone[0].Pos.x + g_PosStone[0].MinStone.x - fWidthMax;
	//					//pMove->x = 0.0f;
	//				}
	//				else if (pPos->x < pPosOld->x && pPosOld->x > g_PosStone[0].Pos.x + g_PosStone[0].MaxStone.x)
	//				{// 右側	
	//					pPos->x = g_PosStone[0].Pos.x + g_PosStone[0].MaxStone.x - fWidthMin;
	//					//pMove->x = 0.0f;
	//				}
	//				else if (pPos->z <= pPosOld->z &&  pPos->z > g_PosStone[0].Pos.z)
	//				{// 奥側
	//					pPos->z = g_PosStone[0].Pos.z + g_PosStone[0].MaxStone.z - fDepthMin;
	//				}
	//				else if (pPos->z >= pPosOld->z && pPos->z < g_PosStone[0].Pos.z)
	//				{// 手前
	//					pPos->z = g_PosStone[0].Pos.z + g_PosStone[0].MinStone.z - fDepthMax;
	//				}
	//			}
	//		}

	//		if (nCount == 3 + (4 * 0))
	//		{
	//			g_nCntSt += 4;
	//			//nCntStone += 1;
	//		}
	//	}
	//}


	//for (int nCount = 0; nCount < MAXSTONEVER; nCount++)
	//{
	//	if (g_PosStone[0].nType == 0)
	//	{
	//		//外積の式2
	//		if (nCount == 0 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - g_PosStone[0].aPos[nCount];
	//		}
	//		else if (nCount == 1 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - g_PosStone[0].aPos[nCount];
	//		}
	//		else if (nCount == 2 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - g_PosStone[0].aPos[nCount];
	//		}
	//		else if (nCount == 3 + g_nCntSt)
	//		{
	//			g_PosStone[0].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - g_PosStone[0].aPos[nCount];
	//		}

	//		g_PosStone[0].PlayerVec[nCount] = (g_PosStone[0].Vec[nCount].z * g_PosStone[0].g_Vec[nCount].x) - (g_PosStone[0].Vec[nCount].x * g_PosStone[0].g_Vec[nCount].z);

	//		if (pPos->y <= g_PosStone[0].Pos.y + g_PosStone[0].MaxStone.y - fHeightMin && pPos->y > g_PosStone[0].Pos.y + g_PosStone[0].MinStone.y - fHeightMax)
	//		{
	//			if (g_PosStone[0].PlayerVec[0 + (nCount * 4)] > 0.0f && g_PosStone[0].PlayerVec[1 + (nCount * 4)] > 0.0f && g_PosStone[0].PlayerVec[2 + (nCount * 4)] > 0.0f && g_PosStone[0].PlayerVec[3 + (nCount * 4)] > 0.0f)
	//			{
	//				if (pPos->x > pPosOld->x && pPosOld->x < g_PosStone[0].Pos.x + g_PosStone[0].MinStone.x)
	//				{// 左側	
	//					pPos->x = g_PosStone[0].Pos.x + g_PosStone[0].MinStone.x - fWidthMax;
	//					//pMove->x = 0.0f;
	//				}
	//				else if (pPos->x < pPosOld->x && pPosOld->x > g_PosStone[0].Pos.x + g_PosStone[0].MaxStone.x)
	//				{// 右側	
	//					pPos->x = g_PosStone[0].Pos.x + g_PosStone[0].MaxStone.x - fWidthMin;
	//					//pMove->x = 0.0f;
	//				}
	//				else if (pPos->z <= pPosOld->z &&  pPos->z > g_PosStone[0].Pos.z)
	//				{// 奥側
	//					pPos->z = g_PosStone[0].Pos.z + g_PosStone[0].MaxStone.z - fDepthMin;
	//				}
	//				else if (pPos->z >= pPosOld->z && pPos->z < g_PosStone[0].Pos.z)
	//				{// 手前
	//					pPos->z = g_PosStone[0].Pos.z + g_PosStone[0].MinStone.z - fDepthMax;
	//				}
	//			}
	//		}

	//		if (nCount == 3 + (4 * nCntStone))
	//		{
	//			g_nCntSt += 4;
	//			//nCntStone += 1;
	//		}
	//	}

	//}
	//for (int nCount = 4; nCount < 8; nCount++)
	//{
	//	//外積の式2
	//	if (nCount == 4)
	//	{
	//		g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - aPos[nCount];
	//	}
	//	else if (nCount == 5)
	//	{
	//		g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - aPos[nCount];
	//	}
	//	else if (nCount == 6)
	//	{
	//		g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - aPos[nCount];
	//	}
	//	else if (nCount == 7)
	//	{
	//		g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - aPos[nCount];
	//	}

	//	PlayerVec[nCount] = (Vec[nCount].z * g_Vec[nCount].x) - (Vec[nCount].x * g_Vec[nCount].z);

	//	if (pPos->y <= g_PosStone[1].Pos.y + g_PosStone[1].MaxStone.y - fHeightMin && pPos->y > g_PosStone[1].Pos.y + g_PosStone[1].MinStone.y - fHeightMax)
	//	{
	//		if (PlayerVec[4] > 0.0f && PlayerVec[5] > 0.0f && PlayerVec[6] > 0.0f && PlayerVec[7] > 0.0f)
	//		{
	//			if (pPos->x > pPosOld->x && pPosOld->x < g_PosStone[1].Pos.x + g_PosStone[1].MinStone.x)
	//			{// 左側	
	//				pPos->x = g_PosStone[1].Pos.x + g_PosStone[1].MinStone.x - fWidthMax;
	//				//pMove->x = 0.0f;
	//			}
	//			else if (pPos->x < pPosOld->x && pPosOld->x > g_PosStone[1].Pos.x + g_PosStone[1].MaxStone.x)
	//			{// 右側	
	//				pPos->x = g_PosStone[1].Pos.x + g_PosStone[1].MaxStone.x - fWidthMin;
	//				//pMove->x = 0.0f;
	//			}
	//			else if (pPos->z <= pPosOld->z &&  pPos->z > g_PosStone[1].Pos.z)
	//			{// 奥側
	//				pPos->z = g_PosStone[1].Pos.z + g_PosStone[1].MaxStone.z - fDepthMin;
	//			}
	//			else if (pPos->z >= pPosOld->z && pPos->z < g_PosStone[1].Pos.z)
	//			{// 手前
	//				pPos->z = g_PosStone[1].Pos.z + g_PosStone[1].MinStone.z - fDepthMax;
	//			}
	//		}
	//	}
	//}

	return bLand;
}
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	BYTE *pVtxBuff;//頂点バッファへのポインタ

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

		//頂点バッファのロック
		g_pMeshWall->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntMat = 0; nCntMat < MAXWALL; nCntMat++)
		{
			if (g_PosWall[nCntMat].bUse == false)//弾を使用しているかどうか
			{
				if (nType == 0)
				{
					//位置設定
					g_PosWall[nCntMat].Pos = pos;

					//位置回転
					g_PosWall[nCntMat].rot = rot;

					//タイプ分け
					g_PosWall[nCntMat].nType = nType;

					//弾の使用
					g_PosWall[nCntMat].bUse = true;
					break;
				}
			}
		}

		//頂点バッファのアンロック
		g_pMeshWall->UnlockVertexBuffer();
}

