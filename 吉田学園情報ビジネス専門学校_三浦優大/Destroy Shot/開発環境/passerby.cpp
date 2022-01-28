//=============================================================================
//
// 通行人処理 [passerby.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "passerby.h"
#include "manager.h"
#include "model.h"
#include "object3D.h"
#include "camera.h"
#include "shadowS.h"
#include "scene3D.h"
#include "player3D.h"
#include "bullet.h"
#include "score.h"
#include "particle.h"
#include "animation.h"
#include "Purpose.h"
#include "fade.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "effect.h"
#include "hp.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
D3DXVECTOR3 CPasserby::m_posHoming;
bool CPasserby::m_bParticle2 = false;

//==============================================================================
// コンストラクタ
//==============================================================================
CPasserby::CPasserby(int nPriority, int nType)
{
	//メンバ変数初期化
	m_bMotionChange = false;
	m_nBrendFrame = 5;
	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}
}

//==============================================================================
// デストラクタ
//==============================================================================
CPasserby::~CPasserby()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CPasserby::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//モーション読み込み
	LoadMotion(m_passerbytype);

	//タイプ分け処理
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);

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
	int nNumVtx;			    // 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

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
		if (m_MaxPasserby.x < vtx.x)
		{
			m_MaxPasserby.x = vtx.x;
		}
		if (m_MaxPasserby.y < vtx.y)
		{
			m_MaxPasserby.y = vtx.y;
		}
		if (m_MaxPasserby.z < vtx.z)
		{
			m_MaxPasserby.z = vtx.z;
		}

		// 各座標の最小値の比較
		if (m_MinPasserby.x > vtx.x)
		{
			m_MinPasserby.x = vtx.x;
		}
		if (m_MinPasserby.y > vtx.y)
		{
			m_MinPasserby.y = vtx.y;
		}
		if (m_MinPasserby.z > vtx.z)
		{
			m_MinPasserby.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_apModel[0]->GetMash()->UnlockVertexBuffer();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pHp[nCnt] = CGame::GetHp(nCnt);
	}

	//位置更新
	m_pos = pos;
	m_poskeep = pos;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CPasserby::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < (int)m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{
			//モデルを生成
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
void CPasserby::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンドを取得
	m_pSound = CManager::GetSound();

	if (m_bParticle == false && m_bParticle2 == false)
	{
		//敵の動き
		PasserbyMove();
	}

	//bulletの位置取得
	D3DXVECTOR3 pos = CBullet::GetPos();
	
	//バレットのポインタ取得
	CBullet *pBullet;
	pBullet = CPlayer3D::GetBullet();

	if (m_bParticle == false)
	{
		//NULLチェック
		if (pBullet != NULL)
		{
			//弾との当たり判定
			if (m_pos.x + 40.0f >= pos.x && m_pos.x - 40.0f <= pos.x&&
				m_pos.z + 40.0f >= pos.z && m_pos.z - 40.0f <= pos.z)
			{
				//スコア加算
				CScore::AddScore(100);

				//敵を減らす
				CPurpose::ReducePurpose(1);

				//パーティクル発生
				CGame::GameParticle(m_pos, 1);

				////NULLチェック
				//if (pBullet != NULL)
				//{
				//	//弾を消す処理
				//	pBullet->Uninit();
				//}

				if (CPurpose::GetPurpose() == 0)
				{
					//リセットのフラグ管理
					m_bParticle = true;
				}

				if (m_bParticle == false)
				{
					//サウンドBGM
					m_pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

					//敵を消す処理
					Uninit();
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
				//パーティクル生成(死んだとき)
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, 1720.0f), 5);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				//パーティクル生成(死んだとき)
				CGame::GameParticle(D3DXVECTOR3(-2000.0f + (nCnt * 400.0f), 0.0f, -1720.0f), 5);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				//パーティクル生成(死んだとき)
				CGame::GameParticle(D3DXVECTOR3(-2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 5);
			}
			for (int nCnt = 0; nCnt < 11; nCnt++)
			{
				//パーティクル生成(死んだとき)
				CGame::GameParticle(D3DXVECTOR3(2200.0f, 0.0f, -1720.0f + (nCnt * 400.0f)), 5);
			}
		}

		//パーティクル発生カウント
		m_nParticlecount++;
	}

	//200カウントいったら
	if (m_nParticlecount == 200)
	{
		//サウンドBGM
		m_pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

		//リザルトにいく
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
	}

	//プレイヤーとの当たり判定
	CollisionPlayer();
}

