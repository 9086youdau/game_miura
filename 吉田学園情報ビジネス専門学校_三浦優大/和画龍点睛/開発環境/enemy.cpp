//=============================================================================
//
// エネミー処理[enemy.cpp]
// Author; miura yuudai
//
//=============================================================================

//=================================================
//インクルード読み込み
//=================================================
#include <time.h>
#include "enemy.h"
#include "Renderer.h"
#include "manager.h"
#include "bullet.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "pause.h"
#include "CircleHp.h"
#include "laser.h"
#include "Experience.h"
#include "effect.h"
#include "particle.h"
#include "game.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_SIZE_PLAYER_BULLET (30)

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[ENEMYTYPE_MAX] = {};
D3DXVECTOR3 CEnemy::m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
CEnemy::ENEMY_DAMAGE_STATE CEnemy::m_EnemyState = ENEMY_DAMAGE_STATE_NONE;

//=============================================================================
//コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	//メンバ変数の初期化
	m_nPatternAnim = 0;
	m_nLife = 100;
}
//=============================================================================
//デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}
//=============================================================================
//エネミーの初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, float fHight, float fWidth, ENEMYTYPE enemyType,float fMove, int nHoming)
{
	//基本クラスの初期化処理
	CScene2D::Init(pos, fHight, fWidth);

	//基本クラスの色初期化処理
	SetEnemyCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//関数呼び出し
	SetTex(1.0f, m_nPatternAnim);

	//値を代入
	m_enemyType = enemyType;

	//エネミーのタイプ分岐
	switch (m_enemyType)
	{
	case ENEMYTYPE_ONE:
		//タイプ分け処理
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_TWO:
		//タイプ分け処理
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_THREE:
		//タイプ分け処理
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_FOR:
		//タイプ分け処理
		CScene::SetObjType(CScene::OBJTYPE_ENEMY);
		break;
	case ENEMYTYPE_BOSS:
		//タイプ分け処理
		CScene::SetObjType(CScene::OBJTYPE_ENEMY_BOSS);
		break;
	}

	//状態がダメージ状態だったら
	if (m_enemyType == ENEMYTYPE_BOSS)
	{
		//hpをクリエイト
		CCircleHp::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 150.0f, 150.0f, CCircleHp::CIRCLEHP_OUT);
		//hpをクリエイト
		CCircleHp::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 150.0f, 150.0f, CCircleHp::CIRCLEHP_IN);
	}

	//引数から値を代入
	m_fMove = fMove;
	m_HomingText = nHoming;

	//返り値
	return S_OK;
}
//=============================================================================
//エネミーの終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	//基本クラスの終了処理
	CScene2D::Uninit();
}
//=============================================================================
//エネミーの更新処理
//=============================================================================
void CEnemy::Update(void)
{
	//ポーズの代入
	bool pause = false;
	pause = CGame::GetPause();

	//ポーズ状態じゃなかったら
	if (pause == false)
	{
		//カウントアップ
		m_nBulletCount++;

		//状態がダメージ状態だったら
		if (m_EnemyState == ENEMY_DAMAGE_STATE_DAMAGE)
		{
			//カウントを減らす
			m_nCountEnemy--;

			//カウントが0以下になったら
			if (m_nCountEnemy <= 0)
			{
				//エネミーのモードを通常状態に変更する
				ENEMY_DAMAGE_STATE_NONE;
				//エネミーの色を元に戻す
				SetCol(D3DXCOLOR(255, 255, 255, 255));
			}
		}

		//エネミーのタイプ分岐
		switch (m_enemyType)
		{
		case ENEMYTYPE_ONE:
			//moveの関数呼び出し
			EnemyMoveOne();
			break;
		case ENEMYTYPE_TWO:
			//moveの関数呼び出し
			EnemyMoveTwo();
			break;
		case ENEMYTYPE_THREE:
			//moveの関数呼び出し
			EnemyMoveThree();
			break;
		case ENEMYTYPE_FOR:
			//moveの関数呼び出し
			EnemyMoveFor();
			break;
		case ENEMYTYPE_BOSS:
			//moveの関数呼び出し
			EnemyBossMove();
			break;
		}
	}
}
//=============================================================================
//エネミーの描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	//基本クラスの描画
	CScene2D::Draw();
}

//=============================================================================
//バレットのテクスチャ処理
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[0]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[1]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[2]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[3]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[4]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy10.png", &m_apTexture[5]);

	return S_OK;
}

