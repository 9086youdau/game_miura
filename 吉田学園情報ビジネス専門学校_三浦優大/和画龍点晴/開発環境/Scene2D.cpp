//================================================
//
// シーン2D処理[Scene2D.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "Scene2D.h"
#include "Renderer.h"
#include "manager.h"
#include "input.h"
#include "enemy.h"

//=============================================================================
//コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ポリゴン2Dの初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//位置情報の代入
	m_pos = D3DXVECTOR3(pos);

	//カラー情報設定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//対角線の長さの設定
	m_fLength = sqrtf(fWidth * fWidth + fHight * fHight);

	//対角線の角度の設定
	m_fAngle = atan2f(fWidth, fHight);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y, m_pos.z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHight, m_pos.z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + fHight, m_pos.z);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴン2Dの終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// ポリゴン2Dの更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// ポリゴン2Dの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	// 頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ポリゴンの設定処理
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーの頂点座標
	pVtx[0].pos.x = pos.x - sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.y = pos.y - cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.z = m_pos.z;

	pVtx[1].pos.x = pos.x + sinf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.z = m_pos.z;

	pVtx[2].pos.x = pos.x - sinf(rot.z + m_fAngle) * m_fLength;
	pVtx[2].pos.y = pos.y - cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[2].pos.z = m_pos.z;

	pVtx[3].pos.x = pos.x + sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[3].pos.z = m_pos.z;

	//値を代入する
	m_pos = pos;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴンのテクスチャ処理
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// 頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture);

	//値を代入する
	m_pTexture = pTexture;
}

//=============================================================================
// ポリゴンのテクスチャ処理
//=============================================================================
void CScene2D::SetTex(float tex,int nCnt)
{
	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(tex + (tex * nCnt), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(tex + (tex * nCnt),1.0f);
	pVtx[2].tex = D3DXVECTOR2(tex * nCnt, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(tex * nCnt, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 背景のテクスチャ処理
//=============================================================================
void CScene2D::SetBg(float size,float nCnt)
{
	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + nCnt);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.5f + nCnt);
	pVtx[2].tex = D3DXVECTOR2(size, 0.0f + nCnt);
	pVtx[3].tex = D3DXVECTOR2(size, 1.5f + nCnt);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// カラー変換関数処理
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ランキングのテクスチャ処理
//=============================================================================
void CScene2D::SetRanking(float nRanking)
{
	//ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f + nRanking);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.2f + nRanking);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f + nRanking);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.2f + nRanking);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
//エネミーの色変更処理
//=============================================================================
D3DXCOLOR CScene2D::SetEnemyCol(D3DXCOLOR col)
{
	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//返り値
	return D3DXCOLOR();
}
//=============================================================================
//衝突判定
//=============================================================================
bool CScene2D::Collision(D3DXVECTOR3 pos, float fsizeX, float fsizeY, OBJTYPE Type, CScene **pColScene)
{
	bool bCol = false;
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < LOOP_POLYGON; nCntScene++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority,nCntScene);

			if (pScene != NULL)
			{
				OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == Type)
				{
					//位置を取得
					D3DXVECTOR3 Epos = pScene->GetPosition();

					float fSizeX, fSizeY;

					//サイズを取得
					fSizeX = pScene->GetSizeX();
					fSizeY = pScene->GetSizeY();

					if (Epos.y + fSizeX >= pos.y - fsizeY &&  Epos.y - fSizeX <= pos.y + fsizeY&&
						pos.x - fsizeX <= Epos.x + fSizeX && pos.x + fsizeX >= Epos.x - fSizeY)
					{
						*pColScene = pScene;

						bCol = true;
					}
				}
			}
		}
	}
	return bCol;
}
