//=============================================================================
//
// ���C������ [main.h]
// Author : MIURA YUDAI
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"//���͏����ɕK�v
#include "xaudio2.h"
#include <stdio.h>

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ID_BUTTON000 (101)//�{�^����ID
#define ID_EDIT000 (111)//�G�f�B�b�g��ID
#define TIMER_INTERVAL (1000/60)//FPS�v�Z�p
#define LOOP_POLYGON (1356)//���[�v�z��p
#define SCREEN_WIDTH (1920)//��ʂ̕�
#define SCREEN_HEIGHT (1080)//��ʂ̍���
#define SCREEN_CENTER_X (1920 / 2)//��ʂ̔����̕�
#define SCREEN_CENTER_Y (1080 / 2)//��ʂ̔����̍���
#define MAXSIZEPLAYER (100) //�T�C�Y�̑傫��
#define MAXTEXTURE (1.0f)//�e�N�X�`���̃T�C�Y
#define MAX_EXPLOSION_TEXTURE (0.125f)//�e�N�X�`���̃T�C�Y
#define MAX_EXPLOSION_SIZE (30)//�����̃T�C�Y
#define MAX_ENEMY_SIZE_X (50)//�G�l�~�[�̃T�C�Y
#define MAX_ENEMY_SIZE_Y (40)//�G�l�~�[�̃T�C�Y
#define MAX_ENEMY2_SIZE_X (40)//�G�l�~�[�̃T�C�Y
#define MAX_ENEMY2_SIZE_Y (60)//�G�l�~�[�̃T�C�Y
#define MAX_ENEMY_TEXTURE (0.1f)//�G�l�~�[�̃T�C�Y
#define MAX_SCORE_SIZE_X (20)//�X�R�A��X�T�C�Y
#define MAX_SCORE_SIZE_Y (20)//�X�R�A��Y�T�C�Y
#define MAX_EFFECT_SIZE (130)//�G�t�F�N�g�̃T�C�Y
#define MAX_PARTICLE_SIZE (6)//�p�[�e�B�N���̃T�C�Y
#define MAX_LIFE_TEXTURE_SIZE (30)//���C�t�̃e�N�X�`���T�C�Y
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CRenderer;
class CScene;

//*****************************************************************************
//�\���̂̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_3D;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GetFPS(void);                                                 
#endif