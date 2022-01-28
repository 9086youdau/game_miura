//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// �C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
// �o���b�g�N���X�̒�`
//=============================================================================
//�p����`��N���X
class CBullet: public CScene3D
{
public:
	CBullet(int nPriority = 0, int nType = 0);								//�R���X�g���N�^
	virtual ~CBullet();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, float fHight, float fWidth);		//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife);   //��������

	void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife);           //�����������p

	static void Load(void);                                                 //�e�N�X�`�����[�h�p
	static void Unload(void);                                               //�e�N�X�`���A�����[�h

	static D3DXVECTOR3 GetPos(void) { return m_getpos; }                    //�ʒu���󂯓n���p

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`�����
	D3DXVECTOR3 m_pos;                              //�ʒu�ۑ�
	static D3DXVECTOR3 m_getpos;                    //�󂯓n���p�ʒu
	D3DXVECTOR3 m_posold;                           //�̂̈ʒu�ۑ�
	D3DXVECTOR3 m_move;                             //�ړ��ʕۑ�
	int m_nLife;                                    //�����ۑ��p
};

#endif

