//================================================
//
//�����_���[����[Renderer.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#define _CRT_SECURE_NO_WARNINGS
#include "Renderer.h"
#include "Scene.h"
#include "manager.h"
#include "number.h"
#include "input.h"
#include "pause.h"
#include "game.h"
#include "sound.h"
#include "camera.h"
#include "bg.h"
#include "meshfield.h"
#include "light.h"
#include "meshwall.h"
#include "player.h"
#include "fade.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
CPause *CRenderer::m_apPause = NULL;
bool CRenderer::m_Pause = false;
CCamera *CRenderer::m_pCamera = NULL;
CMeshfield *CRenderer::m_pField = NULL;
CMeshwall *CRenderer::m_pWall = NULL;
CLight *CRenderer::m_pLight = NULL;

//=================================================
//�R���X�g���N�^
//=================================================
CRenderer::CRenderer()
{
	
}
//=================================================
//�f�X�g���N�^
//=================================================
CRenderer::~CRenderer()
{

}

//=================================================
//����������
//=================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 35, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFont);

		m_dwGameStartTime = timeGetTime();//�Q�[���J�n����

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	//���C�g�̐���
	m_pLight = new CLight;
	m_pLight->Init();

	//�J�����̐���
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//���b�V���t�B�[���h�̐ݒ�	
	m_pField = new CMeshfield;
	m_pField->InitMeshfield();

	//���b�V���E�H�[���̐ݒ�	
	m_pWall = new CMeshwall;
	m_pWall->InitMeshwall();
	m_pWall->SetMeshwall(D3DXVECTOR3(200.0f, 100.0f, 200.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), 50.0f, 50.0f, 50, 4);
	m_pWall->SetMeshwall(D3DXVECTOR3(-200.0f, 100.0f, 200.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f), 50.0f, 50.0f, 50, 4);

	//�|�[�Y�̃N���G�C�g����
	m_apPause = CPause::Create();

	return S_OK;
}

//=================================================
//�I������
//=================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// ���\���p�t�H���g�̔j��
	if (m_pTimeFont != NULL)
	{
		m_pTimeFont->Release();
		m_pTimeFont = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	//�J�����̔j��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//���b�V���t�B�[���h�̔j��
	if (m_pField != NULL)
	{
		m_pField->UninitMeshfield();
		delete m_pField;
		m_pField = NULL;
	}

	//���b�V���E�H�[���̔j��
	if (m_pWall != NULL)
	{
		m_pWall->UninitMeshwall();
		delete m_pWall;
		m_pWall = NULL;
	}

	//���C�g�̔j��
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
}

//=================================================
//�X�V����
//=================================================
void CRenderer::Update(void)
{
	//�V�[���̑S�ẴA�b�v�f�[�g
	CScene::UpdateAll();

	//���C�g�̍X�V
	m_pLight->Update();

	//bool�̒l��Ԃ�
	m_Pause = CGame::GetPause();

	if (m_Pause == false)
	{
		//���b�V���t�B�[���h�̍X�V
		m_pField->UpdateMeshfield();

		//���b�V���E�H�[���̍X�V
		m_pWall->UpdateMeshwall();

		//�Q�[�����I�����Ă��Ȃ��ꍇ�͍X�V����
		if (m_bEndGame == false)
		{
			//���Ԃ̌v�Z����
			m_dwGameTime = timeGetTime() - m_dwGameStartTime;
		}
	}

	if (m_mode == CManager::MODE_TITLE)
	{
		m_dwGameStartTime = timeGetTime();//�Q�[���J�n����
	}

	//�|�[�Y���
	if (m_Pause == true)
	{
		m_apPause->Update();
	}

	//���[�h���
	m_mode = CManager::GetMode();
}

//=================================================
//�`�揈��
//=================================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255),
		1.0f,
		0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ���[�J���ϐ��錾
		D3DVIEWPORT9 viewportDef;

		// ���݂̃r���[�|�[�g���擾
		m_pD3DDevice->GetViewport(&viewportDef);

		for (int nCntCamera = 0; nCntCamera < 2; nCntCamera++)
		{
			//�J�����̐ݒ�
			m_pCamera->SetCamera(nCntCamera);

			if (nCntCamera == 0)
			{
				if (m_mode == CManager::MODE_GAME)
				{
					//���b�V���t�B�[���h�̕`��
					m_pField->DrawMeshfield();

					//���b�V���E�H�[���̍X�V
					m_pWall->DrawMeshwall();

				}
			}
			else if (nCntCamera == 1)
			{
				// �|���S���̕`�揈��
				CScene::DrawAll();

				if (m_mode == CManager::MODE_GAME)
				{
					if (m_Pause == true)
					{
						m_apPause->Draw();
					}
				}
			}
		}

		// �r���[�|�[�g��߂�
		m_pD3DDevice->SetViewport(&viewportDef);

#ifdef _DEBUG
		// FPS�\��
		DrawFPS();
#endif

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// FPS�v�Z����
//=============================================================================
#ifdef _DEBUG
void CRenderer::DrawFPS(void)
{
	int nCnt = GetFPS();
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", nCnt);
	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif
//=============================================================================
// DrawTime�v�Z����
//=============================================================================
void CRenderer::DrawTime(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//char str[256];

	if (m_bEndGame == false)
	{
		//sprintf(str, "TIME:%.3f\n", (float)m_dwGameTime / 1000.f);
	}

	// �e�L�X�g�`��
	//m_pTimeFont->DrawText(NULL, str, -1, &rect, DT_CENTER, D3DCOLOR_ARGB(0xff, 0x00, 0xff, 0xff));
}
