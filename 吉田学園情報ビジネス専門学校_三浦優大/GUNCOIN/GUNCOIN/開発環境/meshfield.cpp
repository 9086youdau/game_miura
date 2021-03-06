//================================================
//
//プレイヤー処理[Meshfeild.h]
// Author; miura yuudai
//
//================================================
#include "meshfield.h"
#include "input.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define WIDTHNUM	(150)		// 列数(幅)
#define DEPTHNUM	(150)		// 行数(奥行)
#define WIDTH		(40.0f)	// 幅の大きさ
#define DEPTH		(40.0f)	// 奥行の大きさ
#define FIELD_MAX	(32)		// フィールドの最大数

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;	// バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;	// インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshfield;							// 位置
D3DXVECTOR3 g_rotMeshfield;							// 向き
D3DXMATRIX g_mtxWorldMeshfield;						// ワールドマトリックス
MeshField g_mesh;
int g_nAllPoint;									// 総頂点数
int g_nPolygon;										// ポリゴン数
int g_nIdxPoint;									// インデックスバッファの必要な確保数
float g_fWidth = WIDTH;				// 横幅の端
float g_fDepth = DEPTH;				// 奥行の端
int g_nWidth = WIDTHNUM + 1;		// 横幅の頂点数
int g_nDepth = DEPTHNUM + 1;		// 奥行の頂点数

//MeshField g_aField[FIELD_MAX];	// フィールドの構造体
//D3DXVECTOR3 g_movePolygon;
//D3DXVECTOR3 g_moveRot;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitMeshfield(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Wall3.png",
		&g_mesh.g_pTextureMeshfield);

	// 変数の初期化
	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nAllPoint = DEPTHNUM * 2 * (WIDTHNUM + 2) - 2;				// 総頂点数
	g_nPolygon = WIDTHNUM * DEPTHNUM * 2 + (4 * (DEPTHNUM - 1));	// 総ポリゴン数
	g_nIdxPoint = g_nWidth * g_nDepth;								// インデックスバッファでの頂点数


	float fWidthMax = g_fWidth * (float)WIDTHNUM;
	float fDepthMax = g_fDepth * (float)DEPTHNUM;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nIdxPoint,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_nDepth; nCnt++)
	{// 奥行軸
		for (int nCntA = 0; nCntA < g_nWidth; nCntA++)
		{// 横軸
		 // ポリゴンの各頂点座標
			pVtx[0].pos = D3DXVECTOR3(-fWidthMax / 2.0f + (float)nCntA * g_fWidth, g_posMeshfield.y, fDepthMax / 2.0f - (float)nCnt * g_fDepth);
			//pVtx[0].pos = D3DXVECTOR3(-g_fWidth + (float)nCntA * fWidthPart, g_posMeshfield.y, g_fDepth - (float)nCnt * fDepthPart);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 各頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ頂点情報の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * nCntA, 0.1f * nCnt);

			pVtx++;
		}
	}

	// ポリゴンの各頂点座標
	//pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	//pVtx[2].pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	//pVtx[3].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
	//pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[5].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	//pVtx[6].pos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	//pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	//pVtx[8].pos = D3DXVECTOR3(100.0f, 0.0f, -100.0f);
	//pVtx[9].pos = D3DXVECTOR3(g_posMeshfield.x - 100.0f, 0.0f, 0.0f);
	//pVtx[0].pos = D3DXVECTOR3(g_posMeshfield.x - 100.0f, 0.0f, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(g_posMeshfield.x - 100.0f, 0.0f, g_posMeshfield.z + 100.0f);
	//pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, g_posMeshfield.z + 100.0f);
	//pVtx[4].pos = D3DXVECTOR3(g_posMeshfield.x + 100.0f, 0.0f, 0.0f);
	//pVtx[5].pos = D3DXVECTOR3(g_posMeshfield.x + 100.0f, 0.0f, g_posMeshfield.z + 100.0f);
	//pVtx[6].pos = D3DXVECTOR3(g_posMeshfield.x + 100.0f, 0.0f, g_posMeshfield.z + 100.0f);
	//pVtx[7].pos = D3DXVECTOR3(g_posMeshfield.x - 100.0f, 0.0f, g_posMeshfield.z - 100.0f);
	//pVtx[8].pos = D3DXVECTOR3(g_posMeshfield.x - 100.0f, 0.0f, g_posMeshfield.z - 100.0f);
	//pVtx[9].pos = D3DXVECTOR3(g_posMeshfield.x - 100.0f, 0.0f, 0.0f);
	//pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, g_posMeshfield.z - 100.0f);
	//pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[12].pos = D3DXVECTOR3(g_posMeshfield.x + 100.0f, 0.0f, g_posMeshfield.z - 100.0f);
	//pVtx[13].pos = D3DXVECTOR3(g_posMeshfield.x + 100.0f, 0.0f, 0.0f);

	// 法線ベクトルの設定
	//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 各頂点カラーの設定
	//pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ頂点情報の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(2.0f, 0.0f);
	//pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[4].tex = D3DXVECTOR2(1.0f, 1.0f);
	//pVtx[5].tex = D3DXVECTOR2(2.0f, 1.0f);
	//pVtx[6].tex = D3DXVECTOR2(0.0f, 2.0f);
	//pVtx[7].tex = D3DXVECTOR2(1.0f, 2.0f);
	//pVtx[8].tex = D3DXVECTOR2(2.0f, 2.0f);
	//pVtx[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[10].tex = D3DXVECTOR2(1.0f, 2.0f);
	//pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);
	//pVtx[12].tex = D3DXVECTOR2(2.0f, 2.0f);
	//pVtx[13].tex = D3DXVECTOR2(2.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * g_nAllPoint,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield,
		NULL);

	// インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntA = 0; nCntA < DEPTHNUM; nCntA++)
	{
		for (int nCnt = 0; nCnt < g_nWidth + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == g_nWidth && nCntA != DEPTHNUM - 1)
			{// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == DEPTHNUM - 1 && nCnt == g_nWidth)
			{// 終了時に無視する
				break;
			}
			else
			{// 通常配置
				pIdx[0] = g_nWidth + (g_nWidth * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - g_nWidth;
			}
		}
	}

	////pIdx[0] = 3;
	////pIdx[1] = 0;
	////pIdx[2] = 4;
	////pIdx[3] = 1;
	////pIdx[4] = 5;
	////pIdx[5] = 2;
	////pIdx[6] = 2;
	////pIdx[7] = 6;
	////pIdx[8] = 6;
	////pIdx[9] = 3;
	////pIdx[10] = 7;
	////pIdx[11] = 4;
	////pIdx[12] = 8;
	////pIdx[13] = 5;

	// インデックスバッファをアンロックする
	g_pIdxBuffMeshfield->Unlock();

	//// インデックスバッファをロックし、番号データへのポインタを取得
	//g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);
	//int a[14];
	//for (int nCnt = 0; nCnt < 14; nCnt++,pIdx++)
	//{
	//	a[nCnt] = pIdx[0];
	//}
	//// インデックスバッファをアンロックする
	//g_pIdxBuffMeshfield->Unlock();
	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitMeshfield(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	// テクスチャの開放
	if (g_mesh.g_pTextureMeshfield != NULL)
	{
		g_mesh.g_pTextureMeshfield->Release();
		g_mesh.g_pTextureMeshfield = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateMeshfield(void)
{
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawMeshfield(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshfield);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_mesh.g_pTextureMeshfield);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		g_nIdxPoint,			// 頂点数
		0,
		g_nPolygon);			// プリミティブ数

}
