//*****************************************************************************
//
//�����_���[����[Renderer.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "manager.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CPause;
class CCamera;
class CMeshfield;
class CLight;
class CMeshwall;
class CFade3D;

//*****************************************************************************
//�����_���[�N���X�̒�`
//*****************************************************************************
class CRenderer
{
public:
	CRenderer(); //�R���X�g���N�^
	~CRenderer();//�f�X�g���N�^

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	void DrawTime(void);

private:
	void DrawFPS(void);                  //FPS�`�揈��
	LPDIRECT3D9 m_pD3D;                  // Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;      // Device�I�u�W�F�N�g(�`��ɕK�v)
	LPD3DXFONT m_pFont;                  // �t�H���g�ւ̃|�C���^

	static CPause *m_apPause;//�|�[�Y��ʃ����o�ϐ�
	static bool m_Pause;//�|�[�Y��ʃ����o�ϐ�
	static CCamera *m_pCamera;//�J�����ݒ胁���o�ϐ�
	static CMeshfield *m_pField;//���b�V���t�B�[���h�ݒ�̃����o�ϐ�
	static CMeshwall *m_pWall;//���b�V���E�H�[���ݒ�̃����o�ϐ�
	static CLight *m_pLight;//���C�g�ݒ�̃����o�ϐ�

	LPD3DXFONT m_pTimeFont;                  //�o�ߎ��ԕ\���p�t�H���g
	DWORD m_dwGameStartTime;					//�Q�[���J�n����
	DWORD m_dwGameTime;							//�Q�[���o�ߎ���
	bool m_bEndGame = false;						//�I���t���O
	CManager::MODE m_mode;

	//�t�B�[�h�o�b�N�G�t�F�N�g�p�����o�ϐ�
	LPDIRECT3DTEXTURE9 m_apTextureMT[2];
	LPDIRECT3DSURFACE9 m_apRendererMT[2];
	LPDIRECT3DSURFACE9 m_apBuffMT[2];
	D3DVIEWPORT9 m_viewportMT;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;
};
#endif
