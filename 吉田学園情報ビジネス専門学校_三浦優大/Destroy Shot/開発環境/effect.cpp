//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "scene3D.h"
#include "renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect(int nPriority, int nType)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col, float fHight, float fWidth)
{
	//初期化処理
	CScene3D::Init(pos, fHight, fWidth);

	//引数用関数
	SetEffect(pos, nLife, col);

	//テクスチャ処理
	SetTex();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	//オブジェクトの破棄
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	//位置更新の関数呼び出し
	SetPosition(m_pos);

	//カラー変更
	SetCol(m_col);

	//アルファ値を下げる
	m_col.a -= 0.15f;

	//ライフ処理
	m_nLife--;
	if (m_nLife == 0)//寿命が0になったら
	{
		//消去処理
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//レンダーステートの設定(加算合成）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//描画処理
	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//レンダーステートの設定(元に戻す）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 引数から変数に代入する
//=============================================================================
void CEffect::SetEffect(D3DXVECTOR3 pos,int nLife, D3DXCOLOR col)
{
	//位置情報
	m_pos = pos;

	//寿命情報
	m_nLife = nLife;

	//色変更
	m_col = col;
}

//=============================================================================
// 生成処理
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float fHight, float fWidth)
{
	//モデルを生成
	CEffect *pEffect = NULL;
	pEffect = new CEffect;

	//NULLチェック
	if (pEffect != NULL)
	{
		//オブジェクトの初期化処理
		pEffect->Init(pos, nLife, col, fHight, fWidth);

		//テクスチャ処理
		pEffect->SetTexture(m_pTexture);
	}

	return pEffect;
}
//=============================================================================
// テクスチャロード処理
//=============================================================================
void CEffect::Load(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);
}
//=============================================================================
// テクスチャアンロード処理
//=============================================================================
void CEffect::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
