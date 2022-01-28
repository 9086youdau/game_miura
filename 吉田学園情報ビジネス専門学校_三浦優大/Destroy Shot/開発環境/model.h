//=============================================================================
//
// ���f������ [model.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// ���f���N���X�̒�`
//=============================================================================;
//�p����`��N���X
class CModel
{
public:
	CModel(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CModel();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);		//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);								//��������

	void MotionPos(D3DXVECTOR3 pos);
	void MotionRot(D3DXVECTOR3 rot);

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);//�e�N�X�`���������Ă���֐�
	void SetMaterial(D3DXMATERIAL *pMat);//�}�e���A�����������Ă���֐�

	void SetParent(CModel *pModel);
	D3DXMATRIX GetMtxWorld(void);
	LPD3DXMESH GetMash(void) { return m_pMesh; }
	LPD3DXBUFFER GetBuff(void) { return m_pBuffMat; }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATERIAL GetMat(void) { return *m_pMat; }

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		//�e�N�X�`���̃|�C���^
	LPD3DXMESH		m_pMesh;			//���b�V��
	LPD3DXBUFFER	m_pBuffMat;			//�o�b�t�@
	DWORD           m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX		m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXVECTOR3		m_rot;				//����
	int				m_nNumVtx;			//���_��
	float			m_fRot;				//
	CModel			*m_pParent;			//
	static D3DXMATERIAL    *m_pMat;	    //�}�e���A���ւ̃|�C���^

};

#endif