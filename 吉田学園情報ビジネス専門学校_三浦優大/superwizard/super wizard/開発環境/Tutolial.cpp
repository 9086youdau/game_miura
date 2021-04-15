#include "Tutolial.h"
#include "title.h"
#include "fade.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureTutolial[3] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutolial = NULL;//頂点情報
TUTOLIAL g_Tutolial[3];
D3DXCOLOR g_colorTutolial;
TUTOLIAL g_PosTitleTutolial;
TUTOLIAL g_PosEnterTutolial;

//====================
//ポリゴンの初期化処理
//====================
HRESULT InitTutolial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutolial.png", &g_pTextureTutolial[0]);
	//D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTutolial[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTextureTutolial[2]);

	for (int nCount = 0; nCount < 3; nCount++)
	{
		g_PosTitleTutolial.Pos = D3DXVECTOR3(850.0f, 350.0f, 0.0f);
		g_PosEnterTutolial.Pos = D3DXVECTOR3(1650.0f, 1000.0f, 0.0f);
		g_Tutolial[nCount].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_colorTutolial = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
		g_Tutolial[nCount].nType = 0;
		g_Tutolial[nCount].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutolial,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTutolial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCounter = 0; nCounter < 3; nCounter++)
	{
		//頂点座標の設定
		SetTutolial(nCounter);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_colorTutolial;
		pVtx[1].col = g_colorTutolial;
		pVtx[2].col = g_colorTutolial;
		pVtx[3].col = g_colorTutolial;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTutolial->Unlock();

	return S_OK;
}
//==================
//ポリゴンの終了処理
//==================
void UninitTutolial(void)
{
	//テクスチャの開放
	for (int nCount = 0; nCount < 3; nCount++)
	{
		if (g_pTextureTutolial[nCount] != NULL)
		{
			g_pTextureTutolial[nCount]->Release();
			g_pTextureTutolial[nCount] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffTutolial != NULL)
	{
		g_pVtxBuffTutolial->Release();
		g_pVtxBuffTutolial = NULL;
	}

}
//==================
//ポリゴンの更新処理
//==================
void UpdateTutolial(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTutolial->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardPress(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}

	//float fAnglee;
	//fAnglee = (float)(rand() % 628 - 314) / 100.0f;

	//g_PosTitleTutolial.Pos.x += sinf(fAnglee) * 0.5f;
	//g_PosTitleTutolial.Pos.y += cosf(fAnglee) * 0.5f;

	////頂点座標の設定
	//pVtx[4].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x - (700 / 2), g_PosTitleTutolial.Pos.y + (300 / 2), 0.0f);//2Dは0.0固定
	//pVtx[5].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x - (700 / 2), g_PosTitleTutolial.Pos.y - (300 / 2), 0.0f);
	//pVtx[6].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x + (1000 / 2), g_PosTitleTutolial.Pos.y + (300 / 2), 0.0f);
	//pVtx[7].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x + (1000 / 2), g_PosTitleTutolial.Pos.y - (300 / 2), 0.0f);//2Dは0.0固定


	// 頂点バッファをアンロック
	g_pVtxBuffTutolial->Unlock();
}

//==================
//ポリゴンの描画処理
//==================
void DrawTutolial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutolial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < 3; nCount++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutolial[nCount]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}
void SetTutolial(int nCount)
{
	TUTOLIAL *pTutolial;
	VERTEX_2D *pVtx;

	pTutolial = &g_Tutolial[0];

	// 頂点バッファをロック
	g_pVtxBuffTutolial->Lock(0, 0, (void**)&pVtx, 0);

	if (nCount == 0)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);//2Dは0.0固定
	}

	//else if (nCount == 1)
	//{
	//	//頂点座標の設定
	//	pVtx[4].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x - (600 / 2), g_PosTitleTutolial.Pos.y + (500 / 2), 0.0f);//2Dは0.0固定
	//	pVtx[5].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x - (600 / 2), g_PosTitleTutolial.Pos.y - (500 / 2), 0.0f);
	//	pVtx[6].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x + (1000 / 2), g_PosTitleTutolial.Pos.y + (500 / 2), 0.0f);
	//	pVtx[7].pos = D3DXVECTOR3(g_PosTitleTutolial.Pos.x + (1000 / 2), g_PosTitleTutolial.Pos.y - (500 / 2), 0.0f);//2Dは0.0固定

	//}

	else if (nCount == 2)
	{
		//頂点座標の設定
		pVtx[8].pos = D3DXVECTOR3(g_PosEnterTutolial.Pos.x - (100 / 2), g_PosEnterTutolial.Pos.y + (100 / 2), 0.0f);//2Dは0.0固定
		pVtx[9].pos = D3DXVECTOR3(g_PosEnterTutolial.Pos.x - (100 / 2), g_PosEnterTutolial.Pos.y - (100 / 2), 0.0f);
		pVtx[10].pos = D3DXVECTOR3(g_PosEnterTutolial.Pos.x + (400 / 2), g_PosEnterTutolial.Pos.y + (100 / 2), 0.0f);
		pVtx[11].pos = D3DXVECTOR3(g_PosEnterTutolial.Pos.x + (400 / 2), g_PosEnterTutolial.Pos.y - (100 / 2), 0.0f);//2Dは0.0固定
	}

	// 頂点バッファをアンロック
	g_pVtxBuffTutolial->Unlock();
}

