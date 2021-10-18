//================================================
//
//ボム処理[bomb.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "bomb.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"
#include "gamepad.h"
#include "bullet.h"
#include "player.h"

//=================================================
//静的メンバ変数宣言
//=================================================
LPDIRECT3DTEXTURE9 CBomb::m_pTexture = NULL;
bool CBomb::m_bBomb = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CBomb::CBomb(int nPriority) : CScene2D(nPriority)
{
	//メンバ変数初期化
	m_bBomb = false;
}

//=============================================================================
//デストラクタ
//=============================================================================
CBomb::~CBomb()
{
}

//=============================================================================
//ボムの初期化処理
//=============================================================================
HRESULT CBomb::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//タイプ分け関数
	CScene::SetObjType(CScene::OBJTYPE_BOMB);

	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//返り値
	return S_OK;
}

//=============================================================================
//ボムの終了処理
//=============================================================================
void CBomb::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}

//=============================================================================
//ボムの更新処理
//=============================================================================
void CBomb::Update(void)
{
	//キーボードのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ローカル変数宣言
	float fAngle;
	float rad = 0.0f;
	D3DXVECTOR3 player;
	D3DXVECTOR3 move;

	//ローカル変数に代入
	player = CPlayer::GetPlayer();
	fAngle = D3DX_PI * 2 / 8;//弾の範囲
	rad = fAngle / 2;

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_ATTACK) == true || pInputKeyboard->GetTrigger(DIK_X) == true)
	{
		if (m_nCnt == 0)
		{
			for (int nCntAppear = 0; nCntAppear < 8; nCntAppear++, rad += fAngle)
			{
				//弾のクリエイト
				CBullet::Create(player, CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPEDAMAGE_BOMB, rad, MAX_SIZE_BOMB_BULLET, MAX_SIZE_BOMB_BULLET, 0.0f);
			}

			//カウントアップ
			m_nCnt++;

			//カウントリセット
			m_nbombcool1 = 0;

			//フラグとなるbool
			m_bBomb = true;
		}
	}

	//ボムのクールタイム処理
	if (m_nCnt == 1)
	{
		m_nbombcool1++;
	}
	//ボムのクールタイム処理
	if (m_nbombcool1 >= 200)
	{
		m_nCnt = 0;
	}
}

//=============================================================================
//ボム描画処理
//=============================================================================
void CBomb::Draw(void)
{
	//基本クラスの描画処理
	CScene2D::Draw();
}

//=============================================================================
//ボムのテクスチャロード処理
//=============================================================================
HRESULT CBomb::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/star.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//ボムのテクスチャ破棄処理
//=============================================================================
void CBomb::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//ボムの設定・作成処理
//=============================================================================
CBomb * CBomb::Create(D3DXVECTOR3 pos)
{
	//インスタンス生成
	CBomb *pBomb = NULL;
	pBomb = new CBomb;

	//初期化呼び出し
	pBomb->Init(pos, 60.0f, 60.0f);
	pBomb->BindTexture(m_pTexture);

	return pBomb;
}

//=============================================================================
//ボムのセット処理
//=============================================================================
void CBomb::SetBomb(bool bBomb)
{
	//引数からboolを参照
	m_bBomb = bBomb;
}
