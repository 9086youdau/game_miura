//================================================
//
//オブジェクトの処理[object.h]
// Author; miura yuudai
//
//================================================

//================================================
//インクルード読み込み
//================================================
#include "object.h"
#include "input.h"
#include "camera.h"
#include "Billboard.h"
#include "bullet.h"
#include "model.h"

//=====================
//グローバル変数
//=====================
//メッシュ（頂点情報）へのポインタ
LPD3DXMESH g_pMeshObject[MAX_OBJECT_TYPE];
//マテリアル（材質情報）へのポインタ
LPD3DXBUFFER g_pBuffMatObject[MAX_OBJECT_TYPE];
DWORD g_nNumMatObject[MAX_OBJECT_TYPE];//マテリアルの数
OBJECT g_PosObject[MAXOBJECT];//xファイル
D3DXMATRIX g_mtxWolrdObject;//ワールドマトリックス

//======================================
//オブジェクトの初期化処理
//======================================
void InitObject(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/kusa2.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[0], NULL, &g_nNumMatObject[0], &g_pMeshObject[0]);
	D3DXLoadMeshFromX("data/MODEL/okimono.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatObject[1], NULL, &g_nNumMatObject[1], &g_pMeshObject[1]);

	for (int nCntMat = 0; nCntMat < MAXOBJECT; nCntMat++)
	{
		//変数初期化
		g_PosObject[nCntMat].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosObject[nCntMat].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosObject[nCntMat].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosObject[nCntMat].rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosObject[nCntMat].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PosObject[nCntMat].MaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_PosObject[nCntMat].MinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_PosObject[nCntMat].bUse = false;
		g_PosObject[nCntMat].nType = 0;
	}
}
//======================================
//オブジェクトの終了処理
//======================================
void UninitObject(void)
{
	for (int nCntMatVer = 0; nCntMatVer < MAX_OBJECT_TYPE; nCntMatVer++)
	{
		//頂点の開放
		if (g_pMeshObject[nCntMatVer] != NULL)
		{
			g_pMeshObject[nCntMatVer]->Release();
			g_pMeshObject[nCntMatVer] = NULL;
		}

		//材質の開放
		if (g_pBuffMatObject[nCntMatVer] != NULL)
		{
			g_pBuffMatObject[nCntMatVer]->Release();
			g_pBuffMatObject[nCntMatVer] = NULL;
		}
	}
}
//======================================
//オブジェクトの更新処理
//======================================
void UpdateObject(void)
{
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE *pVtxBuff;//頂点バッファへのポインタ

	for (int nCntMat = 0; nCntMat < MAXOBJECT; nCntMat++)
	{
		nNumVtx = g_pMeshObject[g_PosObject[nCntMat].nType]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshObject[g_PosObject[nCntMat].nType]->GetFVF());

		//頂点バッファのロック
		g_pMeshObject[g_PosObject[nCntMat].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;//頂点座標の代入

			if (vtx.x > g_PosObject[nCntMat].MaxObject.x)
			{
				g_PosObject[nCntMat].MaxObject.x = vtx.x;
			}

			if (vtx.x < g_PosObject[nCntMat].MinObject.x)
			{
				g_PosObject[nCntMat].MinObject.x = vtx.x;
			}

			if (vtx.y > g_PosObject[nCntMat].MaxObject.y)
			{
				g_PosObject[nCntMat].MaxObject.y = vtx.y;
			}

			if (vtx.y < g_PosObject[nCntMat].MinObject.y)
			{
				g_PosObject[nCntMat].MinObject.y = vtx.y;
			}

			if (vtx.z > g_PosObject[nCntMat].MaxObject.z)
			{
				g_PosObject[nCntMat].MaxObject.z = vtx.z;
			}

			if (vtx.z < g_PosObject[nCntMat].MinObject.z)
			{
				g_PosObject[nCntMat].MinObject.z = vtx.z;
			}

			pVtxBuff += sizeFVF;//頂点フォーマットのサイズ分ポインタを進める
		}

		g_pMeshObject[g_PosObject[nCntMat].nType]->UnlockVertexBuffer();
	}
}
//======================================
//オブジェクトの描画処理
//======================================
void DrawObject(void)
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

	for (int nCntMat = 0; nCntMat < MAXOBJECT; nCntMat++)
	{
		for (int nCntMatVer = 0; nCntMatVer < MAX_OBJECT_TYPE; nCntMatVer++)
		{
			if (g_PosObject[nCntMat].bUse == true)
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWolrdObject);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_PosObject[nCntMat].rot.y, g_PosObject[nCntMat].rot.x, g_PosObject[nCntMat].rot.z);

				D3DXMatrixMultiply(&g_mtxWolrdObject, &g_mtxWolrdObject, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_PosObject[nCntMat].Pos.x, g_PosObject[nCntMat].Pos.y, g_PosObject[nCntMat].Pos.z);

				D3DXMatrixMultiply(&g_mtxWolrdObject, &g_mtxWolrdObject, &mtxTrans);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWolrdObject);

				//現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_pBuffMatObject[nCntMatVer]->GetBufferPointer();

				for (int nCntMatVer = 0; nCntMatVer < (int)g_nNumMatObject; nCntMatVer++)
				{
					if (g_PosObject[nCntMat].nType == 0)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMatVer].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL);

						//モデルパーツの描画
						g_pMeshObject[0]->DrawSubset(nCntMatVer);
						break;
					}

					else if (g_PosObject[nCntMat].nType == 1)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMatVer].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, NULL);

						//モデルパーツの描画
						g_pMeshObject[1]->DrawSubset(nCntMatVer);
						break;
					}
				}

			}
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//======================================
//オブジェクトの受け渡し処理
//======================================
OBJECT *GetObject(void)
{
	return &g_PosObject[0];
}
//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CollisionVecObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	bool bLand = false;

	D3DXVECTOR3 pos = *pPos;// 判定対象の位置

	int g_nCntSt;
	int nCntObject;
	int nCnt = 0;

	g_nCntSt = 0;
	nCntObject = 0;

	for (int nCount = 0; nCount < MAXOBJECT; nCount++)
	{
		if (g_PosObject[nCount].bUse == true)
		{
			//aPosの値設定
			g_PosObject[nCount].aPosObject[0 + (nCount * 4)] = D3DXVECTOR3(g_PosObject[nCount].Pos.x + g_PosObject[nCount].MinObject.x, 0.0f, g_PosObject[nCount].Pos.z + g_PosObject[nCount].MinObject.z);
			g_PosObject[nCount].aPosObject[1 + (nCount * 4)] = D3DXVECTOR3(g_PosObject[nCount].Pos.x + g_PosObject[nCount].MinObject.x, 0.0f, g_PosObject[nCount].Pos.z + g_PosObject[nCount].MaxObject.z);
			g_PosObject[nCount].aPosObject[2 + (nCount * 4)] = D3DXVECTOR3(g_PosObject[nCount].Pos.x + g_PosObject[nCount].MaxObject.x, 0.0f, g_PosObject[nCount].Pos.z + g_PosObject[nCount].MaxObject.z);
			g_PosObject[nCount].aPosObject[3 + (nCount * 4)] = D3DXVECTOR3(g_PosObject[nCount].Pos.x + g_PosObject[nCount].MaxObject.x, 0.0f, g_PosObject[nCount].Pos.z + g_PosObject[nCount].MinObject.z);

			//外積の式
			g_PosObject[nCount].VecObject[0 + (nCount * 4)] = g_PosObject[nCount].aPosObject[1 + (nCount * 4)] - g_PosObject[nCount].aPosObject[0 + (nCount * 4)];
			g_PosObject[nCount].VecObject[1 + (nCount * 4)] = g_PosObject[nCount].aPosObject[2 + (nCount * 4)] - g_PosObject[nCount].aPosObject[1 + (nCount * 4)];
			g_PosObject[nCount].VecObject[2 + (nCount * 4)] = g_PosObject[nCount].aPosObject[3 + (nCount * 4)] - g_PosObject[nCount].aPosObject[2 + (nCount * 4)];
			g_PosObject[nCount].VecObject[3 + (nCount * 4)] = g_PosObject[nCount].aPosObject[0 + (nCount * 4)] - g_PosObject[nCount].aPosObject[3 + (nCount * 4)];
		}
	}

	for (int nCount = 0; nCount < MAXOBJECTVER; nCount++)
	{
		if (g_PosObject[nCntObject].bUse == true)
		{
			//外積の式2
			if (nCount == 0 + g_nCntSt)
			{
				g_PosObject[nCntObject].aVecObject[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - g_PosObject[nCntObject].aPosObject[nCount];
			}
			else if (nCount == 1 + g_nCntSt)
			{
				g_PosObject[nCntObject].aVecObject[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - g_PosObject[nCntObject].aPosObject[nCount];
			}
			else if (nCount == 2 + g_nCntSt)
			{
				g_PosObject[nCntObject].aVecObject[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - g_PosObject[nCntObject].aPosObject[nCount];
			}
			else if (nCount == 3 + g_nCntSt)
			{
				g_PosObject[nCntObject].aVecObject[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - g_PosObject[nCntObject].aPosObject[nCount];
			}

			g_PosObject[nCntObject].ObjectVec[nCount] = (g_PosObject[nCntObject].VecObject[nCount].z * g_PosObject[nCntObject].aVecObject[nCount].x) - (g_PosObject[nCntObject].VecObject[nCount].x * g_PosObject[nCntObject].aVecObject[nCount].z);

			if (pPos->y <= g_PosObject[nCntObject].Pos.y + g_PosObject[nCntObject].MaxObject.y - fHeightMin && pPos->y > g_PosObject[nCntObject].Pos.y + g_PosObject[nCntObject].MinObject.y - fHeightMax)
			{
				if (g_PosObject[nCntObject].ObjectVec[0 + (nCntObject * 4)] > 0.0f && g_PosObject[nCntObject].ObjectVec[1 + (nCntObject * 4)] > 0.0f && g_PosObject[nCntObject].ObjectVec[2 + (nCntObject * 4)] > 0.0f && g_PosObject[nCntObject].ObjectVec[3 + (nCntObject * 4)] > 0.0f)
				{
					if (pPos->x >= pPosOld->x && pPosOld->x < g_PosObject[nCntObject].Pos.x + g_PosObject[nCntObject].MinObject.x)
					{// 左側	
						pPos->x = g_PosObject[nCntObject].Pos.x + g_PosObject[nCntObject].MinObject.x - fWidthMax;
						//pMove->x = 0.0f;
					}
					else if (pPos->x <= pPosOld->x && pPosOld->x > g_PosObject[nCntObject].Pos.x + g_PosObject[nCntObject].MaxObject.x)
					{// 右側	
						pPos->x = g_PosObject[nCntObject].Pos.x + g_PosObject[nCntObject].MaxObject.x - fWidthMin;
						//pMove->x = 0.0f;
					}
					else if (pPos->z <= pPosOld->z &&  pPos->z > g_PosObject[nCntObject].Pos.z)
					{// 奥側
						pPos->z = g_PosObject[nCntObject].Pos.z + g_PosObject[nCntObject].MaxObject.z - fDepthMin;
					}
					else if (pPos->z >= pPosOld->z && pPos->z < g_PosObject[nCntObject].Pos.z)
					{// 手前
						pPos->z = g_PosObject[nCntObject].Pos.z + g_PosObject[nCntObject].MinObject.z - fDepthMax;
					}
				}
			}

			if (nCount == 3 + (4 * nCntObject))
			{
				g_nCntSt += 4;
				nCntObject += 1;
			}
		}
	}

		return bLand;
}
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	BYTE *pVtxBuff;//頂点バッファへのポインタ

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntMat = 0; nCntMat < MAXOBJECT; nCntMat++)
	{
		//頂点バッファのロック
		g_pMeshObject[g_PosObject[nCntMat].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		if (g_PosObject[nCntMat].bUse == false)//弾を使用しているかどうか
		{
			if (nType == 0)
			{
				//位置設定
				g_PosObject[nCntMat].Pos = pos;

				//位置回転
				g_PosObject[nCntMat].rot = rot;

				//タイプ分け
				g_PosObject[nCntMat].nType = nType;

				//弾の使用
				g_PosObject[nCntMat].bUse = true;
				break;
			}

			else if (nType == 1)
			{
				//位置設定
				g_PosObject[nCntMat].Pos = pos;

				//位置回転
				g_PosObject[nCntMat].rot = rot;

				//タイプ分け
				g_PosObject[nCntMat].nType = nType;

				//弾の使用
				g_PosObject[nCntMat].bUse = true;
				break;
			}
		}

		//頂点バッファのアンロック
		g_pMeshObject[g_PosObject[nCntMat].nType]->UnlockVertexBuffer();
	}
}

