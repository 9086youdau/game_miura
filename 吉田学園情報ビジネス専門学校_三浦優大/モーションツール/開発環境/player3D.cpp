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

//==============================================================================
//静的メンバ変数宣言
//==============================================================================
D3DXVECTOR3	CPlayer3D::m_pos;
D3DXVECTOR3 CPlayer3D::m_rot;
MOTIONTYPE CPlayer3D::m_MotionTypeMax = MOTIONTYPE_MAX;
int CPlayer3D::m_MotionType = MOTIONTYPE_NEUTRAL;
int CPlayer3D::m_KeySet[MOTIONTYPE_MAX] = {};
int CPlayer3D::m_nNumKeyKeep = 0;
int CPlayer3D::m_GetKey = 0;
int CPlayer3D::m_MotionChange = 0;
int CPlayer3D::m_nFrame[MOTIONTYPE_MAX][10] = {};
int CPlayer3D::m_nFrameTotal[MOTIONTYPE_MAX] = {};
int CPlayer3D::m_nCounterMotion = 0;
int CPlayer3D::m_nNumModel = 0;
CModel *CPlayer3D::m_apModel[MODEL_PARTS] = {};
char *CPlayer3D::m_SelectData[MODEL_PARTS + 1][256] = {};
int CPlayer3D::m_DataCount = 0;
float CPlayer3D::m_aOffset[MODEL_PARTS][6] = {};

