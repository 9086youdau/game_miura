//*****************************************************************************
//
//ゲームパッド処理[gamepad.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "input.h"

//*****************************************************************************
//インプットゲームパッドクラスの定義
//*****************************************************************************
class CInputGamePad : CInput
{
public:
	//列挙型（ボタンの種類）
	typedef enum
	{
		ButtonStateNone = 0,
		ButtonStateDown,
		ButtonStatePush,
		ButtonStateUp,
		ButtonStateMax,
	} ButtonState;

	CInputGamePad();
	~CInputGamePad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	void ButtonSet(DIJOYSTATE pad_data);

	bool IsButtonPush(KEYINFO button);
	bool IsButtonUp(KEYINFO button);
	bool IsButtonDown(KEYINFO button);

	bool BuPu(KEYINFO button, int nCnt);
	bool BuUp(KEYINFO button, int nCnt);
	bool BuDo(KEYINFO button, int nCnt);

	static BOOL CALLBACK EnumJoysticksCallback(
		const DIDEVICEINSTANCE *pdidoi, VOID *pContext);		// コールバック関数
	int GetNumControl(void) { return m_nController; }								// コントローラーの数を取得

private:
	ButtonState m_ButtonStates[KEYINFO::KEYINFO_MAX];
	static int m_nController;
};
#endif

