//================================================
//
//円のHP処理[CircleHp.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "CircleHp.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"
#include "gamepad.h"
#include "input.h"

//=================================================
//静的メンバ変数宣言
//=================================================
LPDIRECT3DTEXTURE9 CCircleHp::m_pTexture[2] = {};
int CCircleHp::m_sirclelife = 0;

//=============================================================================
//コンストラクタ
//=============================================================================
CCircleHp::CCircleHp(int nPriority) : CScene2D(nPriority)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CCircleHp::~CCircleHp()
{
}

//=============================================================================
//ナンバーの初期化処理
//=============================================================================
HRESULT CCircleHp::Init(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp)
{
	//テクスチャロード
	Load();

	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 50,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//タイプ代入
	m_CircleHp = CircleHp;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//位置情報の代入
	m_pos = D3DXVECTOR3(pos);

	//ローカル変数宣言
	float fAngle;
	float rad;

	fAngle = D3DX_PI * 2 / 49;
	rad = fAngle / 2;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[0].rhw = 1.0f;

	for (int nCntAppear = 1; nCntAppear < 52; nCntAppear++, rad += fAngle)
	{
		pVtx[nCntAppear].pos.x = pVtx[0].pos.x + 350.0f * cosf(rad);
		pVtx[nCntAppear].pos.y = pVtx[0].pos.y + 350.0f * sinf(rad);
		pVtx[nCntAppear].pos.z = 0.0f;
		pVtx[nCntAppear].rhw = 1.0f;
		pVtx[nCntAppear].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCntAppear].tex = D3DXVECTOR2(0.5f, 0.5f);
	}

	//初期化処理
	m_sirclelife = 0;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//ナンバーの終了処理
//=============================================================================
void CCircleHp::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//テクスチャの破棄
	for (int nCount = 0; nCount < 2; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

	Release();
}

//=============================================================================
//ナンバーの更新処理
//=============================================================================
void CCircleHp::Update(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == OBJTYPE_ENEMY_BOSS)
				{
					pVtx[0].pos = D3DXVECTOR3(pScene->GetPosition().x, pScene->GetPosition().y, 0.0f);
				}
			}
		}
	}

	//ローカル変数宣言
	float fAngle;
	float rad;

	fAngle = D3DX_PI * 2 / 49;
	rad = fAngle / 2;

	for (int nCntAppear = 1; nCntAppear < 51; nCntAppear++, rad += fAngle)
	{
		pVtx[nCntAppear].pos.x = pVtx[0].pos.x + 350.0f * cosf(rad);
		pVtx[nCntAppear].pos.y = pVtx[0].pos.y + 350.0f * sinf(rad);
		pVtx[nCntAppear].pos.z = 0.0f;
		pVtx[nCntAppear].rhw = 1.0f;
		pVtx[nCntAppear].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCntAppear].tex = D3DXVECTOR2(0.5f, 0.5f);
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//ライフがなくなったら消す
	if (m_sirclelife >= 50)
	{
		Uninit();
	}
}

//=============================================================================
//ナンバー描画処理
//=============================================================================
void CCircleHp::Draw(void)
{
	// 頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (m_CircleHp == CIRCLEHP_OUT)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[0]);

		for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nCntAppear);
		}
	}
	if (m_CircleHp == CIRCLEHP_IN)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[1]);

		for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nCntAppear - m_sirclelife);
		}
	}
}

//=============================================================================
//ナンバーのテクスチャロード処理
//=============================================================================
HRESULT CCircleHp::Load(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lifeout.png", &m_pTexture[0]);

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lifein.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
//ナンバーのテクスチャ破棄処理
//=============================================================================
void CCircleHp::Unload(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//テクスチャの開放
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
//ナンバーの設定・作成処理
//=============================================================================
CCircleHp * CCircleHp::Create(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp)
{
	//インスタンス生成
	CCircleHp *pCircleHp = NULL;
	pCircleHp = new CCircleHp;

	//初期化呼び出し
	pCircleHp->Init(pos, fHight, fWidth,CircleHp);

	return pCircleHp;
}

//=============================================================================
//ボスの体力を減らす処理
//=============================================================================
void CCircleHp::SetBossLife(int nDamage)
{
	//カウントを増やす
	m_sirclelife += nDamage;
}
