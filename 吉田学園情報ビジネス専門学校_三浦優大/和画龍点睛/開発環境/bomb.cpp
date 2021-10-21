//=============================================================================
//
//ボム処理[bomb.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "bomb.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"
#include "gamepad.h"
#include "bullet.h"
#include "player.h"

//=============================================================================
//ボムのマクロ定義
//=============================================================================
#define BOMB_SIZE (60)         //ボムのテクスチャサイズ
#define BOMB_NUMBER (8)        //ボムの数
#define BOMB_COOLTIME (300)    //ボムのクールタイム
#define BOMB_BULLET_SIZE (120) //ボムのサイズ

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
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
	float fAngle = 0.0f; //ボムの角度
	float frad = 0.0f;  //ボムの半径
	D3DXVECTOR3 player; //プレイヤーの位置

	//ローカル変数に代入
	player = CPlayer::GetPlayer();     //プレイヤーの位置をゲットしてくる
	fAngle = D3DX_PI * 2 / BOMB_NUMBER;//弾の範囲
	frad = fAngle / 2;                 //半径を角度でわる

	//キーボードとゲームパッドの操作
	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_ATTACK) == true || pInputKeyboard->GetTrigger(DIK_X) == true)
	{
		if (m_nCnt == 0)//カウントが0になったら
		{
			for (int nCntAppear = 0; nCntAppear < BOMB_NUMBER; nCntAppear++, frad += fAngle)//ボムの弾の数分回す
			{
				//ボムのクリエイト
				CBullet::Create(player, CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPEDAMAGE_BOMB, frad, BOMB_BULLET_SIZE, BOMB_BULLET_SIZE, 0.0f);
			}

			//カウントアップ
			m_nCnt++;

			//カウントリセット
			m_nbombcool1 = 0;

			//フラグとなるboolをtrueにする
			m_bBomb = true;
		}
	}

	//ボムのクールタイム処理
	if (m_nCnt == 1)//カウントが1になったらカウントアップ
	{
		//カウントが1の時だけ回す
		m_nbombcool1++;
	}

	//ボムのクールタイム処理
	if (m_nbombcool1 >= BOMB_COOLTIME)
	{
		//300以上だったら0にリセット
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

	//返り値
	return S_OK;
}

//=============================================================================
//ボムのテクスチャ破棄処理
//=============================================================================
void CBomb::Unload(void)
{
	//ポインタがNULLじゃないとき
	if (m_pTexture != NULL)
	{
		//テクスチャのポインタをNULLにする
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//ボムの作成処理
//=============================================================================
CBomb * CBomb::Create(D3DXVECTOR3 pos)
{
	//インスタンス生成
	CBomb *pBomb = NULL;
	pBomb = new CBomb;

	//初期化呼び出し
	pBomb->Init(pos, BOMB_SIZE, BOMB_SIZE);

	//テクスチャ割り当て
	pBomb->BindTexture(m_pTexture);

	//返り値
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
