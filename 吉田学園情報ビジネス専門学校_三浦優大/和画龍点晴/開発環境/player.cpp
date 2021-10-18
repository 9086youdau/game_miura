//================================================
//
//プレイヤー処理[player.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "bullet.h"
#include "Renderer.h"
#include "explosion.h"
#include "sound.h"
#include "Score.h"
#include "effect.h"
#include "gamepad.h"
#include "game.h"
#include "particle.h"
#include "life.h"
#include "fade.h"
#include "Experience.h"
#include "playerattack.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
D3DXVECTOR3 CPlayer::m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
D3DXVECTOR3 CPlayer::m_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CPlayer::PLAYERSTATE CPlayer::m_playerstate = PLAYERSTATE_NOMAL;
int CPlayer::m_statecount = 0;
bool CPlayer::m_playerattack = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	//メンバ変数初期化
	m_Pause = false;
	m_bInit = false;
	m_playerattack = false;
}
//=============================================================================
//デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}
//=============================================================================
//プレイヤーの生成・設定
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	//インスタンス生成
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;

	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, MAXSIZEPLAYER, MAXSIZEPLAYER);
	}

	//テクスチャ設定関数
	pPlayer->BindTexture(m_pTexture);

	//返り値
	return pPlayer;
}
//=============================================================================
//プレイヤーの初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//タイプ分け関数
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//メンバ変数初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_oldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_playerstate = PLAYERSTATE_NOMAL;
	m_statecount = 0;

	//返り値
	return S_OK;
}
//=============================================================================
//プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	//終了処理
	CScene2D::Uninit();
}
//=============================================================================
//プレイヤーの更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//関数呼び出し
	PlayerCol();

	//関数呼び出し
	InitPlayerMove();

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 40.0f, 40.0f, D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.6f), CEffect::EFFECTTYPE_PLAYER_MOVE);
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 100.0f, 100.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), CEffect::EFFECTTYPE_NONE);

	//ポインタをNULLにする
	CScene *pScene = NULL;

	//敵とプレイヤーの当たり判定
	if (Collision(m_pos, 50, 50, CScene::OBJTYPE_ENEMY, &pScene) == true)
	{
		if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
		{
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
			{
				//パーティクルを生成
				CParticle::Create(m_pos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_PLAYER);
			}

			//ライフ計算
			m_nLifeCount = CBullet::GetLife();

			//HPを減らす処理
			CLife::SetRelease(m_nLifeCount);

			//カウントアップ
			m_nLifeCount--;

			//セットのライフ処理
			CBullet::SetLife(1);

			if (m_nLifeCount > -1)
			{
				//プレイヤーの削除
				Uninit();

				//初期からリセット
				CPlayer::Create(D3DXVECTOR3(550.0f, 1200.0f, 0.0f));

				//状態変化関数
				CPlayer::SetStatePlayer(CPlayer::PLAYERSTATE_DAMAGE);
			}
			else if (m_nLifeCount == -1)
			{
				//プレイヤーの削除
				Uninit();

				//プレイヤーアタックをtrueに変更
				m_playerattack = true;

				//ライフをリセットする
				m_nLifeCount = 3;

				//ゲーム画面へ遷移
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}
	}
}
//=============================================================================
//プレイヤーの描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//描画処理
	CScene2D::Draw();
}
//=============================================================================
//プレイヤーの移動処理
//=============================================================================
void CPlayer::PlayerMove(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//キーボードのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//位置を取得
	m_pos = GetPosition();

	//昔の位置を設定
	m_oldpos = m_pos;

	//弾の処理のカウント
	m_shotcount++;

	//斜め計算処理
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true
		|| pInputKeyboard->GetPress(DIK_RIGHT) == true || pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_UP) == true || pInputGamePad->IsButtonPush(CInput::KEYINFO_DOWN) == true
			|| pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
		{
			//斜め移動のメンバ変数に値を代入
			m_DiagonalMove = 0.71f;
		}
		else
		{
			//斜め移動のメンバ変数に値を代入
			m_DiagonalMove = 1.0f;
		}
	}
	else if (pInputGamePad->IsButtonPush(CInput::KEYINFO_UP) == true || pInputGamePad->IsButtonPush(CInput::KEYINFO_DOWN) == true
		|| pInputKeyboard->GetPress(DIK_UP) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
	{
		//斜め移動のメンバ変数に値を代入
		m_DiagonalMove = 1.0f;
	}

	//右移動
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		//移動量
		m_move.x += sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;

		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//移動量
			m_move.x += sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//左移動
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		//移動量
		m_move.x -= sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;

		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//移動量
			m_move.x -= sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//上移動
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_UP) == true || pInputKeyboard->GetPress(DIK_UP) == true)
	{
		//移動量
		m_move.y -= sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//移動量
			m_move.y -= sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//下移動
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_DOWN) == true || pInputKeyboard->GetPress(DIK_DOWN) == true)
	{
		//移動量
		m_move.y += sinf(D3DX_PI * 0.5f) * 3.0f * m_DiagonalMove;
		if (pInputGamePad->IsButtonPush(CInput::KEYINFO_RIGHT) == true || pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			//移動量
			m_move.y += sinf(D3DX_PI * 0.5f) * -1.5f * m_DiagonalMove;
		}
	}

	//画面外処理
	if (m_pos.x >= 1000)
	{
		m_pos.x = 1000;
	}
	else if (m_pos.x <= 80)
	{
		m_pos.x = 80;
	}
	//画面外処理
	if (m_pos.y <= 80)
	{
		m_pos.y = 80;
	}
	else if (m_pos.y >= 980)
	{
		m_pos.y = 980;
	}

	//移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.3f;

	//位置更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//ポリゴンの位置関数
	SetPosition(m_pos,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//プレイヤーの初期移動処理
//=============================================================================
void CPlayer::InitPlayerMove(void)
{
	//位置を取得
	m_pos = GetPosition();

	//boolの値を返す
	m_Pause = CGame::GetPause();

	//ポーズ画面
	if (m_Pause == false)
	{
		if (m_bInit == false)
		{
			//移動量
			m_move.y -= sinf(D3DX_PI * 0.5f) * 2.0f;

			if (m_pos.y <= 900)
			{
				m_bInit = true;
			}
		}
		else
		{
			PlayerMove();
		}
	}

	//移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.3f;

	//位置更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//ポリゴンの位置関数
	SetPosition(m_pos,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//プレイヤーの状態変化処理
//=============================================================================
void CPlayer::SetStatePlayer(PLAYERSTATE playerstate)
{
	//引数から代入
	switch (playerstate)
	{
	case PLAYERSTATE_NOMAL:
		m_playerstate = PLAYERSTATE_NOMAL;
		break;
	case PLAYERSTATE_DAMAGE:
		m_playerstate = PLAYERSTATE_DAMAGE;
		m_statecount = 150;
		break;
	}
}
//=============================================================================
//プレイヤーの色変更処理
//=============================================================================
void CPlayer::PlayerCol(void)
{
	switch (m_playerstate)
	{
	case PLAYERSTATE_NOMAL:
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case PLAYERSTATE_DAMAGE:
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		m_statecount--;
		if (m_statecount <= 0)
		{
			m_playerstate = PLAYERSTATE_NOMAL;
		}
		break;
	}
}

//=============================================================================
//バレットのテクスチャ処理
//=============================================================================
HRESULT CPlayer::Load()
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ddd.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//バレットのテクスチャの廃棄処理
//=============================================================================
void CPlayer::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