//==============================================================================
// 描画処理
//==============================================================================
void CPasserby::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ローカル
	D3DXMATRIX mtxRot, mtxTrans,mtxScale;	//計算用マトリックス

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
// 描画処理
//==============================================================================
void CPasserby::PasserbyMove(void)
{
	//ローカル変数宣言
	float fAngle;       //目的地までの角度
	D3DXVECTOR3 vec;    //目的地までのベクトル

	D3DXVECTOR3 PlayerPos = CPlayer3D::GetPosition();

	// 移動量の加算
	m_pos += m_move;
	m_posHoming = m_pos;

	//エフェクト生成
	CEffect::Create(m_pos, D3DXCOLOR(0.3f, 1.0f, 1.0f, 1.0f), 25, 20.0f, 20.0f);

	//モーションの更新
	UpdateMotion(m_MotionType);

	//モーションの変化
	m_MotionType = MOTIONTYPE_PASSERBY_MOVE;

	vec = PlayerPos - m_pos;//現在地から目的地へのベクトル
	fAngle = atan2f(vec.x, vec.z);//ベクトルの角度を求める
	m_move.x = sinf(fAngle) * 3.5f;//移動量を求める
	m_move.z = cosf(fAngle) * 3.5f;//移動量を求める
	m_rotDest.y = fAngle + D3DX_PI;//目的地に合わせて角度を変える
	
	//各パーツの更新処理
	for (int nCnt = 0; nCnt < (int)m_nNumModel; nCnt++)
	{
		m_apModel[nCnt]->MotionPos(D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]));
		m_apModel[nCnt]->MotionRot(D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]));
	}

	// 加速後の減速処理
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.z += (0.0f - m_move.z) * 0.2f;

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
}

//==============================================================================
// 生成処理
//==============================================================================
CPasserby * CPasserby::Create(D3DXVECTOR3 pos,PASSERBY_TYPE passerbytype,int nType)
{
	//プレイヤーを生成
	CPasserby *pPasserby = NULL;
	pPasserby = new CPasserby;

	//NULLチェック
	if (pPasserby != NULL)
	{
		//引数から値を代入
		pPasserby->m_passerbytype = passerbytype;
		pPasserby->m_nPasserbyCount = nType;

		//初期化処理
		pPasserby->Init(pos, 0.0f, 0.0f);
	}

	return pPasserby;
}

