//=============================================================================
//
// �`���[�g���A����ʏ��� [tutorial.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//==============================================================================
//�C���N���[�h�ǂݍ���
//==============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �`���[�g���A����ʃN���X�̒�`
//=============================================================================
class CTutorial : public CScene
{
public:
	CTutorial();									//�R���X�g���N�^
	~CTutorial();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	static CTutorial *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������

private:
};

#endif