//================================================
//
//経験値処理[Experience.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "Experience.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"
#include "gamepad.h"
#include "bullet.h"
#include "player.h"
#include "score.h"
#include "game.h"

//=================================================
//静的メンバ変数宣言
//=================================================
LPDIRECT3DTEXTURE9 CExperience::m_pTexture = NULL;

//=================================================
//コンストラクタ
//=================================================
CExperience::CExperience()
{
}

//=================================================
//デストラクタ
//=================================================
CExperience::~CExperience()
{
}

//=================================================
//経験値のクリエイト
//=================================================
CExperience * CExperience::Create(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//インスタンス生成
	CExperience *pExperience = NULL;
	pExperience = new CExperience;

	//初期化呼び出し
	pExperience->Init(pos, fHight, fWidth);

	//テクスチャ呼び出し
	pExperience->BindTexture(m_pTexture);

	return pExperience;
}

//=================================================
//経験値の初期化処理
//=================================================
HRESULT CExperience::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//引数から代入
	m_pos = pos;

	//メンバ変数初期化
	m_bHoming = false;

	//返り値
	return S_OK;
}

//=================================================
//経験値の終了処理
//=================================================
void CExperience::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}

//=================================================
//経験値の更新処理
//=================================================
void CExperience::Update(void)
{
	//ポーズの代入
	bool pause = false;
	pause = CGame::GetPause();

	//ローカル変数宣言
	D3DXVECTOR3 posplayer;
	D3DXVECTOR3 vec;
	float fAngle;

	if (pause == false)
	{
		//プレイヤーの座標をゲット
		posplayer = CPlayer::GetPlayer();

		vec = posplayer - m_pos;//現在地から目的地へのベクトル
		fAngle = atan2f(vec.x, vec.y);//ベクトルの角度を求める

		if (m_bHoming == false)
		{
			//移動させる
			m_move.x = sinf(D3DX_PI) * -1.5f;//移動量
			m_move.y = cosf(D3DX_PI) * -1.5f;//移動量
		}
		else
		{
			//プレイヤーの状態がNOMALだったら
			if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
			{
				//移動させる
				m_move.x = sinf(fAngle) * 12.0f;//移動量
				m_move.y = cosf(fAngle) * 12.0f;//移動量
			}
			else
			{
				//移動させる
				m_move.x = sinf(D3DX_PI) * -1.5f;//移動量
				m_move.y = cosf(D3DX_PI) * -1.5f;//移動量
			}
		}

		//位置更新
		m_pos += m_move;

		//弾の位置更新
		SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//ポインタをNULLにする
		CScene *pScene = NULL;

		//プレイヤーの弾と敵の当たり判定
		if (Collision(m_pos, 70.0f, 70.0f, CScene::OBJTYPE_PLAYER, &pScene) == true)
		{
			//ホーミングの判定をtrueにする
			m_bHoming = true;

			if (Collision(m_pos, 40.0f, 40.0f, CScene::OBJTYPE_PLAYER, &pScene) == true)
			{
				//スコア加算
				CScore::AddScore(100);

				//経験値を消す
				Uninit();
			}
		}

		if (m_pos.y >= 1200)
		{
			//経験値を消す
			Uninit();
		}
	}
}

//=================================================
//経験値の描画処理
//=================================================
void CExperience::Draw(void)
{
	//基本クラスの描画処理
	CScene2D::Draw();
}

//=================================================
//経験値のテクスチャロード処理
//=================================================
HRESULT CExperience::Load()
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Expre.png", &m_pTexture);

	return S_OK;
}

//=================================================
//経験値のテクスチャアンロード処理
//=================================================
void CExperience::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

