//=============================================================================
//
// メイン処理 [object3D.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "player3D.h"
#include "scene3D.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
D3DXVECTOR3		CObject3D::m_rotStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3		CObject3D::m_moveStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXMATRIX      CObject3D::m_mtxWorld;

//=============================================================================
//グローバル変数宣言
//=============================================================================
int g_nTypeObject = -1;
int CObject3D::m_nType;
D3DXVECTOR3 g_posObjectVector[256][4];

//=============================================================================
// コンストラクタ
//=============================================================================
CObject3D::CObject3D(int nPriority, int nType)
{
	m_fUseScene3D = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	//if (m_nType == 0)
	{
		//D3DXLoadMeshFromX("data/MODEL/ob.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
		D3DXLoadMeshFromX("data/MODEL/city001.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	}

	//位置
	m_pos = pos;

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ffRot = m_rot.y;
	m_move = m_moveStatic;

	BYTE *pVtxBuff;//頂点バッファへのポインタ
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ

	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		m_vtx = *(D3DXVECTOR3*)pVtxBuff;//頂点座標の代入

		if (m_vtx.x > m_MaxStone.x)
		{
			m_MaxStone.x = m_vtx.x;
		}

		if (m_vtx.x < m_MinStone.x)
		{
			m_MinStone.x = m_vtx.x;
		}

		if (m_vtx.y > m_MaxStone.y)
		{
			m_MaxStone.y = m_vtx.y;
		}

		if (m_vtx.y < m_MinStone.y)
		{
			m_MinStone.y = m_vtx.y;
		}

		if (m_vtx.z > m_MaxStone.z)
		{
			m_MaxStone.z = m_vtx.z;
		}

		if (m_vtx.z < m_MinStone.z)
		{
			m_MinStone.z = m_vtx.z;
		}

		pVtxBuff += sizeFVF;//頂点フォーマットのサイズ分ポインタを進める
	}
	if (m_MaxStone.x < 0.0f)
	{
		m_MaxStone.x *= -1.0f;
	}
	if (m_MaxStone.y < 0.0f)
	{
		m_MaxStone.y *= -1.0f;
	}
	if (m_MaxStone.z < 0.0f)
	{
		m_MaxStone.z *= -1.0f;
	}

	if (m_MinStone.x < 0.0f)
	{
		m_MinStone.x *= -1.0f;
	}
	if (m_MinStone.y < 0.0f)
	{
		m_MinStone.y *= -1.0f;
	}
	if (m_MinStone.z < 0.0f)
	{
		m_MinStone.z *= -1.0f;
	}

	m_posMaxBase = m_MaxStone;
	m_posMinBase = m_MinStone;

	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject3D::Uninit(void)
{
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject3D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CObject3D::Draw(void)
{
	//ローカル
	D3DXMATRIX mtxRot, mtxTrans, revmat;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存
	D3DXMATERIAL *pMat;				//マテリアルへのポインタ

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きの反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//m_posMaxBaseに逆行列を加える
	//D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);
	//D3DXVec3TransformCoord(&m_MaxStone, &m_posMaxBase, &mtxRot);
	//D3DXVec3TransformCoord(&m_MinStone, &m_posMinBase, &mtxRot);

	//位置の反転
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_MaxStone.x < 0.0f)
	{
		m_MaxStone.x *= -1.0f;
	}
	if (m_MaxStone.y < 0.0f)
	{
		m_MaxStone.y *= -1.0f;
	}
	if (m_MaxStone.z < 0.0f)
	{
		m_MaxStone.z *= -1.0f;
	}

	if (m_MinStone.x < 0.0f)
	{
		m_MinStone.x *= -1.0f;
	}
	if (m_MinStone.y < 0.0f)
	{
		m_MinStone.y *= -1.0f;
	}
	if (m_MinStone.z < 0.0f)
	{
		m_MinStone.z *= -1.0f;
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存したマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 生成処理
//=============================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nType)
{
	//モデルを生成
	CObject3D *pObject3D = NULL;
	pObject3D = new CObject3D;

	//NULLチェック
	if (pObject3D != NULL)
	{
		m_nType = nType;
		m_moveStatic = move;
		m_rotStatic = rot;
		pObject3D->Init(pos, 0.0f, 0.0f);
	}

	return pObject3D;
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CObject3D::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// カメラ情報の取得
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetRot();

	bool bLand = false;

	D3DXVECTOR3 g_Vec[18][4];
	D3DXVECTOR3 g_oldVec[18][4];
	float PlayerVec[18][4];
	float oldPlayerVec[18][4];
	D3DXVECTOR3 posold;

	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置
	posold = *pPosOld;					// 判定対象の位置

	//読み込んだ座標をもとに当たり判定を作る
	for (int nCnt = 0; nCnt < g_FailCntDateObjectVector; nCnt++)
	{
		m_aPos[nCnt][0] = g_posObjectVector[nCnt][0];
		m_aPos[nCnt][1] = g_posObjectVector[nCnt][1];
		m_aPos[nCnt][2] = g_posObjectVector[nCnt][2];
		m_aPos[nCnt][3] = g_posObjectVector[nCnt][3];

		//外積の式　ベクトル
		m_Vec[nCnt][0] = m_aPos[nCnt][1] - m_aPos[nCnt][0];
		m_Vec[nCnt][1] = m_aPos[nCnt][2] - m_aPos[nCnt][1];
		m_Vec[nCnt][2] = m_aPos[nCnt][3] - m_aPos[nCnt][2];
		m_Vec[nCnt][3] = m_aPos[nCnt][0] - m_aPos[nCnt][3];

		//四頂点分
		for (int nCount = 0; nCount < 4; nCount++)
		{
			//外積の式2
			if (nCount == 0)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 1)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 2)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
			}
			else if (nCount == 3)
			{
				g_Vec[nCnt][nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
			}

			//外積計算
			PlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_Vec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_Vec[nCnt][nCount].z);
		}
		
		if (pPos->y <= m_pos.y + m_MaxStone.y - fHeightMin && pPos->y > m_pos.y - m_MinStone.y - fHeightMax)
		{
			if (PlayerVec[nCnt][0] > 0.0f && PlayerVec[nCnt][1] > 0.0f && PlayerVec[nCnt][2] > 0.0f && PlayerVec[nCnt][3] > 0.0f)
			{
				for (int nCount = 0; nCount < 4; nCount++)
				{
					//外積の式2
					if (nCount == 0)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 1)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 2)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPos[nCnt][nCount];
					}
					else if (nCount == 3)
					{
						g_oldVec[nCnt][nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPos[nCnt][nCount];
					}

					oldPlayerVec[nCnt][nCount] = (m_Vec[nCnt][nCount].z * g_oldVec[nCnt][nCount].x) - (m_Vec[nCnt][nCount].x * g_oldVec[nCnt][nCount].z);
				}

				//左右の当たり判定
				if (PlayerVec[nCnt][1] > 0 && PlayerVec[nCnt][3] > 0)
				{
					//左
					if (oldPlayerVec[nCnt][0] <= 0 && PlayerVec[nCnt][0] >= 0)
					{
						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][1];

						//角度を計算
						float nAtan2 = (float)atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//斜辺の場合
						{
							pPos->z = pPosOld->z;				//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;					//移動量をゼロにする
							pMove->x = sinf(nAtan2) * 3.0f;
						}
						else if (nAtan2 == 0.0f)				//垂直の場合
						{
							pPos->x = pPosOld->x;
							pMove->x = sinf(nAtan2 - rot.y);
						}
					}

					//右
					if (oldPlayerVec[nCnt][2] <= 0 && PlayerVec[nCnt][2] >= 0)
					{
						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][3] - m_aPos[nCnt][2];

						//角度を計算
						float nAtan2 = (float)atan2(a.z, a.x);			//斜辺の場合

						if (nAtan2 > 0.0f)						//1フレーム前の位置に戻す
						{
							pPos->z = pPosOld->z;				//移動量をゼロにする
							pPos->x = pPosOld->x;
							pMove->z = 0.0f;
							pMove->x = sinf(-nAtan2) * 5.0f;	//垂直の場合
						}
						else if (nAtan2 == 0.0f)
						{
							pPos->x = pPosOld->x;
							pMove->x = sinf(nAtan2 - rot.y);
						}
					}
				}

				//前後の当たり判定
				if (PlayerVec[nCnt][0] > 0 && PlayerVec[nCnt][2] > 0)
				{
					//後
					if (oldPlayerVec[nCnt][1] <= 0 && PlayerVec[nCnt][1] >= 0)
					{
						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][1] - m_aPos[nCnt][2];

						//角度を計算
						float nAtan2 = (float)atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//斜辺の場合
						{
							pPos->z = pPosOld->z;				//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;					//移動量をゼロにする
							pMove->z = sinf(-nAtan2) * 10.0f;
						}
						//垂直の場合
						else if (nAtan2 == 0.0f)
						{
							pPos->z = pPosOld->z;
							pMove->z = sinf(nAtan2 - rot.y);
						}
					}

					//前
					if (oldPlayerVec[nCnt][3] <= 0 && PlayerVec[nCnt][3] >= 0)
					{
						//2頂点差を計算
						D3DXVECTOR3 a = m_aPos[nCnt][0] - m_aPos[nCnt][3];

						//角度を計算
						float nAtan2 = (float)atan2(a.z, a.x);

						if (nAtan2 > 0.0f)						//斜辺の場合
						{
							pPos->z = pPosOld->z;				//1フレーム前の位置に戻す
							pPos->x = pPosOld->x;
							pMove->x = 0.0f;					//移動量をゼロにする
							pMove->z = cosf(nAtan2) * 5.0f;
						}
						else if (nAtan2 == 0.0f)				//垂直の場合
						{
							pPos->z = pPosOld->z;
							pMove->z = sinf(nAtan2 - rot.y);
						}
					}
				}
			}
		}
	}
	//可視化
	if (m_fUseScene3D == false)
	{
		//読み込んだ座標をもとに当たり判定を作る
		for (int nCnt = 0; nCnt < g_FailCntDateObjectVector; nCnt++)
		{
			CScene3D::Create(D3DXVECTOR3(0, 0, 0), g_posObjectVector[nCnt][0], g_posObjectVector[nCnt][1], g_posObjectVector[nCnt][3], g_posObjectVector[nCnt][2]);

			//if (nCnt == 18)
			{
				m_fUseScene3D = true;
			}
		}
	}
	
	return bLand;
}

