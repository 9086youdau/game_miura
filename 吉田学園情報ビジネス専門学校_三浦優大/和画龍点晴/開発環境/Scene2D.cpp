//================================================
//
// �V�[��2D����[Scene2D.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "Scene2D.h"
#include "Renderer.h"
#include "manager.h"
#include "input.h"
#include "enemy.h"

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// �|���S��2D�̏���������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu���̑��
	m_pos = D3DXVECTOR3(pos);

	//�J���[���ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�Ίp���̒����̐ݒ�
	m_fLength = sqrtf(fWidth * fWidth + fHight * fHight);

	//�Ίp���̊p�x�̐ݒ�
	m_fAngle = atan2f(fWidth, fHight);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y, m_pos.z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHight, m_pos.z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + fHight, m_pos.z);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S��2D�̏I������
//=============================================================================
void CScene2D::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �|���S��2D�̍X�V����
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// �|���S��2D�̕`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �|���S���̐ݒ菈��
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[�̒��_���W
	pVtx[0].pos.x = pos.x - sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.y = pos.y - cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[0].pos.z = m_pos.z;

	pVtx[1].pos.x = pos.x + sinf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.y = pos.y + cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[1].pos.z = m_pos.z;

	pVtx[2].pos.x = pos.x - sinf(rot.z + m_fAngle) * m_fLength;
	pVtx[2].pos.y = pos.y - cosf(rot.z + m_fAngle) * m_fLength;
	pVtx[2].pos.z = m_pos.z;

	pVtx[3].pos.x = pos.x + sinf(rot.z - m_fAngle) * m_fLength;
	pVtx[3].pos.y = pos.y + cosf(rot.z - m_fAngle) * m_fLength;
	pVtx[3].pos.z = m_pos.z;

	//�l��������
	m_pos = pos;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �|���S���̃e�N�X�`������
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture);

	//�l��������
	m_pTexture = pTexture;
}

//=============================================================================
// �|���S���̃e�N�X�`������
//=============================================================================
void CScene2D::SetTex(float tex,int nCnt)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(tex + (tex * nCnt), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(tex + (tex * nCnt),1.0f);
	pVtx[2].tex = D3DXVECTOR2(tex * nCnt, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(tex * nCnt, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �w�i�̃e�N�X�`������
//=============================================================================
void CScene2D::SetBg(float size,float nCnt)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + nCnt);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 1.5f + nCnt);
	pVtx[2].tex = D3DXVECTOR2(size, 0.0f + nCnt);
	pVtx[3].tex = D3DXVECTOR2(size, 1.5f + nCnt);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �J���[�ϊ��֐�����
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�����L���O�̃e�N�X�`������
//=============================================================================
void CScene2D::SetRanking(float nRanking)
{
	//�|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f + nRanking);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.2f + nRanking);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f + nRanking);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.2f + nRanking);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
//�G�l�~�[�̐F�ύX����
//=============================================================================
D3DXCOLOR CScene2D::SetEnemyCol(D3DXCOLOR col)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�Ԃ�l
	return D3DXCOLOR();
}
//=============================================================================
//�Փ˔���
//=============================================================================
bool CScene2D::Collision(D3DXVECTOR3 pos, float fsizeX, float fsizeY, OBJTYPE Type, CScene **pColScene)
{
	bool bCol = false;
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < LOOP_POLYGON; nCntScene++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority,nCntScene);

			if (pScene != NULL)
			{
				OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == Type)
				{
					//�ʒu���擾
					D3DXVECTOR3 Epos = pScene->GetPosition();

					float fSizeX, fSizeY;

					//�T�C�Y���擾
					fSizeX = pScene->GetSizeX();
					fSizeY = pScene->GetSizeY();

					if (Epos.y + fSizeX >= pos.y - fsizeY &&  Epos.y - fSizeX <= pos.y + fsizeY&&
						pos.x - fsizeX <= Epos.x + fSizeX && pos.x + fsizeX >= Epos.x - fSizeY)
					{
						*pColScene = pScene;

						bCol = true;
					}
				}
			}
		}
	}
	return bCol;
}
