//================================================
//
//ナンバー処理[number.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "number.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"

//=================================================
//静的メンバ変数宣言
//=================================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CNumber::CNumber()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
//ナンバーの初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//位置情報の代入
	m_pos = D3DXVECTOR3(pos);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//ナンバーの終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//ナンバーの更新処理
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
//ナンバー描画処理
//=============================================================================
void CNumber::Draw(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//ナンバーのテクスチャロード処理
//=============================================================================
HRESULT CNumber::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number005.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//ナンバーのテクスチャ破棄処理
//=============================================================================
void CNumber::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//ナンバーの設定・作成処理
//=============================================================================
CNumber * CNumber::Create(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//インスタンス生成
	CNumber *pNumber = NULL;
	pNumber = new CNumber;

	//初期化呼び出し
	pNumber->Init(pos, fHight, fWidth);

	return pNumber;
}

//=============================================================================
//ナンバーのテクスチャ処理
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.1f * nNumber, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f * nNumber, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f * (nNumber + 1), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f * (nNumber + 1), 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
