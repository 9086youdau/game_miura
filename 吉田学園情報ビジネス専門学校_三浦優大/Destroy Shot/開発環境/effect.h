//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
// �G�t�F�N�g�N���X�̒�`
//=============================================================================

//�p����`��N���X
class CEffect : public CScene3D
{
public:
	CEffect(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CEffect();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col, float fHight, float fWidth);;			//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	void SetEffect(D3DXVECTOR3 pos,int nLife, D3DXCOLOR col);        //��������ϐ��ɑ������p�̊֐�

	static CEffect *Create(D3DXVECTOR3 pos,D3DXCOLOR col, int nLife, float fHight, float fWidth);//��������

	static void Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`�����
	D3DXVECTOR3 m_pos;                              //�ʒu���
	D3DXVECTOR3 m_move;                             //�ړ���
	int m_nLife;                                    //����
	D3DXCOLOR m_col;                                //�J���[���
};

#endif
