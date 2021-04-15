#include "Timer.h"
#include "fade.h"

//===========
//マクロ定義
//===========
#define MAX_TYPE_TIMER (3)//敵の種類数
#define MAX_TIMER_SIZE (30)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureTimer = NULL;//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;//頂点バッファへのポインタ
D3DXVECTOR3 g_aTimer[MAX_TIMER];//敵の情報
TIMER g_nTimer;//スコア

HRESULT InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_apTextureTimer);
	
	g_nTimer.nTimer = 99;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		g_aTimer[nCntTimer] = D3DXVECTOR3(920, 50, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x - MAX_TIMER_SIZE) + nCntTimer * 70, g_aTimer[nCntTimer].y + MAX_TIMER_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x - MAX_TIMER_SIZE) + nCntTimer * 70, g_aTimer[nCntTimer].y - MAX_TIMER_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x + MAX_TIMER_SIZE) + nCntTimer * 70, g_aTimer[nCntTimer].y + MAX_TIMER_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x + MAX_TIMER_SIZE) + nCntTimer * 70, g_aTimer[nCntTimer].y - MAX_TIMER_SIZE, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255,255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255,255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255,255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255,255, 255);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

		pVtx += 4;//4頂点ごとずらす
	}
	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();

	return S_OK;
}
void UninitTimer(void)
{
	//テクスチャの開放
	if (g_apTextureTimer != NULL)
	{
		g_apTextureTimer->Release();
		g_apTextureTimer = NULL;
	}
	

	//頂点バッファの開放
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}
void UpdateTimer(void)
{
	VERTEX_2D *pVtx;

	int nNumber[2];

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	nNumber[0] = g_nTimer.nTimer % 100 / 10;
	nNumber[1] = g_nTimer.nTimer % 10 / 1;

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x - MAX_TIMER_SIZE) + nCntTimer * 60, g_aTimer[nCntTimer].y + MAX_TIMER_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x - MAX_TIMER_SIZE) + nCntTimer * 60, g_aTimer[nCntTimer].y - MAX_TIMER_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x + MAX_TIMER_SIZE) + nCntTimer * 60, g_aTimer[nCntTimer].y + MAX_TIMER_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aTimer[nCntTimer].x + MAX_TIMER_SIZE) + nCntTimer * 60, g_aTimer[nCntTimer].y - MAX_TIMER_SIZE, 0.0f);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber[nCntTimer], 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber[nCntTimer], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber[nCntTimer] + 1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber[nCntTimer] + 1), 0.0f);

		if (g_nTimer.nTimer == 0)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}

		pVtx += 4;//4頂点ごとずらす

	}
	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();
}
//==================
//ポリゴンの描画処理
//==================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_apTextureTimer);

	//ポリゴンの描画
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
	}
	
}
void AddTimer(int nValue)
{
	g_nTimer.nTimer += nValue;
}
TIMER *GetTimer(void)
{
	return &g_nTimer;
}


