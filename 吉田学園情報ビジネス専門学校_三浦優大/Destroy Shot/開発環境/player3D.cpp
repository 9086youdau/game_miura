//=============================================================================
//
// プレイヤー3D処理 [player3D.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "player3D.h"
#include "manager.h"
#include "model.h"
#include "object3D.h"
#include "camera.h"
#include "shadowS.h"
#include "bullet.h"
#include "passerby.h"
#include "animation.h"
#include "hp.h"
#include "fade.h"
#include "Purpose.h"
#include "score.h"
#include "effect.h"
#include "gamepad.h"
#include "input.h"
#include "sound.h"
#include "game.h"

//==============================================================================
//マクロ定義
//==============================================================================
#define PLAYER_MOVE (1.0f) //プレイヤーの移動速度
#define BULLET_COUNT (25) //弾の発射速度

//==============================================================================
//静的メンバ変数宣言
//==============================================================================
D3DXVECTOR3	CPlayer3D::m_getpos;
D3DXVECTOR3 CPlayer3D::m_rot;
CModel *CPlayer3D::m_apModel[MODEL_PARTS] = {};
CBullet *CPlayer3D::m_pBullet = NULL;
bool CPlayer3D::m_bReset = false; 

//==============================================================================
// コンストラクタ
//==============================================================================
CPlayer3D::CPlayer3D(int nPriority, int nType)
{
	//メンバ変数初期化
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	m_MotionChange = 0;
	m_nNextKey = false;
	m_nHpCount = 3;
	m_bReset = false;
	m_nParticlecount = 0;
	m_bParticle = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
CPlayer3D::~CPlayer3D()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CPlayer3D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//モーション読み込み
	LoadMotion(); 

	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		//モデルを生成
		m_apModel[nCntModel] = CModel::Create(D3DXVECTOR3(m_aOffset[nCntModel][0], m_aOffset[nCntModel][1], m_aOffset[nCntModel][2]),
			D3DXVECTOR3(m_aOffset[nCntModel][3], m_aOffset[nCntModel][4], m_aOffset[nCntModel][5]), &m_pModelName[nCntModel][0]);

		m_apModel[nCntModel]->SetParent(m_apModel[m_aIndexParent[nCntModel]]);
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ローカル変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ
	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_apModel[0]->GetBuff()->GetBufferPointer();

	// ファイル名を使用してテクスチャを読み込む
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/TX_L_William01_Body_D.png",
		&m_pTexture);

	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		//モデルのパーツ分テクスチャを反映させる
		m_apModel[nCntModel]->SetTexture(m_pTexture);
	}

	// 頂点数を取得
	nNumVtx = m_apModel[0]->GetMash()->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(m_apModel[0]->GetMash()->GetFVF());

	// 頂点バッファをロック
	m_apModel[0]->GetMash()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点座標の比較と更新
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 各座標の最大値の比較
		if (m_MaxPlayer.x < vtx.x)
		{
			m_MaxPlayer.x = vtx.x;
		}
		if (m_MaxPlayer.y < vtx.y)
		{
			m_MaxPlayer.y = vtx.y;
		}
		if (m_MaxPlayer.z < vtx.z)
		{
			m_MaxPlayer.z = vtx.z;
		}

		// 各座標の最小値の比較
		if (m_MinPlayer.x > vtx.x)
		{
			m_MinPlayer.x = vtx.x;
		}
		if (m_MinPlayer.y > vtx.y)
		{
			m_MinPlayer.y = vtx.y;
		}
		if (m_MinPlayer.z > vtx.z)
		{
			m_MinPlayer.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_apModel[0]->GetMash()->UnlockVertexBuffer();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pHp[nCnt] = CGame::GetHp(nCnt);
	}

	//位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//初期のモーション
	UpdateMotion(MOTIONTYPE_NEUTRAL,true);

	//影をクリエイト
	CShadowS::Create();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CPlayer3D::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < 13; nCntModel++)
	{
		//NULLチェック
		if (m_apModel[nCntModel] != NULL)
		{
			//モデルを破棄
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
		}
	}

	//オブジェクトの破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CPlayer3D::Update(void)
{
	//位置更新
	m_getpos = m_pos;

	//エフェクト生成
	CEffect::Create(m_pos, D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f), 45, 20.0f, 20.0f);

	if (m_bParticle == false)
	{
		//プレイヤーの移動処理
		PlayerMove();
	}

	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}

	//プレイヤーの当たり判定
	//CollisionPlayer();
}

