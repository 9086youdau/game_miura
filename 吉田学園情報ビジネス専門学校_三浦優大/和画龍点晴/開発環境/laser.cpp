//================================================
//
//レーザー処理[laser.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "laser.h"
#include "input.h"
#include "manager.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "hp.h"
#include "score.h"
#include "number.h"
#include "bg.h"
#include "life.h"
#include "Graze.h"
#include "fade.h"
#include "CircleHp.h"
#include "bullet.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CLaser::m_pTexture = NULL;
CLaser::LASERTYPE CLaser::m_laserType = CLaser::LASERTYPE_NONE;
CEnemy *CLaser::m_pEnemy = NULL;
int CLaser::m_nLifeCount = 3;

//=============================================================================
//コンストラクタ
//=============================================================================
CLaser::CLaser()
{

}
//=============================================================================
//デストラクタ
//=============================================================================
CLaser::~CLaser()
{

}
//=============================================================================
//レーザーの設定・更新処理
//=============================================================================
CLaser * CLaser::Create(D3DXVECTOR3 pos, LASERTYPE laserType, LASERTYPEDAMAGE laserdamage, float omega, float fHight, float fWidth, float fAngle,D3DXVECTOR3 rot,int nLife)
{
	//インスタンス生成
	CLaser *pLaser = NULL;
	pLaser = new CLaser;

	//初期化呼び出し
	pLaser->Init(pos, laserType, fHight, fWidth, laserdamage, omega, fAngle,rot,nLife);

	//テクスチャ呼び出し
	pLaser->BindTexture(m_pTexture);

	return pLaser;
}
//=============================================================================
//レーザーの初期化処理
//=============================================================================
HRESULT CLaser::Init(D3DXVECTOR3 pos, LASERTYPE laserType, float fHight, float fWidth, LASERTYPEDAMAGE laserdamage, float omega, float fAngle,D3DXVECTOR3 rot,int nLife)
{
	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	if (laserdamage == LASERTYPEDAMAGE_ENEMYNOMALSHOT)
	{
		//カラー変更関数呼び出し
		SetCol(D3DXCOLOR(0, 0, 255, 255));
	}
	else if (laserdamage == LASERTYPEDAMAGE_ENEMYNOTICE)
	{
		//カラー変更関数呼び出し
		SetCol(D3DXCOLOR(255, 255, 255, 255));
	}

	//移動量の初期化
	m_nCountGraze = 0;
	m_han.y = 0.0f;

	//値を代入
	m_fAngle = fAngle;

	//関数呼び出し
	SetLaser(pos, laserType, laserdamage, omega, fHight, fWidth, rot, nLife);

	return S_OK;
}
//=============================================================================
//レーザーの終了処理
//=============================================================================
void CLaser::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}
//=============================================================================
//レーザーの更新処理
//=============================================================================
void CLaser::Update(void)
{
	//バレットのタイプ
	switch (m_laserbomb)
	{
	case LASERTYPEDAMAGE_ENEMYNOMALSHOT:
		MoveEnemyNomalLaser(m_posLaser.x, m_posLaser.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 0.0f, m_omega, 0.0f);
		break;
	case LASERTYPEDAMAGE_ENEMYNOTICE:
		MoveEnemyNomalLaser(m_posLaser.x, m_posLaser.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 0.0f, m_omega, 0.0f);
		break;
	}

	//当たり判定の処理関数呼び出し
	CollisionLaser();

	//ライフを減らす
	m_nLife--;

	//ライフが0になったら
	if (m_nLife <= 0)
	{
		//終了処理
		Uninit();
	}
}
//=============================================================================
//レーザーの描画処理
//=============================================================================
void CLaser::Draw(void)
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
//レーザーのテクスチャ処理
//=============================================================================
HRESULT CLaser::Load(void)
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
//レーザーのテクスチャの廃棄処理
//=============================================================================
void CLaser::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//レーザーの移動処理
//=============================================================================
void CLaser::SetLaser(D3DXVECTOR3 pos, LASERTYPE laserType, LASERTYPEDAMAGE laserdamage, float omega, float fHight, float fWidth,D3DXVECTOR3 rot,int nLife)
{
	if (laserType == LASERTYPE_ENEMY)
	{
		//位置の引数から代入
		m_pos = pos;

		//横幅を代入
		m_fWidth = fWidth;

		//縦幅を代入
		m_fHight = fHight;

		//タイプ分け
		m_laserType = laserType;

		//弾の種類分け
		m_laserbomb = laserdamage;

		//弾の角度変更
		m_omega = omega;

		//レーザー角度変更
		m_Rot = rot;

		//レーザーのライフ
		m_nLife = nLife;
	}
}

//=============================================================================
//レーザーの当たり判定処理
//=============================================================================
void CLaser::CollisionLaser(void)
{
	//ローカル変数宣言
	D3DXVECTOR3 playerpos;//プレイヤーの位置情報
	playerpos = CPlayer::GetPlayer();

	// 矩形の中心を原点とした相対座標を作成する
	D3DXVECTOR2 relateve_position = D3DXVECTOR2(playerpos.x - m_posLaser.x, playerpos.y - m_posLaser.y);

	// 相対座標に対して矩形の回転を打ち消す逆行列を掛ける
	D3DXVECTOR2 transform_pos = D3DXVECTOR2(cosf(-m_Rot.z) * relateve_position.x + sinf(-m_Rot.z) * relateve_position.y,
	-sinf(-m_Rot.z) * relateve_position.x + cosf(-m_Rot.z) * relateve_position.y);

	//変数変換
	D3DXVECTOR3 laserpos = transform_pos;

	//ポインタをNULLにする
	CScene *pScene = NULL;

	//敵とボムの当たり判定
	if (Collision(laserpos, m_fWidth, m_fHight, CScene::OBJTYPE_PLAYER, &pScene) == true)
	{
		pScene->Uninit();
	}
}

//=============================================================================
//レーザーの動き方処理
//=============================================================================
void CLaser::MoveEnemyNomalLaser(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
{
	// 半径を変化させる
	r += vr;

	// 位置を計算する
	x = cx + r * cosf(D3DX_PI * -1.5f);
	y = cy + r * sinf(D3DX_PI * -1.5f);

	// 弾の速度（必要な場合だけ）
	vx = -r * omega * sinf(D3DX_PI * -1.5f);
	vy = r  * omega * cosf(D3DX_PI * -1.5f);

	//弾の位置更新
	SetPosition(D3DXVECTOR3(x, y, 0.0f), m_Rot);

	//引数から代入
	m_omega = theta;
}