//==============================================================================
//Xファイルの読み込み
//==============================================================================
void CObject3D::LoadObject(void)
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

	pFile = fopen("data/TEXT/VectorGet.txt", "r");

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
				if (strcmp("VECTORSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_VECTORSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Vec1POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][0].x);
						}

						if (strcmp("Vec1POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][0].y);
						}

						if (strcmp("Vec1POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][0].z);
						}

						if (strcmp("Vec2POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][1].x);
						}

						if (strcmp("Vec2POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][1].y);
						}

						if (strcmp("Vec2POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][1].z);
						}

						if (strcmp("Vec3POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][2].x);
						}

						if (strcmp("Vec3POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][2].y);
						}

						if (strcmp("Vec3POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][2].z);
						}

						if (strcmp("Vec4POSX", pSetObjectEND) == 0)
						{
							// モデルposX
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][3].x);
						}

						if (strcmp("Vec4POSY", pSetObjectEND) == 0)
						{
							// モデルposY
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][3].y);
						}

						if (strcmp("Vec4POSZ", pSetObjectEND) == 0)
						{
							// モデルposZ
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &g_posObjectVector[g_FailCntDateObjectVector][3].z);
						}
					}
					g_FailCntDateObjectVector++;
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
	fclose(pFile);
}

D3DXMATRIX CObject3D::GetMtxWorld(void)
{
	return m_mtxWorld;
}