//=============================================================================
//バレットのテクスチャの廃棄処理
//=============================================================================
void CEnemy::Unload(void)
{
	//配列分ループする
	for (int nCnt = 0; nCnt < ENEMYTYPE_MAX; nCnt++)
	{
		//テクスチャの開放
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
//エネミーの設定・更新処理
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, float fHight, float fWidth,ENEMYTYPE enemyType,float fMove, int nHoming)
{
	//インスタンス生成
	CEnemy *pEnemy;
	pEnemy = new CEnemy;

	//初期化処理
	pEnemy->Init(pos, fHight, fWidth, enemyType, fMove, nHoming);

	//テクスチャ設定
	pEnemy->BindTexture(m_apTexture[enemyType]);

	return pEnemy;
}
//=============================================================================
//エネミーの動き処理
//=============================================================================
void CEnemy::EnemyMoveOne(void)
{
	//ローカル変数宣言
	float fAngle;
	float rad = 0.0f;

	//変数代入
	fAngle = D3DX_PI * 2 / 10;//弾の範囲
	rad = fAngle / 2;

	//値を代入
	m_pos = GetPosition();

	//カウント加算
	m_nEnemy++;

	//カウントが300までいったら
	if (m_nEnemy < 300)
	{
		//移動量
		m_move.y += 0.5f + m_fMove;
	}
	else if (m_nEnemy >= 300)
	{
		//移動量0.5にする
		m_move.y -= 0.3f + m_fMove;
	}

	if (m_nBulletCount % 125 == 0)
	{
		//弾のクリエイト
		for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++, rad += fAngle)
		{
			//弾のクリエイト
			CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULEETTYPEDAMAGE_SIRCLESHOT, rad, 20.0f, 20.0f, 0.0f);
		}
	}

	//移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.3f;

	//位置更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 85.0f, 85.0f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.6f), CEffect::EFFECTTYPE_NONE);

	//ポリゴンの位置関数
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//画面外処理
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//終了処理
		Uninit();
	}
}

//=============================================================================
//エネミーの動き処理
//=============================================================================
void CEnemy::EnemyMoveTwo(void)
{
	//値を代入
	m_pos = GetPosition();

	//一定のラインまで行ったら
	if (m_pos.y >= 20)
	{
		//カウントアップ
		m_nCountParticle -= 0.6f;
	}

	//ローカル変数宣言
	float fAngle;

	fAngle = (m_nCountParticle) / 100.0f + D3DX_PI * 2.0f;
	m_move.x = sinf(fAngle) * 2.5f;
	m_move.y = cosf(fAngle) * 2.5f;

	if (m_nBulletCount % 105 == 0)
	{
		//弾のクリエイト
		CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYNOMALSHOT, m_fRad, 25.0f, 25.0f, 0.0f);
	}

	//移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.2f + m_fMove;
	m_move.y += (0.0f - m_move.y) * 0.2f + m_fMove;

	//位置更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 85.0f, 85.0f, D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.6f), CEffect::EFFECTTYPE_NONE);

	//ポリゴンの位置関数
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//画面外処理
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//終了処理
		Uninit();
	}
}