//==============================================================================
// 描画処理
//==============================================================================
void CPlayer3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ローカル
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きの反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置の反転
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}

//==============================================================================
// 生成処理
//==============================================================================
CPlayer3D * CPlayer3D::Create()
{
	//プレイヤーを生成
	CPlayer3D *pPlayer3D = NULL;
	pPlayer3D = new CPlayer3D;

	//NULLチェック
	if (pPlayer3D != NULL)
	{
		//初期化する
		pPlayer3D->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	//返り値
	return pPlayer3D;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void CPlayer3D::LoadMotion(void)
{
	// ローカル変数宣言
	FILE *pFile = NULL;
	char aEqual[2] = { NULL };					// 「=」読み取り用変数
	bool bComment = false;						// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };			// テキスト読み取り用変数
	char aCharaSet[DATA_LENGTH] = { NULL };		// キャラデータ読み取り用変数
	char aPartsSet[DATA_LENGTH] = { NULL };		// パーツデータ読み取り用変数
	char aMotionSet[DATA_LENGTH] = { NULL };	// モーション読み取り用変数
	char aKeySet[DATA_LENGTH] = { NULL };		// キー設定情報読み取り用変数
	char aKey[DATA_LENGTH] = { NULL };			// キー情報読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pCharaSet = &aCharaSet[0];
	char *pPartsSet = &aPartsSet[0];
	char *pMotionSet = &aMotionSet[0];
	char *pKeySet = &aKeySet[0];
	char *pKey = &aKey[0];

	// データの読み込み
	pFile = fopen("data/TEXT/motion_player00.txt", "r");

	if (pFile != NULL)
	{ //ファイル展開可能
		aText[0] = { NULL };
		aCharaSet[0] = { NULL };
		aPartsSet[0] = { NULL };
		aMotionSet[0] = { NULL };
		aKeySet[0] = { NULL };
		aKey[0] = { NULL };

		//ローカル変数宣言
		int nIdxParts = 0;			// パーツ数
		int nMotion = 0;			// モーションの数
		int nCntFile = 0;			// Xデータの数
		int nAs = 0;

		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// コメントアウトしていない
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// 文字列の先頭が「#」ならばコメントアウトへ
					bComment = true;
				}
				else
				{// 通常時
					if (strcmp("NUM_MODEL", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &m_nNumModel);
					}
					else if (strcmp("MODEL_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &m_pModelName[nCntFile][0]);
						nCntFile++;
					}
					else if (strcmp("CHARACTERSET", pText) == 0)
					{// キャラの情報
						while (strcmp("END_CHARACTERSET", pCharaSet) != 0)
						{// キャラ情報の読み取り
							fscanf(pFile, "%s", &aCharaSet[0]);
							if (strcmp("NUM_PARTS", pCharaSet) == 0)
							{

							}
							else if (strcmp("PARTSSET", pCharaSet) == 0)
							{// パーツ情報
								aPartsSet[0] = { NULL };

								while (strcmp("END_PARTSSET", pPartsSet) != 0)
								{
									//一度モーションの全てを初期化
									for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
									{
										for (int nCntKey = 0; nCntKey < 10; nCntKey++)
										{
											m_nFrame[nCntMotion][nCntKey] = 40;

											for (int nCntPart = 0; nCntPart < 30; nCntPart++)
											{
												for (int nCntPosRot = 0; nCntPosRot < 6; nCntPosRot++)
												{
													//モーションの初期化
													m_Player3D[nCntMotion][nCntKey][nCntPart][nCntPosRot] = m_aOffset[nCntPart][nCntPosRot];
												}
											}
										}
									}

									fscanf(pFile, "%s", &aPartsSet[0]);
									if (strcmp("INDEX", pPartsSet) == 0)
									{// パーツ情報の読み取り
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &nIdxParts);
									}
									else if (strcmp("PARENT", pPartsSet) == 0)
									{// パーツの親情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &m_aIndexParent[nIdxParts]);
									}
									else if (strcmp("POS", pPartsSet) == 0)
									{// パーツの位置情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][1]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][2]);
									}
									else if (strcmp("ROT", pPartsSet) == 0)
									{// パーツの角度情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][3]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][4]);
										fscanf(pFile, "%f", &m_aOffset[nIdxParts][5]);
									}
								}
							}
						}
					}
					else if (strcmp("MOTIONSET", pText) == 0)
					{
						// 変数の初期化
						aMotionSet[0] = { NULL };

						// ローカル変数宣言
						int nLoop = 0;
						int nKeySet = 0;

						while (strcmp("END_MOTIONSET", pMotionSet) != 0)
						{// モーションの読み込み
						 //値を初期化
							fscanf(pFile, "%s", &aMotionSet[0]);
							if (strcmp("LOOP", pMotionSet) == 0)
							{// ループするか
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &nLoop);
								if (nLoop == 1)
								{// １の場合ループする
									m_bLoopKeep = true;
									m_Loop[nMotion] = true;
								}
							}

							else if (strcmp("NUM_KEY", pMotionSet) == 0)
							{// キー数の読み込み
								fscanf(pFile, "%s", &aEqual[0]);
								fscanf(pFile, "%d", &m_nNumKeyKeep);
							}

								if (strcmp("KEYSET", pMotionSet) == 0)
								{// キーの読み込み
								 // ローカル変数宣言
									int nKey = 0;
									// 変数の初期化
									aKeySet[0] = { NULL };

									while (strcmp("END_KEYSET", pKeySet) != 0)
									{// キー情報項目の読み込み
										fscanf(pFile, "%s", &aKeySet[0]);

										if (aKeySet[0] == '#')
										{// コメントアウト
											// ローカル変数宣言
											char c = NULL;
											char d = NULL;
											fscanf(pFile, "%c", &c);
											while (c != '\n' && d != '\n')
											{
												fscanf(pFile, "%s", &aKey[0]);
												fscanf(pFile, "%c", &d);
											}
										}

										if (strcmp("KEY", pKeySet) == 0)
										{// キー情報の読み込み
											aKey[0] = { NULL };
											while (strcmp("END_KEY", pKey) != 0)
											{// 位置・角度の読み込み
												fscanf(pFile, "%s", &aKey[0]);
												if (strcmp("POS", pKey) == 0)
												{// 位置情報の読み込み
													fscanf(pFile, "%s", &aEqual[0]);

													// 位置情報の読み込み
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][0]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][1]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][2]);
												}
												else if (strcmp("ROT", pKey) == 0)
												{// 角度情報の読み込み
													fscanf(pFile, "%s", &aEqual[0]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][3]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][4]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][5]);
												}
											}
											//元のパーツ位置に移動したPOSの値を足す
											m_Player3D[nMotion][nKeySet][nKey][0] += m_aOffset[nKey][0];
											m_Player3D[nMotion][nKeySet][nKey][1] += m_aOffset[nKey][1];
											m_Player3D[nMotion][nKeySet][nKey][2] += m_aOffset[nKey][2];

											nKey++;
										}
										else if (strcmp("FRAME", pKeySet) == 0)
										{// キーフレーム数の読み込み
											fscanf(pFile, "%s", &aEqual[0]);
											fscanf(pFile, "%d", &m_nFrame[nMotion][nKeySet]);
											m_nFrameTotal[nMotion] += m_nFrame[nMotion][nKeySet];
											if (m_Loop[nMotion] == false && nKeySet == (m_NumKey[nMotion] - 1))
											{
												m_nFrameTotal[nMotion] -= m_nFrame[nMotion][nKeySet];
											}
										}
									}

									//キー分プラス
									nKeySet++;
								}
							}

						// キー数の設定
						m_NumKey[nMotion] = m_nNumKeyKeep;

						//モーションのカウントアップ
						nMotion++;
						m_MotionTypeMax++;
					}
				}
			}
			else if (bComment == true)
			{// コメントアウト処理
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
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//==============================================================================
// モーションの更新
//==============================================================================
void CPlayer3D::UpdateMotion(int MOTIONTYPE, bool bUse)
{
	// キー数とループ情報の代入
	m_Key = m_GetKey;
	m_TotalKey = (m_nNumKeyKeep = m_NumKey[MOTIONTYPE]);
	m_bLoopKeep = m_Loop[MOTIONTYPE];

	if (m_bMove == true)
	{
		m_MotionType = m_MotionChange;
	}

	// モーションカウンターの加算
	if (m_bMotionChange == true)
	{// モーションブレンド実行時
		if (m_nCounterMotion < m_nBrendFrame)
		{// カウント上限
			m_nCounterMotion++;
		}
	}
	else
	{// 通常時
		if (m_nCounterMotion < m_nFrame[MOTIONTYPE][m_Key])
		{// カウント上限
			if (bUse == false)
			{
				//モーションのカウントアップ
				m_nCounterMotion++;
			}
		}
	}

	// モデルパーツごとのモーションアニメーション
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
		{// ループして最初に戻るとき
			m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][0][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][0][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][0][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][0][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][0][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][0][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else if (m_bMotionChange == true)
		{// モーションブレンド
			m_aOffset[nCnt][0] = m_aOffsetMemo[nCnt][0] + (m_Player3D[m_MotionType][0][nCnt][0] - m_aOffsetMemo[nCnt][0]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][1] = m_aOffsetMemo[nCnt][1] + (m_Player3D[m_MotionType][0][nCnt][1] - m_aOffsetMemo[nCnt][1]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][2] = m_aOffsetMemo[nCnt][2] + (m_Player3D[m_MotionType][0][nCnt][2] - m_aOffsetMemo[nCnt][2]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][3] = m_aOffsetMemo[nCnt][3] + (m_Player3D[m_MotionType][0][nCnt][3] - m_aOffsetMemo[nCnt][3]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][4] = m_aOffsetMemo[nCnt][4] + (m_Player3D[m_MotionType][0][nCnt][4] - m_aOffsetMemo[nCnt][4]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][5] = m_aOffsetMemo[nCnt][5] + (m_Player3D[m_MotionType][0][nCnt][5] - m_aOffsetMemo[nCnt][5]) * ((float)m_nCounterMotion / (float)m_nBrendFrame);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else
		{// 通常時
			if (m_nNextKey == false && (m_NumKey[MOTIONTYPE] - 1) != 0)
			{
				m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][m_Key + 1][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
				m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
				m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
				m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
				m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
				m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
			}
			if (m_nNextKey == false && (m_NumKey[MOTIONTYPE] - 1) == 0)
			{
				m_aOffset[nCnt][0] = m_Player3D[MOTIONTYPE][m_Key][nCnt][0] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][0] - m_Player3D[MOTIONTYPE][m_Key][nCnt][0]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][1] = m_Player3D[MOTIONTYPE][m_Key][nCnt][1] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][1] - m_Player3D[MOTIONTYPE][m_Key][nCnt][1]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][2] = m_Player3D[MOTIONTYPE][m_Key][nCnt][2] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][2] - m_Player3D[MOTIONTYPE][m_Key][nCnt][2]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][3] = m_Player3D[MOTIONTYPE][m_Key][nCnt][3] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][3] - m_Player3D[MOTIONTYPE][m_Key][nCnt][3]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][4] = m_Player3D[MOTIONTYPE][m_Key][nCnt][4] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][4] - m_Player3D[MOTIONTYPE][m_Key][nCnt][4]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffset[nCnt][5] = m_Player3D[MOTIONTYPE][m_Key][nCnt][5] + (m_Player3D[MOTIONTYPE][m_Key][nCnt][5] - m_Player3D[MOTIONTYPE][m_Key][nCnt][5]) * ((float)m_nCounterMotion / (float)m_nFrame[MOTIONTYPE][m_Key]);
				m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
				m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
				m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
				m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
				m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
				m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
			}
		}

		if (m_bMotionChange == false)
		{// モーション変化なし
			if (m_nCounterMotion == m_nFrame[MOTIONTYPE][m_Key])
			{// キーフレーム終了時
				if (m_Key != m_TotalKey - 2 || m_bLoopKeep == true)
				{// ジャンプ以外でキー終了時リセット
					if (m_nNextKey == false && m_nNextKey == true || m_bLoopKeep == true)
					{
						m_nCounterMotion = 0;
					}
				}

				if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
				{// ループするとき
					m_Key = 0;
					m_GetKey = m_Key;
				}
				else if (m_Key == m_TotalKey - 2 && m_bLoopKeep == false || m_Key == 0 && m_bLoopKeep == false)
				{// ループせず終了するとき
					if (m_Key != 0)
					{
						m_nCounterMotion = m_nFrame[MOTIONTYPE][m_Key];
						m_Key++;
						m_GetKey = m_Key;
					}
					m_nNextKey = true;
				}

				else if (m_Key < m_TotalKey && m_nNextKey != true)
				{// キーのカウント
					m_Key++;
					m_GetKey = m_Key;
				}
			}
			else if (m_MotionTypeOld != m_MotionType)
			{// モーション変化時	
				m_Key = 0;
				m_nCounterMotion = 0;
				m_bMotionChange = true;

				if (m_MotionTypeOld == MOTIONTYPE_NEUTRAL && m_MotionType == MOTIONTYPE_MOVE)
				{// 移動からニュートラル時
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_MOVE && m_MotionType == MOTIONTYPE_NEUTRAL)
				{// ニュートラルから移動時
					m_nBrendFrame = 30;
				}
				//if (m_MotionTypeOld == MOTIONTYPE_LEFTMOVE && m_MotionType == MOTIONTYPE_RIGHTMOVE)
				//{// ニュートラルから移動時
				//	m_nBrendFrame = 10;
				//}
				//if (m_MotionTypeOld == MOTIONTYPE_RIGHTMOVE && m_MotionType == MOTIONTYPE_LEFTMOVE)
				//{// ニュートラルから移動時
				//	m_nBrendFrame = 10;
				//}
				break;
			}
		}
		else if (m_bMotionChange == true)
		{// モーションブレンド時のカウントリセット
			if (m_nCounterMotion == m_nBrendFrame || m_MotionTypeOld != m_MotionType)
			{
				m_Key = 0;
				m_nCounterMotion = 0;
				m_bMotionChange = false;
			}
		}
	}

	// 直前のモーション状態の保存
	m_MotionTypeOld = m_MotionType;
}