//==============================================================================
// .txtフォルダの読み込み
//==============================================================================
void CPasserby::LoadMotion(PASSERBY_TYPE passerbytype)
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

	switch (passerbytype)
	{
	case PASSERBY_TYPE_MEN:
		// データの読み込み
		pFile = fopen("data/TEXT/motion_passerby00.txt", "r");
		break;
	default:
		break;
	}

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
							else if (strcmp("KEYSET", pMotionSet) == 0)
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
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][0]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][1]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][2]);
												m_Passerby[nMotion][nKeySet][nKey][0] += m_aOffset[nKey][0];
												m_Passerby[nMotion][nKeySet][nKey][1] += m_aOffset[nKey][1];
												m_Passerby[nMotion][nKeySet][nKey][2] += m_aOffset[nKey][2];
											}
											else if (strcmp("ROT", pKey) == 0)
											{// 角度情報の読み込み
												fscanf(pFile, "%s", &aEqual[0]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][3]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][4]);
												fscanf(pFile, "%f", &m_Passerby[nMotion][nKeySet][nKey][5]);
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
								nKeySet++;
							}
						}
						m_NumKey[nMotion] = nKeySet;	// キー数の設定
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
void CPasserby::UpdateMotion(MOTIONTYPE_PASSERBY passerby)
{
	// キー数とループ情報の代入
	m_nNumKeyKeep = m_NumKey[passerby];
	m_bLoopKeep = m_Loop[passerby];

	// モーションカウンターの加算
	if (m_bMotionChange == true)
	{// モーションブレンド実行時
		if (m_fCounterMotion < m_nBrendFrame)
		{// カウント上限
			m_fCounterMotion++;
		}
	}
	else
	{// 通常時
		if (m_fCounterMotion < m_nFrame[passerby][m_Key])
		{// カウント上限
			m_fCounterMotion++;
		}
	}

	// モデルパーツごとのモーションアニメーション
	for (int nCnt = 0; nCnt < MAX_PASSERBY_PARTS; nCnt++)
	{
		if (m_Key == m_nNumKeyKeep - 1 && m_bLoopKeep == true)
		{// ループして最初に戻るとき
			m_aOffset[nCnt][0] = m_Passerby[passerby][m_Key][nCnt][0] + (m_Passerby[passerby][0][nCnt][0] - m_Passerby[passerby][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][1] = m_Passerby[passerby][m_Key][nCnt][1] + (m_Passerby[passerby][0][nCnt][1] - m_Passerby[passerby][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][2] = m_Passerby[passerby][m_Key][nCnt][2] + (m_Passerby[passerby][0][nCnt][2] - m_Passerby[passerby][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][3] = m_Passerby[passerby][m_Key][nCnt][3] + (m_Passerby[passerby][0][nCnt][3] - m_Passerby[passerby][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][4] = m_Passerby[passerby][m_Key][nCnt][4] + (m_Passerby[passerby][0][nCnt][4] - m_Passerby[passerby][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][5] = m_Passerby[passerby][m_Key][nCnt][5] + (m_Passerby[passerby][0][nCnt][5] - m_Passerby[passerby][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else if (m_bMotionChange == true)
		{// モーションブレンド
			m_aOffset[nCnt][0] = m_aOffsetMemo[nCnt][0] + (m_Passerby[m_MotionType][0][nCnt][0] - m_aOffsetMemo[nCnt][0]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][1] = m_aOffsetMemo[nCnt][1] + (m_Passerby[m_MotionType][0][nCnt][1] - m_aOffsetMemo[nCnt][1]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][2] = m_aOffsetMemo[nCnt][2] + (m_Passerby[m_MotionType][0][nCnt][2] - m_aOffsetMemo[nCnt][2]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][3] = m_aOffsetMemo[nCnt][3] + (m_Passerby[m_MotionType][0][nCnt][3] - m_aOffsetMemo[nCnt][3]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][4] = m_aOffsetMemo[nCnt][4] + (m_Passerby[m_MotionType][0][nCnt][4] - m_aOffsetMemo[nCnt][4]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffset[nCnt][5] = m_aOffsetMemo[nCnt][5] + (m_Passerby[m_MotionType][0][nCnt][5] - m_aOffsetMemo[nCnt][5]) * ((float)m_fCounterMotion / (float)m_nBrendFrame);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}
		else
		{// 通常時
			m_aOffset[nCnt][0] = m_Passerby[passerby][m_Key][nCnt][0] + (m_Passerby[passerby][m_Key + 1][nCnt][0] - m_Passerby[passerby][m_Key][nCnt][0]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][1] = m_Passerby[passerby][m_Key][nCnt][1] + (m_Passerby[passerby][m_Key + 1][nCnt][1] - m_Passerby[passerby][m_Key][nCnt][1]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][2] = m_Passerby[passerby][m_Key][nCnt][2] + (m_Passerby[passerby][m_Key + 1][nCnt][2] - m_Passerby[passerby][m_Key][nCnt][2]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][3] = m_Passerby[passerby][m_Key][nCnt][3] + (m_Passerby[passerby][m_Key + 1][nCnt][3] - m_Passerby[passerby][m_Key][nCnt][3]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][4] = m_Passerby[passerby][m_Key][nCnt][4] + (m_Passerby[passerby][m_Key + 1][nCnt][4] - m_Passerby[passerby][m_Key][nCnt][4]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffset[nCnt][5] = m_Passerby[passerby][m_Key][nCnt][5] + (m_Passerby[passerby][m_Key + 1][nCnt][5] - m_Passerby[passerby][m_Key][nCnt][5]) * ((float)m_fCounterMotion / (float)m_nFrame[passerby][m_Key]);
			m_aOffsetMemo[nCnt][0] = m_aOffset[nCnt][0];
			m_aOffsetMemo[nCnt][1] = m_aOffset[nCnt][1];
			m_aOffsetMemo[nCnt][2] = m_aOffset[nCnt][2];
			m_aOffsetMemo[nCnt][3] = m_aOffset[nCnt][3];
			m_aOffsetMemo[nCnt][4] = m_aOffset[nCnt][4];
			m_aOffsetMemo[nCnt][5] = m_aOffset[nCnt][5];
		}

		if (m_bMotionChange == false)
		{// モーション変化なし
			if (m_fCounterMotion == m_nFrame[passerby][m_Key])
			{// キーフレーム終了時
			 //if (m_MotionType != MOTIONTYPE_JUMP || m_Key != m_nNumKeyKeep - 2)
				{// ジャンプ以外でキー終了時リセット
					m_fCounterMotion = 0;
				}

				if (m_Key == m_nNumKeyKeep - 1 && m_bLoopKeep == true)
				{// ループするとき
					m_Key = 0;
				}
				else if (m_Key == m_nNumKeyKeep - 2 && m_bLoopKeep == false)
				{// ループせず終了するとき
				 //if (m_MotionTypeOld != MOTIONTYPE_JUMP)
					{// 直前がジャンプ以外の時
						m_MotionType = MOTIONTYPE_PASSERBY_NEUTRAL;	// ニュートラルへ移行
						m_fCounterMotion = 0;
						break;
					}
				}
				else if (m_Key < m_nNumKeyKeep)
				{// キーのカウント
					m_Key++;
				}
			}
			else if (m_MotionTypeOld != m_MotionType)
			{// モーション変化時	
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = true;

				if (m_MotionTypeOld == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_NEUTRAL && m_MotionType == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_MOVE)
				{// 移動からニュートラル時
					m_nBrendFrame = 20;
				}
				else if (m_MotionTypeOld == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_MOVE && m_MotionType == MOTIONTYPE_PASSERBY::MOTIONTYPE_PASSERBY_NEUTRAL)
				{// ニュートラルから移動時
					m_nBrendFrame = 40;
				}
				break;
			}
		}
		else if (m_bMotionChange == true)
		{// モーションブレンド時のカウントリセット
			if (m_fCounterMotion == m_nBrendFrame || m_MotionTypeOld != m_MotionType)
			{
				m_Key = 0;
				m_fCounterMotion = 0;
				m_bMotionChange = false;
			}
		}
	}

	// 直前のモーション状態の保存
	m_MotionTypeOld = m_MotionType;
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CPasserby::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	bool bLand = false;

	D3DXVECTOR3 pos = *pPos;// 判定対象の位置

	//aPosの値設定
  	m_aPosColision[0] = D3DXVECTOR3(m_pos.x + m_MinPasserby.x, 0.0f, m_pos.z + m_MinPasserby.z);
	m_aPosColision[1] = D3DXVECTOR3(m_pos.x + m_MinPasserby.x, 0.0f, m_pos.z + m_MaxPasserby.z);
	m_aPosColision[2] = D3DXVECTOR3(m_pos.x + m_MaxPasserby.x, 0.0f, m_pos.z + m_MaxPasserby.z);
	m_aPosColision[3] = D3DXVECTOR3(m_pos.x + m_MaxPasserby.x, 0.0f, m_pos.z + m_MinPasserby.z);

	//外積の式
	m_aVec[0] = m_aPosColision[1] - m_aPosColision[0];
	m_aVec[1] = m_aPosColision[2] - m_aPosColision[1];
	m_aVec[2] = m_aPosColision[3] - m_aPosColision[2];
	m_aVec[3] = m_aPosColision[0] - m_aPosColision[3];

	//四頂点分
	for (int nCount = 0; nCount < 4; nCount++)
	{
		//外積の式2
		if (nCount == 0)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 1)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPosColision[nCount];
		}
		else if (nCount == 2)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 3)
		{
			m_bVec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPosColision[nCount];
		}

		m_fPlayerVec[nCount] = (m_aVec[nCount].z * m_bVec[nCount].x) - (m_aVec[nCount].x * m_bVec[nCount].z);

   		if (pPos->y <= m_pos.y + m_MaxPasserby.y - fHeightMin && pPos->y > m_pos.y + m_MinPasserby.y - fHeightMax)
		{
			if (m_fPlayerVec[0] > 0.0f && m_fPlayerVec[1] > 0.0f && m_fPlayerVec[2] > 0.0f && m_fPlayerVec[3] > 0.0f)
			{
				pMove->x = 0.0f;
			}
		}
	}

	return bLand;
}

//==============================================================================
// プレイヤーとの当たり判定
//==============================================================================
void CPasserby::CollisionPlayer(void)
{
	//プレイヤーの位置を取得
	D3DXVECTOR3 playerpos = CPlayer3D::GetPosition();

	if (m_bParticle2 == false)
	{
		//弾との当たり判定
		if (m_pos.x + 40.0f >= playerpos.x && m_pos.x - 40.0f <= playerpos.x&&
			m_pos.z + 40.0f >= playerpos.z && m_pos.z - 40.0f <= playerpos.z)
		{
			//スコア初期化
			CScore::SetScore(0);

			//敵の目的の数も初期化する
			CPurpose::SetPurpose(CGame::GetEnemyPurpose());

			//リセットのフラグ管理
			m_bParticle2 = true;
			m_bParticle3 = true;
		}
	}

	if (m_bParticle2 == true && m_bParticle3 == true)
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
			//体力を減らす
			m_pHp[CGame::GetLife()]->Uninit();
			CGame::SetLife(1);

			//エネミーカウントをリセット
			CGame::SetCount(0);

			//初期化する
			m_bReset = false;
			m_bParticle2 = false;
			m_nParticlecount = 0;
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
							//敵の破棄
							pScene->Uninit();
						}
					}
				}
			}
		}
		//ライフが0になったら
		else if (CGame::GetLife() == 0)
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
							//敵の破棄
							pScene->Uninit();
						}
					}
				}
			}

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
