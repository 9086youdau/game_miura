//================================================
//
//ゲーム処理[game.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "player.h"
#include "explosion.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "number.h"
#include "score.h"
#include "effect.h"
#include "particle.h"
#include "hp.h"
#include "life.h"
#include "title.h"
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "Graze.h"
#include "pause.h"
#include "gamepad.h"
#include "ui.h"
#include "bomb.h"
#include "CircleHp.h"
#include "camera.h"
#include "laser.h"
#include "playerattack.h"
#include "Directing.h"
#include "SceneX.h"

//=================================================
//静的メンバ変数宣言
//=================================================
CPlayer *CGame::m_pPlayer = NULL;
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CScore *CGame::m_pScore = NULL;
CLife *CGame::m_pLife = NULL;
CGraze *CGame::m_pGraze = NULL;
CPause *CGame::m_pPause = NULL;
CBomb *CGame::m_pBomb = NULL;
CBomb *CGame::m_pBomb2 = NULL;
CBomb *CGame::m_pBomb3 = NULL;
CUi *CGame::m_pUi = NULL;
CDirecting *CGame::m_Directing = NULL;
bool CGame::m_Pause = false;

//=================================================
//コンストラクタ
//=================================================
CGame::CGame()
{
	//カウントメンバ変数初期化
	m_nCnt = 0;
	m_nCountText = 0;
	for (int nCnt = 0; nCnt < MAX_DATA; nCnt++)
	{
		m_nInterval[nCnt] = 0;
		m_EnemyType[nCnt] = 0;
		m_Size[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Pos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_nNumberEnemy = 0;
}

//=================================================
//デストラクタ
//=================================================
CGame::~CGame()
{
}

//=================================================
//ゲームの初期化処理
//=================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//グレイズの初期化
	CGraze::CalculatioGraze(0);

	//アタックポイントの初期化
	CLaser::SetPlayerAttack(false);

	//値を初期化
	m_Pause = false;

	//ライフ管理
	CBullet::SetLifeRecovery(3);

	//サウンドのポインタ宣言
	CSound *pSound;
	pSound = CManager::GetSound();

	//BGM設定
	pSound->Stop(pSound->SOUND_LABEL_BGM000);

	//BGM設定
	pSound->Play(pSound->SOUND_LABEL_BGM001);

	//敵配置のテキストの読み込み
	LoadText();

	//背景のテクスチャロード
	CBg::Load();

	//弾のテクスチャロード
	CBullet::Load();

	//レーザーのテクスチャロード
	CLaser::Load();

	//爆発のテクスチャロード
	CExplosion::Load();

	//エネミーのテクスチャロード
	CEnemy::Load();

	//スコアのテクスチャロード
	CNumber::Load();

	//演出面のテクスチャロード
	CDirecting::Load();

	//エフェクトのテクスチャロード
	CEffect::Load();

	//パーティクルのテクスチャロード
	CParticle::Load();

	//ボムのテクスチャロード
	CBomb::Load();

	//HPのテクスチャロード
	CHp::Load();

	//プレイヤーのテクスチャロード
	CPlayer::Load();

	//背景作成・設定関数
	m_pBg = CBg::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//スコアの作成・設定関数
	m_pScore = CScore::Create(CScore::SCORETYPE_GAME_SCORE);

	//グレイズの作成・設定関数
	m_pGraze = CGraze::Create(1550.0f, 325.0f, 20.0f, 20.0f);

	//体力の作成・設定処理
	m_pLife = CLife::Create();

	//アタックするプレイヤー部分のテクスチャロード
	CPlayerAttack::Load();

	//アタックするプレイヤー部分のクリエイト
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_RIGHT);
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_LEFT);

	//uiのテクスチャロード
	CUi::Load();

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//uiをクリエイト
		CUi::Create(D3DXVECTOR3(1050.0f, (nCnt * 100.0f) + 40.0f, 0.0f), 300.0f, 850.0f, CUi::UITYPE_LINE);
	}

	//uiをクリエイト
	CUi::Create(D3DXVECTOR3(1120.0f, 80.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_MAXSCORE);
	CUi::Create(D3DXVECTOR3(1120.0f, 180.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_SCORE);
	CUi::Create(D3DXVECTOR3(1120.0f, 280.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_GRAZE);
	CUi::Create(D3DXVECTOR3(1120.0f, 380.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_LIFE);
	CUi::Create(D3DXVECTOR3(1120.0f, 480.0f, 0.0f), 100.0f, 400.0f, CUi::UITYPE_BOMB);

	//uiをクリエイト
	CUi::Create(D3DXVECTOR3(950.0f, 650.0f, 0.0f), 400.0f, 1100.0f, CUi::UITYPE_LOGO);

	//ボムのクリエイト生成
	m_pBomb = CBomb::Create(D3DXVECTOR3(1535.0f, 500.0f, 0.0f));
	m_pBomb2 = CBomb::Create(D3DXVECTOR3(1568.0f, 500.0f, 0.0f));
	m_pBomb3 = CBomb::Create(D3DXVECTOR3(1601.0f, 500.0f, 0.0f));

	//プレイヤー作成・設定関数
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f));

	//シーンXのクリエイト
	CSceneX::Create(D3DXVECTOR3(-5.0f, -25.0f, 0.0f),CSceneX::SCENEXTYPE_LEFT);
	CSceneX::Create(D3DXVECTOR3(-30.0f, -25.0f, 100.0f),CSceneX::SCENEXTYPE_LEFT);
	CSceneX::Create(D3DXVECTOR3(-60.0f, -25.0f, 200.0f),CSceneX::SCENEXTYPE_LEFT);
	CSceneX::Create(D3DXVECTOR3(-90.0f, -25.0f, 300.0f),CSceneX::SCENEXTYPE_LEFT);

	//シーンXのクリエイト
	CSceneX::Create(D3DXVECTOR3(-135.0f, -25.0f, 0.0f),CSceneX::SCENEXTYPE_RIGHT);
	CSceneX::Create(D3DXVECTOR3(-175.0f, -25.0f, 100.0f),CSceneX::SCENEXTYPE_RIGHT);
	CSceneX::Create(D3DXVECTOR3(-205.0f, -25.0f, 200.0f),CSceneX::SCENEXTYPE_RIGHT);
	CSceneX::Create(D3DXVECTOR3(-235.0f, -25.0f, 300.0f),CSceneX::SCENEXTYPE_RIGHT);

	return S_OK;
}

//=================================================
//ゲームの終了処理
//=================================================
void CGame::Uninit(void)
{
	//背景のアンロード
	CBg::Unload();

	//プレイヤーのアンロード
	CPlayer::Unload();

	//弾のアンロード
	CBullet::Unload();

	//レーザーのテクスチャアンロード
	CLaser::Unload();

	//爆発のアンロード
	CExplosion::Unload();

	//エネミーのアンロード
	CEnemy::Unload();

	//スコアのテクスチャアンロード
	CNumber::Unload();

	//エフェクトのテクスチャアンロード
	CEffect::Unload();

	//パーティクルのテクスチャアンロード
	CParticle::Unload();

	//体力のテクスチャアンロード
	CHp::Unload();

	//uiのテクスチャアンロード
	CUi::Unload();

	//ボムのテクスチャアンロード
	CBomb::Unload();

	//演出面のテクスチャアンロード
	CDirecting::Unload();

	//経験値のアンテクスチャロード
	CExplosion::Unload();

	if (m_pBg != NULL)
	{
		//終了処理
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	else if (m_pScore != NULL)
	{
		//終了処理
		m_pScore->Uninit();
		m_pScore = NULL;
	}

	else if (m_pGraze != NULL)
	{
		//終了処理
		m_pGraze->Uninit();
		m_pGraze = NULL;
	}

	else if (m_pLife != NULL)
	{
		//終了処理
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	else if (m_Directing != NULL)
	{
		//終了処理
		m_Directing->Uninit();
		m_Directing = NULL;
	}

	//破棄関数
	Release();
}

//=================================================
//ゲームの更新処理
//=================================================
void CGame::Update(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//ゲームパッドのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ローカル変数宣言
	bool benemy = CDirecting::GetEnemy();
	bool bbomb = CBomb::GetBomb();

	//ポーズ画面
	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_PAUSE) == true || pInputKeyboard->GetTrigger(DIK_P) == true)
	{
		m_Pause = m_Pause ? false : true;
	}

	//ボムの破棄処理
	if (bbomb == true && m_bombCnt == 0)
	{
		//ボムのUi破棄
		if (m_pBomb != NULL)
		{
			m_pBomb->Uninit();
			m_pBomb = NULL;
		}

		//初期化
		CBomb::SetBomb(false);

		//カウントアップ
		m_bombCnt++;

		//値をゲットしてくる
		bbomb = CBomb::GetBomb();
	}
	//ボムの破棄処理
	if (bbomb == true && m_bombCnt == 1)
	{
		//ボムのUi破棄
		if (m_pBomb2 != NULL)
		{
			m_pBomb2->Uninit();
			m_pBomb2 = NULL;
		}

		//初期化
		CBomb::SetBomb(false);

		//カウントアップ
		m_bombCnt++;

		//値をゲットしてくる
		bbomb = CBomb::GetBomb();
	}
	//ボムの破棄処理
	if (bbomb == true && m_bombCnt == 2)
	{
		//ボムのUi破棄
		if (m_pBomb3 != NULL)
		{
			m_pBomb3->Uninit();
			m_pBomb3 = NULL;
		}
	}

	//ポーズ状態ではない場合
	if (m_Pause == false)
	{
		//カウントアップ
		m_nCnt++;

		//カウントで管理
		if (m_nCnt == 100)
		{
			//演出のクリエイト
			m_Directing = CDirecting::Create(D3DXVECTOR3(170.0f, 200.0f, 0.0f), 200, 800, CDirecting::FADE_OUT, CDirecting::DIRECTINGTYPE_START);
		}

		//カウントで管理
		for (int nCnt = 0; nCnt < m_nCountText; nCnt++)
		{
			if (m_nCnt == m_nStartCount[nCnt])//テキストのカウント数がきたら
			{
				//エネミーのクリエイト
				m_pEnemy = CEnemy::Create(D3DXVECTOR3(m_Pos[nCnt].x, m_Pos[nCnt].y, m_Pos[nCnt].z), m_Size[nCnt].x, m_Size[nCnt].y, (CEnemy::ENEMYTYPE)m_EnemyType[nCnt],m_fEnemySpeed[nCnt], m_nHomingCount[nCnt]);
			}
		}
	}
}
//=================================================
//ゲームの描画処理
//=================================================
void CGame::Draw(void)
{
}

//=================================================
//エネミーの動きをテキストから読み込み
//=================================================
void CGame::LoadText(void)
{
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aSetObject[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/EnemySet.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// コメントアウトしていない
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// 文字列の先頭が「#」ならばコメントアウトへ
				bComment = true;
			}

			else
			{
				if (strcmp("ENEMYCOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_ENEMYCOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("StartCount", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nStartCount[m_nCountText]);
						}
						if (strcmp("Pos", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_Pos[m_nCountText].x);
							fscanf(pFile, "%f", &m_Pos[m_nCountText].y);
							fscanf(pFile, "%f", &m_Pos[m_nCountText].z);
						}
						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_Size[m_nCountText].x);
							fscanf(pFile, "%f", &m_Size[m_nCountText].y);
						}
						if (strcmp("EnemyType", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_EnemyType[m_nCountText]);
						}
						if (strcmp("EnemyMove", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fEnemySpeed[m_nCountText]);
						}
						if (strcmp("HomingCount", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nHomingCount[m_nCountText]);
						}
					}
					//カウントアップ
					m_nCountText++;
				}
			}
		}
		else if (bComment == true)
		{
			// コメントアウト処理
			// ローカル変数宣言
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}

//=================================================
//ゲームの作成・設定処理
//=================================================
CGame * CGame::Create(void)
{
	//インスタンス生成
	CGame *pGame= NULL;
	pGame = new CGame;

	//初期化呼び出し
	pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pGame;
}
