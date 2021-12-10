//=============================================================================
//
//�|�[�Y����[pause.cpp]
// Author; miura yuudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "pause.h"
#include "Scene2D.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "bg.h"
#include "Renderer.h"
#include "CircleHp.h"
#include "game.h"
#include "gamepad.h"
#include "bullet.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_pTexture[PAUSETYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 CPause::m_pVtxBuff = NULL;
CPause::PAUSETYPE CPause::m_pauseType = PAUSETYPE_RETURN_GAME;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CPause::CPause()
{
	//�����o�ϐ�������
	m_pauseType = PAUSETYPE_RETURN_GAME;
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//�|�[�Y�̏���������
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�e�L�X�`�����[�h
	Load();

	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < PAUSETYPE_MAX; nCntTitle++,pVtx += 4)
	{
		if (nCntTitle == PAUSETYPE_BG)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);
		}

		else if (nCntTitle == PAUSETYPE_PAUSE)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(1300.0f, 200.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1300.0f, 400.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(600.0f, 200.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 400.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);

			//�e�N�X�`���ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		else if (nCntTitle == PAUSETYPE_RETURN_GAME)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(1300.0f, 550.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1300.0f, 700.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(600.0f, 550.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 700.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[1].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[2].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[3].col = D3DXCOLOR(255, 0, 0, 255);

			//�e�N�X�`���ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		else if (nCntTitle == PAUSETYPE_RETURN_TITLE)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(1300.0f, 750.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1300.0f, 900.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(600.0f, 750.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(600.0f, 900.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[1].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[2].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[3].col = D3DXCOLOR(255, 255, 255, 255);

			//�e�N�X�`���ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//�|�[�Y�̏I������
//=============================================================================
void CPause::Uninit(void)
{
	//�e�N�X�`���A�����[�h
	Unload();

	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//�^�C�g���̍X�V����
//=============================================================================
void CPause::Update(void)
{
	//�֐��Ăяo��
	SelectPause();
}

//=============================================================================
//�|�[�Y�̕`�揈��
//=============================================================================
void CPause::Draw(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSETYPE_MAX; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[nCntPause]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//=============================================================================
//�|�[�Y�̍쐬�E�ݒ菈��
//=============================================================================
CPause * CPause::Create(void)
{
	//�C���X�^���X����
	CPause *pPause = NULL;
	pPause = new CPause;

	//�������Ăяo��
	pPause->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	return pPause;
}

//=============================================================================
//�|�[�Y�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CPause::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pausebg.png", &m_pTexture[PAUSETYPE_BG]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/PAUSEMENU.png", &m_pTexture[PAUSETYPE_PAUSE]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RESETGAME.png", &m_pTexture[PAUSETYPE_RETURN_GAME]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RETURNTITLE.png", &m_pTexture[PAUSETYPE_RETURN_TITLE]);

	return S_OK;
}

//=============================================================================
//�|�[�Y�̃e�N�X�`���A�����[�h����
//=============================================================================
void CPause::Unload(void)
{
	//���[�v
	for (int nCnt = 0; nCnt < PAUSETYPE_MAX; nCnt++)
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
//�|�[�Y�̃Z���N�g����
//=============================================================================
void CPause::SelectPause(void)
{
	//�Q�[���p�b�h�̃|�C���^�錾
	CInputGamePad *pInputGamePad;
	pInputGamePad = CManager::GetInputGamePad();

	//�L�[�{�[�h�̃|�C���^�錾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//���[�h�X�V
	CManager::MODE mode;
	mode = CManager::GetMode();

	//bool�̒l��Ԃ�
	bool bPause = CGame::GetPause();

	if (m_pauseType == PAUSETYPE_RETURN_GAME)
	{
		//���[�h���^�C�g���̎�
		if (mode == CManager::MODE_GAME)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				//�Q�[����ʂ֑J��
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
				//�̗͂����Z�b�g����
				CBullet::SetLifeRecovery(3);
			}
		}
	}
	else if (m_pauseType == PAUSETYPE_RETURN_TITLE)
	{
		//���[�h���^�C�g���̎�
		if (mode == CManager::MODE_GAME)
		{
			if (pInputGamePad->IsButtonDown(CInput::KEYINFO_OK) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				//�Q�[����ʂ֑J��
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
			}
		}
	}


	//�|�C���^�̍\����
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputGamePad->IsButtonDown(CInput::KEYINFO_DOWN) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		if (m_pauseType == PAUSETYPE_RETURN_GAME)
		{
			//���_�J���[�̐ݒ�
			pVtx[8].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[9].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[10].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[11].col = D3DXCOLOR(255, 255, 255, 255);

			//���_�J���[�̐ݒ�
			pVtx[12].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[13].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[14].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[15].col = D3DXCOLOR(255, 0, 0, 255);

			m_pauseType = PAUSETYPE_RETURN_TITLE;
		}
		else if (m_pauseType == PAUSETYPE_RETURN_TITLE)
		{
			//���_�J���[�̐ݒ�
			pVtx[12].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[13].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[14].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[15].col = D3DXCOLOR(255, 255, 255, 255);

			//���_�J���[�̐ݒ�
			pVtx[8].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[9].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[10].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[11].col = D3DXCOLOR(255, 0, 0, 255);

			m_pauseType = PAUSETYPE_RETURN_GAME;
		}
	}

	else if (pInputGamePad->IsButtonDown(CInput::KEYINFO_UP) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		if (m_pauseType == PAUSETYPE_RETURN_GAME)
		{
			//���_�J���[�̐ݒ�
			pVtx[8].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[9].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[10].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[11].col = D3DXCOLOR(255, 255, 255, 255);

			//���_�J���[�̐ݒ�
			pVtx[12].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[13].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[14].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[15].col = D3DXCOLOR(255, 0, 0, 255);

			//�^�C�v�ύX
			m_pauseType = PAUSETYPE_RETURN_TITLE;
		}
		else if (m_pauseType == PAUSETYPE_RETURN_TITLE)
		{
			//���_�J���[�̐ݒ�
			pVtx[12].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[13].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[14].col = D3DXCOLOR(255, 255, 255, 255);
			pVtx[15].col = D3DXCOLOR(255, 255, 255, 255);

			//���_�J���[�̐ݒ�
			pVtx[8].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[9].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[10].col = D3DXCOLOR(255, 0, 0, 255);
			pVtx[11].col = D3DXCOLOR(255, 0, 0, 255);

			//�^�C�v�ύX
			m_pauseType = PAUSETYPE_RETURN_GAME;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}

