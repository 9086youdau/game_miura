//=============================================================================
//
// ui���� [ui.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _UI_H_
#define _UI_H_

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CSound;

//=============================================================================
// ui�N���X�̒�`
//=============================================================================

//�p����`��N���X
class CUi : public CScene2D
{
public:
	//ui�̏ڍ׏��
	typedef enum
	{
		UITYPE_FREAM = 0,
		UITYPE_BG,
		UITYPE_START,
		UITYPE_TUTORIAL,
		UITYPE_EXIT,
		UITYPE_POINTER,
		UITYPE_GAMEMESSAGE,
		UITYPE_RESULTMENU,
		UITYPE_ENEMYREMAINING,
		UITYPE_RESULTHP,
		UITYPE_RESULTADD,
		UITYPE_RESULTEQUAL,
		RANKING_FIRST,
		RANKING_SECOND,
		RANKING_THIRD,
		RANKING_FOUR,
		RANKING_FIVE,
		TITLE_LOGO,
		TUTORIAL_MENU,
		PAUSE_FASE,
		PAUSE_MENU,
		PAUSE_TITLE,
		PAUSE_RESTART,
		PAUSE_FREAM,
		UITYPE_MAX
	}UITYPE;
	CUi(int nPriority = 1, int nType = 0);								//�R���X�g���N�^
	virtual ~CUi();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,int nType);			//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static void Load(void);
	static void Unload(void);

	void TitleUi(void);//�^�C�g����UI�ݒ�
	static void MoveUi(D3DXVECTOR3 pos, int nType);

	static CUi *Create(D3DXVECTOR3 pos, float fHight, float fWidth, int nType);//��������

	static int GetPointCount(void) { return m_PointerCount; }

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[UITYPE_MAX];			//�e�N�X�`�����
	D3DXVECTOR3 m_pos;                                          //�ʒu���
	static D3DXVECTOR3 m_posPause;                              //�ʒu���
	D3DXVECTOR3 m_firstpos;                                     //�ŏ��̈ʒu
	D3DXVECTOR3 m_finalpos;                                     //�Ō�̈ʒu
	D3DXVECTOR3 m_move;                                         //�ړ���
	int m_nType;                                                //ui�̃^�C�v
	static int m_nPauseType;                                    //ui�̃^�C�v
	static int m_PointerCount;                                  //�^�C�g���̃|�C���^�̏����p
	int m_BlinkingCount;                                        //�_�ł̃J�E���g�p
	CSound		*m_pSound;			          					//�T�E���h�|�C���^
};
//=============================================================================
// ui�N���X�̒�`(Pause)
//=============================================================================
class CUiPause : public CUi
{
public:
	CUiPause(int nPriority = 0, int nType = 2);				//�R���X�g���N�^
	~CUiPause();												//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);				//����������
	void Uninit(void);																	//�I������
	void Update(void);																	//�X�V����
	void Draw(void);																	//�`�揈��

	static void Unload(void);                                                            //�e�N�X�`���̃A�����[�h

	void PauseUi(void);//�|�[�Y��UI�ݒ�

	static CUiPause *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);	//��������
};

#endif

