//=============================================================================
//
// シーン3D処理[Scene3D.cpp]
// Author; Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "Scene3D.h"
#include "Renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
D3DXVECTOR3 CScene3D::size[4];
bool CScene3D::m_bCount;

//=============================================================================
//コンストラクタ
//=============================================================================
CScene3D::CScene3D()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{
}

//=============================================================================
//ポリゴン3Dの初期化処理
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noname.png", &m_pTexture);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//ポリゴン3Dのポインタ
	VERTEX_3D *pVtx;

	//位置情報の代入
	m_pos = pos;

	//カラー情報設定
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//メンバ変数初期化
	m_nLife = 10;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(size[0].x, m_pos.y + 100.0f, size[0].z);
	pVtx[1].pos = D3DXVECTOR3(size[1].x, m_pos.y + 100.0f, size[1].z);
	pVtx[2].pos = D3DXVECTOR3(size[2].x, m_pos.y + 100.0f, size[2].z);
	pVtx[3].pos = D3DXVECTOR3(size[3].x, m_pos.y + 100.0f, size[3].z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//ポリゴン3Dの終了処理
//=============================================================================
void CScene3D::Uninit(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//ポリゴン3Dの更新処理
//=============================================================================
void CScene3D::Update(void)
{
	//寿命計算
	m_nLife--;
	//値が0になったら
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
//ポリゴン3Dの描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	//pDevice->SetTexture(0, CManager::GetRenderer()->GetTextureMT());

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//ポリゴン3Dのクリエイト処理
//=============================================================================
CScene3D * CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY)
{
	//シーン3D生成
	CScene3D *pScene3D = NULL;
	pScene3D = new CScene3D;
	
	size[0] = fMaxSizeX;
	size[1] = fMinSizeX;
	size[2] = fMaxSizeY;
	size[3] = fMinSizeY;

	//初期化呼び出し
	pScene3D->Init(pos, 30.0f, 30.0f);

	return pScene3D;
}
