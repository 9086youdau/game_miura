//================================================
//
// メッシュウォール処理[meshwall.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//マクロ定義
//=================================================
#define WALL_POINT_MAX	(65535)		// 頂点数の最大
#define WALL_IDX_MAX	(65535)		// インデックスバッファの最大確保数

//=================================================
//インクルード読み込み
//=================================================
#include "meshwall.h"
#include "manager.h"
#include "Renderer.h"

//=================================================
//コンストラクタ
//=================================================
CMeshwall::CMeshwall()
{
}

//=================================================
//デストラクタ
//=================================================
CMeshwall::~CMeshwall()
{
}

//=================================================
//メッシュウォールの初期化処理
//=================================================
HRESULT CMeshwall::InitMeshwall(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	int nCount = 0;

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/walltex.jpg",
		&m_pTextureMeshwall);

	// 変数の初期化
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_meshwall[nCnt].m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_meshwall[nCnt].m_posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_meshwall[nCnt].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_meshwall[nCnt].m_fWidth = 0.0f;
		m_meshwall[nCnt].m_fHeight = 0.0f;
		m_meshwall[nCnt].m_fWidthMax = 0.0f;
		m_meshwall[nCnt].m_fHeightMax = 0.0f;
		m_meshwall[nCnt].m_nAllPoint = 0;
		m_meshwall[nCnt].m_nPolygon = 0;
		m_meshwall[nCnt].m_nIdxPoint = 0;
		m_meshwall[nCnt].m_nWidth = 0;
		m_meshwall[nCnt].m_nHeight = 0;
		m_meshwall[nCnt].m_bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * WALL_IDX_MAX * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMeshwall,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);

	// 初期化処理
	for (int nCntWall = 0; nCntWall < 2; nCntWall++)
	{
		for (int nCnt = 0; nCnt < m_meshwall[nCntWall].m_nHeight - 1; nCnt++)
		{
			for (int nCntA = 0; nCntA < m_meshwall[nCntWall].m_nWidth - 1; nCntA++, pVtx++)
			{
				// ポリゴンの各頂点座標
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// 各頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

				// テクスチャ頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			}
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuffMeshwall->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * WALL_POINT_MAX * 2,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffMeshwall,
		NULL);

	return S_OK;
}

//=================================================
//メッシュウォールの終了処理
//=================================================
void CMeshwall::UninitMeshwall(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuffMeshwall != NULL)
	{
		m_pVtxBuffMeshwall->Release();
		m_pVtxBuffMeshwall = NULL;
	}

	// テクスチャの開放
	if (m_pTextureMeshwall != NULL)
	{
		m_pTextureMeshwall->Release();
		m_pTextureMeshwall = NULL;
	}

	// インデックスバッファの開放
	if (m_pIdxBuffMeshwall != NULL)
	{
		m_pIdxBuffMeshwall->Release();
		m_pIdxBuffMeshwall = NULL;
	}
}

