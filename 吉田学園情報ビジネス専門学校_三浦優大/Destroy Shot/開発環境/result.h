//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : Miura Yudai
//
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_

//==============================================================================
//�C���N���[�h
//==============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//==============================================================================
//�O���錾
//==============================================================================
class CScore;
class CSound;
class CNumber;

//=============================================================================
// ���U���g��ʃN���X�̒�`
//=============================================================================
class CResult : public CScene
{
public:
	CResult();									//�R���X�g���N�^
	~CResult();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//����������
	void Uninit(void);												//�I������
	void Update(void);												//�X�V����
	void Draw(void);												//�`�揈��

	static CResult *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//��������

private:
	LPD3DXFONT					m_pTimeFontGame;		// �\���p�t�H���g
	int							m_nRankintData[5];		//�����L���O�f�[�^�ۊ�

	CNumber *m_apNumber[5][8];			//����
	static CScore		*m_pScore[5];					//�A�h���X�ۑ��p�|�C���^
	CSound		*m_pSound;								//�T�E���h�|�C���^
};

#endif