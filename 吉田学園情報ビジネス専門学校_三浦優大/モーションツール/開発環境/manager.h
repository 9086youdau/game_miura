//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
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
// �O���錾
//=============================================================================
class CLight;
class CCamera;

//=============================================================================
// �}�l�[�W���[�N���X�̒�`
//=============================================================================
class CManager
{
public:
	//���[�h�̎��
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

	CManager();								//�R���X�g���N�^
	~CManager();							//�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		//����������
	static void SetMode(MODE mode);									//�I������
	void Uninit(void);												//�X�V����
	void Update(void);												//�`�揈��
	void Draw(void);
	
	//�ÓI�����o�錾
	static CInputKeyboard	*GetInputKeyboard(void);				
	static CRenderer		*GetRenderer(void);
	static CSound			*GetSound(void);
	static CCamera			*GetCamera(void);
	static CTexture			*GetTexture(void);

	//���[�h���
	static MODE GetMode(void) { return m_mode; }

	static CGamepad *GetGamepad(void) { return m_pGamepad; }
	static CGamepad *m_pGamePad;

	//Pause
	static bool GetPause(void);

	// �^�C�}�[�\��
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

	LPD3DXFONT	m_pTimeFont;					// �\���p�t�H���g
	DWORD		m_dwGameStartTime;				// �Q�[���J�n����
	DWORD		m_dwGameTime;					// �Q�[���o�ߎ���
	static bool m_bEndGame;						// �I���t���O
	float		m_fTime;						// ����
};

#endif
