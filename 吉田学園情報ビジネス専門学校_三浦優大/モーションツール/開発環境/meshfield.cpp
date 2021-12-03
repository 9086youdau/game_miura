//=============================================================================
//
// メッシュフィールド処理[meshfield.cpp]
// Author; Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "meshfield.h"
#include "Scene3D.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
//コンストラクタ
//=============================================================================
CMeshfield::CMeshfield()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CMeshfield::~CMeshfield()
{

}

//=============================================================================
//メッシュフィールドの初期化処理
//=============================================================================
HRESULT CMeshfield::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//中心を合わせる
	m_pos = D3DXVECTOR3(-(WIDTHNUM*WIDTH / 2), 0.0f, (DEPTHNUM*DEPTH / 2));

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nAllPoint = DEPTHNUM * 2 * (WIDTHNUM + 2) - 2;				// 総頂点数
	m_nPolygon = WIDTHNUM * DEPTHNUM * 2 + (4 * (DEPTHNUM - 1));	// 総ポリゴン数
	m_nIdxPoint = m_nWidth * m_nDepth;								// インデックスバッファでの頂点数

	//最大サイズの計算
	float fWidthMax = fWidth * (float)WIDTHNUM;
	float fDepthMax = fHight * (float)DEPTHNUM;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nIdxPoint,D3DUSAGE_WRITEONLY,FVF_VERTEX_3D,D3DPOOL_MANAGED,&m_pVtxBuff,NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nDepth; nCnt++)
	{
		// 奥行軸
		for (int nCntA = 0; nCntA < m_nWidth; nCntA++)
		{
			// 横軸	 
			// ポリゴンの各頂点座標
			pVtx[0].pos = D3DXVECTOR3(-fWidthMax / 2.0f + (float)nCntA * m_fWidth, m_pos.y, fDepthMax / 2.0f - (float)nCnt * m_fDepth);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 各頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

			// テクスチャ頂点情報の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntA, 1.0f * nCnt);

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nAllPoint,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIdxBuff,NULL);

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntA = 0; nCntA < DEPTHNUM; nCntA++)
	{
		for (int nCnt = 0; nCnt < m_nWidth + 1; nCnt++, pIdx += 2)
		{
			if (nCnt != 0 && nCnt == m_nWidth && nCntA != DEPTHNUM - 1)
			{
				// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntA == DEPTHNUM - 1 && nCnt == m_nWidth)
			{
				// 終了時に無視する
				break;
			}
			else
			{
				// 通常配置
				pIdx[0] = m_nWidth + (m_nWidth * nCntA) + nCnt;
				pIdx[1] = pIdx[0] - m_nWidth;
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}


//=============================================================================
//メッシュフィールドの終了処理
//=============================================================================
void CMeshfield::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//メッシュフィールドの更新処理
//=============================================================================
void CMeshfield::Update(void)
{

}

//=============================================================================
//メッシュフィールドの描画処理
//=============================================================================
void CMeshfield::Draw(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;// 計算用マトリックス

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0,NULL);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nIdxPoint,			// 頂点数
		0,
		m_nPolygon);			// プリミティブ数
}

//=============================================================================
//メッシュフィールドのクリエイト処理
//=============================================================================
CMeshfield * CMeshfield::Create(D3DXVECTOR3 pos,float fHight, float fWidth)
{
	//インスタンス生成
	CMeshfield *pMeshfield = NULL;
	pMeshfield = new CMeshfield;

	//初期化呼び出し
	pMeshfield->Init(pos, fHight, fWidth);

	return pMeshfield;
}