//==============================================================================
// コンストラクタ
//==============================================================================
CPlayer3D::CPlayer3D(int nPriority, int nType)
{
	//メンバ変数初期化
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	m_bUse = false;
	m_MotionChange = 0;
	m_AddKey = 0;
	m_nCntMat = 0;
	m_nCntMatOld = 1;
	m_bData = false;
	m_SelectData[0][256] = "●";
	for (int nCntData = 1; nCntData < m_nNumModel; nCntData++)
	{
		//選択しているモデル表示ログ設定
		m_SelectData[nCntData][256] = "u";
	}
	m_OldDataCount = 1;
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
	////メンバ変数初期化
	//for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	//{
	//	for (int nCnt = 0; nCnt < 10; nCnt++)
	//	{
	//		m_nFrame[nCntMotion][nCnt] = 40;
	//	}
	//}

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

	//位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//初期のモーション
	UpdateMotion(MOTIONTYPE_NEUTRAL,false);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_apModel[m_nCntMat]->GetBuff()->GetBufferPointer();

	//マテリアルの色変更
	pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);

	 //頂点バッファをアンロック
	m_apModel[m_nCntMat]->GetMash()->UnlockVertexBuffer();

	//影をクリエイト
	CShadowS::Create();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CPlayer3D::Uninit(void)
{
	//オブジェクトの破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CPlayer3D::Update(void)
{
	//関数呼び出し
	MotionTool();

	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
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

	//オブジェクトへのポインタ生成
	//CObject3D *pObj;
	//pObj = CGame::GetObject();

	//オブジェクトの当たり判定
	//pObj->CollisionVec(&m_pos, &m_posold, &m_move, m_MaxPlayer.x, m_MinPlayer.x, m_MaxPlayer.z, m_MinPlayer.z, m_MaxPlayer.y, m_MinPlayer.y);
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

							//値を初期化
							//パーツ分回す
							for (int nCnt = 0; nCnt < 10; nCnt++)
							{
								for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
								{
									//オフセットの初期化
									m_Player3D[nMotion][nCnt][nCntKey][0] = m_aOffset[nCntKey][0];
									m_Player3D[nMotion][nCnt][nCntKey][1] = m_aOffset[nCntKey][1];
									m_Player3D[nMotion][nCnt][nCntKey][2] = m_aOffset[nCntKey][2];
								}
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
													for (int nCnt = 0; nCnt < 10; nCnt++)
													{
														for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
														{
															m_Player3D[nMotion][nCnt][nCntKey][0] = m_aOffset[nCntKey][0];
															m_Player3D[nMotion][nCnt][nCntKey][1] = m_aOffset[nCntKey][1];
															m_Player3D[nMotion][nCnt][nCntKey][2] = m_aOffset[nCntKey][2];
														}
													}
												}
												else if (strcmp("ROT", pKey) == 0)
												{// 角度情報の読み込み
													fscanf(pFile, "%s", &aEqual[0]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][3]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][4]);
													fscanf(pFile, "%f", &m_Player3D[nMotion][nKeySet][nKey][5]);
												}
											}
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
	m_TotalKey += m_AddKey;
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

		if (m_bMotionChange == false)
		{// モーション変化なし
			if (m_nCounterMotion == m_nFrame[MOTIONTYPE][m_Key])
			{// キーフレーム終了時
				//if (m_MotionType != MOTIONTYPE_JUMP || m_Key != m_nNumKeyKeep - 2)
				{// ジャンプ以外でキー終了時リセット
					m_nCounterMotion = 0;
				}

				if (m_Key == m_TotalKey - 1 && m_bLoopKeep == true)
				{// ループするとき
					m_Key = 0;
					m_GetKey = m_Key;
				}
				else if (m_Key == m_TotalKey - 1 && m_bLoopKeep == false)
				{// ループせず終了するとき
					//if (m_MotionTypeOld != MOTIONTYPE_JUMP)
					//{// 直前がジャンプ以外の時
						//m_MotionType = MOTIONTYPE_NEUTRAL;	// ニュートラルへ移行
						m_nCounterMotion = 40;
						//break;
					//}
				}

				else if (m_Key < m_TotalKey)
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
					m_nBrendFrame = 100;
				}
				if (m_MotionTypeOld == MOTIONTYPE_LEFTMOVE && m_MotionType == MOTIONTYPE_RIGHTMOVE)
				{// ニュートラルから移動時
					m_nBrendFrame = 10;
				}
				if (m_MotionTypeOld == MOTIONTYPE_RIGHTMOVE && m_MotionType == MOTIONTYPE_LEFTMOVE)
				{// ニュートラルから移動時
					m_nBrendFrame = 10;
				}
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
// モーションツール用関数
//==============================================================================
void CPlayer3D::MotionTool(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (m_bData == false)
	{
		//選択しているモデル表示ログ設定
		m_SelectData[m_OldDataCount][256] = "u";
	}
	else
	{
		//元のカウントを保存
		m_OldDataCount = m_DataCount;

		//カウントアップ
		m_DataCount++;

		if (m_DataCount >= m_nNumModel)
		{
			//カウントを0に戻す
			m_DataCount = 0;
		}

		//選択しているモデル表示ログ設定
		m_SelectData[m_DataCount][256] = "●";

		//データフラグを戻してあげる
		m_bData = false;
	}

	if (m_bData2 == false)
	{
		//選択しているモデル表示ログ設定
		m_SelectData[m_OldDataCount][256] = "u";
	}
	else
	{
		//元のカウントを保存
		m_OldDataCount = m_DataCount;

		//カウントアップ
		m_DataCount--;

		if (m_DataCount <= -1)
		{
			//カウントを0に戻す
			m_DataCount = m_nNumModel - 1;
		}

		//選択しているモデル表示ログ設定
		m_SelectData[m_DataCount][256] = "●";

		//データフラグを戻してあげる
		m_bData2 = false;
	}

	//F2キーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{
		//trueからfalseにする
		m_bUse = m_bUse ? false : true;

		//trueからfalseにする
		m_bMove = m_bMove ? false : true;
	}

	//F9キーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		//モーション情報を書き込み
		WriteMotion();
	}

	if (m_bUse == true)
	{
		//カウントが1以上の時
		if (m_nCnt >= 1)
		{
			//モーションカウントリセット
			m_nCounterMotion = 0;
			m_Key = 0;
			m_GetKey = m_Key;
			m_nCnt = 0;
		}
		//モーションの更新
		UpdateMotion(m_MotionChange, false);
	}
	else
	{
		//カウントアップ
		m_nCnt++;
	}

	//RIGHTキーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		//モーションカウントリセット
		m_nCounterMotion = 0;
		m_Key = 0;
		m_GetKey = m_Key;

		//モーションの最大数までいったら
		if (m_MotionChange == MOTIONTYPE_MAX - 1)
		{
			//初期化処理
			m_MotionChange = 0;
		}
		else
		{
			//カウントアップ
			m_MotionChange++;
		}

		//モーションの更新
		UpdateMotion(m_MotionChange, false);
	}
	//LEFTキーを押したとき
	else if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		//モーションカウントリセット
		m_nCounterMotion = 0;
		m_Key = 0;
		m_GetKey = m_Key;

		//モーションキーが0の場合
		if (m_MotionChange == 0)
		{
			//初期化処理
			m_MotionChange = MOTIONTYPE_MAX - 1;
		}
		else
		{
			//カウントアップ
			m_MotionChange--;
		}

		//モーションの更新
		UpdateMotion(m_MotionChange, false);
	}

	//Iキーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_I) == true)
	{
		//キーを増やす
		m_nNumKeyKeep++;
		m_NumKey[m_MotionChange] = m_nNumKeyKeep;
	}
	//Kキーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_K) == true)
	{
		//キーを増やす
		m_nNumKeyKeep--;
		m_NumKey[m_MotionChange] = m_nNumKeyKeep;
	}
	//2キーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		//キーを進める
		if (m_GetKey >= m_TotalKey - 1)
		{
			//キーの総数を代入
			m_GetKey = 0;
			//モーションカウントリセット
			m_nCounterMotion = 0;
			//モーションの更新
			UpdateMotion(m_MotionChange, true);
		}
		//キーを進める
		else if (m_GetKey < m_TotalKey - 1)
		{
			//カウントアップ
			m_GetKey++;
			//モーションカウントリセット
			m_nCounterMotion = 0;
			//モーションの更新
			UpdateMotion(m_MotionChange, true);
		}
	}
	//2キーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		//キーを進める
		if (m_GetKey <= 0)
		{
			//キーの総数を代入
			m_GetKey = m_TotalKey - 1;
			//モーションカウントリセット
			m_nCounterMotion = 0;
			//モーションの更新
			UpdateMotion(m_MotionChange, true);
		}
		else if (m_GetKey <= m_TotalKey)
		{
			//カウントダウン
			m_GetKey--;
			//モーションカウントリセット
			m_nCounterMotion = 0;
			//モーションの更新
			UpdateMotion(m_MotionChange, true);
		}
	}
	//2キーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_N) == true)
	{
		m_nFrame[m_MotionChange][m_GetKey]++;
	}
	//2キーを押したとき
	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		m_nFrame[m_MotionChange][m_GetKey]--;
	}

	//フレームが1までいったら1以下までいかないようにする
	if (m_nFrame[m_MotionChange][m_GetKey] <= 1)
	{
		m_nFrame[m_MotionChange][m_GetKey] = 1;
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	//モデルのRotを変更する時パーツの色を変更
	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		//選択しています用
		m_bData = true;

		//カウント数がパーツ数より数値が高かったら
		if (m_nCntMat >= m_nNumModel - 1)
		{
			//カウントを-1に戻してあげる
			m_nCntMat = -1;
		}

		//値を保存
		m_nCntMatOld = m_nCntMat;

		//カウントアップ
		m_nCntMat++;

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_apModel[m_nCntMat]->GetBuff()->GetBufferPointer();

		//マテリアルの色変更
		pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		m_apModel[m_nCntMat]->GetMash()->UnlockVertexBuffer();

		if (m_nCntMat == 0)
		{
			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)m_apModel[m_nNumModel - 1]->GetBuff()->GetBufferPointer();

			//マテリアルの色変更
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロック
			m_apModel[m_nNumModel - 1]->GetMash()->UnlockVertexBuffer();
		}

		//-1の値だった場合
		if (m_nCntMatOld == -1)
		{
			//1に戻してあ上げる
			m_nCntMatOld = 1;
		}

		//保存用との比較
		if (m_nCntMatOld <= m_nCntMat)
		{
			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)m_apModel[m_nCntMatOld]->GetBuff()->GetBufferPointer();

			//マテリアルの色変更
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロック
			m_apModel[m_nCntMatOld]->GetMash()->UnlockVertexBuffer();
		}
	}

	//モデルのRotを変更する時パーツの色を変更
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		//選択しています用
		m_bData2 = true;

		if (m_nCntMat <= 0)
		{
			m_nCntMat = m_nNumModel;
		}

		//値を保存
		m_nCntMatOld = m_nCntMat;

		if (m_nCntMat >= m_nNumModel)
		{
			m_nCntMatOld = 0;
		}

		//カウントアップ
		m_nCntMat--;

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_apModel[m_nCntMat]->GetBuff()->GetBufferPointer();

		//マテリアルの色変更
		pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.4f, 1.0f, 1.0f);

		// 頂点バッファをアンロック
		m_apModel[m_nCntMat]->GetMash()->UnlockVertexBuffer();

		//保存用との比較
		if (m_nCntMatOld >= m_nCntMat)
		{
			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)m_apModel[m_nCntMatOld]->GetBuff()->GetBufferPointer();

			//マテリアルの色変更
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロック
			m_apModel[m_nCntMatOld]->GetMash()->UnlockVertexBuffer();
		}
		//保存用との比較
		if (m_nCntMatOld <= m_nCntMat)
		{
			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)m_apModel[m_nCntMatOld]->GetBuff()->GetBufferPointer();

			//マテリアルの色変更
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点バッファをアンロック
			m_apModel[m_nCntMatOld]->GetMash()->UnlockVertexBuffer();
		}
	}

	//回転させたパーツを保存する
	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{
		//パーツ保存
		for (int nPart = 0; nPart < m_nNumModel; nPart++)
		{
			for (int nOffset = 0; nOffset < 6; nOffset++)
			{
				//回転させたパーツを保存させる
				m_Player3D[m_MotionChange][m_GetKey][nPart][nOffset] = m_aOffset[nPart][nOffset];
			}
		}
	}

	//モーションをコピーする
	if (pInputKeyboard->GetTrigger(DIK_F8) == true)
	{
		//パーツ保存
		for (int nPart = 0; nPart < m_nNumModel; nPart++)
		{
			for (int nOffset = 0; nOffset < 6; nOffset++)
			{
				//保存用変数に値を代入する
				m_aOldOffset[nPart][nOffset] = m_aOffset[nPart][nOffset];
			}
		}
	}

	//モーションコピーしたやつをペーストする
	if (pInputKeyboard->GetTrigger(DIK_F7) == true)
	{
		//パーツ保存
		for (int nPart = 0; nPart < m_nNumModel; nPart++)
		{
			for (int nOffset = 0; nOffset < 6; nOffset++)
			{
				//保存しておいた値を代入する
				m_aOffset[nPart][nOffset] = m_aOldOffset[nPart][nOffset];
			}
		}
	}

	//モデルのパーツX軸変更
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{
		//X軸を回転
		m_aOffset[m_DataCount][3] -= D3DX_PI / 100.0f;
	}
	//モデルのパーツX軸変更
	else if (pInputKeyboard->GetPress(DIK_G) == true)
	{
		//X軸を回転
		m_aOffset[m_DataCount][3] += D3DX_PI / 100.0f;
	}
	//モデルのパーツX軸変更
	if (pInputKeyboard->GetPress(DIK_B) == true)
	{
		//X軸をリセット
		m_aOffset[m_DataCount][3] = m_Player3D[m_MotionChange][m_GetKey][m_DataCount][3];
	}

	//モデルのパーツY軸変更
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{
		//Y軸を回転
		m_aOffset[m_DataCount][4] -= D3DX_PI / 100.0f;
	}
	//モデルのパーツY軸変更
	else if (pInputKeyboard->GetPress(DIK_H) == true)
	{
		//Y軸を回転
		m_aOffset[m_DataCount][4] += D3DX_PI / 100.0f;
	}
	//モデルのパーツY軸変更
	if (pInputKeyboard->GetPress(DIK_V) == true)
	{
		//Y軸をリセット
		m_aOffset[m_DataCount][4] = m_Player3D[m_MotionChange][m_GetKey][m_DataCount][4];
	}

	//モデルのパーツZ軸変更
	if (pInputKeyboard->GetPress(DIK_U) == true)
	{
		//Z軸を回転
		m_aOffset[m_DataCount][5] -= D3DX_PI / 100.0f;
	}
	//モデルのパーツZ軸変更
	else if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		//Z軸を回転
		m_aOffset[m_DataCount][5] += D3DX_PI / 100.0f;
	}
	//モデルのパーツZ軸変更
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{
		//Z軸をリセット
		m_aOffset[m_DataCount][5] = m_Player3D[m_MotionChange][m_GetKey][m_DataCount][5];
	}

	if (m_bMove == false)
	{
		//親パーツのPOSを変更
		if (pInputKeyboard->GetPress(DIK_Q) == true)
		{
			//X軸を移動
			m_aOffset[0][0] += D3DX_PI / 100.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			//X軸を移動
			m_aOffset[0][0] -= D3DX_PI / 100.0f;
		}
		//親パーツのPOSを変更
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			//Y軸を移動
			m_aOffset[0][1] += D3DX_PI / 100.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			//Y軸を移動
			m_aOffset[0][1] -= D3DX_PI / 100.0f;
		}
		//親パーツのPOSを変更
		if (pInputKeyboard->GetPress(DIK_E) == true)
		{
			//Z軸を移動
			m_aOffset[0][2] += D3DX_PI / 100.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			//Z軸を移動
			m_aOffset[0][2] -= D3DX_PI / 100.0f;
		}

		//親パーツの全てをリセット
		if (pInputKeyboard->GetPress(DIK_RETURN) == true)
		{
			//軸をリセット
			for (int nCntPos = 0; nCntPos < 3; nCntPos++)
			{
				m_aOffset[0][nCntPos] = m_Player3D[m_MotionChange][m_GetKey][0][nCntPos];
			}
		}
	}
	else
	{
		//プレイヤーを移動できるようにする
		PlayerMove();
	}
}

