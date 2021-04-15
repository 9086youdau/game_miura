#include "Coin.h"
#include "fade.h"

//===========
//マクロ定義
//===========
#define MAX_TYPE_COIN (3)//敵の種類数
#define MAX_COIN_SIZE (30)

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCoin = NULL;//頂点バッファへのポインタ
D3DXVECTOR3 g_aCoin[MAX_COIN];//敵の情報
COIN g_nCoin;//スコア
int nCoin = 0;

HRESULT InitCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_nCoin.g_apTextureCoin);

	//初期化
	nCoin = 0;
	for (int nCntTimer = 0; nCntTimer < MAX_COIN; nCntTimer++)
	{
		g_aCoin[nCntTimer] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TYPE_COIN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCoin,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTimer = 0; nCntTimer < MAX_COIN; nCntTimer++)
	{
		g_aCoin[nCntTimer] = D3DXVECTOR3(220, 50, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x - MAX_COIN_SIZE) + nCntTimer * 70, g_aCoin[nCntTimer].y + MAX_COIN_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x - MAX_COIN_SIZE) + nCntTimer * 70, g_aCoin[nCntTimer].y - MAX_COIN_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x + MAX_COIN_SIZE) + nCntTimer * 70, g_aCoin[nCntTimer].y + MAX_COIN_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x + MAX_COIN_SIZE) + nCntTimer * 70, g_aCoin[nCntTimer].y - MAX_COIN_SIZE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}
	//頂点バッファをアンロック
	g_pVtxBuffCoin->Unlock();

	return S_OK;
}
void UninitCoin(void)
{
	//テクスチャの開放
	if (g_nCoin.g_apTextureCoin != NULL)
	{
		g_nCoin.g_apTextureCoin->Release();
		g_nCoin.g_apTextureCoin = NULL;
	}


	//頂点バッファの開放
	if (g_pVtxBuffCoin != NULL)
	{
		g_pVtxBuffCoin->Release();
		g_pVtxBuffCoin = NULL;
	}
}
void UpdateCoin(void)
{
	VERTEX_2D *pVtx;

	int nNumber[2];

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVtx, 0);

	nNumber[0] = nCoin % 100 / 10;
	nNumber[1] = nCoin % 10 / 1;

	for (int nCntTimer = 0; nCntTimer < MAX_COIN; nCntTimer++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x - MAX_COIN_SIZE) + nCntTimer * 60, g_aCoin[nCntTimer].y + MAX_COIN_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x - MAX_COIN_SIZE) + nCntTimer * 60, g_aCoin[nCntTimer].y - MAX_COIN_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x + MAX_COIN_SIZE) + nCntTimer * 60, g_aCoin[nCntTimer].y + MAX_COIN_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aCoin[nCntTimer].x + MAX_COIN_SIZE) + nCntTimer * 60, g_aCoin[nCntTimer].y - MAX_COIN_SIZE, 0.0f);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber[nCntTimer], 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber[nCntTimer], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber[nCntTimer] + 1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber[nCntTimer] + 1), 0.0f);
		pVtx += 4;//4頂点ごとずらす


	}
	//頂点バッファをアンロック
	g_pVtxBuffCoin->Unlock();
}
//==================
//ポリゴンの描画処理
//==================
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCoin, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_nCoin.g_apTextureCoin);

	//ポリゴンの描画
	for (int nCntTimer = 0; nCntTimer < MAX_COIN; nCntTimer++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
	}

}
void AddCoin(int nValue)
{
	nCoin += nValue;
}
COIN *GetCoin(void)
{
	return &g_nCoin;
}