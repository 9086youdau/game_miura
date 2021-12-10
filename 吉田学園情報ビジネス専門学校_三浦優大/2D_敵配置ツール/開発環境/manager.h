//*****************************************************************************
//
//�}�l�[�W���[����[manager.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CInputKeyboard;
class CPlayer;
class CSound;
class CBg;
class CEnemy;
class CGame;
class CInputGamePad;

//*****************************************************************************
//�}�l�[�W���[�N���X�̒�`
//*****************************************************************************
class CManager
{
public:
	//�񋓌^�i��ʑJ�ڂ̎�ށj
	typedef enum
	{
		MODE_GAME = 0,
		MODE_MAX
	}MODE;

	CManager();												  //�R���X�g���N�^
	~CManager();											  //�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); //����������
	void Uninit(void);                                        //�I������
	void Update(void);                                        //�X�V����
	void Draw(void);                                          //�`�揈��

	static CRenderer *GetRenderer(void) { return m_pRenderer; }//�����_���[�̏��󂯓n��
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }//�L�[�{�[�h�̏��󂯓n��
	static CInputGamePad *GetInputGamePad(void) { return m_pInputGamePad; }//�Q�[���p�b�h�̂̏��󂯓n��
	static CSound *GetSound(void) { return m_pSound; }//�T�E���h�̏��󂯓n��
	static void SetMode(MODE mode);//���[�h�̏��Z�b�g
	static MODE GetMode(void) { return m_mode; }//���[�h�̏��󂯓n��

private:
	//�|�C���^�����o�ϐ�
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputGamePad *m_pInputGamePad;
	static CSound *m_pSound;

	//��ʑJ�ڃ����o�ϐ�
	static CGame *m_pGame;
	static MODE m_mode;
};
#endif
