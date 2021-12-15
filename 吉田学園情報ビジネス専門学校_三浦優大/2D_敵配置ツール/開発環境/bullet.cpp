//=============================================================================
//
// バレット処理[bullet.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include <time.h>
#include "bullet.h"
#include "input.h"
#include "manager.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "enemy.h"
#include "player.h"
#include "bg.h"
#include "fade.h"
#include "game.h"
#include "sound.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture[2] = {};
CBullet::BULLETTYPE CBullet::m_bulletplayerType = CBullet::BULLETTYPE_NONE;
CBullet::BULLETTYPE CBullet::m_bulletenemyType = CBullet::BULLETTYPE_NONE;
int CBullet::m_nLifeCount = 3;
bool CBullet::m_playerattack = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	//メンバ変数初期化
	m_nLife = 700;                                //弾の体力
	m_nCountGraze = 0;                            //グレイズ用のカウント変数　　　　　　　　　　　
	m_han = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //データ保存用
	m_han2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //データ保存用
	m_posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //敵の打つ玉の位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        //位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //移動量
	m_bulletplayerType = BULLETTYPE_NONE;         //バレットタイプ
	m_bulletenemyType = BULLETTYPE_NONE;          //バレットタイプ
	m_bulletbomb = BULLETTYPEDAMAGE_NONE;         //バレットタイプ
	m_posplayerBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//プレイヤーの弾
	m_BombBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //ボムの弾の位置
	m_playerattack = false;                       //攻撃する所フラグ
	m_fAngle = 0.0f;                              //角度保存用変数
	m_omega = 0.0f;                               //弾のカウント用変数
}
//=============================================================================
//デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}
//=============================================================================
//バレットの設定・更新処理
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType, BULLETTYPEDAMAGE bulletbomb, float omega, float fHight, float fWidth,float fAngle)
{
	//インスタンス生成
	CBullet *pBullet = NULL;
	pBullet = new CBullet;

	//初期化呼び出し
	pBullet->Init(pos, bulletplayerType, bulletenemyType, fHight, fWidth, bulletbomb, omega, fAngle);

	//タイプ別にテクスチャ切り替え
	if (bulletplayerType == BULLETTYPE_PLAYER)
	{
		//テクスチャ呼び出し
		pBullet->BindTexture(m_pTexture[0]);
	}
	else if (bulletenemyType == BULLETTYPE_ENEMY)
	{
		//テクスチャ呼び出し
		pBullet->BindTexture(m_pTexture[1]);
	}

	//ポインタ返り値
	return pBullet;
 }
//=============================================================================
//バレットの初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos,BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb, float omega,float fAngle)
{
	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);
		
	//値を代入
	m_fAngle = fAngle;

	//関数呼び出し
	SetBullet(pos, bulletplayerType, bulletenemyType, bulletbomb, omega, fHight, fWidth);

	return S_OK;
}
//=============================================================================
//バレットの終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}
//=============================================================================
//バレットの更新処理
//=============================================================================
void CBullet::Update(void)
{
	//ポーズの代入
	bool pause = false;
	pause = CGame::GetPause();

	//ポーズ状態では無い場合
	if (pause == false)
	{
		//バレットのタイプ分け処理
		switch (m_bulletbomb)
		{
		case BULLETTYPEDAMAGE_NOMALSHOT:
			//タイプ分け処理
			SetObjType(CScene::OBJTYPE_PLAYER_BULLET);
			MoveNomalBullet(m_posplayerBullet.x, m_posplayerBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 20.0f, m_omega, 10.0f);
			break;
		}

		//当たり判定関数呼び出し
		CollisionBullet();

		//ライフを減らす
		m_nLife--;

		//ライフが0になったら
		if (m_nLife == 0)
		{
			//終了処理
			Uninit();
		}
	}
}
//=============================================================================
//バレットの描画処理
//=============================================================================
void CBullet::Draw(void)
{
	//基本クラスの描画処理
	CScene2D::Draw();
}

