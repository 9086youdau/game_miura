//-----------------------------------------------------------------
//
// リザルト画面 (bg.cpp)
// Author:miura yudai
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "bg.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "cavegame.h"
#include "Player.h"
#include "Block.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_PATTERN     (3)										// アニメーションパターンNo.の最大数
#define TITLELOGO_X		(1000.0f)								// タイトルロゴ幅
#define TITLELOGO_Y		(200.0f)								// タイトルロゴの高さ
#define HEIGHT_Y		(500.0f * -1.0f)						// タイトルロゴの初期中心Y座標
#define PRESSENTER_X	(800.0f)								// プレスエンターの幅
#define PRESSENTER_Y	(300.0f)								// プレスエンターの高さ

//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffbg = NULL;
LPDIRECT3DTEXTURE9 g_pTexturebg[3] = {};
D3DXVECTOR3 g_posbgLogo;						// タイトルロゴの中心座標
D3DXVECTOR3 g_posPressEnterbg;					// プレスエンターの中心座標
D3DXCOLOR g_bgcolor;
BG  g_abg[256];
float g_nCntbg;
int g_nCntbg2;
float nCounter;

//-----------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------
HRESULT InitBG(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();
	int nCntEnemy = GetGameNumber();
	int nCntEEnemy = GetcaveGameNumber();

	// 変数の初期化
	g_posbgLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, HEIGHT_Y, 0.0f);
	g_posPressEnterbg = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4), 0.0f);
	g_bgcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntbg = 0;
	float nCounter = 0.0f;

	//テキスチャの読み込み
	if (nCntEnemy == 1)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stage.png", &g_pTexturebg[1]);
	}

	if (nCntEEnemy == 1)
	{
		D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/840bfa1514e9209cf3a445f6d4502f47_t.jpeg", &g_pTexturebg[1]);
	}

	for (int nCntbg = 0; nCntbg < 256; nCntbg++)
	{
		g_abg[nCntbg].nType = 0;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PATTERN, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffbg,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffbg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntbg = 0; nCntbg < MAX_PATTERN; nCntbg++)
	{
		// 頂点座標の設定
		SetVertexBG(nCntbg);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_bgcolor;
		pVtx[1].col = g_bgcolor;
		pVtx[2].col = g_bgcolor;
		pVtx[3].col = g_bgcolor;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 情報の更新
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffbg->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------
void UninitBG(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffbg != NULL)
	{
		g_pVtxBuffbg->Release();
		g_pVtxBuffbg = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
	{
		if (g_pTexturebg[nCntTexture] != NULL)
		{
			g_pTexturebg[nCntTexture]->Release();
			g_pTexturebg[nCntTexture] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// タイトルの更新処理
//-----------------------------------------------------------------
void UpdateBG(void)
{

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	BLOCK *pBlock;

	pBlock = GetBlock();

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffbg->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardPress(DIK_D) == true)//右に進む
	{
		nCounter -= pBlock->move.x / 10000;
	}
	if (GetKeyboardPress(DIK_A) == true)//右に進む
	{
		nCounter -= pBlock->move.x / 10000;
	}
	

	//頂点情報の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f + nCounter, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0f + nCounter, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f + nCounter, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f + nCounter, 0.0f);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + nCounter, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + nCounter, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f + nCounter, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f + nCounter, 0.0f);


	//頂点バッファをアンロック
	g_pVtxBuffbg->Unlock();
}

//-----------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------
void DrawBG(void)
{

	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();


	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffbg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTexturebg[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}

}

//-----------------------------------------------------------------
// タイトル画面に貼るテクスチャの指定
//-----------------------------------------------------------------
void SetVertexBG(int nCntBG)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffbg->Lock(0, 0, (void**)&pVtx, 0);

	g_nCntbg2 = nCntBG;

	if (g_nCntbg2 == 0)
	{
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}

	else if (g_nCntbg2 == 1)
	{
		pVtx[4].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffbg->Unlock();
}