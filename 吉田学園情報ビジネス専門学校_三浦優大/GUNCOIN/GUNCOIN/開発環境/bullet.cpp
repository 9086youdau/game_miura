//================================================
//
//弾の処理[Player.h]
// Author; miura yuudai
//
//================================================
#include "bullet.h"
#include "input.h"
#include "Billboard.h"

//マクロ定義
#define MAX_BULLET_SIZE (5)//エフェクトのサイズ
#define MAX_BULLET (256)//配列

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturebullet = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffbullet;//頂点ポインタ
BULLET g_abullet[MAX_BULLET];//弾の情報
D3DXCOLOR g_colorbullet;

//==================
//弾の初期化処理
//==================
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/10.png", &g_pTexturebullet);

	//初期化
	for (int nCntEffect = 0; nCntEffect < MAX_BULLET; nCntEffect++)
	{
		g_abullet[nCntEffect].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abullet[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abullet[nCntEffect].bUse = false;
		g_colorbullet = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		g_abullet[nCntEffect].nLife = 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffbullet, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtx;

	BULLET *pBullet;
	pBullet = &g_abullet[0];

	//頂点バッファをロック
	g_pVtxBuffbullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_BULLET; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pBullet->Pos.x - MAX_BULLET_SIZE, pBullet->Pos.y + MAX_BULLET_SIZE, pBullet->Pos.z);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(pBullet->Pos.x + MAX_BULLET_SIZE, pBullet->Pos.y + MAX_BULLET_SIZE, pBullet->Pos.z);
		pVtx[2].pos = D3DXVECTOR3(pBullet->Pos.x - MAX_BULLET_SIZE, pBullet->Pos.y - MAX_BULLET_SIZE, pBullet->Pos.z);
		pVtx[3].pos = D3DXVECTOR3(pBullet->Pos.x + MAX_BULLET_SIZE, pBullet->Pos.y - MAX_BULLET_SIZE, pBullet->Pos.z);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = g_colorbullet;
		pVtx[1].col = g_colorbullet;
		pVtx[2].col = g_colorbullet;
		pVtx[3].col = g_colorbullet;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //4頂点ごとずらす
	}

	//頂点バッファをアンロック
	g_pVtxBuffbullet->Unlock();

	return  S_OK;
}

//====================
//弾の終了処理
//====================
void UninitBullet(void)
{
	//テクスチャの開放
	if (g_pTexturebullet != NULL)
	{
		g_pTexturebullet->Release();
		g_pTexturebullet = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffbullet != NULL)
	{
		g_pVtxBuffbullet->Release();
		g_pVtxBuffbullet = NULL;
	}
}

//====================
//弾の更新処理
//====================
void UpdateBullet(void)
{
	BULLET *pBullet;
	pBullet = &g_abullet[0];


	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	g_pVtxBuffbullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_BULLET; nCntEffect++, pBullet++)
	{
		if (pBullet->bUse == true)//エフェクトを使用しているかどうか
		{
			//int nLife = 20;//寿命をランダム

			//SetBillboard(pBullet->Pos, nLife, pBullet->move ,0)

			//寿命チェック
			pBullet->nLife--;
			if (pBullet->nLife == 0)//寿命が0になったら
			{
				pBullet->bUse = false;//使用しない
			}

			pBullet->Pos += pBullet->move;

		}
		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffbullet->Unlock();
}

//====================
//弾の描画処理
//====================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	BULLET *pBullet;
	pBullet = &g_abullet[0];

	//デバイスの取得
	pDevice = GetDevice();

	//ビューマトリックス取得用
	D3DXMATRIX mtxView;

	//計算用マトリックス
	D3DXMATRIX mtxTrans;

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntEffect = 0; nCntEffect < MAX_BULLET; nCntEffect++,pBullet++)
	{
		if (g_abullet[nCntEffect].bUse == true)
		{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pBullet->mtxWorld);

		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&pBullet->mtxWorld, NULL, &mtxView);//逆行列を求める
		pBullet->mtxWorld._41 = 0.0f;
		pBullet->mtxWorld._42 = 0.0f;
		pBullet->mtxWorld._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pBullet->Pos.x, pBullet->Pos.y, pBullet->Pos.z);

		D3DXMatrixMultiply(&pBullet->mtxWorld, &pBullet->mtxWorld, &mtxTrans);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffbullet, 0, sizeof(VERTEX_3D));

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pBullet->mtxWorld);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturebullet);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

		}

	}
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//================
//弾の設定
//================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife)
{
	VERTEX_3D *pVtx;

	BULLET *pBullet;
	pBullet = &g_abullet[0];

	// 頂点バッファをロック
	g_pVtxBuffbullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_BULLET; nCntEffect++, pBullet++)
	{
		if (pBullet->bUse == false)//弾を使用しているかどうか
		{
			//位置設定
			pBullet->Pos = pos;

			pBullet->nLife = nLife;

			pBullet->move = move;

			//弾の使用
			pBullet->bUse = true;
			break;
		}
		//4頂点ごとずらす
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffbullet->Unlock();
}
BULLET *GetBullet(void)
{
	return &g_abullet[0];
}