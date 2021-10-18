//================================================
//
//�~��HP����[CircleHp.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "CircleHp.h"
#include "manager.h"
#include "Renderer.h"
#include "Scene2D.h"
#include "gamepad.h"
#include "input.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
LPDIRECT3DTEXTURE9 CCircleHp::m_pTexture[2] = {};
int CCircleHp::m_sirclelife = 0;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CCircleHp::CCircleHp(int nPriority) : CScene2D(nPriority)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CCircleHp::~CCircleHp()
{
}

//=============================================================================
//�i���o�[�̏���������
//=============================================================================
HRESULT CCircleHp::Init(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp)
{
	//�e�N�X�`�����[�h
	Load();

	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 50,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
	}

	VERTEX_2D *pVtx;

	//�^�C�v���
	m_CircleHp = CircleHp;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ʒu���̑��
	m_pos = D3DXVECTOR3(pos);

	//���[�J���ϐ��錾
	float fAngle;
	float rad;

	fAngle = D3DX_PI * 2 / 49;
	rad = fAngle / 2;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[0].rhw = 1.0f;

	for (int nCntAppear = 1; nCntAppear < 52; nCntAppear++, rad += fAngle)
	{
		pVtx[nCntAppear].pos.x = pVtx[0].pos.x + 350.0f * cosf(rad);
		pVtx[nCntAppear].pos.y = pVtx[0].pos.y + 350.0f * sinf(rad);
		pVtx[nCntAppear].pos.z = 0.0f;
		pVtx[nCntAppear].rhw = 1.0f;
		pVtx[nCntAppear].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCntAppear].tex = D3DXVECTOR2(0.5f, 0.5f);
	}

	//����������
	m_sirclelife = 0;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//�i���o�[�̏I������
//=============================================================================
void CCircleHp::Uninit(void)
{
	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�e�N�X�`���̔j��
	for (int nCount = 0; nCount < 2; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

	Release();
}

//=============================================================================
//�i���o�[�̍X�V����
//=============================================================================
void CCircleHp::Update(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPriority = 0; nCntPriority < 6; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCnt);

			if (pScene != NULL)
			{
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == OBJTYPE_ENEMY_BOSS)
				{
					pVtx[0].pos = D3DXVECTOR3(pScene->GetPosition().x, pScene->GetPosition().y, 0.0f);
				}
			}
		}
	}

	//���[�J���ϐ��錾
	float fAngle;
	float rad;

	fAngle = D3DX_PI * 2 / 49;
	rad = fAngle / 2;

	for (int nCntAppear = 1; nCntAppear < 51; nCntAppear++, rad += fAngle)
	{
		pVtx[nCntAppear].pos.x = pVtx[0].pos.x + 350.0f * cosf(rad);
		pVtx[nCntAppear].pos.y = pVtx[0].pos.y + 350.0f * sinf(rad);
		pVtx[nCntAppear].pos.z = 0.0f;
		pVtx[nCntAppear].rhw = 1.0f;
		pVtx[nCntAppear].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCntAppear].tex = D3DXVECTOR2(0.5f, 0.5f);
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//���C�t���Ȃ��Ȃ��������
	if (m_sirclelife >= 50)
	{
		Uninit();
	}
}

//=============================================================================
//�i���o�[�`�揈��
//=============================================================================
void CCircleHp::Draw(void)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (m_CircleHp == CIRCLEHP_OUT)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[0]);

		for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nCntAppear);
		}
	}
	if (m_CircleHp == CIRCLEHP_IN)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[1]);

		for (int nCntAppear = 0; nCntAppear < 50; nCntAppear++)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nCntAppear - m_sirclelife);
		}
	}
}

//=============================================================================
//�i���o�[�̃e�N�X�`�����[�h����
//=============================================================================
HRESULT CCircleHp::Load(void)
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lifeout.png", &m_pTexture[0]);

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lifein.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
//�i���o�[�̃e�N�X�`���j������
//=============================================================================
void CCircleHp::Unload(void)
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
//�i���o�[�̐ݒ�E�쐬����
//=============================================================================
CCircleHp * CCircleHp::Create(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp)
{
	//�C���X�^���X����
	CCircleHp *pCircleHp = NULL;
	pCircleHp = new CCircleHp;

	//�������Ăяo��
	pCircleHp->Init(pos, fHight, fWidth,CircleHp);

	return pCircleHp;
}

//=============================================================================
//�{�X�̗̑͂����炷����
//=============================================================================
void CCircleHp::SetBossLife(int nDamage)
{
	//�J�E���g�𑝂₷
	m_sirclelife += nDamage;
}
