//=============================================================================
//
// ui���� [ui.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "ui.h"
#include "manager.h"
#include "scene3D.h"
#include "title.h"
#include "renderer.h"
#include "sound.h"
#include "gamepad.h"
#include "input.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CUi::m_pTexture[UITYPE_MAX] = {}; //�e�N�X�`�����
int CUi::m_PointerCount = 0;                         //UI�̃|�C���^���
D3DXVECTOR3 CUi::m_posPause;                         //ui�̈ʒu���
int  CUi::m_nPauseType = 0;                          //ui�̃^�C�v���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUi::CUi(int nPriority, int nType) : CScene2D(nPriority, nType)
{
	//�����o�ϐ�������
	m_BlinkingCount = 0;
	m_nType = 0;
	m_PointerCount = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, float fHight, float fWidth,int nType)
{
	//����������
	CScene2D::Init(pos, fHight, fWidth);

	//�J���[�ݒ�֐�
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�ϐ��ɑ��
	m_nType = nType;
	m_pos = pos;
	m_firstpos = pos;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUi::Uninit(void)
{
	//�I������
	CScene2D::Uninit();

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUi::Update(void)
{
	//�֐��Ăяo��
	TitleUi();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUi::Draw(void)
{
	//�`�揈��
	CScene2D::Draw();
}

//=============================================================================
// �e�N�X�`���ݒ菈��
//=============================================================================
void CUi::Load(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/trop.png", &m_pTexture[UITYPE_FREAM]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg.jpg", &m_pTexture[UITYPE_BG]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamestart.png", &m_pTexture[UITYPE_START]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutoriallogo.png", &m_pTexture[UITYPE_TUTORIAL]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/exit.png", &m_pTexture[UITYPE_EXIT]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pointer.png", &m_pTexture[UITYPE_POINTER]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamemes.png", &m_pTexture[UITYPE_GAMEMESSAGE]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rankingmenu.png", &m_pTexture[UITYPE_RESULTMENU]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/EnemyMa.png", &m_pTexture[UITYPE_ENEMYREMAINING]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ResultHp.png", &m_pTexture[UITYPE_RESULTHP]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/addscore.png", &m_pTexture[UITYPE_RESULTADD]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/equal-red3.png", &m_pTexture[UITYPE_RESULTEQUAL]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/1��.png", &m_pTexture[RANKING_FIRST]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/2��.png", &m_pTexture[RANKING_SECOND]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/3��.png", &m_pTexture[RANKING_THIRD]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/4��.png", &m_pTexture[RANKING_FOUR]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/5��.png", &m_pTexture[RANKING_FIVE]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamelogo.png", &m_pTexture[TITLE_LOGO]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture[TUTORIAL_MENU]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pausebg.png", &m_pTexture[PAUSE_FASE]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/PauseMenu.png", &m_pTexture[PAUSE_MENU]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ReturnTitle.png", &m_pTexture[PAUSE_TITLE]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GameRestart.png", &m_pTexture[PAUSE_RESTART]);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pausefream.png", &m_pTexture[PAUSE_FREAM]);
}


//=============================================================================
//ui�̃e�N�X�`���j������
//=============================================================================
void CUi::Unload(void)
{
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
// �^�C�g����UI�ݒ菈��
//=============================================================================
void CUi::TitleUi(void)
{
	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	if (CTitle::GetNotMove() == false)
	{
		//ui�̃^�C�v���^�C�g���̃|�C���^�������ꍇ
		if (m_nType == UITYPE_POINTER)
		{
			//S�L�[�������ꂽ�ꍇ
			if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN) == true)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				m_pos.y += 120.0f;        //�ʒu�����v���X����
				m_PointerCount++;         //�J�E���g����
				if (m_PointerCount == 3)  //�J�E���g��3�ɂȂ����ꍇ
				{
					m_PointerCount = 0;   //�J�E���g��0�ɖ߂��Ă�����
					m_pos = m_firstpos;   //�ŏ��̈ʒu�ɖ߂�
				}
			}
			//S�L�[�������ꂽ�ꍇ
			else if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP) == true)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				if (m_PointerCount == 0)  //�J�E���g��0�ɂȂ����ꍇ
				{
					m_PointerCount = 3;   //�J�E���g��3�ɂ��Ă�����
					m_pos.y = m_firstpos.y + 120.0f * 3;  //��ԍŌ�̈ʒu�ɂ���
				}
				m_pos.y -= 120.0f;        //�ʒu�����炷
				m_PointerCount--;         //�J�E���g�������Ă���
			}
			//�ʒu������������ǂݍ��ݔ��f������
			SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	//ui�̃^�C�v���^�C�g���̏ꍇ�ƃ|�C���^�̃J�E���g��0�̎�
	if (m_nType == UITYPE_START && m_PointerCount == 0)
	{
		//�Ԋu�̃J�E���g�A�b�v
		m_BlinkingCount++;

		if (m_BlinkingCount % 30 == 0)//�Ԋu����
		{
			//�����ɂ���
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		else if (m_BlinkingCount % 15 == 0 && m_BlinkingCount % 30)//�Ԋu����
		{
			//���ɖ߂�
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//ui�̃^�C�v���`���[�g���A���������ꍇ
	else if (m_nType == UITYPE_TUTORIAL && m_PointerCount == 1)
	{
		//�Ԋu�̃J�E���g�A�b�v
		m_BlinkingCount++;

		if (m_BlinkingCount % 30 == 0)//�Ԋu����
		{
			//�����ɂ���
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		else if (m_BlinkingCount % 15 == 0 && m_BlinkingCount % 30)//�Ԋu����
		{
			//���ɖ߂�
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//ui�̃^�C�v���C�O�W�b�g�������ꍇ
	else if (m_nType == UITYPE_EXIT && m_PointerCount == 2)
	{
		//�Ԋu�̃J�E���g�A�b�v
		m_BlinkingCount++;

		if (m_BlinkingCount % 30 == 0)//�Ԋu����
		{
			//�����ɂ���
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		else if (m_BlinkingCount % 15 == 0 && m_BlinkingCount % 30)//�Ԋu����
		{
			//���ɖ߂�
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//�|�C���^�̃J�E���g��1�̏ꍇ
	if (m_PointerCount == 1)
	{
		//ui�^�C�v���X�^�[�g���������̓C�O�W�b�g��
		if (m_nType == UITYPE_START || m_nType == UITYPE_EXIT)
		{
			//�J���[�����ɖ߂�
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//�|�C���^�̃J�E���g��2�̏ꍇ
	else if (m_PointerCount == 2)
	{
		//ui�^�C�v���`���[�g���A�����������̓X�^�[�g��
		if (m_nType == UITYPE_TUTORIAL || m_nType == UITYPE_START)
		{
			//�J���[�����ɖ߂�
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	//�|�C���^�̃J�E���g��0�̏ꍇ
	else if (m_PointerCount == 0)
	{
		//ui�^�C�v���C�O�W�b�g���������̓`���[�g���A����
		if (m_nType == UITYPE_EXIT || m_nType == UITYPE_TUTORIAL)
		{
			//�J���[�����ɖ߂�
			SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
//=============================================================================
// �|�[�Y��UI�ݒ菈��
//=============================================================================
void CUiPause::PauseUi(void)
{
	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�|�[�Y���
	if (CManager::GetPause() == true)
	{
		//ui�̃^�C�v���^�C�g���̃|�C���^�������ꍇ
		if (m_nType == PAUSE_FREAM)
		{
			//S�L�[�������ꂽ�ꍇ
			if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN) == true)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				m_pos.y += 210.0f;        //�ʒu�����v���X����
				m_PointerCount++;         //�J�E���g����
				if (m_PointerCount == 2)  //�J�E���g��2�ɂȂ����ꍇ
				{
					m_PointerCount = 0;   //�J�E���g��0�ɖ߂��Ă�����
					m_pos = m_firstpos;   //�ŏ��̈ʒu�ɖ߂�
				}
			}
			//S�L�[�������ꂽ�ꍇ
			else if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP) == true)
			{
				//�T�E���hBGM
				m_pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
				if (m_PointerCount == 0)  //�J�E���g��0�ɂȂ����ꍇ
				{
					m_PointerCount = 2;   //�J�E���g��2�ɂ��Ă�����
					m_pos.y = m_firstpos.y + 210.0f * 2;  //��ԍŌ�̈ʒu�ɂ���
				}
				m_pos.y -= 210.0f;        //�ʒu�����炷
				m_PointerCount--;         //�J�E���g�������Ă���
			}
			//�ʒu������������ǂݍ��ݔ��f������
			SetPosition(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
}
//=============================================================================
// UI�𓮂����ݒ菈��
//=============================================================================
void CUi::MoveUi(D3DXVECTOR3 pos, int nType)
{
	//�ʒu���擾
	m_posPause = pos;
	//�^�C�v���擾
	m_nPauseType = nType;
}

//=============================================================================
// ��������
//=============================================================================
CUi * CUi::Create(D3DXVECTOR3 pos, float fHight, float fWidth,int nType)
{
	//���f���𐶐�
	CUi *pUi = NULL;
	pUi = new CUi;

	//NULL�`�F�b�N
	if (pUi != NULL)
	{
		//�I�u�W�F�N�g�̏���������
		pUi->Init(pos, fHight, fWidth, nType);

		//�e�N�X�`������
		pUi->BindTexture(m_pTexture[nType]);
	}

	return pUi;
}


//=============================================================================
//
// ���C������ [uipause.cpp]
// Author : Miura Yudai
//
//=============================================================================

//�|�[�Y��ʂɕ\�����������ꍇ

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CUiPause::CUiPause(int nPriority, int nType) : CUi(nPriority, nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUiPause::~CUiPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUiPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	//����������
	CScene2D::Init(pos, fSizeY, fSizeX);

	//�J���[�ݒ�֐�
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�ϐ��ɑ��
	m_nType = nType;
	m_pos = pos;
	m_firstpos = pos;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUiPause::Uninit(void)
{
	//�I������
	CScene2D::Uninit();

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUiPause::Update(void)
{
	//�֐��Ăяo��
	PauseUi();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUiPause::Draw(void)
{
	//�`�揈��
	CScene2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CUiPause * CUiPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// ���[�J���ϐ��錾
	CUiPause *pUiPause = NULL;
	pUiPause = new CUiPause;

	// ������
	if (pUiPause != NULL)
	{
		pUiPause->Init(pos, fSizeX, fSizeY, nType);
	}

	// �e�N�X�`�����蓖��
	pUiPause->BindTexture(m_pTexture[nType]);

	return pUiPause;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CUiPause::Unload(void)
{
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (m_pTexture != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
