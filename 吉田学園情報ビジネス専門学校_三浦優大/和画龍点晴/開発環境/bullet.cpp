//================================================
//
//バレット処理[bullet.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "bullet.h"
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
#include "game.h"
#include "sound.h"
#include "Experience.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture[2] = {};
CBullet::BULLETTYPE CBullet::m_bulletplayerType = CBullet::BULLETTYPE_NONE;
CBullet::BULLETTYPE CBullet::m_bulletenemyType = CBullet::BULLETTYPE_NONE;
CEnemy *CBullet::m_pEnemy = NULL;
int CBullet::m_nLifeCount = 3;
int g_debug;
bool CBullet::m_playerattack = false;

//=============================================================================
//コンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{

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
	pBullet->Init(pos, bulletplayerType, bulletenemyType,fHight, fWidth,bulletbomb,omega,fAngle);

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

	return pBullet;
 }
//=============================================================================
//バレットの初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos,BULLETTYPE bulletplayerType, BULLETTYPE bulletenemyType,float fHight, float fWidth, BULLETTYPEDAMAGE bulletbomb, float omega,float fAngle)
{
	//初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//移動量の初期化
	m_nLife = 700;
	m_nCountGraze = 0;
	m_han.y = 0.0f;
	m_posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bulletplayerType = BULLETTYPE_NONE;
	m_bulletenemyType = BULLETTYPE_NONE;
	m_bulletbomb = BULLETTYPEDAMAGE_NONE;
	m_posplayerBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BombBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_playerattack = false;
		
	//値を代入
	m_fAngle = fAngle;

	//関数呼び出し
	SetBullet(pos, bulletplayerType, bulletenemyType,bulletbomb,omega,fHight,fWidth);

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

	//ローカル変数宣言
	D3DXVECTOR3 playerpos;//プレイヤーの位置情報
	playerpos = CPlayer::GetPlayer();

	if (pause == false)
	{
		//バレットのタイプ
		switch (m_bulletbomb)
		{
		case BULLETTYPEDAMAGE_BOMB:
			MoveLoopingBullet2(m_BombBullet.x, m_BombBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 1.0f, m_omega, 0.05f);
			break;
		case BULLETTYPEDAMAGE_NOMALSHOT:
			//タイプ分け処理
			SetObjType(CScene::OBJTYPE_PLAYER_BULLET);
			MoveNomalBullet(m_posplayerBullet.x, m_posplayerBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 20.0f, m_omega, 10.0f);
			break;
		case BULLETTYPEDAMAGE_ENEMYNOMALSHOT:
			//タイプ分け処理
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			//カラー変更関数呼び出し
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemyNomalBullet(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 10.0f, m_omega, 10.0f);
			break;
		case BULEETTYPEDAMAGE_SIRCLESHOT:
			//タイプ分け処理
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemySircleBullet(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 2.0f, m_omega, 10.0f);
			break;
		case BULLETTYPEDAMAGE_ENEMYMOVESIRCLE:
			//タイプ分け処理
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemyLoopingBullet2(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 2.0f, m_omega, 0.008f);
			break;
		case BULLETTYPEDAMAGE_ENEMYCOUNT:
			//タイプ分け処理
			SetObjType(CScene::OBJTYPE_ENEMY_BULLET);
			SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
			MoveEnemyCountBullet2(m_posBullet.x, m_posBullet.y, m_pos.x, m_pos.y, m_move.x, m_move.y, m_han.y, 1.5f, m_han2.y, 1.0f, m_fAngle);
			break;
		case BULLETTYPEDAMAGE_ENEMYHOMING:
			//MoveHomingBullet(m_posBullet.x, m_posBullet.y, m_move.x, m_move.y, m_vec.x, m_vec.y, playerpos.x, playerpos.y, m_vecangle);
			break;
		}

		//ポインタをNULLにする
		CScene *pScene = NULL;

		//プレイヤーの弾と敵の当たり判定
		if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY, &pScene) == true)
		{
			//エネミーの敵の位置を代入
			D3DXVECTOR3 EnemyPos = pScene->GetPosition();

			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
			{
				//パーティクルを生成
				CParticle::Create(EnemyPos, D3DXCOLOR(15, 15, 255, 255), CParticle::PARTICLE_ENEMY);
			}

			//ポインタをエネミーに設定する
			CEnemy *pEnemy = (CEnemy*)pScene;
			pEnemy->SetEnemyState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE, 5);

			//弾の削除
			Uninit();
		}

		//グレイズの当たり判定処理
		if (playerpos.x - 50 <= m_posBullet.x && m_posBullet.x <= playerpos.x + 50 &&
			playerpos.y - 50 <= m_posBullet.y && m_posBullet.y <= playerpos.y + 50)
		{
			if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
			{
				//カウントアップ
				m_nCountGraze++;

				//カウントが1の時
				if (m_nCountGraze == 1)
				{
					//かすり判定
					CGraze::AddGraze(1);
				}
			}
		}

		//敵の弾とプレイヤーの当たり判定
		if (Collision(m_posBullet, m_fWidth, m_fHight, CScene::OBJTYPE_PLAYER, &pScene) == true)
		{
			if (CPlayer::GetState() == CPlayer::PLAYERSTATE_NOMAL)
			{
				for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
				{
					//パーティクルを生成
					CParticle::Create(playerpos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_PLAYER);
				}

				//ライフカウントが-1の時
				if (m_nLifeCount == -1)
				{
					m_nLifeCount = 3;
				}

				//HPを減らす処理
				CLife::SetRelease(m_nLifeCount);

				//カウントアップ
				m_nLifeCount--;

				if (m_nLifeCount > -1)
				{
					//プレイヤーの終了処理
					pScene->Uninit();

					//初期からリセット
					CPlayer::Create(D3DXVECTOR3(550.0f, 1200.0f, 0.0f));

					//状態変化関数
					CPlayer::SetStatePlayer(CPlayer::PLAYERSTATE_DAMAGE);
				}
				else if (m_nLifeCount == -1)
				{
					//プレイヤーの終了処理
					pScene->Uninit();

					//プレイヤーアタックをtrueに変更
					m_playerattack = true;

					//ライフリセット
					m_nLifeCount = 3;

					//ゲーム画面へ遷移
					CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
				}

				//弾の削除
				Uninit();
			}
		}

		//敵の弾とボムの当たり判定
		else if (Collision(m_BombBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BULLET, &pScene) == true)
		{
			//弾の削除
			pScene->Uninit();
		}

		//敵とボムの当たり判定
		else  if (Collision(m_BombBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY, &pScene) == true)
		{
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
			{
				//パーティクルを生成
				CParticle::Create(pScene->GetPosition(), D3DXCOLOR(25, 255, 25, 255), CParticle::PARTICLE_PLAYER);
			}

			for (int nCntExperience = 0; nCntExperience < 10; nCntExperience++)
			{
				//経験値をだす
				CExperience::Create(D3DXVECTOR3(pScene->GetPosition().x + rand() % 100 + 1, pScene->GetPosition().y + rand() % 100 + 1, 0.0f), 25.0f, 25.0f);
			}

			//エネミーの敵の位置を代入
			D3DXVECTOR3 EnemyPos = pScene->GetPosition();

			//爆発処理
			CExplosion::Create(EnemyPos);

			//弾の削除
			pScene->Uninit();
		}

		//敵とボムの当たり判定
		if (Collision(m_posplayerBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BOSS, &pScene) == true)
		{
			//弾の終了処理
			Uninit();

			//ライフを減らす処理
			CCircleHp::SetBossLife(1);

			//ポインタをエネミーに設定する
			CEnemy *pEnemy = (CEnemy*)pScene;
			pEnemy->SetEnemyBossState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

			if (CCircleHp::GetBossLife() == 50)
			{
				//ボスの終了処理
				pScene->Uninit();

				//ゲーム画面へ遷移
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}

		//敵とボムの当たり判定
		if (Collision(m_BombBullet, m_fWidth, m_fHight, CScene::OBJTYPE_ENEMY_BOSS, &pScene) == true)
		{
			//弾の終了処理
			Uninit();

			//ライフを減らす処理
			CCircleHp::SetBossLife(1);

			//ポインタをエネミーに設定する
			CEnemy *pEnemy = (CEnemy*)pScene;
			pEnemy->SetEnemyBossState(CEnemy::ENEMY_DAMAGE_STATE_DAMAGE);

			if (CCircleHp::GetBossLife() == 50)
			{
				//ボスの終了処理
				pScene->Uninit();

				//ゲーム画面へ遷移
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
			}
		}

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
//バレットの移動処理
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

	//エフェクト処理
	//CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 120.0f, 120.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

	//引数から代入
	m_omega = theta;
}

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

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 50.0f, 50.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f), CEffect::EFFECTTYPE_NONE);

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

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 40.0f, 40.0f, D3DXCOLOR(1.0f, 1.0f, 0.2f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

	//引数から代入
	m_omega = theta;
}

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

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 50.0f, 50.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

	//引数から代入
	m_omega = theta;
}

void CBullet::MoveEnemyCountBullet2(float & x, float & y, float cx, float cy, float & vx, float & vy, float & r, float vr, float & R, float VR,float angle)
{
	// 半径を変化させる
	r += vr;

	// 位置を計算する
	x = cx + r * cosf(D3DX_PI * angle);
	y = cy + r * sinf(D3DX_PI * angle);

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(x, y, 0.0f), 50.0f, 50.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f),CEffect::EFFECTTYPE_NONE);

	//弾の位置更新
	SetPosition(D3DXVECTOR3(x, y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面外処理
	if (x >= 1300 || x <= 0 || y <= 0 || y >= 1300)
	{
		//終了処理
		Uninit();
	}

}
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

void CBullet::SetLife(int Damage)
{
	//引数から代入
	m_nLifeCount -= Damage;
}

void CBullet::SetLifeRecovery(int Recovery)
{
	//引数から代入
	m_nLifeCount = Recovery;
}

	
