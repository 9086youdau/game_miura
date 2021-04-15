#include "input.h"

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyBoard = NULL;
BYTE g_aKeyState[256];
BYTE g_aKeyStateTrigger[256];

//入力デバイス（キーボード）ポインタ
//キーボードへの情報
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DIRECTIPUTの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard,&g_pDevKeyBoard,NULL)))
	{
		return E_FAIL;
	}
	
	//データフォーマットの設定
	if (FAILED(g_pDevKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	
	//協調モードの設定
	if (FAILED(g_pDevKeyBoard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権の取得
	g_pDevKeyBoard->Acquire();
	return S_OK;
}
void UninitKeyboard(void)
{
	if (g_pDevKeyBoard != NULL)
	{
		//アクセス権の破棄
		g_pDevKeyBoard->Unacquire();
		g_pDevKeyBoard->Release();
		g_pDevKeyBoard = NULL;
	}
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}
void UpdateKeyboard(void)
{
	BYTE aKeystate[256];
	//デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyBoard->GetDeviceState(sizeof(aKeystate), &aKeystate[0])))
	{
		for (int nKey = 0; nKey < 256; nKey++)
		{
			g_aKeyStateTrigger[nKey] = (g_aKeyState[nKey] & aKeystate[nKey])^ aKeystate[nKey];
			g_aKeyState[nKey] = aKeystate[nKey];
		}
	}
	//アクセス権の取得
	g_pDevKeyBoard->Acquire();
}
bool GetKeyboardPress(int nKey)
{
	//キーボード情報の取得
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}
bool GetKeyboardTrigger(int nKey)
{
	//キーボード情報の取得
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}



