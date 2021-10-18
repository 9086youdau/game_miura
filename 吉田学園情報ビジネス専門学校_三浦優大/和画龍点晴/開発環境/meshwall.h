//*****************************************************************************
//
//���b�V���E�H�[������[meshwall.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _MESHFWALL_H_
#define _MESHFWALL_H_

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
//���b�V���E�H�[���N���X�̒�`
//*****************************************************************************
class CMeshwall
{
public:
	CMeshwall(); //�R���X�g���N�^
	~CMeshwall();//�f�X�g���N�^
	HRESULT InitMeshwall(void);	// 3D�|���S���̏���������
	void UninitMeshwall(void);		// 3D�|���S���̏I������
	void UpdateMeshwall(void);		// 3D�|���S���̍X�V����
	void DrawMeshwall(void);		// 3D�|���S���̕`�揈��
	void SetMeshwall(				// ���b�V���ǂ̐ݒ菈��
		D3DXVECTOR3 pos,
		D3DXVECTOR3 rot,
		float fWidth,
		float fHeight,
		int nWidth,
		int nHeight);
private:
	// �ǂ̍\����
	typedef struct
	{
		D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
		D3DXVECTOR3 m_pos;		// �����ʒu
		D3DXVECTOR3	m_posMove;	// �����ʒu����̈ړ������ʒu
		D3DXVECTOR3 m_rot;		// ����
		float m_fWidth;			// ��
		float m_fHeight;			// ����
		float m_fWidthMax;
		float m_fHeightMax;
		int m_nWidth;				// ��
		int m_nHeight;			// �s��
		int m_nWidthPoint;		// ���̒��_��
		int m_nHeightPoint;		// �����̒��_��
		int m_nAllPoint;
		int m_nPolygon;
		int m_nIdxPoint;
		bool m_bUse;				// �g�p���
	} Meshwall;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshwall = NULL;	// �o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshwall = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTextureMeshwall = NULL;		// �e�N�X�`���ւ̃|�C���^
	Meshwall m_meshwall[2];
	float anCnt;
};
#endif

