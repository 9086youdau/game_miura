//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルードファイル読み込み
//=============================================================================
#include "game.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "object3D.h"
#include "player3D.h"
#include "meshfield.h"
#include "scene3D.h"
#include "passerby.h"
#include "effect.h"
#include "animation.h"
#include "hp.h"
#include "ui.h"
#include "score.h"
#include "number.h"
#include "particle.h"
#include "Purpose.h"
#include "bullet.h"
#include "sound.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CObject3D *CGame::m_pObject3D[256] = {};                 //オブジェクトの保存用ポインタメンバ変数
int CGame::m_nGParicleMax[DATA_GAME_LENGTH] = {};             //パーティクルの発生数
float CGame::m_GSizeX[DATA_GAME_LENGTH] = {};                 //パーティクルのサイズX
float CGame::m_GSizeY[DATA_GAME_LENGTH] = {};                 //パーティクルのサイズY
float CGame::m_fGSpeed[DATA_GAME_LENGTH] = {};                //パーティクルの移動速度
D3DXCOLOR CGame::m_GParticlecol[DATA_GAME_LENGTH] = {};       //パーティクルのカラー情報
int CGame::m_nGLife[DATA_GAME_LENGTH] = {};                   //パーティクルの寿命
float CGame::m_fGGravity[DATA_GAME_LENGTH] = {};              //パーティクルの重力
CHp *CGame::m_pHp[4] = {};                               //プレイヤーのライフUI
int CGame::m_nCnt = 0;                                   //敵の出現カウント
int CGame::m_nHpCount = 0;                               //プレイヤーのHPカウント
int CGame::m_nCountEnemyText = 0;                        //敵の出現回数

//==============================================================================
// コンストラクタ
//==============================================================================
CGame::CGame()
{
	//メンバ変数初期化
	m_nHpCount = 3;
	m_nCnt = 0;
	m_nCountEnemyText = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CGame::~CGame()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//サウンドを取得
	m_pSound = CManager::GetSound();

	//サウンドBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM001);

	//テキスト読み込み
	LoadEnemy();    //敵の情報
	LoadObject();   //オブジェクト情報
	LoadParticle(); //パーティクル情報

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// フォントオブジェクトの生成
	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFontGame);	

	//メッシュフィールドのテクスチャロード
	CMeshfield::Load();

	//メッシュフィールド
	CMeshfield::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	//uiのテクスチャロード
	CUi::Load();
	
	//ui生成
	CUi::Create(D3DXVECTOR3(50.0f, 30.0f, 0.0f), 110.0f, 400.0f, 0);
	CUi::Create(D3DXVECTOR3(1400.0f, 30.0f, 0.0f), 120.0f, 500.0f,0);
	CUi::Create(D3DXVECTOR3(700.0f, 30.0f, 0.0f), 120.0f, 500.0f, 0);
	CUi::Create(D3DXVECTOR3(700.0f, 37.0f, 0.0f), 100.0f, 300.0f, 8);

	//体力のテクスチャロード
	CHp::Load();

	//体力生成
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pHp[nCnt] = CHp::Create(D3DXVECTOR3((nCnt * 70.0f) + 100.0f, 50.0f, 0.0f), 80.0f, 80.0f);
	}

	//3Dプレイヤーの生成
	CPlayer3D::Create();

	//アニメーションテクスチャロード
	CAnimation::Load();

	//スコアのテクスチャロード
	CNumber::Load();

	//パーティクルのテクスチャロード
	CParticle::Load();

	//エフェクトのテクスチャロード
	CEffect::Load();

	//バレットのテクスチャロード
	CBullet::Load();

	//テキストから読み込まれた数分回す
	for (int nCnt = 0; nCnt < m_nCountObjectText; nCnt++)
	{
		//オブジェクト生成
		m_pObject3D[nCnt] = CObject3D::Create(m_ObjectPos[nCnt], m_ObjectRot[nCnt], &m_sObjectName[m_nModelType[nCnt]][0], &m_sTextureName[m_nTextureType[nCnt]][0]);
	}

	//スコア生成
	CScore::Create(D3DXVECTOR3(1480.0f, 87.0f, 0.0f));
	//スコア初期化
	CScore::SetScore(0);

	//目的生成
	CPurpose::Create();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CGame::Uninit(void)
{
	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//サウンドの破棄
	m_pSound->Stop(CSound::SOUND_LABEL_BGM001);

	//スコアのテクスチャアンロード
	CNumber::Unload();

	//uiのテクスチャアンロード
	CUi::Unload();

	//体力のテクスチャアンロード
	CHp::Unload();

	//アニメーションテクスチャアンロード
	CAnimation::Unload();

	//パーティクルのテクスチャアンロード
	CParticle::Unload();

	//エフェクトのテクスチャアンロード
	CEffect::Unload();

	//バレットのテクスチャアンロード
	CBullet::Unload();

	//メッシュフィールドのアンテクスチャロード
	CMeshfield::Unload();

	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CGame::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//bulletの位置取得
	D3DXVECTOR3 pos = CPlayer3D::GetPosition();

	//敵のカウントをアップ
	m_nCnt++;

	//テキストから読み込まれた数分回す
	for (int nCnt = 0; nCnt < m_nCountEnemyText; nCnt++)
	{
		if (m_nCnt == m_nStartEnemyText[nCnt])//テキストのカウント数がきたら
		{
			//エネミーのクリエイト
			CAnimation::Create(m_EnemyPos[nCnt], m_EnemySize[nCnt].x, m_EnemySize[nCnt].y, nCnt);
		}
	}

	// Wキーのモデルの移動
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
	}

	//パーティクル生成)
	GameParticle(D3DXVECTOR3(2000.0f, 0.0f, 1760.0f), 4);
	GameParticle(D3DXVECTOR3(2000.0f, 0.0f, -1760.0f), 4);
	GameParticle(D3DXVECTOR3(-2000.0f, 0.0f, 1760.0f), 4);
	GameParticle(D3DXVECTOR3(-2000.0f, 0.0f, -1760.0f), 4);
}

