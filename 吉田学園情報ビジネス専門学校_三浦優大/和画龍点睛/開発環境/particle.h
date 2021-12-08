//*****************************************************************************
//
//�p�[�e�B�N������[effect.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "effect.h"

//*****************************************************************************
//�p�[�e�B�N���N���X�̒�`
//*****************************************************************************
class CParticle : public CEffect
{
public:
	//�񋓌^�i�p�[�e�B�N���̎�ށj
	typedef enum
	{
		PARTICLE_PLAYER,
		PARTICLE_ENEMY,
		PARTICLE_MAX
	}PARTICLE;

	CParticle();
	~CParticle();
	static CParticle *Create(D3DXVECTOR3 pos,D3DXCOLOR col,PARTICLE particle);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,D3DXCOLOR col,PARTICLE particle);	 //����������
	void Uninit(void);									 //�I������
	void Update(void);									 //�X�V����
	void Draw(void);									 //�`�揈��

	void SetParticleMove(void);					 //�e�̈ړ�����

	static HRESULT Load();								 //�e�N�X�`���̃��[�h
	static void Unload(void);							 //�e�N�X�`���̃A�����[�h

private:
	D3DXVECTOR3 m_move;//�e�̈ړ���
	D3DXVECTOR3 m_pos;//�e�̈ʒu
	static LPDIRECT3DTEXTURE9 m_pTexture;				 //���L�e�N�X�`���̃|�C���^
	int m_nLife;//�G�t�F�N�g�̎���
	float m_fAngle;//�p�x�v�Z�p
};
#endif
