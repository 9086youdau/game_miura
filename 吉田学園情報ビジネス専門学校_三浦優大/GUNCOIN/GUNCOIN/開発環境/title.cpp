#include "title.h"
#include "fade.h"
#include "gamepad.h"

//==============
//グローバル変数
//==============
LPDIRECT3DTEXTURE9 g_pTextureTitle[3] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点情報
TITLE g_Title[3];
D3DXCOLOR g_colorTitle;
D3DXCOLOR g_colorTitleEnter;
TITLE g_PosTitle;
TITLE g_PosEnter;
TITLETYPE g_cnt;

//====================
//ポリゴンの初期化処理
//====================
HRESULT InitTitle(void)
{
	VERTEX_2D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();


	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/背景.jpg", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTextureTitle[2]);

	for (int nCount = 0; nCount < 3; nCount++)
	{
		g_PosTitle.Pos = D3DXVECTOR3(850.0f, 350.0f, 0.0f);
		g_PosEnter.Pos = D3DXVECTOR3(900.0f, 800.0f, 0.0f);
		g_Title[nCount].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_colorTitle = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);
		g_colorTitleEnter = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_Title[nCount].nType = 0;
		g_Title[nCount].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCounter = 0; nCounter < 3; nCounter++)
	{
		//頂点座標の設定
		SetTitle(nCounter);

		//頂点カラーの設定
		pVtx[0].col = g_colorTitle;
		pVtx[1].col = g_colorTitle;
		pVtx[2].col = g_colorTitle;
		pVtx[3].col = g_colorTitle;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();

	return S_OK;
}
//==================
//ポリゴンの終了処理
//==================
void UninitTitle(void)
{
	//テクスチャの開放
	for (int nCount = 0; nCount < 3; nCount++)
	{
		if (g_pTextureTitle[nCount] != NULL)
		{
			g_pTextureTitle[nCount]->Release();
			g_pTextureTitle[nCount] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

}
//==================
//ポリゴンの更新処理
//==================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;

	int nCntTitle = 0;

	nCntTitle++;

	//頂点バッファをロックし、頂点情報のポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);


	if (IsButtonPush(KEYINFO_OK) == true)
	{
		//画面切り替え
		SetFade(FADE_OUT, MODE_TUTORIAL);
	}

	float fAnglee;
	fAnglee = (float)(rand() % 628 - 314) / 100.0f;

	g_PosTitle.Pos.x += sinf(fAnglee) * 0.5f;
	g_PosTitle.Pos.y += cosf(fAnglee) * 0.5f;

	//頂点座標の設定
	pVtx[4].pos = D3DXVECTOR3(g_PosTitle.Pos.x - (1000 / 2), g_PosTitle.Pos.y + (300 / 2), 0.0f);//2Dは0.0固定
	pVtx[5].pos = D3DXVECTOR3(g_PosTitle.Pos.x - (1000 / 2), g_PosTitle.Pos.y - (300 / 2), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_PosTitle.Pos.x + (1400 / 2), g_PosTitle.Pos.y + (300 / 2), 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_PosTitle.Pos.x + (1400 / 2), g_PosTitle.Pos.y - (300 / 2), 0.0f);//2Dは0.0固定

	//頂点座標の設定
	pVtx[8].col = g_colorTitleEnter;
	pVtx[9].col = g_colorTitleEnter;
	pVtx[10].col = g_colorTitleEnter;
	pVtx[11].col = g_colorTitleEnter;

	if (nCntTitle % 60 == 0)//間隔
	{
		g_colorTitleEnter.a = 0.0f;
	}

	else if (nCntTitle % 30 == 0 && nCntTitle % 60)//間隔
	{
		g_colorTitleEnter.a = 1.0f;
	}



	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//==================
//ポリゴンの描画処理
//==================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < 3; nCount++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCount]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}
void SetTitle(int nCount)
{
	TITLE *pTitle;
	VERTEX_2D *pVtx;

	pTitle = &g_Title[0];

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		if (nCount == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);//2Dは0.0固定
			pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);//2Dは0.0固定
		}

		else if (nCount == 1)
		{
			//頂点座標の設定
			pVtx[4].pos = D3DXVECTOR3(g_PosTitle.Pos.x - (800 / 2), g_PosTitle.Pos.y + (500 / 2), 0.0f);//2Dは0.0固定
			pVtx[5].pos = D3DXVECTOR3(g_PosTitle.Pos.x - (800 / 2), g_PosTitle.Pos.y - (500 / 2), 0.0f);
			pVtx[6].pos = D3DXVECTOR3(g_PosTitle.Pos.x + (1400 / 2), g_PosTitle.Pos.y + (500 / 2), 0.0f);
			pVtx[7].pos = D3DXVECTOR3(g_PosTitle.Pos.x + (1400 / 2), g_PosTitle.Pos.y - (500 / 2), 0.0f);//2Dは0.0固定

		}

		else if (nCount == 2)
		{
			//頂点座標の設定
			pVtx[8].pos = D3DXVECTOR3(g_PosEnter.Pos.x - (150 / 2), g_PosEnter.Pos.y + (100 / 2), 0.0f);//2Dは0.0固定
			pVtx[9].pos = D3DXVECTOR3(g_PosEnter.Pos.x - (150 / 2), g_PosEnter.Pos.y - (100 / 2), 0.0f);
			pVtx[10].pos = D3DXVECTOR3(g_PosEnter.Pos.x + (300 / 2), g_PosEnter.Pos.y + (100 / 2), 0.0f);
			pVtx[11].pos = D3DXVECTOR3(g_PosEnter.Pos.x + (300 / 2), g_PosEnter.Pos.y - (100 / 2), 0.0f);//2Dは0.0固定
		}

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}
