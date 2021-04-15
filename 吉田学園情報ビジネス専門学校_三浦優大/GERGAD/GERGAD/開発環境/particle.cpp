//================================================
//
//パーティクル処理[patitle.h]
// Author; miura yuudai
//
//================================================

//================================================
//ヘッダー読み込み
//================================================
#include "particle.h"
#include "effect.h"
#include "enemy.h"

//================================================
//マクロ定義
//================================================
#define MAX_PARTICLE_SIZE (4)//パーティクルのサイズ
#define MAX_PARTICLE (3056)//パーティクルの最大数

//==============================
//グローバル変数
//==============================
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;//テクスチャ設定
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle;//頂点ポインタ
PARTICLE g_aparticle[MAX_PARTICLE];//弾の情報
D3DXCOLOR g_colorparticle;//カラー設定
D3DXVECTOR3 g_posBase;//パーティクルの中心

//========================================
//パーティクルの初期化
//========================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureParticle);

	//グローバル変数の初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aparticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aparticle[nCntParticle].bUse = false;
		g_colorparticle = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		g_posBase = D3DXVECTOR3(600.0f, 350.0f, 0.0f);
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL)))
	{
		return E_FAIL;
	}

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aparticle[nCntParticle].pos.x - MAX_PARTICLE_SIZE, g_aparticle[nCntParticle].pos.y + MAX_PARTICLE_SIZE, 0.0f);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(g_aparticle[nCntParticle].pos.x - MAX_PARTICLE_SIZE, g_aparticle[nCntParticle].pos.y - MAX_PARTICLE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aparticle[nCntParticle].pos.x + MAX_PARTICLE_SIZE, g_aparticle[nCntParticle].pos.y + MAX_PARTICLE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aparticle[nCntParticle].pos.x + MAX_PARTICLE_SIZE, g_aparticle[nCntParticle].pos.y - MAX_PARTICLE_SIZE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_colorparticle;
		pVtx[1].col = g_colorparticle;
		pVtx[2].col = g_colorparticle;
		pVtx[3].col = g_colorparticle;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		 pVtx += 4; //4頂点ごとずらす
	}

	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();

	return  S_OK;
}

//====================================
//パーティクルの終了処理
//====================================
void UninitParticle(void)
{
	//テクスチャの開放
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//======================================
//パーティクルの更新処理
//======================================
void UpdateParticle(void)
{
	//ポインタ変数
	PARTICLE *pParticle;
	pParticle = &g_aparticle[0];
	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == true)//パーティクルを使用しているかどうか
		{
			//位置の更新
			pParticle->pos += pParticle->move;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pParticle->pos.x - MAX_PARTICLE_SIZE, pParticle->pos.y + MAX_PARTICLE_SIZE, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(pParticle->pos.x - MAX_PARTICLE_SIZE, pParticle->pos.y - MAX_PARTICLE_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pParticle->pos.x + MAX_PARTICLE_SIZE, pParticle->pos.y + MAX_PARTICLE_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pParticle->pos.x + MAX_PARTICLE_SIZE, pParticle->pos.y - MAX_PARTICLE_SIZE, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = pParticle->color;
			pVtx[1].col = pParticle->color;
			pVtx[2].col = pParticle->color;
			pVtx[3].col = pParticle->color;

			//寿命チェック
			pParticle->nLife--;
			if (pParticle->nLife == 0)//寿命が0になったら
			{
				pParticle->bUse = false;//使用しない
			}
		}

		//4頂点ごとずらす
		pVtx += 4;
	}
	// 頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();

	for (int nCntAppear = 0; nCntAppear < 9; nCntAppear++)
	{
		// 頂点バッファをロック
		g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pParticle = &g_aparticle[0];

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
		{
			if (pParticle->bUse == false)//パーティクルを使用しているかどうか
			{
				//位置の更新
				pParticle->pos = pEnemy->Pos;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pParticle->pos.x - pParticle->fRadius, pParticle->pos.y + pParticle->fRadius, 0.0f);//2Dは0.0固定
				pVtx[1].pos = D3DXVECTOR3(pParticle->pos.x - pParticle->fRadius, pParticle->pos.y - pParticle->fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pParticle->pos.x + pParticle->fRadius, pParticle->pos.y + pParticle->fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pParticle->pos.x + pParticle->fRadius, pParticle->pos.y - pParticle->fRadius, 0.0f);

				//カラー設定
				pParticle->color = g_colorparticle;

				//頂点カラーの設定
				pVtx[0].col = pParticle->color;
				pVtx[1].col = pParticle->color;
				pVtx[2].col = pParticle->color;
				pVtx[3].col = pParticle->color;

				//パーティクル設定
				pParticle->nLife = 100 + rand() % 150;

				float fAngle;

				fAngle = (float)(rand() % 628 - 314) / 100.0f;

				pParticle->move.x = sinf(fAngle)*3.5f;

				pParticle->move.y = cosf(fAngle)*3.5f;

				pParticle->bUse = true;
				break;
			}

			//4頂点ごとずらす
			pVtx += 4;

		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}

//=========================================
//パーティクルの描画処理
//=========================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//レンダーステートの設定(加算合成）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aparticle[nCntParticle].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle *4, 2);
		}
	}

	//レンダーステートの設定(元に戻す）
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
//パーティクルの設定
//========================================
void SetParticle(D3DXVECTOR3 pos,int nLife, D3DXVECTOR3 move)
{
	//ポインタ変数
	PARTICLE *pParticle;
	pParticle = &g_aparticle[0];

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
	{
		if (pParticle->bUse == false)//弾を使用しているかどうか
		{
			//位置設定
			pParticle->pos = pos;

			//体力の設定
			pParticle->nLife = nLife;

			//移動量
			pParticle->move = move;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pParticle->pos.x - MAX_PARTICLE_SIZE, pParticle->pos.y + MAX_PARTICLE_SIZE, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(pParticle->pos.x - MAX_PARTICLE_SIZE, pParticle->pos.y - MAX_PARTICLE_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pParticle->pos.x + MAX_PARTICLE_SIZE, pParticle->pos.y + MAX_PARTICLE_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pParticle->pos.x + MAX_PARTICLE_SIZE, pParticle->pos.y - MAX_PARTICLE_SIZE, 0.0f);

			//弾の使用
			pParticle->bUse = true;
			break;
		}

		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}