//==============================================================================
// 描画処理
//==============================================================================
void CGame::Draw(void)
{

}

//==============================================================================
// 生成処理
//==============================================================================
CGame * CGame::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CGame *pGame;
	pGame = new CGame;

	// NULLチェック
	if (pGame != NULL)
	{
		pGame->Init(pos, fSizeX, fSizeY);
	}

	return pGame;
}

//===========================================================================
//エネミーの動きをテキストから読み込み
//===========================================================================
void CGame::LoadEnemy(void)
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
							fscanf(pFile, "%d", &m_nStartEnemyText[m_nCountEnemyText]);
						}
						if (strcmp("Pos", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_EnemyPos[m_nCountEnemyText].x);
							fscanf(pFile, "%f", &m_EnemyPos[m_nCountEnemyText].y);
							fscanf(pFile, "%f", &m_EnemyPos[m_nCountEnemyText].z);
						}
						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_EnemySize[m_nCountEnemyText].x);
							fscanf(pFile, "%f", &m_EnemySize[m_nCountEnemyText].y);
						}
					}
					//カウントアップ
					m_nCountEnemyText++;
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
//オブジェクトの位置をテキストから読み込み
//===========================================================================
void CGame::LoadObject(void)
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

	pFile = fopen("data/TEXT/ObjectSet.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	if (pFile != NULL)
	{ //ファイル展開可能

		int nModelCntFile = 0;			// Xデータの数
		int nTextureCntFile = 0;		// テクスチャの数

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
					if (strcmp("NUM_MODEL", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &m_nObjectCount);
					}
					if (strcmp("TEXTURE_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_sTextureName[nTextureCntFile][0]);
						nTextureCntFile++;
					}
					if (strcmp("MODEL_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_sObjectName[nModelCntFile][0]);
						nModelCntFile++;
					}
					else if (strcmp("OBJECTCOUNTSET", aText) == 0)
					{
						pSetObjectEND[0] = { NULL };

						while (strcmp("END_OBJECTCOUNTSET", pSetObjectEND) != 0)
						{
							fscanf(pFile, "%s", &pSetObjectEND[0]);

							if (strcmp("Pos", pSetObjectEND) == 0)
							{
								//敵のカウント
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &m_ObjectPos[m_nCountObjectText].x);
								fscanf(pFile, "%f", &m_ObjectPos[m_nCountObjectText].y);
								fscanf(pFile, "%f", &m_ObjectPos[m_nCountObjectText].z);
							}
							if (strcmp("Rot", pSetObjectEND) == 0)
							{
								//敵のカウント
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%f", &m_ObjectRot[m_nCountObjectText].x);
								fscanf(pFile, "%f", &m_ObjectRot[m_nCountObjectText].y);
								fscanf(pFile, "%f", &m_ObjectRot[m_nCountObjectText].z);
							}

							if (strcmp("ModelType", pSetObjectEND) == 0)
							{
								//敵のカウント
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nModelType[m_nCountObjectText]);
							}
							if (strcmp("TextureType", pSetObjectEND) == 0)
							{
								//敵のカウント
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nTextureType[m_nCountObjectText]);
							}
						}
						//カウントアップ
						m_nCountObjectText++;
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
}
void CGame::SetCount(int nCnt)
{
	//引数から代入
	m_nCnt = nCnt;
}
//===========================================================================
//パーティクルの設定をテキストから読み込み
//===========================================================================
void CGame::LoadParticle(void)
{
	FILE *pFile = NULL;

	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[128] = { NULL };			// テキスト読み取り用変数
	char aSetObject[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// テキスト読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/ParticleSet.txt", "r");

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
				if (strcmp("PARTICLECOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_PARTICLECOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GSizeX[m_nGParicleText]);
							fscanf(pFile, "%f", &m_GSizeY[m_nGParicleText]);
						}
						if (strcmp("Speed", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGSpeed[m_nGParicleText]);
						}
						if (strcmp("Col", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].r);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].g);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].b);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].a);
						}
						if (strcmp("Life", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGLife[m_nGParicleText]);
						}
						if (strcmp("Gravity", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGGravity[m_nGParicleText]);
						}
						if (strcmp("ParticleMax", pSetObjectEND) == 0)
						{
							//敵のカウント
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGParicleMax[m_nGParicleText]);
						}
					}
					//カウントアップ
					m_nGParicleText++;
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
//パーティクルの設定
//===========================================================================
void CGame::GameParticle(D3DXVECTOR3 pos,int nType)
{
	for (int nCnt = 0; nCnt < m_nGParicleMax[nType]; nCnt++)
	{
		//パーティクル発生
		CParticle::Create(pos, m_GSizeX[nType], m_GSizeY[nType], m_fGSpeed[nType], m_GParticlecol[nType], m_nGLife[nType], m_fGGravity[nType]);
	}
}
//===========================================================================
//プレイヤーのライフ設定
//===========================================================================
void CGame::SetLife(int nLife)
{
	//引数から代入
	m_nHpCount -= nLife;
}