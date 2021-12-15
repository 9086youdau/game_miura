//===========================================================================
//
//ゲーム処理[game.cpp]
// Author; miura yuudai
//
//===========================================================================

//===========================================================================
//インクルード読み込み
//===========================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "Scene2D.h"
#include "Renderer.h"
#include "input.h"
#include "player.h"
#include "player.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "gamepad.h"
#include "playerattack.h"
#include "bullet.h"

//===========================================================================
//静的メンバ変数宣言
//===========================================================================
CPlayer *CGame::m_pPlayer = NULL;
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
bool CGame::m_Pause = false;
int CGame::m_nCnt = 0;
int CGame::m_nSubCnt = 0;
int CGame::m_nStartCount[MAX_DATA] = {};
int CGame::m_nCountText = 0;

//===========================================================================
//コンストラクタ
//===========================================================================
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
}

//===========================================================================
//デストラクタ
//===========================================================================
CGame::~CGame()
{
}

//===========================================================================
//ゲームの初期化処理
//===========================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//値を初期化
	m_Pause = false;

	//敵配置のテキストの読み込み
	LoadText();

	//背景のテクスチャロード
	CBg::Load();

	//エネミーのテクスチャロード
	CEnemy::Load();

	//プレイヤーのテクスチャロード
	CPlayer::Load();

	//弾のテクスチャロード
	CBullet::Load();

	//背景作成・設定関数
	m_pBg = CBg::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//アタックするプレイヤー部分のテクスチャロード
	CPlayerAttack::Load();

	//アタックするプレイヤー部分のクリエイト
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_RIGHT);
	CPlayerAttack::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f), CPlayerAttack::PLAYERATTACK_LEFT);

	//プレイヤー作成・設定関数
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(570.0f, 1200.0f, 0.0f));

	return S_OK;
}

//===========================================================================
//ゲームの終了処理
//===========================================================================
void CGame::Uninit(void)
{
	//背景のアンロード
	CBg::Unload();

	//プレイヤーのアンロード
	CPlayer::Unload();

	//エネミーのアンロード
	CEnemy::Unload();

	//弾のテクスチャアンロード
	CBullet::Unload();

	if (m_pBg != NULL)
	{
		//終了処理
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	//破棄関数
	Release();
}

//===========================================================================
//ゲームの更新処理
//===========================================================================
void CGame::Update(void)
{
	//敵の配置操作用関数呼び出し
	EnemyTool();
}

//===========================================================================
//ゲームの描画処理
//===========================================================================
void CGame::Draw(void)
{
}

//===========================================================================
//エネミーの動きをテキストから読み込み
//===========================================================================
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
				if (strcmp("PLAYER_BULLET_DAMAGE", pText) == 0)
				{
					//敵のカウント
					fscanf(pFile, "%s", &aEqual[0]);
					fscanf(pFile, "%d", &m_BulletDamage);
				}

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
						if (strcmp("EnemyHp", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nLife[m_nCountText]);
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

//===========================================================================
// ツール操作用関数
//===========================================================================
void CGame::EnemyTool(void)
{
	//ゲームパッドのポインタ宣言
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//ゲームパッドのポインタ宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ポーズ画面
	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_PAUSE) == true || pInputKeyboard->GetTrigger(DIK_P) == true)
	{
		m_Pause = m_Pause ? false : true;
	}

	//ポーズ状態ではない場合
	if (m_Pause == false)
	{
		//カウントアップ
		m_nCnt++;
	}

	//指定したカウントで再生
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetTrigger(DIK_F8) == true)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(OBJTYPE_ENEMY, nCnt);

			if (pScene != NULL)
			{
				//全ての敵を消滅させる
				pScene->Uninit();
			}
		}

		//エネミーカウントに保存したカウントを代入
		m_nCnt = m_nSubCnt;
	}
	//カウントを調整する
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetPress(DIK_F6) == true)
	{
		//エネミーカウントをリセットする
		m_nSubCnt++;
	}

	//カウントを調整する
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetPress(DIK_F4) == true)
	{
		//エネミーカウントをリセットする
		m_nSubCnt--;
	}

	//カウントをリセットする
	if (pInputGamePad->IsButtonPush(CInput::KEYINFO_LEFT) == true || pInputKeyboard->GetTrigger(DIK_F5) == true)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(OBJTYPE_ENEMY, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				//全ての敵を消滅させる
				pScene->Uninit();
			}
		}
		//エネミーカウントをリセットする
		m_nCnt = 0;
	}

	//カウントで管理
	for (int nCnt = 0; nCnt < m_nCountText; nCnt++)
	{
		if (m_nCnt == m_nStartCount[nCnt])//テキストのカウント数がきたら
		{
			//エネミーのクリエイト
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(m_Pos[nCnt].x, m_Pos[nCnt].y, m_Pos[nCnt].z), m_Size[nCnt].x, m_Size[nCnt].y, (CEnemy::ENEMYTYPE)m_EnemyType[nCnt], m_fEnemySpeed[nCnt], m_nHomingCount[nCnt], m_nLife[nCnt], m_BulletDamage);
		}
	}

	//カウントの制御
	if (m_nSubCnt < 0)
	{
		//0より下になった場合最大数を代入
		m_nSubCnt = m_nStartCount[m_nCountText - 1];
	}
	else if (m_nSubCnt > m_nStartCount[m_nCountText - 1])
	{
		//最大数より上になったら値を0に戻す
		m_nSubCnt = 0;
	}
}

//===========================================================================
//ゲームの作成・設定処理
//===========================================================================
CGame * CGame::Create(void)
{
	//インスタンス生成
	CGame *pGame= NULL;
	pGame = new CGame;

	//初期化呼び出し
	pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pGame;
}