//==============================================================================
// プレイヤーの移動処理関数
//==============================================================================
void CPlayer3D::PlayerMove(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//カメラ角度取得
	D3DXVECTOR3 RotationCamera = CManager::GetCamera()->GetRot();
	float RotCameraY = RotationCamera.y;

	// 角度修正
	if (RotCameraY > D3DX_PI) { RotCameraY -= D3DX_PI * 2; }
	if (RotCameraY < -D3DX_PI) { RotCameraY += D3DX_PI * 2; }

	//位置更新
	m_posold = m_pos;

	// 移動量の加算
	m_pos += m_move;

	// Wキーのモデルの移動
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		//モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		// 左上方向
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			//カウントアップ
			m_nCounter++;

			//モーションの変化
			m_MotionChange = MOTIONTYPE_LEFTMOVE;

			//移動量計算
			m_move.x -= cosf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_move.z += sinf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY + (D3DX_PI * 3 / 4);
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{// 右上方向

			//カウントアップ
			m_nCounter++;

		    //モーションの変化
		    m_MotionChange = MOTIONTYPE_RIGHTMOVE;
			
			m_move.x += cosf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_move.z -= sinf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY - (D3DX_PI * 3 / 4);
		}
		else
		{// 上方向
			m_move.x += sinf(RotCameraY) * 2.0f;
			m_move.z += cosf(RotCameraY) * 2.0f;
			m_rotDest.y = RotCameraY + D3DX_PI;
		}
	}

	// Sキーのモデルの移動
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		if (m_nMoveCnt < 20)
		{
			//カウントアップ
			m_nMoveCnt++;
		}

		//モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		if (pInputKeyboard->GetPress(DIK_A) == true)
		{// 左下方向

		    //カウントアップ
			m_nCounter++;

			//モーションの変化
			m_MotionChange = MOTIONTYPE_RIGHTMOVE;

			//移動量計算
			m_move.x -= cosf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_move.z += sinf(RotCameraY - D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY - D3DX_PI / -4;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{// 右下方向

		    //カウントアップ
			m_nCounter++;

			//モーションの変化
			m_MotionChange = MOTIONTYPE_LEFTMOVE;

			//移動量計算
			m_move.x += cosf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_move.z -= sinf(RotCameraY + D3DX_PI / 4) * 2.0f;
			m_rotDest.y = RotCameraY - D3DX_PI / 4;
		}
		else
		{// 下方向
			m_move.x -= sinf(RotCameraY) * 2.0f;
			m_move.z -= cosf(RotCameraY) * 2.0f;
			m_rotDest.y = RotCameraY;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{// 左方向

	    //モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}

		m_move.x -= cosf(RotCameraY) * 2.0f;
		m_move.z += sinf(RotCameraY) * 2.0f;
		m_rotDest.y = RotCameraY + (D3DX_PI / 2);
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{// 右方向

	    //モーションの変化
		m_MotionChange = MOTIONTYPE_MOVE;

		if (m_nMoveCnt < 20)
		{
			m_nMoveCnt++;
		}
		m_move.x += cosf(RotCameraY) * 2.0f;
		m_move.z -= sinf(RotCameraY) * 2.0f;
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

	// 移動モーションから待機モーションへ
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nMoveCnt == 10)
	{
		//メンバ変数初期化
		m_nCounterMotion = 0;//フレームカウントをリセット
		m_Key = 0;//フレームキーをリセット
		m_GetKey = 0;//フレームキーをリセット
		m_MotionChange = MOTIONTYPE_NEUTRAL;//モーションをニュートラルにする
	}

	//右移動モーションの時 && カウントが1の時
	if (m_MotionChange == MOTIONTYPE_RIGHTMOVE && m_nCounter == 1)
	{
		//メンバ変数初期化
		m_nCounterMotion = 0;//フレームカウントをリセット
		m_Key = 0;//フレームキーをリセット
		m_GetKey = 0;//フレームキーをリセット
	}
	//左移動モーションの時 && カウントが1の時
	if (m_MotionChange == MOTIONTYPE_LEFTMOVE && m_nCounter == 1)
	{
		//メンバ変数初期化
		m_nCounterMotion = 0;//フレームカウントをリセット
		m_Key = 0;//フレームキーをリセット
		m_GetKey = 0;//フレームキーをリセット
	}
	//移動モーションを切り替える時 && カウントが1の時
	if (m_MotionChange == MOTIONTYPE_MOVE && m_nCounter >= 1)
	{
		//メンバ変数初期化
		m_nCounterMotion = 0;//フレームカウントをリセット
		m_Key = 0;//フレームキーをリセット
		m_GetKey = 0;//フレームキーをリセット
		m_nCounter = 0;//カウントをリセット
	}

	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
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
}

//==============================================================================
// プレイヤー3DのPosのGet関数
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetPosition(void)
{
	//位置情報
	return m_pos;
}

//==============================================================================
// プレイヤー3DのRotのGet関数
//==============================================================================
D3DXVECTOR3 CPlayer3D::GetRotation(void)
{
	//回転情報
	return m_rot;
}

//==============================================================================
//モーションの書き込み
//==============================================================================
void CPlayer3D::WriteMotion(void)
{
	//ファイルポインタ
	FILE *fp;

	//テキストに書き込み
	fp = fopen("data/TEXT/key_info.txt", "w");

	//書き込み内容
	fprintf(fp, "#=======================================================\n");
	fprintf(fp, "#\n");
	fprintf(fp, "# モーション情報\n");
	fprintf(fp, "# [モーション情報 : %d]\n",m_MotionChange);
	fprintf(fp, "#\n");
	fprintf(fp, "#=======================================================\n\n");

	//書き込み内容
	fprintf(fp, "MOTIONSET\n");
	fprintf(fp, "     LOOP = %d    # ループするかどうか[0:ループしない / 1:ループする]\n", m_Loop[m_MotionChange]);
	fprintf(fp, "     NUM_KEY = %d # キー数\n\n", m_NumKey[m_MotionChange]);

	//書き込み内容
	for (int nCntMotion = 0; nCntMotion < m_NumKey[m_MotionChange]; nCntMotion++)
	{
		fprintf(fp, "     KEYSET       # --- << KEY : %d / %d >> ---\n", nCntMotion, m_NumKey[m_MotionChange]);
		fprintf(fp, "         FRAME = %d\n", m_nFrame[m_MotionChange][nCntMotion]);
		for (int nCntNum = 0; nCntNum < m_nNumModel; nCntNum++)
		{
			//書き込み内容
			fprintf(fp, "         KEY # ----- [ %d ] ----- \n", nCntNum);
			fprintf(fp, "             POS = %.2f %.2f %.2f\n", m_aOffset[nCntNum][0], m_aOffset[nCntNum][1], m_aOffset[nCntNum][2]);
			fprintf(fp, "             ROT = %.2f %.2f %.2f\n", m_aOffset[nCntNum][3], m_aOffset[nCntNum][4], m_aOffset[nCntNum][5]);
			if (nCntNum == m_nNumModel - 1)
			{
				fprintf(fp, "         END_KEY\n");
			}
			else
			{
				fprintf(fp, "         END_KEY\n\n");
			}
		}
		if (nCntMotion == m_NumKey[m_MotionChange] - 1)
		{
			fprintf(fp, "     END_KEYSET\n");
		}
		else
		{
			fprintf(fp, "     END_KEYSET\n\n");
		}
	}

	//書き込み内容(終了合図)
	fprintf(fp, " END_MOTIONSET\n\n");

	//「txt」のファイルを閉じる
	fclose(fp);
}