//=============================================================================
//エネミーの動き処理
//=============================================================================
void CEnemy::EnemyMoveThree(void)
{
	//値を代入
	m_pos = GetPosition();

	//一定のラインまで行ったら
	if (m_pos.y >= 20)
	{
		//カウントアップ
		m_nCountParticle += 0.6f;
	}

	float rad = 0.0f;

	if (m_nBulletCount % 105 == 0)
	{
		//弾のクリエイト
		CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYNOMALSHOT, m_fRad, 25.0f, 25.0f,0.0f);
	}

	//ローカル変数宣言
	float fAngle;

	fAngle = (m_nCountParticle) / 100.0f + D3DX_PI * 2.0f;
	m_move.x = sinf(fAngle) * 2.5f + m_fMove;
	m_move.y = cosf(fAngle) * 2.5f + m_fMove;

	//移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//位置更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 85.0f, 85.0f,D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.6f), CEffect::EFFECTTYPE_NONE);

	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//画面外処理
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//終了処理
		Uninit();
	}
}
//=============================================================================
//エネミーの動き処理
//=============================================================================
void CEnemy::EnemyMoveFor(void)
{
	//値を代入
	m_pos = GetPosition();

	//カウントを回す
	m_HomingCount++;

	if (m_HomingCount <= m_HomingText)
	{
		m_vec = CPlayer::GetPlayer() - m_pos;//現在地から目的地へのベクトル
		m_homingAngle = atan2f(m_vec.x, m_vec.y);//ベクトルの角度を求める

		if (m_nBulletCount % 85 == 0)
		{
			//弾のクリエイト
			CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYNOMALSHOT, m_fRad, MAX_SIZE_PLAYER_BULLET, MAX_SIZE_PLAYER_BULLET, 0.0f);
		}

		m_move.x = sinf(m_homingAngle) * (1.0f + m_fMove);//移動量を求める
		m_move.y = cosf(m_homingAngle) * (1.0f + m_fMove);//移動量を求める
	}
	else
	{ 
		m_move.y += (m_fMove / 2 + 0.5f);
	}

	//移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//位置更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//エフェクト処理
	CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), 100.0f, 100.0f,D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), CEffect::EFFECTTYPE_NONE);

	//ポリゴンの位置関数
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//画面外処理
	if (m_pos.x >= 1300 || m_pos.x <= 0 || m_pos.y <= 0 || m_pos.y >= 1300)
	{
		//終了処理
		Uninit();
	}
}
//=============================================================================
//エネミーの動き処理
//=============================================================================
void CEnemy::EnemyBossMove(void)
{
	//ローカル変数宣言
	float fAngle;
	float rad = 0.0f;

	//変数代入
	fAngle = D3DX_PI * 2 / 40;//弾の範囲
	rad = fAngle / 2;

	//値を代入
	m_pos = GetPosition();

	//カウントアップ　
	m_BossCount++;
	m_SircleCnt++;

	//移動量
	m_move.y += sinf(D3DX_PI * 0.5f) * 0.5f + m_fMove;

	//ボスのposが300以上になったら
	if (m_pos.y >= 300)
	{
		//移動量0.0にする
		m_move.y = 0.0f;
	}

	//カウントが700以下だったら
	if (m_BossCount <= 700)
	{
		if (m_BossCount % 150 == 0)
		{
			//弾のクリエイト
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++, rad += fAngle)
			{
				//弾のクリエイト
				CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULEETTYPEDAMAGE_SIRCLESHOT, rad, 20.0f, 20.0f, 0.0f);
			}
		}
		if (m_BossCount % 350 == 0)
		{
			for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++, rad += fAngle)
			{
				//弾のクリエイト
				CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYMOVESIRCLE, rad, 20.0f, 20.0f, 0.0f);
			}
		}
	}

	//カウントが700以上の時
	if (m_BossCount >= 700)
	{
		if (m_SircleCnt % 5 == 0)
		{
			//角度変更
			m_fAngle += 0.05f;

			//弾のクリエイト
			CBullet::Create(m_pos, CBullet::BULLETTYPE_NONE, CBullet::BULLETTYPE_ENEMY, CBullet::BULLETTYPEDAMAGE_ENEMYCOUNT, rad, 20.0f, 20.0f, m_fAngle);
		}
	}

	//移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	//位置更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;

	//ポリゴンの位置関数
	SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//敵の状態引数
//=============================================================================
void CEnemy::SetEnemyState(ENEMY_DAMAGE_STATE damage,int nLifeDamage)
{
	//値を代入
	m_pos = GetPosition();

	//引数から代入
	m_EnemyState = damage;

	//引数から体力をひく
	m_nLife -= nLifeDamage;

	//体力が0以上だったら
	if (m_nLife >= 0)
	{
		//ダメージ状態にする
		if (damage == ENEMY_DAMAGE_STATE_DAMAGE)
		{
			//カウントを設定
			m_nCountEnemy = 10;

			//色変更
			SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f));
		}
	}
	else
	{
		for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
		{
			//パーティクルを生成
			CParticle::Create(m_pos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_PLAYER);
		}

		//乱数系列の変更
		srand((unsigned)time(NULL));

		for (int nCntExperience = 0; nCntExperience < 5; nCntExperience++)
		{
			//経験値をだす
			CExperience::Create(D3DXVECTOR3(m_pos.x + rand() % 100 + 10, m_pos.y + rand() % 100 + 10, 0.0f), 35.0f, 35.0f);
		}

		//敵をけす
		Uninit();
	}
}

//=============================================================================
//ボスの状態処理
//=============================================================================
void CEnemy::SetEnemyBossState(ENEMY_DAMAGE_STATE damage)
{
	//値を代入
	m_pos = GetPosition();

	//引数から代入
	m_EnemyState = damage;

	//ダメージ状態にする
	if (damage == ENEMY_DAMAGE_STATE_DAMAGE)
	{
		//カウントを設定
		m_nCountEnemy = 10;

		//色変更
		SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f));

		for (int nCntAppear = 0; nCntAppear < 40; nCntAppear++)
		{
			//パーティクルを生成
			CParticle::Create(m_pos, D3DXCOLOR(255, 255, 25, 255), CParticle::PARTICLE_ENEMY);
		}
	}
}



