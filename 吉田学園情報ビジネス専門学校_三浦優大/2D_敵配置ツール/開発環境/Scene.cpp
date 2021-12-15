//================================================
//
//�V�[������[Scene.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//�C���N���[�h�ǂݍ���
//=================================================
#include "Scene.h"
#include "Renderer.h"
#include "enemy.h"
#include "game.h"
#include "input.h"

//=================================================
//�ÓI�����o�ϐ��錾
//=================================================
CScene *CScene::m_apScene[OBJTYPE_MAX][LOOP_POLYGON] = {};
int CScene::m_nNumAll = 0;
int CScene::m_nIndex = 0;

//=================================================
//�R���X�g���N�^
//=================================================
CScene::CScene(int nPriority)
{
	for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
	{
		if (m_apScene[nPriority][nCnt] == NULL)
		{
			//�����̔ԍ���ݒ�
			m_apScene[nPriority][nCnt] = this;
			m_nID = nCnt;
			m_nNumAll++;
			m_nPriority = nPriority;
			break;
		}
	}
}
//=================================================
//�f�X�g���N�^
//=================================================
CScene::~CScene()
{

}

//=================================================
//�S�Ă̔p������
//=================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				// �I������
				m_apScene[nCntPriority][nCnt]->Uninit();
			}
		}
	}
}

//=================================================
//�S�Ă̍X�V����
//=================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				// �X�V����
				m_apScene[nCntPriority][nCnt]->Update();
			}
		}
	}
}

//=================================================
//�S�Ă̕`�揈��
//=================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				// �X�V����
				m_apScene[nCntPriority][nCnt]->Draw();
			}
		}
	}
}

//=================================================
//�g���Ă��鏊�̔j������
//=================================================
void CScene::Release(void)
{
	//�V�[���̔j��
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		int nID = m_nID;
		int nPriority = m_nPriority;
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;
	}
}

//=================================================
//�^�C�v���򏈗�
//=================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=================================================
//�^�C�v���򏈗�
//=================================================
CScene * CScene::GetScene(int nPriority, int nIndex)
{
	return m_apScene[nPriority][nIndex];
}
