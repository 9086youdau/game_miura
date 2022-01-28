//=============================================================================
//
// レンダラー処理 [renderer.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _RENDERE_H_
#define _RENDERE_H_

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "main.h"

//=============================================================================
//前方宣言
//=============================================================================
class CDebugProc;

//=============================================================================
// クラスの定義
//=============================================================================
class CRenderer
{
public:
	CRenderer();								//コンストラクタ
	~CRenderer();								//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);						//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
	void Draw2(void);											//描画処理(残像あり)

																//デバイス情報
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT[0]; }

private:
	void DrawFPS();

	LPDIRECT3D9					m_pD3D = NULL;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9			m_pD3DDevice = NULL;		// Deviceオブジェクト(描画に必要)

															//FPSの情報
	LPD3DXFONT					m_pFont = NULL;				// フォントへのポインタ

	LPDIRECT3DTEXTURE9 m_pTextureMT[2];
	LPDIRECT3DSURFACE9 m_pRenderMT[2];
	LPDIRECT3DSURFACE9 m_pBuffMT[2];
	D3DVIEWPORT9 m_ViewportMT;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;
	static CDebugProc *m_pDebug;	//デバック表示のメンバ変数

	int m_nCnt;                     //デバック表示用カウント(並び変え）
	int m_nCnt2;                    //デバック表示用カウント2(並び変え2）
	int m_nMaxCnt;                  //デバック表示用カウント(最大数並び変え）
	int m_nMinCnt;                  //デバック表示用カウント(最小数並び変え）
	int m_nCntMaxPart;              //デバック表示用カウントの最大数
	int m_nCntOldMax;               //デバック表示用カウントの保存用
	int m_nForCnt;                  //デバック表示用のFor
	int m_nCntOld;                  //デバック表示用カウントの保存用2
	int m_nForMin;                  //最小数
	int m_nForMax;                  //最大値
	int m_nMaxPlayerPart;
	int m_nMaxPlayerPart2;
	int m_nMinPlayerPart;
	bool m_bUse;
	int m_SideCnt;
};

#endif
