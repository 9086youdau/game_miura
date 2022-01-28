//=============================================================================
//
// アニメーション処理 [animation.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "animation.h"
#include "manager.h"
#include "scene3D.h"
#include "effect.h"
#include "renderer.h"
#include "passerby.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CAnimation::m_pTexture = NULL; //テクスチャメンバ変数
bool CAnimation::m_bPasserbyColision = false;     //敵の当たり判定フラグ
CPasserby *CAnimation::m_pPasserby[256] = {};     //敵の保存用ポインタ変数

//=============================================================================
// コンストラクタ
//=============================================================================
CAnimation::CAnimation(int nPriority, int nType)
{
	//メンバ変数初期化
	m_nPatternAnim = 0;
	m_nCounterAnim = 0; 
	m_nCnt = 0;
	m_bPasserbyColision = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CAnimation::~CAnimation()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CAnimation::Init(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt)
{
	//初期化処理
	CScene3D::Init(pos, fHight, fWidth);

	//位置情報の受け渡し
	m_pos = pos;
	
	//敵数の受け渡し
	m_nCnt = nCnt;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CAnimation::Uninit(void)
{
	//オブジェクトの破棄
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CAnimation::Update(void)
{
	//カウントアップ
	m_nCounterAnim++;

	if ((m_nCounterAnim % 4) == 0)
	{
		//アニーメーション動かす
		m_nPatternAnim++;

		//アニメーションパターンが来たら消す
		if (m_nPatternAnim <= 5)
		{
			//関数呼び出し
			SetAnim(0.2f, m_nPatternAnim, 0.0f, 0.25f + 0.02f);
		}
		//アニメーションパターンが来たら消す
		if (m_nPatternAnim >= 5 && m_nPatternAnim <= 10)
		{
			//関数呼び出し
			SetAnim(0.2f, m_nPatternAnim, 0.25f, 0.5f + 0.02f);
		}
		//アニメーションパターンが来たら消す
		if (m_nPatternAnim >= 10 && m_nPatternAnim <= 15)
		{
			//関数呼び出し
			SetAnim(0.2f, m_nPatternAnim, 0.50f, 0.75f + 0.02f);
		}
		//アニメーションパターンが来たら消す
		if (m_nPatternAnim >= 15 && m_nPatternAnim <= 20)
		{
			//関数呼び出し
			SetAnim(0.2f, m_nPatternAnim, 0.75f, 1.0f + 0.02f);
		}
		if (m_nPatternAnim == 13)
		{
			//敵を生成(アニメーションのパターンが来たら)
			m_pPasserby[m_nCnt] = CPasserby::Create(m_pos, PASSERBY_TYPE_MEN, 0);
			//当たり判定を有効にする
			m_bPasserbyColision = true;
		}
		//アニメーションパターンが来たら消す
		if (m_nPatternAnim == 20)
		{
			//消去する
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CAnimation::Draw(void)
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
// 生成処理
//=============================================================================
CAnimation * CAnimation::Create(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt)
{
	//モデルを生成
	CAnimation *pAnimation = NULL;
	pAnimation = new CAnimation;

	//NULLチェック
	if (pAnimation != NULL)
	{
		//アニメーションの初期化処理
		pAnimation->Init(pos, fHight, fWidth, nCnt);

		//テクスチャ処理
		pAnimation->SetTexture(m_pTexture);
	}

	return pAnimation;
}
//=============================================================================
// テクスチャロード処理
//=============================================================================
void CAnimation::Load(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pipo-btleffect213_480.png", &m_pTexture);
}
//=============================================================================
// テクスチャアンロード処理
//=============================================================================
void CAnimation::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
