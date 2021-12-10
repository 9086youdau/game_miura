//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "gamepad.h"
#include "camera.h"
#include "light.h"
#include "sceneX.h"
#include "texture.h"
#include <stdio.h>

//=============================================================================
// 前方宣言
//=============================================================================
class CLight;
class CCamera;

//=============================================================================
// マネージャークラスの定義
//=============================================================================
class CManager
{
public:
	//モードの種類
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_MENU,
		MODE_SELECT,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_GAME2,
		MODE_RESULT,
		MODE_PAUSE,
		MODE_MAX
	}MODE;

	CManager();								//コンストラクタ
	~CManager();							//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		//初期化処理
	static void SetMode(MODE mode);									//終了処理
	void Uninit(void);												//更新処理
	void Update(void);												//描画処理
	void Draw(void);
	
	//静的メンバ宣言
	static CInputKeyboard	*GetInputKeyboard(void);				
	static CRenderer		*GetRenderer(void);
	static CSound			*GetSound(void);
	static CCamera			*GetCamera(void);
	static CTexture			*GetTexture(void);

	//モード情報
	static MODE GetMode(void) { return m_mode; }

	static CGamepad *GetGamepad(void) { return m_pGamepad; }
	static CGamepad *m_pGamePad;

	//Pause
	static bool GetPause(void);

	// タイマー表示
	HRESULT InitTime(void);
	void UninitTime(void);
	void UpdateTime(void);
	void DrawTime(void);

private:
	static CRenderer		*m_pRenderer;
	static CInputKeyboard	*m_pInputKeyboard;
	static CInput			*m_pInput;
	static CSound			*m_pSound;
	static CScene			*m_pScene;
	static CGamepad			*m_pGamepad;

	static CGame			*m_pGame;
	static CCamera			*m_pCamera;
	static CLight			*m_pLight;
	static CSceneX			*m_pSceneX;
	static CTexture			*m_pTexture;
	static MODE				m_mode;

	LPD3DXFONT	m_pTimeFont;					// 表示用フォント
	DWORD		m_dwGameStartTime;				// ゲーム開始時刻
	DWORD		m_dwGameTime;					// ゲーム経過時間
	static bool m_bEndGame;						// 終了フラグ
	float		m_fTime;						// 時間
};

#endif
