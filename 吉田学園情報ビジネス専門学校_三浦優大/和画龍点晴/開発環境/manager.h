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
class CScore;
class CHp;
class CLife;
class CTitle;
class CGame;
class CResult;
class CInputGamePad;
class CTutorial;
class CCamera;
class CRanking;

//*****************************************************************************
//マネージャークラスの定義
//*****************************************************************************
class CManager
{
public:
	//列挙型（画面遷移の種類）
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX
	}MODE;
	CManager();												  //コンストラクタ
	~CManager();											  //デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); //初期化処理
	void Uninit(void);                                        //終了処理
	void Update(void);                                        //更新処理
	void Draw(void);                                          //描画処理
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputGamePad *GetInputGamePad(void) { return m_pInputGamePad; }
	static CSound *GetSound(void) { return m_pSound; }
	static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; }
private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputGamePad *m_pInputGamePad;
	static CSound *m_pSound;
	//画面遷移メンバ変数
	static CTitle *m_pTitle;
	static CTutorial *m_pTutorial;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CRanking *m_pRanking;
	static MODE m_mode;
};
#endif
