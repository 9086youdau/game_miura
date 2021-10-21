//*****************************************************************************
//
//�V�[��X����[SceneX.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//�V�[��X�N���X�̒�`
//*****************************************************************************
class CSceneX : public CScene
{
public:
	//�񋓌^�i���f���̎�ށj
	typedef enum
	{
		SCENEXTYPE_NONE = 0,
		SCENEXTYPE_RIGHT,
		SCENEXTYPE_LEFT,
		SCENEXTYPE_MAX
	}SCENEXTYPE;

	CSceneX(); //�R���X�g���N�^
	~CSceneX();//�f�X�g���N�^
	static CSceneX *Create(D3DXVECTOR3 pos,SCENEXTYPE scenextype);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);       //����������
	void Uninit(void);                       //�I������
	void Update(void);						 //�X�V����
	void Draw(void);                         //�`�揈��

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };

private:
	LPD3DXMESH m_pMesh;// ���b�V���ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;// �}�e���A���ւ̃|�C���^
	DWORD m_nNumMat;// �}�e���A���̐�
	D3DXMATRIX m_mtxWorld;// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_rot;//��]����
	D3DXVECTOR3 m_pos;//�ʒu����
	float m_fx, m_fy;
	SCENEXTYPE m_scenetype;//���f���̃^�C�v
};
#endif
