//=============================================================================
//
//ui処理[ui.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "ui.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "gamepad.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CUi::m_pTexture[UITYPE_MAX] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CUi::CUi(int nPriority) : CScene2D(nPriority)
{
	
}
//=============================================================================
//デストラクタ
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
//uiの初期化処理
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//タイプ分け関数
	CScene::SetObjType(CScene::OBJTYPE_UI);

	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//返り値
	return S_OK;
}

//=============================================================================
//uiの終了処理
//=============================================================================
void CUi::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//uiの更新処理
//=============================================================================
void CUi::Update(void)
{
	
}

//=============================================================================
//uiの描画処理
//=============================================================================
void CUi::Draw(void)
{
	//基本クラスの描画処理
	CScene2D::Draw();
}

//=============================================================================
//uiの作成・設定処理
//=============================================================================
CUi * CUi::Create(D3DXVECTOR3 pos, float fHight, float fWidth,UITYPE uitype)
{
	//インスタンス生成
	CUi *pUi = NULL;
	pUi = new CUi;

	//初期化呼び出し
	pUi->Init(pos, fHight, fWidth);

	//テクスチャ呼び出し
	pUi->BindTexture(m_pTexture[uitype]);

	return pUi;
}

//=============================================================================
//uiのテクスチャロード処理
//=============================================================================
HRESULT CUi::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Max.png", &m_pTexture[UITYPE_MAXSCORE]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Score.png", &m_pTexture[UITYPE_SCORE]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/graze.png", &m_pTexture[UITYPE_GRAZE]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/life.png", &m_pTexture[UITYPE_LIFE]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bomb.png", &m_pTexture[UITYPE_BOMB]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lineui.png", &m_pTexture[UITYPE_LINE]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titlelogo.png", &m_pTexture[UITYPE_LOGO]);

	return S_OK;
}

//=============================================================================
//uiのテクスチャアンロード処理
//=============================================================================
void CUi::Unload(void)
{
	//ループ
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
