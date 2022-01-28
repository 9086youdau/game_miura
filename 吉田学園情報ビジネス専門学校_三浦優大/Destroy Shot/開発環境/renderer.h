//=============================================================================
//
// �����_���[���� [renderer.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _RENDERE_H_
#define _RENDERE_H_

//==============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "main.h"

//=============================================================================
//�O���錾
//=============================================================================
class CDebugProc;

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CRenderer
{
public:
	CRenderer();								//�R���X�g���N�^
	~CRenderer();								//�f�X�g���N�^
	HRESULT Init(HWND hWnd, bool bWindow);						//����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��
	void Draw2(void);											//�`�揈��(�c������)

																//�f�o�C�X���
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT[0]; }

private:
	void DrawFPS();

	LPDIRECT3D9					m_pD3D = NULL;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9			m_pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)

															//FPS�̏��
	LPD3DXFONT					m_pFont = NULL;				// �t�H���g�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_pTextureMT[2];
	LPDIRECT3DSURFACE9 m_pRenderMT[2];
	LPDIRECT3DSURFACE9 m_pBuffMT[2];
	D3DVIEWPORT9 m_ViewportMT;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;
	static CDebugProc *m_pDebug;	//�f�o�b�N�\���̃����o�ϐ�

	int m_nCnt;                     //�f�o�b�N�\���p�J�E���g(���ѕς��j
	int m_nCnt2;                    //�f�o�b�N�\���p�J�E���g2(���ѕς�2�j
	int m_nMaxCnt;                  //�f�o�b�N�\���p�J�E���g(�ő吔���ѕς��j
	int m_nMinCnt;                  //�f�o�b�N�\���p�J�E���g(�ŏ������ѕς��j
	int m_nCntMaxPart;              //�f�o�b�N�\���p�J�E���g�̍ő吔
	int m_nCntOldMax;               //�f�o�b�N�\���p�J�E���g�̕ۑ��p
	int m_nForCnt;                  //�f�o�b�N�\���p��For
	int m_nCntOld;                  //�f�o�b�N�\���p�J�E���g�̕ۑ��p2
	int m_nForMin;                  //�ŏ���
	int m_nForMax;                  //�ő�l
	int m_nMaxPlayerPart;
	int m_nMaxPlayerPart2;
	int m_nMinPlayerPart;
	bool m_bUse;
	int m_SideCnt;
};

#endif