//=============================================================================
//バレットのテクスチャ処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/cardbullet.png", &m_pTexture[0]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
//バレットのテクスチャの廃棄処理
//=============================================================================
void CBullet::Unload(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
//バレットのセット処理
//=============================================================================
void CBullet::SetBullet(D3DXVECTOR3 pos, BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,BULLETTYPEDAMAGE bulletbomb,float omega,float fHight, float fWidth)
{
	if (bulletplayerType == BULLETTYPE_PLAYER)
	{
		//位置の引数から代入
		m_pos = pos;

		//横幅を代入
		m_fWidth = fWidth;

		//縦幅を代入
		m_fHight = fHight;

		//タイプ分け
		m_bulletplayerType = bulletplayerType;

		//弾の種類分け
		m_bulletbomb = bulletbomb;

		//弾の角度変更
		m_omega = omega;
	}
	else if (bulletenemyType == BULLETTYPE_ENEMY)
	{
		//位置の引数から代入
		m_pos = pos;

		//横幅を代入
		m_fWidth = fWidth;

		//縦幅を代入
		m_fHight = fHight;

		//タイプ分け
		m_bulletenemyType = bulletenemyType;

		//弾の種類分け
		m_bulletbomb = bulletbomb;

		//弾の角度変更
		m_omega = omega;
	}
}

//=============================================================================
//バレットの当たり判定処理
//=============================================================================
void CBullet::CollisionBullet(void)
{
	//ローカル変数宣言
	//プレイヤーの位置情報
	D3DXVECTOR3 playerpos;
	playerpos = CPlayer::GetPlayer();

	//ポインタをNULLにする
	CScene *pScene = NULL;

	//プレイヤーの弾と敵の当たり判定
	if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY, &pScene) == true)
	{
		//エネミーの敵の位置を代入
		D3DXVECTOR3 EnemyPos = pScene->GetPosition();

		//ポインタをエネミーに設定する
		CEnemy *pEnemy = (CEnemy*)pScene;
		pEnemy->SetEnemyState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

		//弾の削除
		Uninit();
	}

	//プレイヤーの弾と敵の当たり判定
	else if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BOSS, &pScene) == true)
	{
		//エネミーの敵の位置を代入
		D3DXVECTOR3 EnemyPos = pScene->GetPosition();

		//ポインタをエネミーに設定する
		CEnemy *pEnemy = (CEnemy*)pScene;
		pEnemy->SetEnemyState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

		//弾の削除
		Uninit();
	}
}
//=============================================================================
//バレットの移動処理処理
//=============================================================================
void CBullet::MoveLoopingBullet2(float& x, float& y,float cx, float cy, float& vx, float& vy, float& r, float vr, float theta,float omega)
{
	// 角度を変化させる
	theta += omega;

	// 半径を変化させる
	r += vr;

	// 位置を計算する
	x = cx + r * cosf(theta);
	y = cy + r * sinf(theta);

	// 弾の速度（必要な場合だけ）
	vx = -r * omega * sinf(theta);
	vy = r  * omega * cosf(theta);

	//弾の位置更新
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

	//引数から代入
	m_omega = theta;
}

//=============================================================================
//バレットの移動処理処理
//=============================================================================
void CBullet::MoveNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
{ 
	// 半径を変化させる
	r += vr;

	// 位置を計算する
	x = cx + r * cosf(D3DX_PI * 1.5f);
	y = cy + r * sinf(D3DX_PI * 1.5f);

	//引数から代入
	m_omega = theta;

	//弾の位置更新
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}
}

//=============================================================================
//バレットの移動処理処理
//=============================================================================
void CBullet::MoveEnemyNomalBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
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
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

	//引数から代入
	m_omega = theta;
}

//=============================================================================
//バレットの移動処理処理
//=============================================================================
void CBullet::MoveEnemySircleBullet(float& x, float& y, float cx, float cy, float& vx, float& vy, float& r, float vr, float theta, float omega)
{
	// 半径を変化させる
	r += vr;

	// 位置を計算する
	x = cx + r * cosf(theta);
	y = cy + r * sinf(theta);

	// 弾の速度（必要な場合だけ）
	vx = -r * omega * sinf(theta);
	vy = r  * omega * cosf(theta);

	//弾の位置更新
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

	//引数から代入
	m_omega = theta;
}
//=============================================================================
//バレットの移動処理処理
//=============================================================================
void CBullet::MoveEnemyLoopingBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float theta, float omega)
{
	// 角度を変化させる
	theta += omega;

	// 半径を変化させる
	r += vr;

	// 位置を計算する
	x = cx + r * cosf(theta);
	y = cy + r * sinf(theta);

	// 弾の速度（必要な場合だけ）
	vx = -r * omega * sinf(theta);
	vy = r  * omega * cosf(theta);

	//弾の位置更新
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

	//引数から代入
	m_omega = theta;
}

//=============================================================================
//バレットの移動処理処理
//=============================================================================
void CBullet::MoveEnemyCountBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float & R, float VR,float angle)
{
	// 半径を変化させる
	r += vr;

	// 位置を計算する
	x = cx + r * cosf(D3DX_PI * angle);
	y = cy + r * sinf(D3DX_PI * angle);

	//弾の位置更新
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

}
//=============================================================================
//バレットの移動処理処理
//=============================================================================
void CBullet::MoveHomingBullet(float& x, float& y, float& vx, float& vy, float& rx,float& ry ,float ex, float ey,float fAngle)
{
	////敵の座標に追尾
	//rx = ex - x;
	//ry = ey - y;

	//fAngle = atan2f(rx, ry);//ベクトルの角度を求める

	//// 弾の速度（必要な場合だけ）
	//vx = sinf(fAngle) * 2.0f;
	//vy = cosf(fAngle) * 2.0f;

	////位置更新計算
	//x += vx;
	//y += vy;

	////弾の位置更新
	//SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	////エフェクト処理
	////CEffect::Create(D3DXVECTOR3(x, y, 0.0f));
}

//=============================================================================
//バレットのライフカウント処理
//=============================================================================
void CBullet::SetLife(int Damage)
{
	//引数から代入
	m_nLifeCount -= Damage;
}

//=============================================================================
//バレットのライフ初期化処理
//=============================================================================
void CBullet::SetLifeRecovery(int Recovery)
{
	//引数から代入
	m_nLifeCount = Recovery;
}

	
