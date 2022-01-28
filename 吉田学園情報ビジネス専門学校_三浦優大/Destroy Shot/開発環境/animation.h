//=============================================================================
//
// �A�j���[�V�������� [animation.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CPasserby;

//=============================================================================
// �A�j���[�V�����N���X�̒�`
//=============================================================================
//�p����`��N���X
class CAnimation : public CScene3D
{
public:
	CAnimation(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CAnimation();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt);               //����������
	void Uninit(void);														         //�I������
	void Update(void);														         //�X�V����
	void Draw(void);														         //�`�揈��

	static CAnimation *Create(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt);  //��������
	static bool GetColision(void) { return m_bPasserbyColision; }                    //�G�̓����蔻��t���O�󂯓n���p�֐�
	static CPasserby *GetPasserby(int nCnt) { return m_pPasserby[nCnt]; }            //�G�̕ۑ��p�|�C���^�ϐ��̎󂯓n���p

	static void Load(void);                                                 //�e�N�X�`�����[�h����
	static void Unload(void);                                               //�e�N�X�`���A�����[�h

private:

	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`�����
	static CPasserby *m_pPasserby[256];             //�G�̃|�C���^�ۑ��p
	D3DXVECTOR3 m_pos;                              //�ʒu���
	int m_nCounterAnim;                             //�A�j���[�V�����̃J�E���g�p
	int m_nPatternAnim;                             //�A�j���[�V�����̃p�^�[���p
	static bool m_bPasserbyColision;                //�����蔻��̃t���O
	int m_nCnt;                                     //�G�̃|�C���^�̔z��p
};

#endif