//==============================================================================
// プレイヤーの移動処理関数
//==============================================================================
void CPlayer3D::PlayerMove(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//ゲームパッドのポインタ宣言
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//サウンドを取得
	m_pSound = CManager::GetSound();

	//カメラ角度取得
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	// 角度修正
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	//初期のモーション
	UpdateMotion(m_MotionChange, false);

	//位置更新
	m_posold = m_pos;

	// 移動量の加算
	m_pos += m_move;

	//弾発射間隔カウント
	m_nBulletCount++;

	// Wキーのモデルの移動
	if (pInputKeyboard->GetPress(DIK_W) == true || pGamepad->IsButtonPush(CInput::UP) == true)
	{
		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		//カウントアップ
		m_nCounter++;

		//モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		// 左上方向
		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
		{
			//カウントアップ
			m_nCounter++;

			//モーションの変化
			m_MotionChange = MOTIONTYPE_MOVE;

			//移動量計算
			m_move.x -= cosf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z += sinf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY + (D3DX_PI * 3 / 4);
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
		{// 右上方向

			//カウントアップ
			m_nCounter++;

		    //モーションの変化
		    m_MotionChange = MOTIONTYPE_MOVE;
			
			m_move.x += cosf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z -= sinf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY - (D3DX_PI * 3 / 4);
		}
		else
		{// 上方向
			m_move.x += sinf(RotCameraY) * PLAYER_MOVE;
			m_move.z += cosf(RotCameraY) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY + D3DX_PI;
		}
	}

	// Sキーのモデルの移動
	else if (pInputKeyboard->GetPress(DIK_S) == true || pGamepad->IsButtonPush(CInput::DOWN) == true)
	{
		if (m_nMoveCnt < 20)
		{
			//カウントアップ
			m_nMoveCnt++;
		}

		//モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		//カウントアップ
		m_nCounter++;

		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
		{// 左下方向

		    //カウントアップ
			m_nCounter++;

			//モーションの変化
			m_MotionChange = MOTIONTYPE_MOVE;

			//移動量計算
			m_move.x -= cosf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z += sinf(RotCameraY - D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY - D3DX_PI / -4;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
		{// 右下方向

		    //カウントアップ
			m_nCounter++;

			//モーションの変化
			m_MotionChange = MOTIONTYPE_MOVE;

			//移動量計算
			m_move.x += cosf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_move.z -= sinf(RotCameraY + D3DX_PI / 4) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY - D3DX_PI / 4;
		}
		else
		{// 下方向
			m_move.x -= sinf(RotCameraY) * PLAYER_MOVE;
			m_move.z -= cosf(RotCameraY) * PLAYER_MOVE;
			m_rotDest.y = RotCameraY;
		}
	}

	else if (pGamepad->IsButtonPush(CInput::LEFT) == true)
	{// 左方向

	    //モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		//カウントアップ
		m_nCounter++;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		m_move.x -= cosf(RotCameraY) * PLAYER_MOVE;
		m_move.z += sinf(RotCameraY) * PLAYER_MOVE;
		m_rotDest.y = RotCameraY + (D3DX_PI / 2);
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
	{// 右方向

	    //モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		//カウントアップ
		m_nCounter++;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}
		m_move.x += cosf(RotCameraY) * PLAYER_MOVE;
		m_move.z -= sinf(RotCameraY) * PLAYER_MOVE;
		m_rotDest.y = RotCameraY + (D3DX_PI / -2);
	}

	else
	{
		if (m_nMoveCnt > 10)
		{
			//カウントを減らしていく
			m_nMoveCnt--;
		}
	}

	if (pGamepad->IsButtonPush(CInput::LEFT_UP) == true)
	{
		// 左上方向
		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_LEFT) == true)
		{
			m_rotDest.y = RotCameraY - (D3DX_PI * 3 / 4);
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		// 右上方向
		else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_RIGHT) == true)
		{
			m_rotDest.y = RotCameraY + (D3DX_PI * 3 / 4);
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		else
		{
			m_rotDest.y = RotCameraY + D3DX_PI;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
	}
	else if (pGamepad->IsButtonPush(CInput::LEFT_DOWN) == true)
	{
		// 左上方向
		if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_LEFT) == true)
		{
			m_rotDest.y = RotCameraY - D3DX_PI / 4;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		// 右上方向
		else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_RIGHT) == true)
		{
			m_rotDest.y = RotCameraY - D3DX_PI / -4;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
		else
		{
			m_rotDest.y = RotCameraY;
			if (m_nBulletCount % BULLET_COUNT == 0)
			{
				//サウンドBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
				m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
					D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
			}
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_LEFT) == true)
	{
		m_rotDest.y = RotCameraY + (D3DX_PI / -2);
		if (m_nBulletCount % BULLET_COUNT == 0)
		{//サウンドBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
			m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
				D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT_RIGHT) == true)
	{
		m_rotDest.y = RotCameraY + (D3DX_PI / 2);
		if (m_nBulletCount % BULLET_COUNT == 0)
		{
			//サウンドBGM
			m_pSound->Play(CSound::SOUND_LABEL_SE_SHOT);
			m_pBullet = CBullet::Create(D3DXVECTOR3(m_apModel[5]->GetMtxWorld()._41, m_apModel[5]->GetMtxWorld()._42, m_apModel[5]->GetMtxWorld()._43),
				D3DXVECTOR3(sinf(D3DX_PI - m_rot.y) * -35.0f, 0.0f, cosf(D3DX_PI - m_rot.y) * 35.0f), 30);
		}
	}

	// 移動モーションから待機モーションへ
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nMoveCnt == 10)
	{
		//メンバ変数初期化
		m_nCounterMotion = 0;//フレームカウントをリセット
		m_Key = 0;//フレームキーをリセット
		m_GetKey = 0;//フレームキーをリセット
		m_nCounter = 0;
		m_MotionChange = MOTIONTYPE_NEUTRAL;//モーションをニュートラルにする
	}
	// 移動モーションから待機モーションへ
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nCounter == 1)
	{
		//メンバ変数初期化
		m_nCounterMotion = 0;//フレームカウントをリセット
		m_Key = 0;//フレームキーをリセット
		m_GetKey = 0;//フレームキーをリセット
	}

	// 目的の回転角の上限
	if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;

	// 現在の回転角の上限
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		for (int nCntRot = 3; nCntRot < 6; nCntRot++)
		{
			// 現在の回転角の上限
			if (m_aOffset[nCnt][nCntRot] < -D3DX_PI)
			{
				m_aOffset[nCnt][nCntRot] += D3DX_PI * 2.0f;
			}
			else if (m_aOffset[nCnt][nCntRot] > D3DX_PI)
			{
				m_aOffset[nCnt][nCntRot] -= D3DX_PI * 2.0f;
			}
		}
	}

	// 加速後の減速処理
	m_move.x += (0.0f - m_move.x) * 0.07f;
	m_move.z += (0.0f - m_move.z) * 0.07f;

	for (int nCnt = 0; nCnt < 256; nCnt++)
	{
		//オブジェクトへのポインタ生成
		CObject3D *pObj;
		pObj = CGame::GetObject(nCnt);

		if (pObj != NULL)
		{
			//オブジェクト3Dの当たり判定
			pObj->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x - 0.0f, m_MinPlayer.x + 0.0f, m_MaxPlayer.z - 0.0f, m_MinPlayer.z + 0.0f, m_MaxPlayer.y, m_MinPlayer.y);
		}
	}
}

