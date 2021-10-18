//*****************************************************************************
//
//レンダラー処理[Renderer.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "manager.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CPause;
class CCamera;
class CMeshfield;
class CLight;
class CMeshwall;
class CFade3D;

//*****************************************************************************
//レンダラークラスの定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer(); //コンストラクタ
	~CRenderer();//デストラクタ
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	void DrawTime(void);
private:
	void DrawFPS(void);                  //FPS描画処理
	LPDIRECT3D9 m_pD3D;                  // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;      // Deviceオブジェクト(描画に必要)
	LPD3DXFONT m_pFont;                  // フォントへのポインタ
	static CPause *m_apPause;
	//ポーズ画面メンバ変数
	static bool m_Pause;

	//カメラ設定メンバ変数
	static CCamera *m_pCamera;
	//メッシュフィールド設定のメンバ変数
	static CMeshfield *m_pField;
	//メッシュウォール設定のメンバ変数
	static CMeshwall *m_pWall;
	//ライト設定のメンバ変数
	static CLight *m_pLight;

	//タイムメンバ関数
	LPD3DXFONT m_pTimeFont;                  //経過時間表示用フォント
	DWORD m_dwGameStartTime;					//ゲーム開始時刻
	DWORD m_dwGameTime;							//ゲーム経過時間
	bool m_bEndGame = false;						//終了フラグ
	CManager::MODE m_mode;
};
#endif
