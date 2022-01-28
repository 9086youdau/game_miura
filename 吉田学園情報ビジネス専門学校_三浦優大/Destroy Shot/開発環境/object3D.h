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
#include "sceneX.h"

//=============================================================================
// �I�u�W�F�N�g�N���X�̒�`
//=============================================================================

//�p����`��N���X
class CObject3D : public CSceneX
{
public:
	CObject3D(int nPriority = 0, int nType = 0);						    //�R���X�g���N�^
	virtual ~CObject3D();												    //�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char * TextName, char * TextureName);             //��������

	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);//�����蔻��

private:
	char *m_TextName;                                                //X�f�[�^�̓ǂݍ���
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_MaxObject;
	D3DXVECTOR3 m_MinObject;

	//�����蔻��p�����o�ϐ�
	D3DXVECTOR3 m_aPosColision[4];
	D3DXVECTOR3 m_aVec[4];
	D3DXVECTOR3 m_bVec[4];
	float m_fPlayerVec[4];
	
};

#endif