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
#include "input.h"
#include "game.h"
#include "sound.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "debugproc.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
CPause *CRenderer::m_apPause = NULL;
bool CRenderer::m_Pause = false;
CCamera *CRenderer::m_pCamera = NULL;
CMeshfield *CRenderer::m_pField = NULL;
CMeshwall *CRenderer::m_pWall = NULL;
CLight *CRenderer::m_pLight = NULL;
CDebugProc *CRenderer::m_pDebug = NULL;

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

	//���[�J���ϐ��錾
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;

	//�c������
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�����_�����O�^�[�Q�b�g�̃e�N�X�`������
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_apTextureMT[nCnt], NULL);

		//�e�N�X�`�������_�����O�p�̃C���^�[�t�F�[�X�̐���
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRendererMT[nCnt]);

		//Z�o�b�t�@�̐���
		m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_apBuffMT[nCnt], NULL);

		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		m_pD3DDevice->SetRenderTarget(0, m_apRendererMT[nCnt]);

		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		//�����_�����O�^�[�Q�b�g�̃e�N�X�`���N���A
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	//�t�B�[�h�o�b�N�p�̃|���S������
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
	}

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f - 0.5f, SCREEN_HEIGHT - 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f - 0.5f, 0.0f - 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - 0.5f, SCREEN_HEIGHT - 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - 0.5f, 0.0f - 0.5f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuffMT->Unlock();

	//�f�o�b�N�̐ݒ�
	m_pDebug = new CDebugProc;
	m_pDebug->Init();

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

	//���_�o�b�t�@�̊J��
	if (m_pVtxBuffMT != NULL)
	{
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = NULL;
	}

	//�e�N�X�`���C���^�[�t�F�[�X�̔j��
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}

		if (m_apRendererMT[nCnt] != NULL)
		{
			m_apRendererMT[nCnt]->Release();
			m_apRendererMT[nCnt] = NULL;
		}

		if (m_apBuffMT[nCnt] != NULL)
		{
			m_apBuffMT[nCnt]->Release();
			m_apBuffMT[nCnt] = NULL;
		}
	}
}

//=================================================
//�X�V����
//=================================================
void CRenderer::Update(void)
{
	//�V�[���̑S�ẴA�b�v�f�[�g
	CScene::UpdateAll();

	//bool�̒l��Ԃ�
	m_Pause = CGame::GetPause();

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
		// �|���S���̕`�揈��
		CScene::DrawAll();

		//�f�o�b�N�f�[�^�̕�����
		m_pDebug->Print("�G�l�~�[�J�E���g�y%d�z\n�G�l�~�[�̃T�u�J�E���g�y%d / %d�z\n�w�肵���J�E���g�ōĐ��yF8�z\n�G�l�~�[�J�E���g���������yF5�z\n�G�l�~�[�J�E���g�𒲐��yF4�z�yF6�z\n�|�[�Y��ԁyP�z",CGame::GetEnemyCount(),CGame::GetEnemySubCount(), CGame::GetEnemyMaxCount());

		//�f�o�b�N�v���b�N�̕`��
		m_pDebug->Draw();

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
