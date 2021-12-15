//================================================
//
//プレイヤーアタック処理[playerattack.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "playerattack.h"
#include "input.h"
#include "manager.h"
#include "Renderer.h"
#include "sound.h"
#include "gamepad.h"
#include "game.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_SIZE_PLAYER_BULLET (30)//バレットのサイズ

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayerAttack::m_pTexture = NULL;
D3DXVECTOR3 CPlayerAttack::m_pos[2] = {};

//=============================================================================
//コンストラクタ
//=============================================================================
CPlayerAttack::CPlayerAttack(int nPriority) : CScene2D(nPriority)
{
	//メンバ変数初期化
	m_raise[0] = 2;
	m_toggle = false;
}

//=============================================================================
//デストトラクタ
//=============================================================================
CPlayerAttack::~CPlayerAttack()
{
}

//=============================================================================
//プレイヤーアタックの生成・設定
//=============================================================================
CPlayerAttack * CPlayerAttack::Create(D3DXVECTOR3 pos,PLAYERATTACK playerattack)
{
	//インスタンス生成
	CPlayerAttack *pPlayerAttack = NULL;
	pPlayerAttack = new CPlayerAttack;

	if (pPlayerAttack != NULL)
	{
		pPlayerAttack->Init(pos, 12.0f, 12.0f, playerattack);
	}

	//テクスチャ設定関数
	pPlayerAttack->BindTexture(m_pTexture);

	//返り値
	return pPlayerAttack;
}

//=============================================================================
//プレイヤーアタックの初期化処理
//=============================================================================
HRESULT CPlayerAttack::Init(D3DXVECTOR3 pos, float fHight, float fWidth,PLAYERATTACK playerattack)
{
	//タイプ分け関数
	CScene::SetObjType(CScene::OBJTYPE_PLAYER_ATTACK);

	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//メンバ変数初期化
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_move[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_pos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//メンバ変数初期化
	m_playerattack = playerattack;

	//返り値
	return S_OK;
}

//=============================================================================
//プレイヤーアタックの終了処理
//=============================================================================
void CPlayerAttack::Uninit(void)
{
	//終了処理
	CScene2D::Uninit();
}

//=============================================================================
//プレイヤーアタックの更新処理
//=============================================================================
void CPlayerAttack::Update(void)
{
	//ポーズの代入
	bool pause = false;
	pause = CGame::GetPause();

	if (pause == false)
	{
		//関数呼び出し
		InitPlayerAttackMove();
	}
}

//=============================================================================
//プレイヤーアタックの描画処理
//=============================================================================
void CPlayerAttack::Draw(void)
{
	//描画処理
	CScene2D::Draw();
}

//=============================================================================
//プレイヤーアタックの移動処理
//=============================================================================
void CPlayerAttack::PlayerAttackMove(void)
{
}

//=============================================================================
//プレイヤーアタックの移動処理
//=============================================================================
void CPlayerAttack::InitPlayerAttackMove(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンドのポインタ宣言
	CSound *pSound;
	pSound = CManager::GetSound();

	//カウントアップ
	m_shotcount++;

	if (m_playerattack == PLAYERATTACK_RIGHT)
	{
		//位置を取得
		m_pos[0] = GetPosition();

		//回転処理
		m_rot[0].z += 0.1f;

		//プレイヤーに対しての処理
		m_homingAngle[0] += m_raise[0];

		m_pos[0].x = CPlayer::GetPlayer().x + 70.0f;//移動量を求める
		m_pos[0].y = CPlayer::GetPlayer().y + sinf(m_homingAngle[0] * D3DX_PI / 180.0f) * 15.0f;

		if (m_homingAngle[0] == 90)
		{
			m_toggle = true;
		}
		else if (m_homingAngle[0] == -90)
		{
			m_toggle = false;
		}

		if (m_toggle)
		{
			m_raise[0] = -2;
		}
		else
		{
			m_raise[0] = 2;
		}

		//弾発射
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_OK) == true || pInputKeyboard->GetPress(DIK_Z) == true)
		{
			float rad = 0.0f;

			if (m_shotcount % 7 == 0)
			{
				//弾のクリエイト
				CBullet::Create(m_pos[0], CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPEDAMAGE_NOMALSHOT, rad, MAX_SIZE_PLAYER_BULLET, MAX_SIZE_PLAYER_BULLET, 0.0f);
			}
		}

		//ポリゴンの位置関数
		SetPosition(m_pos[0], m_rot[0]);
	}
	else
	{
		//位置を取得
		m_pos[1] = GetPosition();

		//回転処理
		m_rot[1].z -= 0.1f;

		m_homingAngle[1] += m_raise[0];

		m_pos[1].x = CPlayer::GetPlayer().x - 70.0f;//移動量を求める
		m_pos[1].y = CPlayer::GetPlayer().y + sinf(m_homingAngle[1] * D3DX_PI / 180.0f) * 15.0f;

		if (m_homingAngle[1] == 90)
		{
			m_toggle = true;
		}
		else if (m_homingAngle[1] == -90)
		{
			m_toggle = false;
		}

		if (m_toggle)
		{
			m_raise[1] = -2;
		}
		else
		{
			m_raise[1] = 2;
		}

		//弾発射
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_OK) == true || pInputKeyboard->GetPress(DIK_Z) == true)
		{
			float rad = 0.0f;

			if (m_shotcount % 7 == 0)
			{
				//弾のクリエイト
				CBullet::Create(m_pos[1], CBullet::BULLETTYPE_PLAYER, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPEDAMAGE_NOMALSHOT, rad, MAX_SIZE_PLAYER_BULLET, MAX_SIZE_PLAYER_BULLET, 0.0f);
			}
		}

		//ポリゴンの位置関数
		SetPosition(m_pos[1], m_rot[1]);
	}
}

//=============================================================================
//プレイヤーアタックのテクスチャロード処理
//=============================================================================
HRESULT CPlayerAttack::Load()
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/AttackPoint.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//プレイヤーアタックのテクスチャアンロード処理
//=============================================================================
void CPlayerAttack::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
