//*****************************************************************************
//
//�C���v�b�g����[input.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�C���v�b�g�N���X�̒�`
//*****************************************************************************
class CInput
{
public:
	//�񋓌^�i�{�^���̎��)
	typedef enum
	{
		KEYINFO_B = 0,
		KEYINFO_A,
		KEYINFO_UP,
		KEYINFO_DOWN,
		KEYINFO_LEFT,
		KEYINFO_RIGHT,
		KEYINFO_UP_CENTER,
		KEYINFO_DOWN_CENTER,
		KEYINFO_LEFT_TURN,
		KEYINFO_RIGHT_TURN,
		KEYINFO_UP_SIGHT,
		KEYINFO_DOWN_SIGHT,
		KEYINFO_UP_MODEL,
		KEYINFO_DOWN_MODEL,
		KEYINFO_TURN_LEFT_MODEL,
		KEYINFO_TURN_RIGHT_MODEL,
		KEYINFO_SHOT,
		KEYINFO_ZOOMIN,
		KEYINFO_ZOOMOUT,
		KEYINFO_FRONT_PLUS,
		KEYINFO_FRONT_MINUS,
		KEYINFO_PAUSE,
		KEYINFO_PRESET,
		KEYINFO_CRESET,
		KEYINFO_R1,
		KEYINFO_L1,
		KEYINFO_ATTACK,
		KEYINFO_OK,
		KEYINFO_CANCEL,
		KEYINFO_CAMERA_FRONT,
		KEYINFO_WEAPON_DROP,
		KEYINFO_MAX
	} KEYINFO;

	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	LPDIRECTINPUTDEVICE8 m_pDeviceKey;
	static LPDIRECTINPUTDEVICE8 m_pDevice;
	static LPDIRECTINPUT8 m_pInput;
};
//*****************************************************************************
//�C���v�b�g�L�[�{�[�h�N���X�̒�`
//*****************************************************************************
class CInputKeyboard : CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
private:
	BYTE m_aKeyState[256];
	BYTE m_aKeyStateTrigger[256];
};
#endif
