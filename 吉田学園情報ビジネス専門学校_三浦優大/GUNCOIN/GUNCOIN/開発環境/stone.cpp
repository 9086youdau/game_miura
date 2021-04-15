//================================================
//
//石の処理[stone.h]
// Author; miura yuudai
//
//================================================

#include "stone.h"
#include "input.h"
#include "camera.h"
#include "Billboard.h"
#include "bullet.h"
#include "model.h"

//=====================
//グローバル変数
//=====================
//メッシュ（頂点情報）へのポインタ
LPD3DXMESH g_pMeshStone[4];

//マテリアル（材質情報）へのポインタ
LPD3DXBUFFER g_pBuffMatStone[4];

DWORD g_nNumMatStone[4];//マテリアルの数
STONE g_PosStone[MAXSTONE];//xファイル
D3DXMATRIX g_mtxWolrdStone;//ワールドマトリックス

//====================
//モデルの初期化処理
//====================
void InitStone(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/kusa2.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatStone[0], NULL, &g_nNumMatStone[0], &g_pMeshStone[0]);
	D3DXLoadMeshFromX("data/MODEL/sekizou.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatStone[1], NULL, &g_nNumMatStone[1], &g_pMeshStone[1]);
	D3DXLoadMeshFromX("data/MODEL/kusa.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatStone[2], NULL, &g_nNumMatStone[2], &g_pMeshStone[2]);
	D3DXLoadMeshFromX("data/MODEL/okimono.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatStone[3], NULL, &g_nNumMatStone[3], &g_pMeshStone[3]);


	for (int nCntMat = 0; nCntMat < MAXSTONE; nCntMat++)
	{
		//変数初期化
		g_PosStone[nCntMat].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosStone[nCntMat].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosStone[nCntMat].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosStone[nCntMat].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosStone[nCntMat].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosStone[nCntMat].MaxStone = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_PosStone[nCntMat].MinStone = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_PosStone[nCntMat].bUse = false;
		g_PosStone[nCntMat].nType = 0;

	}
}
//=======================
//モデルの終了処理
//=======================
void UninitStone(void)
{
	for (int nCntMatVer = 0; nCntMatVer < 4; nCntMatVer++)
	{
		//頂点の開放
		if (g_pMeshStone[nCntMatVer] != NULL)
		{
			g_pMeshStone[nCntMatVer]->Release();
			g_pMeshStone[nCntMatVer] = NULL;
		}

		//材質の開放
		if (g_pBuffMatStone[nCntMatVer] != NULL)
		{
			g_pBuffMatStone[nCntMatVer]->Release();
			g_pBuffMatStone[nCntMatVer] = NULL;
		}

		//テクスチャの開放
		if (g_PosStone[0].g_apTextureStone[0] != NULL)
		{
			g_PosStone[0].g_apTextureStone[0]->Release();
			g_PosStone[0].g_apTextureStone[0] = NULL;
		}
	}
}
//=======================
//モデルの更新処理
//=======================
void UpdateStone(void)
{

	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE *pVtxBuff;//頂点バッファへのポインタ

	for (int nCntMat = 0; nCntMat < MAXSTONE; nCntMat++)
	{
		nNumVtx = g_pMeshStone[g_PosStone[nCntMat].nType]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshStone[g_PosStone[nCntMat].nType]->GetFVF());

		//頂点バッファのロック
		g_pMeshStone[g_PosStone[nCntMat].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;//頂点座標の代入

			if (vtx.x > g_PosStone[nCntMat].MaxStone.x)
			{
				g_PosStone[nCntMat].MaxStone.x = vtx.x;
			}

			if (vtx.x < g_PosStone[nCntMat].MinStone.x)
			{
				g_PosStone[nCntMat].MinStone.x = vtx.x;
			}

			if (vtx.y > g_PosStone[nCntMat].MaxStone.y)
			{
				g_PosStone[nCntMat].MaxStone.y = vtx.y;
			}

			if (vtx.y < g_PosStone[nCntMat].MinStone.y)
			{
				g_PosStone[nCntMat].MinStone.y = vtx.y;
			}

			if (vtx.z > g_PosStone[nCntMat].MaxStone.z)
			{
				g_PosStone[nCntMat].MaxStone.z = vtx.z;
			}

			if (vtx.z < g_PosStone[nCntMat].MinStone.z)
			{
				g_PosStone[nCntMat].MinStone.z = vtx.z;
			}

			pVtxBuff += sizeFVF;//頂点フォーマットのサイズ分ポインタを進める
		}

		g_pMeshStone[g_PosStone[nCntMat].nType]->UnlockVertexBuffer();
	}
}
//=======================
//モデルの描画処理
//=======================
void DrawStone(void)
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

	for (int nCntMat = 0; nCntMat < MAXSTONE; nCntMat++)
	{
		for (int nCntMatVer = 0; nCntMatVer < 4; nCntMatVer++)
		{
			if (g_PosStone[nCntMat].bUse == true)
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWolrdStone);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PosStone[nCntMat].rot.y, g_PosStone[nCntMat].rot.x, g_PosStone[nCntMat].rot.z);

				D3DXMatrixMultiply(&g_mtxWolrdStone, &g_mtxWolrdStone, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_PosStone[nCntMat].Pos.x, g_PosStone[nCntMat].Pos.y, g_PosStone[nCntMat].Pos.z);

				D3DXMatrixMultiply(&g_mtxWolrdStone, &g_mtxWolrdStone, &mtxTrans);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWolrdStone);

				//現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_pBuffMatStone[nCntMatVer]->GetBufferPointer();

				for (int nCntMatVer = 0; nCntMatVer < (int)g_nNumMatStone; nCntMatVer++)
				{
					if (g_PosStone[nCntMat].nType == 0)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMatVer].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL);

						//モデルパーツの描画
						g_pMeshStone[0]->DrawSubset(nCntMatVer);
						break;
					}

					else if (g_PosStone[nCntMat].nType == 1)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMatVer].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL);

						//モデルパーツの描画
						g_pMeshStone[1]->DrawSubset(nCntMatVer);
						break;
					}

					else if (g_PosStone[nCntMat].nType == 2)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMatVer].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL);

						//モデルパーツの描画
						g_pMeshStone[2]->DrawSubset(nCntMatVer);
						break;
					}

					else if (g_PosStone[nCntMat].nType == 3)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMatVer].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL);

						//モデルパーツの描画
						g_pMeshStone[3]->DrawSubset(nCntMatVer);
						break;
					}
				}

			}
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
STONE *GetStone(void)
{
	return &g_PosStone[0];
}
//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	bool bLand = false;

	D3DXVECTOR3 pos = *pPos;// 判定対象の位置

	int g_nCntSt;
	int nCntStone;
	int nCnt = 0;

	g_nCntSt = 0;
	nCntStone = 0;

	for (int nCount = 0; nCount < MAXSTONE; nCount++)
	{
		if (g_PosStone[nCount].bUse == true)
		{
			//aPosの値設定
			g_PosStone[nCount].aPos[0 + (nCount * 4)] = D3DXVECTOR3(g_PosStone[nCount].Pos.x + g_PosStone[nCount].MinStone.x, 0.0f, g_PosStone[nCount].Pos.z + g_PosStone[nCount].MinStone.z);
			g_PosStone[nCount].aPos[1 + (nCount * 4)] = D3DXVECTOR3(g_PosStone[nCount].Pos.x + g_PosStone[nCount].MinStone.x, 0.0f, g_PosStone[nCount].Pos.z + g_PosStone[nCount].MaxStone.z);
			g_PosStone[nCount].aPos[2 + (nCount * 4)] = D3DXVECTOR3(g_PosStone[nCount].Pos.x + g_PosStone[nCount].MaxStone.x, 0.0f, g_PosStone[nCount].Pos.z + g_PosStone[nCount].MaxStone.z);
			g_PosStone[nCount].aPos[3 + (nCount * 4)] = D3DXVECTOR3(g_PosStone[nCount].Pos.x + g_PosStone[nCount].MaxStone.x, 0.0f, g_PosStone[nCount].Pos.z + g_PosStone[nCount].MinStone.z);

			//外積の式
			g_PosStone[nCount].Vec[0 + (nCount * 4)] = g_PosStone[nCount].aPos[1 + (nCount * 4)] - g_PosStone[nCount].aPos[0 + (nCount * 4)];
			g_PosStone[nCount].Vec[1 + (nCount * 4)] = g_PosStone[nCount].aPos[2 + (nCount * 4)] - g_PosStone[nCount].aPos[1 + (nCount * 4)];
			g_PosStone[nCount].Vec[2 + (nCount * 4)] = g_PosStone[nCount].aPos[3 + (nCount * 4)] - g_PosStone[nCount].aPos[2 + (nCount * 4)];
			g_PosStone[nCount].Vec[3 + (nCount * 4)] = g_PosStone[nCount].aPos[0 + (nCount * 4)] - g_PosStone[nCount].aPos[3 + (nCount * 4)];
		}
	}

	for (int nCount = 0; nCount < MAXSTONEVER; nCount++)
	{
		if (g_PosStone[nCntStone].bUse == true)
		{
			//外積の式2
			if (nCount == 0 + g_nCntSt)
			{
				g_PosStone[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - g_PosStone[nCntStone].aPos[nCount];
			}
			else if (nCount == 1 + g_nCntSt)
			{
				g_PosStone[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - g_PosStone[nCntStone].aPos[nCount];
			}
			else if (nCount == 2 + g_nCntSt)
			{
				g_PosStone[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - g_PosStone[nCntStone].aPos[nCount];
			}
			else if (nCount == 3 + g_nCntSt)
			{
				g_PosStone[nCntStone].g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - g_PosStone[nCntStone].aPos[nCount];
			}

			g_PosStone[nCntStone].PlayerVec[nCount] = (g_PosStone[nCntStone].Vec[nCount].z * g_PosStone[nCntStone].g_Vec[nCount].x) - (g_PosStone[nCntStone].Vec[nCount].x * g_PosStone[nCntStone].g_Vec[nCount].z);

			if (pPos->y <= g_PosStone[nCntStone].Pos.y + g_PosStone[nCntStone].MaxStone.y - fHeightMin && pPos->y > g_PosStone[nCntStone].Pos.y + g_PosStone[nCntStone].MinStone.y - fHeightMax)
			{
				if (g_PosStone[nCntStone].PlayerVec[0 + (nCntStone * 4)] > 0.0f && g_PosStone[nCntStone].PlayerVec[1 + (nCntStone * 4)] > 0.0f && g_PosStone[nCntStone].PlayerVec[2 + (nCntStone * 4)] > 0.0f && g_PosStone[nCntStone].PlayerVec[3 + (nCntStone * 4)] > 0.0f)
				{
					if (pPos->x >= pPosOld->x && pPosOld->x < g_PosStone[nCntStone].Pos.x + g_PosStone[nCntStone].MinStone.x)
					{// 左側	
						pPos->x = g_PosStone[nCntStone].Pos.x + g_PosStone[nCntStone].MinStone.x - fWidthMax;
						//pMove->x = 0.0f;
					}
					else if (pPos->x <= pPosOld->x && pPosOld->x > g_PosStone[nCntStone].Pos.x + g_PosStone[nCntStone].MaxStone.x)
					{// 右側	
						pPos->x = g_PosStone[nCntStone].Pos.x + g_PosStone[nCntStone].MaxStone.x - fWidthMin;
						//pMove->x = 0.0f;
					}
					else if (pPos->z <= pPosOld->z &&  pPos->z > g_PosStone[nCntStone].Pos.z)
					{// 奥側
						pPos->z = g_PosStone[nCntStone].Pos.z + g_PosStone[nCntStone].MaxStone.z - fDepthMin;
					}
					else if (pPos->z >= pPosOld->z && pPos->z < g_PosStone[nCntStone].Pos.z)
					{// 手前
						pPos->z = g_PosStone[nCntStone].Pos.z + g_PosStone[nCntStone].MinStone.z - fDepthMax;
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

		return bLand;
}
void SetStone(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	BYTE *pVtxBuff;//頂点バッファへのポインタ

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntMat = 0; nCntMat < MAXSTONE; nCntMat++)
	{
		//頂点バッファのロック
		g_pMeshStone[g_PosStone[nCntMat].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		if (g_PosStone[nCntMat].bUse == false)//弾を使用しているかどうか
		{
			if (nType == 0)
			{
				//位置設定
				g_PosStone[nCntMat].Pos = pos;

				//位置回転
				g_PosStone[nCntMat].rot = rot;

				//タイプ分け
				g_PosStone[nCntMat].nType = nType;

				//弾の使用
				g_PosStone[nCntMat].bUse = true;
				break;
			}

			else if (nType == 1)
			{
				//位置設定
				g_PosStone[nCntMat].Pos = pos;

				//位置回転
				g_PosStone[nCntMat].rot = rot;

				//タイプ分け
				g_PosStone[nCntMat].nType = nType;

				//弾の使用
				g_PosStone[nCntMat].bUse = true;
				break;
			}

			else if (nType == 2)
			{
				//位置設定
				g_PosStone[nCntMat].Pos = pos;

				//位置回転
				g_PosStone[nCntMat].rot = rot;

				//タイプ分け
				g_PosStone[nCntMat].nType = nType;

				//弾の使用
				g_PosStone[nCntMat].bUse = true;
				break;
			}

			else if (nType == 3)
			{
				//位置設定
				g_PosStone[nCntMat].Pos = pos;

				//位置回転
				g_PosStone[nCntMat].rot = rot;

				//タイプ分け
				g_PosStone[nCntMat].nType = nType;

				//弾の使用
				g_PosStone[nCntMat].bUse = true;
				break;
			}
		}

		//頂点バッファのアンロック
		g_pMeshStone[g_PosStone[nCntMat].nType]->UnlockVertexBuffer();
	}
}

