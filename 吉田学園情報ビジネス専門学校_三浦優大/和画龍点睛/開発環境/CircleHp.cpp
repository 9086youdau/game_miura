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

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_SIRCLE (50)
#define MAX_TEXTURE (2)

//=================================================
//静的メンバ変数宣言
//=================================================
LPDIRECT3DTEXTURE9 CCircleHp::m_pTexture[MAX_TEXTURE] = {};
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
//円ゲージの初期化処理
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
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SIRCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	//2Dのポインタ
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

	//角度を求める計算
	fAngle = D3DX_PI * 2 / (MAX_SIRCLE - 1);
	rad = fAngle / 2;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[0].rhw = 1.0f;

	//位置設定
	for (int nCntAppear = 1; nCntAppear < (MAX_SIRCLE + 2); nCntAppear++, rad += fAngle)
	{
		pVtx[nCntAppear].pos.x = pVtx[0].pos.x + 150.0f * cosf(rad);
		pVtx[nCntAppear].pos.y = pVtx[0].pos.y + 150.0f * sinf(rad);
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
//円ゲージの終了処理
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
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

	//破棄処理
	Release();
}

//=============================================================================
//円ゲージの更新処理
//=============================================================================
void CCircleHp::Update(void)
{
	//2Dのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//当たり判定処理
	for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			//シーンからゲットしてくる
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCnt);

			//ポインタがNULLじゃなかったら
			if (pScene != NULL)
			{
				//OBJTYPEを代入する
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				//OBJTYPEがボスタイプだったら
				if (objType == OBJTYPE_ENEMY_BOSS)
				{
					//位置を参照する
					pVtx[0].pos = D3DXVECTOR3(pScene->GetPosition().x, pScene->GetPosition().y, 0.0f);
				}
			}
		}
	}

	//ローカル変数宣言
	float fAngle;
	float rad;

	//角度を求める
	fAngle = D3DX_PI * 2 / (MAX_SIRCLE - 1);
	rad = fAngle / 2;

	//位置設定
	for (int nCntAppear = 1; nCntAppear < 51; nCntAppear++, rad += fAngle)
	{
		pVtx[nCntAppear].pos.x = pVtx[0].pos.x + 150.0f * cosf(rad);
		pVtx[nCntAppear].pos.y = pVtx[0].pos.y + 150.0f * sinf(rad);
		pVtx[nCntAppear].pos.z = 0.0f;
		pVtx[nCntAppear].rhw = 1.0f;
		pVtx[nCntAppear].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCntAppear].tex = D3DXVECTOR2(0.5f, 0.5f);
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//ライフがなくなったら消す
	if (m_sirclelife >= MAX_SIRCLE)
	{
		//終了処理	
		Uninit();
	}
}

//=============================================================================
//円ゲージの描画処理
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

		for (int nCntAppear = 0; nCntAppear < MAX_SIRCLE; nCntAppear++)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nCntAppear);
		}
	}
	if (m_CircleHp == CIRCLEHP_IN)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[1]);

		for (int nCntAppear = 0; nCntAppear < MAX_SIRCLE; nCntAppear++)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nCntAppear - m_sirclelife);
		}
	}
}

//=============================================================================
//円ゲージのテクスチャロード処理
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
//円ゲージのテクスチャ破棄処理
//=============================================================================
void CCircleHp::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
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
//円ゲージの設定・作成処理
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
//円ゲージのボスの体力を減らす処理
//=============================================================================
void CCircleHp::SetBossLife(int nDamage)
{
	//カウントを増やす
	m_sirclelife += nDamage;
}
