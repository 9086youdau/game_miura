//=============================================================================
//
// �V�[������ [scene.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_

//==============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_3D;

//=============================================================================
// �`��N���X�̒�`
//=============================================================================
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,				// �v���C���[
		OBJTYPE_ENEMY,				// �G
		OBJTYPE_BULLET,				// ���{�X
		OBJTYPE_PARTICLE,				// ���{�X
		OBJTYPE_ENEMY3,				// �{�X
		OBJTYPE_BULET_PLAYER,		// �v���C���[���甭�˂��ꂽ�e
		OBJECT_MAX
	}OBJTYPE;

	CScene(int nPriority = 1, int nType = 0);				//�R���X�g���N�^
	virtual ~CScene();										//�f�X�g���N�^

	virtual HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY) = 0;	//����������
	virtual void Uninit(void) = 0;											//�I������
	virtual void Update(void) = 0;											//�X�V����
	virtual void Draw(void) = 0;											//�`�揈��

	static void ReleaseAll(void);											//���ׂĂ̏I������
	static void UpdateAll(void);											//���ׂĂ̍X�V����
	static void DrawAll(void);												//���ׂĂ̕`�揈��

	static CScene *GetScene(int nIdx, int nPriority);

	//�^�C�v�Ǘ�
	OBJTYPE GetObjType(void) { return m_objType; }
	void SetObjType(OBJTYPE objType) { m_objType = objType; }

	//�T�C�Y
	float GetSizeX(void) { return m_fSizeX; }
	void SetSizeXScene(float nSize) { m_fSizeX = nSize; }
	float GetSizeY(void) { return m_fSizeY; }
	void SetSizeYScene(float nSize) { m_fSizeY = nSize; }

	//�F���
	D3DXCOLOR GetCollar(void) { return m_col; }
	void SetCollar(D3DXCOLOR col) { m_col = col; }

	//�ʒu�Ǘ�
	virtual D3DXVECTOR3 GetPosition(void);

protected:
	void Release(void);

private:
	static int		m_NumAll;								//�����ԍ�
	int				m_nID;									//�i�[��̔ԍ�
	OBJTYPE			m_objType;								//�I�u�W�F�N�g�̎��
	float			m_fSizeX;								//�T�C�Y
	float			m_fSizeY;								//�T�C�Y
	int				m_nPriority;							// �D�揇��
	int				m_nType;								//�^�C�v
	int				m_nLife;								//���C�t

	//�F���
	D3DXCOLOR		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���X�g���p�ϐ�
	static CScene *m_pTop[OBJECT_MAX][3];	//�őO��
	static CScene *m_pCur[OBJECT_MAX][3];	//�Ō��
	CScene *m_pPrev;				//�O�̃|�C���^�[
	CScene *m_pNext;				//��̃|�C���^�[
	bool m_bDeath  = false;			//���S�t���O
};

#endif
