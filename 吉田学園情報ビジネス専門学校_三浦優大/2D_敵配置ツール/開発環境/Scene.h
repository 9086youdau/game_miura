//*****************************************************************************
//
//�V�[������[Scene.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�V�[���N���X�̒�`
//*****************************************************************************
class CScene
{
public:
	//�񋓌^(�I�u�W�F�N�g�̐��A�D�揇��)
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_ENEMY_BOSS,
		OBJTYPE_ENEMY_BULLET,
		OBJTYPE_PLAYER_BULLET,
		OBJTYPE_PLAYER_ATTACK,
		OBJTYPE_BG,
		OBJTYPE_FADE,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = OBJTYPE_BG);                                      //�R���X�g���N�^
	virtual ~CScene();                                                      //�f�X�g���N�^

	virtual HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth) = 0;         //����������
	virtual void Uninit(void) = 0;                                     //�I������
	virtual void Update(void) = 0;									   //�X�V����
	virtual void Draw(void) = 0;                                       //�`�揈��

	static void ReleaseAll(void);                                           //�S�Ă̔p������
	static void UpdateAll(void);                                            //�S�Ă̍X�V����
	static void DrawAll(void);												//�S�Ă̕`�揈��
	void Release(void);														//������̔j��

	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void) { return m_objType; }
	static CScene *GetScene(int nPriority,int nIndex);
	void SetDamage(int nDamage);
	virtual D3DXVECTOR3 GetPosition(void) = 0;
	virtual float GetSizeX(void) = 0;
	virtual float GetSizeY(void) = 0;

private:
	static CScene *m_apScene[OBJTYPE_MAX][LOOP_POLYGON];//�V�[���̑S�Ă̏��
	static int m_nNumAll;                               //�J�E���g�p�����o�ϐ�
	int m_nID;                                          //�i�[��̔ԍ�
	OBJTYPE m_objType;                                  //�I�u�W�F�N�g�̎��
	static int m_nIndex;                                //�ԍ��̏���
	int m_nPriority;									//�D�揇�Ԃ̔ԍ�
};
#endif
