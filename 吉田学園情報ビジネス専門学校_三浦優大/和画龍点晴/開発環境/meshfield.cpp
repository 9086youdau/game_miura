//================================================
//
// メッシュフィールド処理[meshfield.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "meshfield.h"
#include "manager.h"
#include "Renderer.h"

CMeshfield::CMeshfield()
{
}

CMeshfield::~CMeshfield()
{
}

HRESULT CMeshfield::InitMeshfield(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/mesh.jpg",
		&m_pTextureMeshfield);

	// 変数の初期化
	m_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nAllPoint = DEPTHNUM * 2 * (WIDTHNUM + 2) - 2;				// 総頂点数
	m_nPolygon = WIDTHNUM * DEPTHNUM * 2 + (4 * (DEPTHNUM - 1));	// 総ポリゴン数
	m_nIdxPoint = m_nWidth * m_nDepth;								// インデックスバッファでの頂点数


	m_fWidthMax = m_fWidth * (float)WIDTHNUM;
	m_fDepthMax = m_fDepth * (float)DEPTHNUM;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nIdxPoint,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMeshfield,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepth; nCnt++)
	{// 奥行軸
		for (int nCntA = 0; nCntA < m_nWidth; nCntA++)
		{// 横軸
		 // ポリゴンの各頂点座標
			pVtx[0].pos = D3DXVECTOR3(-m_fWidthMax / 2.0f + (float)nCntA * m_fWidth, m_posMeshfield.y, m_fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 各頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ頂点情報の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * nCntA, (0.1f * nCnt) + m_meshcount);

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuffMeshfield->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nAllPoint,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffMeshfield,
		NULL);

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntA = 0; nCntA < DEPTHNUM; nCntA++)
	{
		for (int nCnt = 0; nCnt < m_nWidth + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == m_nWidth && nCntA != DEPTHNUM - 1)
			{// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == DEPTHNUM - 1 && nCnt == m_nWidth)
			{// 終了時に無視する
				break;
			}
			else
			{// 通常配置
				pIdx[0] = m_nWidth + (m_nWidth * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - m_nWidth;
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuffMeshfield->Unlock();

	return S_OK;
}

void CMeshfield::UninitMeshfield(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuffMeshfield != NULL)
	{
		m_pVtxBuffMeshfield->Release();
		m_pVtxBuffMeshfield = NULL;
	}

	// テクスチャの開放
	if (m_pTextureMeshfield != NULL)
	{
		m_pTextureMeshfield->Release();
		m_pTextureMeshfield = NULL;
	}

	// インデックスバッファの開放
	if (m_pIdxBuffMeshfield != NULL)
	{
		m_pIdxBuffMeshfield->Release();
		m_pIdxBuffMeshfield = NULL;
	}
}

void CMeshfield::UpdateMeshfield(void)
{
	VERTEX_3D *pVtx;

	m_meshcount -= 0.003f;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepth; nCnt++)
	{// 奥行軸
		for (int nCntA = 0; nCntA < m_nWidth; nCntA++)
		{// 横軸
		 // ポリゴンの各頂点座標
			pVtx[0].pos = D3DXVECTOR3(-m_fWidthMax / 2.0f + (float)nCntA * m_fWidth, m_posMeshfield.y, m_fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 各頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

			// テクスチャ頂点情報の設定
			pVtx[0].tex = D3DXVECTOR2(0.1f * nCntA, (0.1f * nCnt) + m_meshcount);

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuffMeshfield->Unlock();
}

void CMeshfield::DrawMeshfield(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;// 計算用マトリックス

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldMeshfield);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMeshfield.y, m_rotMeshfield.x, m_rotMeshfield.z);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_posMeshfield.x, m_posMeshfield.y, m_posMeshfield.z);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldMeshfield);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffMeshfield);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTextureMeshfield);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nIdxPoint,			// 頂点数
		0,
		m_nPolygon);			// プリミティブ数
}
