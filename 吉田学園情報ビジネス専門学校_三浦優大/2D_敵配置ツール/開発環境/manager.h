//*****************************************************************************
//
//マネージャー処理[manager.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CInputKeyboard;
class CPlayer;
class CSound;
class CBg;
class CEnemy;
class CGame;
class CInputGamePad;

//*****************************************************************************
//マネージャークラスの定義
//*****************************************************************************
class CManager
{
public:
	//列挙型（画面遷移の種類）
	typedef enum
	{
		MODE_GAME = 0,
		MODE_MAX
	}MODE;

	CManager();												  //コンストラクタ
	~CManager();											  //デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); //初期化処理
	void Uninit(void);                                        //終了処理
	void Update(void);                                        //更新処理
	void Draw(void);                                          //描画処理

	static CRenderer *GetRenderer(void) { return m_pRenderer; }//レンダラーの情報受け渡し
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }//キーボードの情報受け渡し
	static CInputGamePad *GetInputGamePad(void) { return m_pInputGamePad; }//ゲームパッドのの情報受け渡し
	static CSound *GetSound(void) { return m_pSound; }//サウンドの情報受け渡し
	static void SetMode(MODE mode);//モードの情報セット
	static MODE GetMode(void) { return m_mode; }//モードの情報受け渡し

private:
	//ポインタメンバ変数
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputGamePad *m_pInputGamePad;
	static CSound *m_pSound;

	//画面遷移メンバ変数
	static CGame *m_pGame;
	static MODE m_mode;
};
#endif
