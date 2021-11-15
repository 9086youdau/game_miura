//*****************************************************************************
//
//���b�V���t�B�[���h����[meshfield.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define WIDTHNUM	(10)		// ��(��)
#define DEPTHNUM	(60)		// �s��(���s)
#define WIDTH		(40.0f)	// ���̑傫��
#define DEPTH		(40.0f)	// ���s�̑傫��
#define FIELD_MAX	(32)		// �t�B�[���h�̍ő吔

//*****************************************************************************
//���b�V���t�B�[���h�N���X�̒�`
//*****************************************************************************
class CMeshfield
{
public:
	CMeshfield(); //�R���X�g���N�^
	~CMeshfield();//�f�X�g���N�^

	HRESULT InitMeshfield(void);	// 3D�|���S���̏���������
	void UninitMeshfield(void);		// 3D�|���S���̏I������
	void UpdateMeshfield(void);		// 3D�|���S���̍X�V����
	void DrawMeshfield(void);		// 3D�|���S���̕`�揈��

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshfield = NULL;	// �o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshfield = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTextureMeshfield = NULL;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_posMeshfield;							// �ʒu
	D3DXVECTOR3 m_rotMeshfield;							// ����
	D3DXMATRIX m_mtxWorldMeshfield;						// ���[���h�}�g���b�N�X
	int m_nAllPoint;									// �����_��
	int m_nPolygon;										// �|���S����
	int m_nIdxPoint;									// �C���f�b�N�X�o�b�t�@�̕K�v�Ȋm�ې�
	float m_fWidth = WIDTH;				// �����̒[
	float m_fDepth = DEPTH;				// ���s�̒[
	int m_nWidth = WIDTHNUM + 1;		// �����̒��_��
	int m_nDepth = DEPTHNUM + 1;		// ���s�̒��_��
	float m_meshcount = 0.0f;
	float m_fWidthMax = 0.0f;
	float m_fDepthMax = 0.0f;
};
#endif

