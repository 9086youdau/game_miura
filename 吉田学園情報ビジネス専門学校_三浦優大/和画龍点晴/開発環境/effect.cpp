//================================================
//
//エフェクト処理[effect.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "effect.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=============================================================================
//コンストラクタ
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
//エフェクトの設定・更新処理
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos,float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect)
{
	//インスタンス生成
	CEffect *pEffect = NULL;
	pEffect = new CEffect;

	//初期化呼び出し
	pEffect->Init(pos, fHight, fWidth,col,effect);

	//テクスチャ呼び出し
	pEffect->BindTexture(m_pTexture);

	return pEffect;
}

//=============================================================================
//エフェクトの初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect)
{
	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//タイプ分け関数
	CScene::SetObjType(CScene::OBJTYPE_EFFECT);

	//移動量の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_effecttype = effect;

	switch (m_effecttype)
	{
	case EFFECTTYPE_NONE:
		m_nLife = 2;
		break;
	case EFFECTTYPE_PLAYER_MOVE:
		m_nLife = 15;
		break;
	}

	//カラー変更関数呼び出し
	SetCol(col);

	return S_OK;
}
//=============================================================================
//エフェクトの終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}
//=============================================================================
//エフェクトの更新処理
//=============================================================================
void CEffect::Update(void)
{
	//弾の処理関数呼び出し
	SetEffectMove();
}
//=============================================================================
//エフェクトの描画処理
//=============================================================================
void CEffect::Draw(void)
{
	// 頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//レンダーステートの設定(加算合成）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//基本クラスの描画処理
	CScene2D::Draw();

	//レンダーステートの設定(元に戻す）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============================================================================
//エフェクトのテクスチャ処理
//=============================================================================
HRESULT CEffect::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}
//=============================================================================
//エフェクトのテクスチャの廃棄処理
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
//=============================================================================
//エフェクトの移動処理
//=============================================================================
void CEffect::SetEffectMove(void)
{
	if (m_effecttype == EFFECTTYPE_NONE)
	{
		//値を代入
		m_pos = GetPosition();

		//位置更新
		m_pos += m_move;

		//弾の位置更新
		SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//ライフを減らす
		m_nLife--;

		//ライフが0になったら
		if (m_nLife == 0)
		{
			//終了処理
			Uninit();
		}
	}
	if (m_effecttype == EFFECTTYPE_PLAYER_MOVE)
	{
		//値を代入
		m_pos = GetPosition();

		//ライフを減らす
		m_nLife--;

		//moveの計算処理
		m_fAngle = (float)(rand() % 100 - 50) / 100.0f;
		m_move.x = sinf(m_fAngle * D3DX_PI) * 10.5f;
		m_move.y = cosf(m_fAngle * D3DX_PI) * 10.5f;

		//位置更新
		m_pos += m_move;

		//弾の位置更新
		SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//ライフが0になったら
		if (m_nLife == 0)
		{
			//終了処理
			Uninit();
		}
	}
}