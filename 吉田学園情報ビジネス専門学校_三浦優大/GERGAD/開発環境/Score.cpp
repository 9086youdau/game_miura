#include "Score.h"
//makuro
#define MAX_TYPE_SCORE (3)//敵の種類数
#define MAX_SCORE_SIZE (30)

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureScore = NULL;//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//頂点バッファへのポインタ
D3DXVECTOR3 g_aScore[MAX_SCORE];//敵の情報
int g_nScore = 0;//スコア

HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_apTextureScore);

	g_nScore = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_SCORE; nCntEnemy++)
	{
		g_aScore[nCntEnemy] = D3DXVECTOR3(1390, 100, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x - MAX_SCORE_SIZE) + nCntEnemy * 70, g_aScore[nCntEnemy].y + MAX_SCORE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x - MAX_SCORE_SIZE) + nCntEnemy * 70, g_aScore[nCntEnemy].y - MAX_SCORE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x + MAX_SCORE_SIZE) + nCntEnemy * 70, g_aScore[nCntEnemy].y + MAX_SCORE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x + MAX_SCORE_SIZE) + nCntEnemy * 70, g_aScore[nCntEnemy].y - MAX_SCORE_SIZE, 0.0f);

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
	g_pVtxBuffScore->Unlock();

	return S_OK;
}
void UninitScore(void)
{
	//テクスチャの開放
	for (int nCount = 0; nCount < MAX_SCORE; nCount++)
	{
		if (g_apTextureScore != NULL)
		{
			g_apTextureScore->Release();
			g_apTextureScore = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
void UpdateScore(void)
{
	VERTEX_2D *pVtx;
	int nNumber[8];
	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	nNumber[0] = g_nScore % 100000000 / 10000000;
	nNumber[1] = g_nScore % 10000000 / 1000000;
	nNumber[2] = g_nScore % 1000000 / 100000;
	nNumber[3] = g_nScore % 100000 / 10000;
	nNumber[4] = g_nScore % 10000 / 1000;
	nNumber[5] = g_nScore % 1000 / 100;
	nNumber[6] = g_nScore % 100 / 10;
	nNumber[7] = g_nScore % 10 / 1;

	for (int nCntEnemy = 0; nCntEnemy < MAX_SCORE; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x - MAX_SCORE_SIZE) + nCntEnemy * 60, g_aScore[nCntEnemy].y + MAX_SCORE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x - MAX_SCORE_SIZE) + nCntEnemy * 60, g_aScore[nCntEnemy].y - MAX_SCORE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x + MAX_SCORE_SIZE) + nCntEnemy * 60, g_aScore[nCntEnemy].y + MAX_SCORE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((g_aScore[nCntEnemy].x + MAX_SCORE_SIZE) + nCntEnemy * 60, g_aScore[nCntEnemy].y - MAX_SCORE_SIZE, 0.0f);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.1f*nNumber[nCntEnemy], 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f*nNumber[nCntEnemy], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f*(nNumber[nCntEnemy] + 1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f*(nNumber[nCntEnemy] + 1), 0.0f);

		pVtx += 4;//4頂点ごとずらす

	}
	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}
//==================
//ポリゴンの描画処理
//==================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_apTextureScore);

	//ポリゴンの描画
	for (int nCutEnemy = 0; nCutEnemy < MAX_SCORE; nCutEnemy++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCutEnemy * 4, 2);
	}
	
}
void AddScore(int nValue)
{
	g_nScore += nValue;
}
void SetScore(void)
{
	g_nScore = 0;
}


