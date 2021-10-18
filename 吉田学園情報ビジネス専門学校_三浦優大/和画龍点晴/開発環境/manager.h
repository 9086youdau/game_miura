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
//�}�l�[�W���[�N���X�̒�`
//*****************************************************************************
class CManager
{
public:
	//�񋓌^�i��ʑJ�ڂ̎�ށj
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX
	}MODE;
	CManager();												  //�R���X�g���N�^
	~CManager();											  //�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); //����������
	void Uninit(void);                                        //�I������
	void Update(void);                                        //�X�V����
	void Draw(void);                                          //�`�揈��
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
	//��ʑJ�ڃ����o�ϐ�
	static CTitle *m_pTitle;
	static CTutorial *m_pTutorial;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CRanking *m_pRanking;
	static MODE m_mode;
};
#endif
