//================================================
//
// シーンX処理[SceneX.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "SceneX.h"
#include "manager.h"
#include "Renderer.h"

//=================================================
//静的メンバ変数宣言
//=================================================


//=================================================
//コンストラクタ
//=================================================
CSceneX::CSceneX()
{
}

//=================================================
//デストラクタ
//=================================================
CSceneX::~CSceneX()
{
}

//=================================================
//シーンXの初期化処理
//=================================================
HRESULT CSceneX::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/prant2.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//回転処理
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//位置処理
	m_pos = pos;

	return S_OK;
}

//=================================================
//シーンXの終了処理
//=================================================
void CSceneX::Uninit(void)
{
	if (m_pMesh != NULL)
	{// メッシュの破棄
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	else if (m_pBuffMat != NULL)
	{// バッファの破棄
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//破棄処理
	Release();
}

//=================================================
//シーンXの更新処理
//=================================================
void CSceneX::Update(void)
{
	//タイプ分岐
	if (m_scenetype == SCENEXTYPE_LEFT)
	{
		//回転処理
		m_rot.y -= 0.02f;
	}

	else if (m_scenetype == SCENEXTYPE_RIGHT)
	{
		//回転処理
		m_rot.y += 0.02f;
	}
}

//=================================================
//シーンXの描画処理
//=================================================
void CSceneX::Draw(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//マテリアルへのポインタ
	D3DXMATERIAL *pMat;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
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

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=================================================
//シーンXのクリエイト処理
//=================================================
CSceneX * CSceneX::Create(D3DXVECTOR3 pos,SCENEXTYPE scenextype)
{
	//インスタンス生成
	CSceneX *pSceneX = NULL;
	pSceneX = new CSceneX;

	//初期化呼び出し
	pSceneX->Init(pos, 0.0f, 0.0f);

	//タイプを引数から代入
	pSceneX->m_scenetype = scenextype;

	return pSceneX;
}
