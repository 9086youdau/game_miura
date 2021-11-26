//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CObject3D;

//=============================================================================
// �Q�[����ʃN���X�̒�`
//=============================================================================
//�p����`��N���X
class CGame : public CScene
{
public:
	CGame();									//�R���X�g���N�^
	~CGame();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��
	static CObject3D *GetObject(void) { return m_pObject3D; }

	//��������
	static CGame *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	int FailObject();

private:
	LPD3DXFONT	m_pTimeFontGame;					// �\���p�t�H���g
	static CObject3D *m_pObject3D;					//����Get
};

#endif