//=================================================
//メッシュウォールの更新処理
//=================================================
void CMeshwall::UpdateMeshwall(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	WORD *pIdx;

	anCnt -= 0.008f;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuffMeshwall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntMax = 0; nCntMax < 2; nCntMax++)
	{
		if (m_meshwall[nCntMax].m_bUse == true)
		{
			for (int nCntWall = 0; nCntWall < m_meshwall[nCntMax].m_nHeightPoint; nCntWall++)
			{
				for (int nCnt = 0; nCnt < m_meshwall[nCntMax].m_nWidthPoint; nCnt++, pVtx++)
				{
					pVtx[0].pos = D3DXVECTOR3(
						-m_meshwall[nCntMax].m_fWidthMax / 2.0f + (float)nCnt * m_meshwall[nCntMax].m_fWidth,
						m_meshwall[nCntMax].m_fHeightMax / 2.0f - (float)nCntWall * m_meshwall[nCntMax].m_fHeight,
						0.0f);

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 各頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					// テクスチャ頂点情報の設定
					pVtx[0].tex = D3DXVECTOR2((0.5f * nCnt) + anCnt, 0.5f * nCntWall);
				}
			}
		}

		for (int nCntA = 0; nCntA < m_meshwall[nCntMax].m_nHeight; nCntA++)
		{
			for (int nCnt = 0; nCnt < (m_meshwall[nCntMax].m_nWidthPoint + 1); nCnt++)
			{
				if (nCnt != 0 && nCnt == m_meshwall[nCntMax].m_nWidthPoint && nCntA != m_meshwall[nCntMax].m_nHeight - 1)
				{// 右端から折り返す時
					pIdx[0] = pIdx[-1];
					pIdx[1] = pIdx[-2] + 1;
				}
				else if (nCntA == m_meshwall[nCntMax].m_nHeight - 1 && nCnt == m_meshwall[nCntMax].m_nWidthPoint)
				{// 終了時に無視する
					break;
				}
				else
				{// 通常配置
					pIdx[0] = m_meshwall[nCntMax].m_nWidthPoint + (m_meshwall[nCntMax].m_nWidthPoint * nCntA) + nCnt;
					pIdx[1] = pIdx[0] - (m_meshwall[nCntMax].m_nWidthPoint);
				}
				pIdx += 2;
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuffMeshwall->Unlock();

	// 頂点バッファをアンロックする
	m_pVtxBuffMeshwall->Unlock();
}

//=================================================
//メッシュウォールの描画処理
//=================================================
void CMeshwall::DrawMeshwall(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
									
	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

		if (m_meshwall[nCnt].m_bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_meshwall[nCnt].m_mtxWorld);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				m_meshwall[nCnt].m_rot.y,
				m_meshwall[nCnt].m_rot.x,
				m_meshwall[nCnt].m_rot.z);

			D3DXMatrixMultiply(
				&m_meshwall[nCnt].m_mtxWorld,
				&m_meshwall[nCnt].m_mtxWorld,
				&mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(
				&mtxTrans,
				m_meshwall[nCnt].m_pos.x,
				m_meshwall[nCnt].m_pos.y,
				m_meshwall[nCnt].m_pos.z);

			D3DXMatrixMultiply(
				&m_meshwall[nCnt].m_mtxWorld,
				&m_meshwall[nCnt].m_mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_meshwall[nCnt].m_mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuffMeshwall, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(m_pIdxBuffMeshwall);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTextureMeshwall);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				0,
				0,
				m_meshwall[nCnt].m_nIdxPoint/*g_nIdxPointWall*/,				// 頂点数
				0,
				m_meshwall[nCnt].m_nPolygon/*g_nPolygonWall*/);			// プリミティブ数
		}
	}
}

void CMeshwall::SetMeshwall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nWidth, int nHeight)
{
	// ローカル変数宣言
	Meshwall *pMeshwall;
	pMeshwall = &m_meshwall[0];

	// 壁の設定
	for (int nCntWall = 0; nCntWall < 2; nCntWall++, pMeshwall++)
	{
		if (pMeshwall->m_bUse == false)
		{
			pMeshwall->m_pos = pos;				// 位置

			pMeshwall->m_rot = rot;				// 向き

			pMeshwall->m_fWidth = fWidth;			// 幅

			pMeshwall->m_fHeight = fHeight;		// 高さ

			pMeshwall->m_nWidth = nWidth;			// 列の数

			pMeshwall->m_nHeight = nHeight;		// 行の数

			pMeshwall->m_nWidthPoint = nWidth + 1;		// 幅の頂点数

			pMeshwall->m_nHeightPoint = nHeight + 1;		// 高さの頂点数

			pMeshwall->m_fWidthMax = pMeshwall->m_fWidth * (float)nWidth;

			pMeshwall->m_fHeightMax = pMeshwall->m_fHeight * (float)nHeight;

			pMeshwall->m_nAllPoint = nHeight * 2 * (nWidth + 2) - 2;
			pMeshwall->m_nPolygon = nWidth * nHeight * 2 + (4 * (nHeight - 1));
			pMeshwall->m_nIdxPoint = (nWidth + 1) * (nHeight + 1);

			pMeshwall->m_bUse = true;			// 使用状態
			break;
		}
	}
}
