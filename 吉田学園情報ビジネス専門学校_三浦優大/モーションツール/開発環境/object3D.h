//=============================================================================
//
// ���C������ [object3D.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "player3D.h"

//=============================================================================
// �I�u�W�F�N�g�N���X�̒�`
//=============================================================================

//�p����`��N���X
class CObject3D : public CScene
{
public:
	CObject3D(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CObject3D();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nType);			//��������
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	void CObject3D::LoadObject(void);			//�e�L�X�g�̓ǂݍ���

	D3DXMATRIX GetMtxWorld(void);

private:
	LPD3DXMESH		m_pMesh;				//���b�V��
	LPD3DXBUFFER	m_pBuffMat;				//�o�b�t�@
	DWORD			m_nNumMat;				//�}�g���b�N�X
	static D3DXMATRIX		m_mtxWorld;				//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;					//�ʒu
	D3DXVECTOR3		m_posold;					//�ʒu
	D3DXVECTOR3		m_CollisionVecPos[4];
	D3DXVECTOR3		m_move;					//�ʒu
	D3DXVECTOR3		m_rot;					//�ʒu
	int				m_nNumVtx;				//���_��
	float			m_fRot;					//
	static char		*m_pTextName;
	D3DXVECTOR3     m_vtx;
	D3DXVECTOR3		m_MaxStone;				//�ő�l
	D3DXVECTOR3		m_MinStone;				//�ŏ��l
	D3DXVECTOR3		m_aPos[18][4];
	D3DXVECTOR3		m_Vec[18][4];
	float			m_fLength;				//�Ίp���̒���
	float			m_fAngle;				//�Ίp���̊p�x
	float			m_fLength2;				//�Ίp���̒���
	D3DXVECTOR3		m_posMaxBase;
	D3DXVECTOR3		m_posMinBase;

	static D3DXVECTOR3	m_rotStatic;
	static D3DXVECTOR3	m_moveStatic;
	static int m_nType;
	float m_ffRot;
	int g_FailCntDateObjectVector;
	bool m_fUseScene3D;
};

#endif