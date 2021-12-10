//=============================================================================
//
//�f�o�b�N�\������[debugproc.cpp]
// Author; Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "debugproc.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXFONT CDebugProc::m_pFont = NULL; //�t�H���g�p�ϐ�
char CDebugProc::m_aStr[MAX_CHAR] = {};//�����i�[�p��

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{
}

//=============================================================================
//�f�o�b�N�̏���������
//=============================================================================
void CDebugProc::Init(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

//#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
//#endif

	//�o�b�t�@�̃N���A
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//=============================================================================
//�f�o�b�N�̏I������
//=============================================================================
void CDebugProc::Uninit(void)
{
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
//�f�o�b�N�̉ψ�������
//=============================================================================
void CDebugProc::Print(const char * fmt, ...)
{
	char aStar[MAX_CHAR] = {};
	int nCnt = 0;

	va_list ap;

	va_start(ap, fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			//�J�E���g�A�b�v
			fmt++;
			switch (*fmt)
			{
			case 's'://S�̏ꍇ
				nCnt += wsprintf(&aStar[nCnt], "%s", va_arg(ap, char *));
				break;
			case 'd'://d�̏ꍇ
				nCnt += wsprintf(&aStar[nCnt], "%d", va_arg(ap, int));
				break;
			case 'c'://c�̏ꍇ
				nCnt += wsprintf(&aStar[nCnt], "%c", va_arg(ap, char));
				break;
			case 'f'://f�̏ꍇ
				nCnt += sprintf(&aStar[nCnt], "%f", va_arg(ap, double));
				break;
			default:
				break;
			}
		}
		else
		{
			nCnt += wsprintf(&aStar[nCnt], "%c", *fmt);
		}

		//�J�E���g�A�b�v
		fmt++;
	}

	//�I��
	va_end(ap);

	//�e�L�X�g�̕`��
	strcat(m_aStr, aStar);
}

//=============================================================================
//�f�o�b�N�̕`�揈��
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 255, 255, 15));

	//�o�b�t�@�̃N���A
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}
