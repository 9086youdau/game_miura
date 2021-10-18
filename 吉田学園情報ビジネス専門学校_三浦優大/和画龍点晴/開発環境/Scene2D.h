//*****************************************************************************
//
//�V�[��2D����[Scene2D.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;
//*****************************************************************************
//�V�[��2D�N���X�̒�`
//*****************************************************************************
class CScene2D : public CScene 
{
public:
	CScene2D(int nPriority = OBJTYPE_ENEMY_BOSS); //�R���X�g���N�^
	~CScene2D();//�f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos,float fHight,float fWidth);       //����������
	void Uninit(void);                       //�I������
	void Update(void);						 //�X�V����
	void Draw(void);                         //�`�揈��
	void SetPosition(D3DXVECTOR3 pos,D3DXVECTOR3 rot);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetAllPosition(void) { return m_pos; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetTex(float tex, int nCnt);
	void SetBg(float size, float nCnt);
	void SetCol(D3DXCOLOR col);
	void SetRanking(float nRanking);
	D3DXCOLOR SetEnemyCol(D3DXCOLOR col);
	bool Collision(D3DXVECTOR3 pos, float fsizeX, float fsizeY, OBJTYPE Type, CScene **pColScene);
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
private:
	LPDIRECT3DTEXTURE9 m_pTexture;		 //�e�N�X�`�����
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  //���_���
	D3DXVECTOR3 m_pos;                   //�ʒu���
	D3DXVECTOR3 m_Rot;                   //��]���
	D3DXCOLOR m_col;                     //�J���[���
	float  m_fLength;                    //���̒���
	float m_fAngle;                      //�p�x�ۑ��p
	float m_fx, m_fy;
};
#endif