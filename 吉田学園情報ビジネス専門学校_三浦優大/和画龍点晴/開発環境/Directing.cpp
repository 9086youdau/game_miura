//=============================================================================
//
//演出処理[Directing.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "Directing.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "gamepad.h"
#include "sound.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CDirecting::m_pTexture[DIRECTINGTYPE_MAX] = {};
CDirecting::FADETYPE CDirecting::m_fadetype = FADE_NONE;
CDirecting::FADETYPE CDirecting::m_fadetype2 = FADE_NONE;
CDirecting::DIRECTINGTYPE CDirecting::m_directingType = DIRECTINGTYPE_NONE;
bool CDirecting::m_benemycol = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CDirecting::CDirecting(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
//デストトラクタ
//=============================================================================
CDirecting::~CDirecting()
{
}

//=============================================================================
//演出処理の初期化処理
//=============================================================================
HRESULT CDirecting::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//タイプ分け関数
	CScene::SetObjType(CScene::OBJTYPE_DIRECTING);

	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//メンバ変数初期化
	m_FadeCount = 300;
	m_pos = pos;
	m_colstart = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_bcol = false;
	m_nCnt = 0;
	m_benemycol = false;

	return S_OK;
}

//=============================================================================
//演出処理の終了処理
//=============================================================================
void CDirecting::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//演出処理の更新処理
//=============================================================================
void CDirecting::Update(void)
{
	//値を代入
	m_pos = GetPosition();

	//フェードのカウントマイナス
	m_FadeCount--;

	//フェードインの時
	if (m_fadetype == FADE_IN)
	{
		m_colstart.a -= 0.01f;
		if (m_colstart.a <= 0.0f)
		{
			m_colstart.a = 0.0f;
			m_fadetype = FADE_NONE;
		}
	}

	//フェードアウトの時
	if (m_fadetype == FADE_OUT)
	{
		m_colstart.a += 0.01f;
		if (m_colstart.a >= 1.0f)
		{
			m_colstart.a = 1.0f;
			if (m_FadeCount <= 0)
			{
				m_fadetype = FADE_IN;
			}
		}
	}

	SetCol(m_colstart);
}

//=============================================================================
//演出処理の描画処理
//=============================================================================
void CDirecting::Draw(void)
{
	//基本クラスの描画処理
	CScene2D::Draw();
}

//=============================================================================
//演出処理の設定処理
//=============================================================================
CDirecting * CDirecting::Create(D3DXVECTOR3 pos,float fHight, float fWidth,FADETYPE fadetype, DIRECTINGTYPE directing)
{
	//インスタンス生成
	CDirecting *pDirecting = NULL;
	pDirecting = new CDirecting;

	//引数から代入
	m_directingType = directing;
	switch (directing)
	{
	case DIRECTINGTYPE_START:
		m_fadetype = fadetype;
		pDirecting->BindTexture(m_pTexture[DIRECTINGTYPE_START]);
		break;
	}

	//初期化呼び出し
	pDirecting->Init(pos, fHight, fWidth);

	//ポインタの返り値
	return pDirecting;
}

//=============================================================================
//演出処理のテクスチャロード処理
//=============================================================================
HRESULT CDirecting::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlecole.png", &m_pTexture[DIRECTINGTYPE_START]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_pTexture[DIRECTINGTYPE_BOSS]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_pTexture[DIRECTINGTYPE_PLAYER]);

	return S_OK;
}

//=============================================================================
//演出処理のテクスチャアンロード処理
//=============================================================================
void CDirecting::Unload(void)
{
	//ループ
	for (int nCnt = 0; nCnt < DIRECTINGTYPE_MAX; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
