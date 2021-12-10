//=============================================================================
//
// シーンX処理 [sceneX.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "sceneX.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPriority, int nType) : CScene(nPriority, nType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//model読み込み
	D3DXLoadMeshFromX("data/MODEL/model.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	//BYTE *pVtxBuff;		//頂点バッファへのポインタ 

	D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	//頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();
	m_nNumVtx = nNumVtx;

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//位置
	m_pos = D3DXVECTOR3(40.0f, 60.0f, 0.0f);

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.02f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void)
{
	m_fRot += 0.01f;
	m_rot.y = m_fRot;
}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	//ローカル
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存
	D3DXMATERIAL *pMat;				//マテリアルへのポインタ

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	////Zバッファ
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライティングを無効する
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	////加算合成の設定
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

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

	////Zバッファ
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	////通常の合成に戻す
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 生成処理
//=============================================================================
CSceneX *CSceneX::Create()
{
	//シーンXを生成
	CSceneX *pSceneX = NULL;
	pSceneX = new CSceneX;

	//NULLチェック
	if (pSceneX != NULL)
	{
		pSceneX->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}

	return pSceneX;
}