//==============================================================================
// プレイヤーの当たり判定処理
//==============================================================================
void CPlayer3D::CollisionPlayer(void)
{
	//目的の敵の数が0より上だったら
	if (CPurpose::GetPurpose() > 0)
	{
		// 敵との当たり判定
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			for (int nCntPriority = 0; nCntPriority < 3; nCntPriority++)
			{
				// オブジェクト情報の取得
				CScene *pScene = NULL;
				pScene = GetScene(nCntScene, nCntPriority);

				// NULLチェック
				if (pScene != NULL)
				{
					// オブジェクトの種類の取得
					CScene::OBJTYPE objType = pScene->GetObjType();

					//オブジェクトタイプがエネミーだったら
					if (objType == OBJTYPE_ENEMY)
					{
						//エネミーのポインタ取得
						for (int nCnt = 0; nCnt < 256; nCnt++)
						{
							//敵を作成したポインタを取得
							CPasserby *pPasserby = NULL;
							pPasserby = CAnimation::GetPasserby(nCnt);

							//NULLチェック
							if (pPasserby != NULL)
							{
								//弾との当たり判定
								if (m_pos.x + 40.0f >= pPasserby->GetPasserbyPos().x && m_pos.x - 40.0f <= pPasserby->GetPasserbyPos().x&&
									m_pos.z + 40.0f >= pPasserby->GetPasserbyPos().z && m_pos.z - 40.0f <= pPasserby->GetPasserbyPos().z)
								{
									//敵を全て消す
									pScene->Uninit();

									//スコア初期化
									CScore::SetScore(0);

									//敵の目的の数も初期化する
									CPurpose::SetPurpose(CGame::GetEnemyPurpose());

									//リセットのフラグ管理
									m_bParticle = true;
								}
							}
						}
					}
				}
			}
		}
	}
	if (m_bParticle == true)
	{
		if (m_nParticlecount < 200)
		{
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, 1720.0f), 3);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, -1720.0f), 3);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(-2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 3);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				CGame::GameParticle(D3DXVECTOR3(2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 3);
			}
		}

		//パーティクル発生カウント
		m_nParticlecount++;

		//エネミーカウントをリセット
		CGame::SetCount(0);
	}

	if (m_nParticlecount == 200)
	{
		//リセットのフラグ管理
		m_bReset = true;
	}

	//フラグが有効になったら
	if (m_bReset == true)
	{
		if (CGame::GetLife() > 0)
		{
			//プレイヤー初期位置にする
			m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//体力を減らす
			m_pHp[CGame::GetLife()]->Uninit();
			CGame::SetLife(1);

			//エネミーカウントをリセット
			CGame::SetCount(0);

			//初期化する
			m_bReset = false;
			m_bParticle = false;
			m_nParticlecount = 0;
		}
		//ライフが0になったら
		else if (CGame::GetLife() == 0)
		{
			//体力を減らす
			m_pHp[CGame::GetLife()]->Uninit();
			CGame::SetLife(1);

			//自分を消去する
			Uninit();

			//リザルトに行く
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
		}
	}
}

//==============================================================================
// プレイヤー3DのPosのGet関数
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetPosition(void)
{
	//位置情報
	return m_getpos;
}

//==============================================================================
// プレイヤー3DのRotのGet関数
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetRotation(void)
{
	//回転情報
	return m_rot;
}