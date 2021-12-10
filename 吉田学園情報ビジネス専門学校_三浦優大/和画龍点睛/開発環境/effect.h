//*****************************************************************************
//
// �G�t�F�N�g����[effect.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//�G�t�F�N�g�N���X�̒�`
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	//�񋓌^�i�G�t�F�N�g�̎�ށj
	typedef enum
	{
		EFFECTTYPE_NONE = 0,
		EFFECTTYPE_PLAYER_MOVE,
		EFFECTTYPE_BOSS_END,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect(int nPriority = OBJTYPE_EFFECT);//�R���X�g���N�^
	~CEffect();//�f�X�g���N�^
	static CEffect *Create(D3DXVECTOR3 pos,float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect);//����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��

	void SetEffectMove(void);					        //�G�t�F�N�g�̈ړ�����

	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h

private:
	D3DXVECTOR3 m_move;//�e�̈ړ���
	D3DXVECTOR3 m_pos;//�e�̈ʒu
	static LPDIRECT3DTEXTURE9 m_pTexture;				 //���L�e�N�X�`���̃|�C���^
	int m_nLife;//�G�t�F�N�g�̎���
	float m_fAngle;//�p�x�ۑ��p
	EFFECTTYPE m_effecttype;//�G�t�F�N�g�ۑ��p
};
#endif
