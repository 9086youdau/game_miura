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
#include "camera.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CObject3D::CObject3D(int nPriority, int nType)
{
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
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeY)
{
	//初期化処理
	CSceneX::Init(pos, fSizeX, fSizeY);

	// ローカル変数宣言
	int nNumVtx;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;			// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = CSceneX::GetMesh()->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(CSceneX::GetMesh()->GetFVF());

	// 頂点バッファをロック
	CSceneX::GetMesh()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点座標の比較と更新
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 各座標の最大値の比較
		if (m_MaxObject.x < vtx.x)
		{
			m_MaxObject.x = vtx.x;
		}
		if (m_MaxObject.y < vtx.y)
		{
			m_MaxObject.y = vtx.y;
		}
		if (m_MaxObject.z < vtx.z)
		{
			m_MaxObject.z = vtx.z;
		}

		// 各座標の最小値の比較
		if (m_MinObject.x > vtx.x)
		{
			m_MinObject.x = vtx.x;
		}
		if (m_MinObject.y > vtx.y)
		{
			m_MinObject.y = vtx.y;
		}
		if (m_MinObject.z > vtx.z)
		{
			m_MinObject.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	CSceneX::GetMesh()->UnlockVertexBuffer();

	//引数から代入
	m_pos = pos;
	m_rot = rot;

	//モデルの回転設定
	SetRevolution(m_rot);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject3D::Uninit(void)
{
	//終了処理
	CSceneX::Uninit();

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
	if (m_MaxObject.x < 0.0f)
	{
		m_MaxObject.x *= -1.0f;
	}
	if (m_MaxObject.y < 0.0f)
	{
		m_MaxObject.y *= -1.0f;
	}
	if (m_MaxObject.z < 0.0f)
	{
		m_MaxObject.z *= -1.0f;
	}

	if (m_MinObject.x < 0.0f)
	{
		m_MinObject.x *= -1.0f;
	}
	if (m_MinObject.y < 0.0f)
	{
		m_MinObject.y *= -1.0f;
	}
	if (m_MinObject.z < 0.0f)
	{
		m_MinObject.z *= -1.0f;
	}

	//描画処理
	CSceneX::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char * TextName,char * TextureName)
{
	//モデルを生成
	CObject3D *pObject3D = NULL;
	pObject3D = new CObject3D;

	//NULLチェック
	if (pObject3D != NULL)
	{
		//Xファイル設定
		pObject3D->BindTextName(TextName);

		//テクスチャ設定
		pObject3D->BindTextureName(TextureName);

		//オブジェクトの初期化処理
		pObject3D->Init(pos, rot, 0.0f, 0.0f);
	}

	return pObject3D;
}

//==============================================================================
// 外積を利用した当たり判定
//==============================================================================
bool CObject3D::CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin)
{
	// ローカル変数

	CCamera *pCamera = CManager::GetCamera();	// カメラ情報の取得
	D3DXVECTOR3 rot = pCamera->GetRot();		// カメラ角度
	D3DXVECTOR3 CameraPos = pCamera->GetposV();	// カメラの位置
	bool bLand = false;

	D3DXVECTOR3		g_Vec[4];			// ベクトル
	D3DXVECTOR3		g_oldVec[4];		// 前のベクトル
	D3DXVECTOR3		posold;				// 前の位置
	float			PlayerVec[4];		// プレイヤーベクトル
	float			oldPlayerVec[4];	// 前のプレイヤーベクトル

	D3DXVECTOR3 pos = *pPos;			// 判定対象の位置
	posold = *pPosOld;					// 判定対象の位置

	//壁ずりベクトル計算用
	D3DXVECTOR3 VecP = *pPos - *pPosOld;						// 進行ベクトル
	fabsf(VecP.x);												// 絶対値
	fabsf(VecP.z);												// 絶対値
	D3DXVec3Normalize(&VecP, &VecP);

	float fLength[4];
	fLength[0] = sqrtf(m_MinObject.x * m_MinObject.x + m_MinObject.z * m_MinObject.z);
	fLength[1] = sqrtf(m_MinObject.x * m_MinObject.x + m_MaxObject.z * m_MaxObject.z);
	fLength[2] = sqrtf(m_MaxObject.x * m_MaxObject.x + m_MaxObject.z * m_MaxObject.z);
	fLength[3] = sqrtf(m_MaxObject.x * m_MaxObject.x + m_MinObject.z * m_MinObject.z);

	// 対角線角度
	float fAngle[4];
	fAngle[0] = atan2f(m_MinObject.z, m_MinObject.x);
	fAngle[1] = atan2f(m_MinObject.z, m_MaxObject.x);
	fAngle[2] = atan2f(m_MaxObject.z, m_MaxObject.x);
	fAngle[3] = atan2f(m_MaxObject.z, m_MinObject.x);

	//頂点の座標
	m_aPosColision[0].x = (m_pos.x + cosf(fAngle[1] - m_rot.y) * fLength[2]);
	m_aPosColision[0].z = (m_pos.z + sinf(fAngle[1] - m_rot.y) * fLength[2]);

	m_aPosColision[1].x = (m_pos.x + cosf(fAngle[2] + m_rot.y) * fLength[3]);
	m_aPosColision[1].z = (m_pos.z - sinf(fAngle[2] + m_rot.y) * fLength[3]);

	m_aPosColision[2].x = (m_pos.x - cosf(fAngle[3] - m_rot.y) * fLength[0]);
	m_aPosColision[2].z = (m_pos.z - sinf(fAngle[3] - m_rot.y) * fLength[0]);

	m_aPosColision[3].x = (m_pos.x - cosf(fAngle[0] + m_rot.y) * fLength[1]);
	m_aPosColision[3].z = (m_pos.z + sinf(fAngle[0] + m_rot.y) * fLength[1]);

	//並び方をただす
	//90ずれている場合
	if (m_aPosColision[0].x > m_aPosColision[2].x && m_aPosColision[0].z < m_aPosColision[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPosColision[0];

		m_aPosColision[0] = m_aPosColision[3];
		m_aPosColision[3] = m_aPosColision[2];
		m_aPosColision[2] = m_aPosColision[1];
		m_aPosColision[1] = vecold;
	}
	//180ずれている場合
	else if (m_aPosColision[0].x < m_aPosColision[2].x && m_aPosColision[0].z < m_aPosColision[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPosColision[0];

		D3DXVECTOR3 vecold2;
		vecold2 = m_aPosColision[1];

		m_aPosColision[0] = m_aPosColision[2];
		m_aPosColision[2] = vecold;
		m_aPosColision[1] = m_aPosColision[3];
		m_aPosColision[3] = vecold2;
	}
	//270ずれている場合
	else if (m_aPosColision[0].x < m_aPosColision[2].x && m_aPosColision[0].z > m_aPosColision[2].z)
	{
		D3DXVECTOR3 vecold;
		vecold = m_aPosColision[0];

		m_aPosColision[0] = m_aPosColision[1];
		m_aPosColision[1] = m_aPosColision[2];
		m_aPosColision[2] = m_aPosColision[3];
		m_aPosColision[3] = vecold;
	}

	//外積の式
	m_aVec[0] = m_aPosColision[1] - m_aPosColision[0];	// 右
	m_aVec[1] = m_aPosColision[2] - m_aPosColision[1];	// 下								 
	m_aVec[2] = m_aPosColision[3] - m_aPosColision[2];	// 左
	m_aVec[3] = m_aPosColision[0] - m_aPosColision[3];	// 上

	for (int nCount = 0; nCount < 4; nCount++)
	{
		// プレイヤーベクトル保存
		if (nCount == 0)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 1)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPosColision[nCount];
		}
		else if (nCount == 2)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 3)
		{
			g_oldVec[nCount] = posold + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPosColision[nCount];
		}

		// プレイヤーがm_Vecより右にいるかどうかの判定用
		oldPlayerVec[nCount] = (m_aVec[nCount].z * g_oldVec[nCount].x) - (m_aVec[nCount].x * g_oldVec[nCount].z);
	}
	for (int nCount = 0; nCount < 4; nCount++)
	{
		//外積の式2		
		if (nCount == 0)
		{// 右上からプレイヤーのベクトル
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMin, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 1)
		{// 右下プレイヤーのベクトル
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMax) - m_aPosColision[nCount];
		}
		else if (nCount == 2)
		{// 左下プレイヤーのベクトル
			g_Vec[nCount] = pos + D3DXVECTOR3(fWidthMax, 0.0f, 0.0f) - m_aPosColision[nCount];
		}
		else if (nCount == 3)
		{// 左上プレイヤーのベクトル
			g_Vec[nCount] = pos + D3DXVECTOR3(0.0f, 0.0f, fDepthMin) - m_aPosColision[nCount];
		}

		// プレイヤーがm_Vecより右にいるかどうかの判定用
		PlayerVec[nCount] = (m_aVec[nCount].z * g_Vec[nCount].x) - (m_aVec[nCount].x * g_Vec[nCount].z);
	}

	if (pPos->y <= m_pos.y + m_MaxObject.y - fHeightMin && pPos->y > m_pos.y - m_MinObject.y - fHeightMax)
	{
		if (PlayerVec[0] > 0.0f && PlayerVec[1] > 0.0f && PlayerVec[2] > 0.0f && PlayerVec[3] > 0.0f)
		{
			// 上下の判定　（間にいるかどうか）
			if (PlayerVec[3] > 0.0f || PlayerVec[1] > 0.0f)
			{// 前のベクトルが0より下の時
				if (oldPlayerVec[0] < 0.0f)
				{// 右の当たり判定			
					if (PlayerVec[0] > 0.0f)
					{
						//2頂点差を計算
						D3DXVECTOR3 a = m_aPosColision[0] - m_aPosColision[1];

						//角度を計算
						float fA = atan2f(a.z, a.x);

						//壁ずりベクトル計算用
						D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 法線ベクトル
						D3DXVec3Cross(&norVec, &a, &m_rot);
						D3DXVec3Normalize(&norVec, &norVec);
						fabsf(norVec.x);											// 絶対値
						fabsf(norVec.z);											// 絶対値
																					//norVec.y = 0;

						D3DXVECTOR3 Wvec;											// 壁ずりベクトル
						Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);

						if (m_rot.y == 0.0f)
						{
							pPos->x = pPosOld->x;				// 1フレーム前の位置に戻す																//pPos->z = pPosOld->z;
							pMove->x = 0.0f;
						}
						else
						{
							pPos->x = pPosOld->x;				// 1フレーム前の位置に戻す
							pPos->z = pPosOld->z;
							pMove->x = (Wvec.x) * 5;
							pMove->z = (Wvec.z) * 5;
						}
						bLand = true;

					}
				}
				// 左の当たり判定
				if (oldPlayerVec[2] < 0.0f)
				{
					if (PlayerVec[2] > 0.0f)
					{// 左

						//2頂点差を計算
						D3DXVECTOR3 a = m_aPosColision[2] - m_aPosColision[3];

						//角度を計算
						float fAtan2 = atan2f(a.z, a.x);

						//壁ずりベクトル計算用
						D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 法線ベクトル
						D3DXVec3Cross(&norVec, &a, &m_rot);
						D3DXVec3Normalize(&norVec, &norVec);
						fabsf(norVec.x);											// 絶対値
						fabsf(norVec.z);											// 絶対値
																					//norVec.y = 0;
						D3DXVECTOR3 Wvec;											// 壁ずりベクトル
						Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);

						if (m_rot.y == 0.0f)
						{
							pPos->x = pPosOld->x;				// 1フレーム前の位置に戻す																//pPos->z = pPosOld->z;
							pMove->x = 0.0f;
						}
						else
						{
							pPos->x = pPosOld->x;				// 1フレーム前の位置に戻す
							pPos->z = pPosOld->z;
							pMove->x = (Wvec.x) * 5;
							pMove->z = (Wvec.z) * 5;
						}

						bLand = true;
					}
				}

				// 左右に触れていないとき
				if (PlayerVec[0] > 0.0f || PlayerVec[2] > 0.0f)
				{
					if (oldPlayerVec[3] < 0.0f)
					{// 上の当たり判定			
						if (PlayerVec[3] > 0.0f)
						{

							//2頂点差を計算
							D3DXVECTOR3 a = m_aPosColision[0] - m_aPosColision[3];

							//角度を計算
							float fAtan2 = (float)atan2(a.z, a.x);
							//壁ずりベクトル計算用
							D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 法線ベクトル
							D3DXVec3Cross(&norVec, &a, &m_rot);
							D3DXVec3Normalize(&norVec, &norVec);
							fabsf(norVec.x);											// 絶対値
							fabsf(norVec.z);											// 絶対値
																						//norVec.y = 0;
							D3DXVECTOR3 Wvec;											// 壁ずりベクトル
							Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);

							if (m_rot.y == 0.0f)
							{
								pPos->z = pPosOld->z;				// 1フレーム前の位置に戻す																//pPos->z = pPosOld->z;
								pMove->z = 0.0f;
							}
							else
							{

								pPos->x = pPosOld->x;				// 1フレーム前の位置に戻す
								pPos->z = pPosOld->z;
								pMove->x = (Wvec.x) * 5;
								pMove->z = (Wvec.z) * 5;
							}
							bLand = true;
						}
					}
					if (oldPlayerVec[1] < 0.0f)
					{// 下の当たり判定			
						if (PlayerVec[1] > 0.0f)
						{// 
							//2頂点差を計算
							D3DXVECTOR3 a = m_aPosColision[2] - m_aPosColision[1];

							//角度を計算
							float fAtan2 = (float)atan2(a.z, a.x);

							//壁ずりベクトル計算用
							D3DXVECTOR3 norVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 法線ベクトル
							D3DXVec3Cross(&norVec, &a, &m_rot);							// 2つの3Dベクトルの積間を決定
							D3DXVec3Normalize(&norVec, &norVec);						// 3Dベクターの正規化
							fabsf(norVec.x);											// 絶対値
							fabsf(norVec.z);											// 絶対値

							D3DXVECTOR3 Wvec;											// 壁ずりベクトル
							Wvec = VecP - (D3DXVec3Dot(&VecP, &norVec) * norVec);		// 壁ずりベクトル計算

							if (m_rot.y == 0.0f)
							{
								pPos->z = pPosOld->z;				// 1フレーム前の位置に戻す																//pPos->z = pPosOld->z;
								pMove->z = 0.0f;
							}
							else
							{
								pPos->x = pPosOld->x;				// 1フレーム前の位置に戻す
								pPos->z = pPosOld->z;
								pMove->x = (Wvec.x) * 5;
								pMove->z = (Wvec.z) * 5;
							}
							bLand = true;
						}
					}
				}
			}
		}
	}

	return bLand;
}
