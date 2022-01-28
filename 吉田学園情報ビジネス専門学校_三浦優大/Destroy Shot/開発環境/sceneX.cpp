//=============================================================================
//
// シーンX処理 [sceneX.cpp]
// Author : Miura Yudai
//
//=============================================================================

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "player3D.h"
#include "renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
char *CSceneX::m_pTextName;
char *CSceneX::m_pTextureName;
LPD3DXMESH CSceneX::m_pGetMesh;

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
	D3DXLoadMeshFromX(m_pTextName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	// マテリアルへのポインタ
	D3DXMATERIAL *pMat;

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// ファイル名を使用してテクスチャを読み込む
	D3DXCreateTextureFromFile(
		pDevice,
		m_pTextureName,
		&m_pTexture);

	//位置
	m_pos = pos;
	m_pGetMesh = m_pMesh;

	//向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	//メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneX::Draw()
{
	//ローカル
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存
	D3DXMATERIAL *pMat;				//マテリアルへのポインタ
	D3DMATERIAL9 shadowDef;			// 現在のシャドウマテリアル保存用
	D3DXMATRIX ShadowMatrix;        //シャドウマトリックス
	D3DXVECTOR4 veclight;           //ライトのベクトル
	D3DXVECTOR3 pos, normal;        //平面上の座標、ベクトル
	D3DXPLANE plane;                //平面

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	veclight = D3DXVECTOR4(0.2f, 0.8f, 0.4f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//シャドウマトリックスの初期化
	D3DXMatrixIdentity(&ShadowMatrix);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//シャドウの位置設定
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

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

	// 現在のシャドウマテリアルを取得
	pDevice->GetMaterial(&shadowDef);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存したマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		shadowDef.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//マテリアルの設定
		pDevice->SetMaterial(&shadowDef);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//シャドウマトリックスの生成
		D3DXMatrixShadow(&ShadowMatrix, &veclight, &plane);

		D3DXMatrixMultiply(&ShadowMatrix, &m_mtxWorld, &ShadowMatrix);

		//各ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &ShadowMatrix);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&shadowDef);
}

//=============================================================================
// モデルの読み込み
//=============================================================================
void CSceneX::BindTextName(char *TextName)
{
	//モデルの読み込みを引数から
	m_pTextName = TextName;
}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CSceneX::BindTextureName(char * TextureName)
{
	//テクスチャの読み込みを引数から
	m_pTextureName = TextureName;
}

//=============================================================================
// Set位置
//=============================================================================
void CSceneX::SetPosition(D3DXVECTOR3 pos)
{
	//位置情報を引数から読み込み
	m_pos = pos;
}

//=============================================================================
// Set向き
//=============================================================================
void CSceneX::SetRevolution(D3DXVECTOR3 rot)
{
	//回転情報を引数から読み込み
	m_rot = rot;
}

//=============================================================================
// 生成処理
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, char *TextName)
{
	//シーンXを生成
	CSceneX *pSceneX = NULL;
	pSceneX = new CSceneX;

	m_pTextName = TextName;

	//NULLチェック
	if (pSceneX != NULL)
	{
		pSceneX->Init(pos, 0.0f, 0.0f);
	}

	return pSceneX;
}
