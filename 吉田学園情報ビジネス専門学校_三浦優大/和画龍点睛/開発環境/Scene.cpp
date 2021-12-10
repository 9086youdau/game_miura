//================================================
//
//シーン処理[Scene.cpp]
// Author; miura yuudai
//
//================================================

//=================================================
//インクルード読み込み
//=================================================
#include "Scene.h"
#include "Renderer.h"
#include "enemy.h"
#include "hp.h"
#include "game.h"
#include "pause.h"
#include "input.h"
#include "score.h"
#include "particle.h"
#include "camera.h"


//=================================================
//静的メンバ変数宣言
//=================================================
CScene *CScene::m_apScene[OBJTYPE_MAX][LOOP_POLYGON] = {};
int CScene::m_nNumAll = 0;
bool CScene::m_state = false;
int CScene::m_statecount = 0;
int CScene::m_nIndex = 0;

//=================================================
//コンストラクタ
//=================================================
CScene::CScene(int nPriority)
{
	for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
	{
		if (m_apScene[nPriority][nCnt] == NULL)
		{
			//自分の番号を設定
			m_apScene[nPriority][nCnt] = this;
			m_nID = nCnt;
			m_nNumAll++;
			m_nPriority = nPriority;
			break;
		}
	}

	//メンバ変数初期化
	m_nLife = 80;
}
//=================================================
//デストラクタ
//=================================================
CScene::~CScene()
{

}

//=================================================
//全ての廃棄処理
//=================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				// 終了処理
				m_apScene[nCntPriority][nCnt]->Uninit();
			}
		}
	}
}

//=================================================
//全ての更新処理
//=================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				// 更新処理
				m_apScene[nCntPriority][nCnt]->Update();
			}
		}
	}
}

//=================================================
//全ての描画処理
//=================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < OBJTYPE_MAX; nCntPriority++)
	{
		for (int nCnt = 0; nCnt < LOOP_POLYGON; nCnt++)
		{
			if (m_apScene[nCntPriority][nCnt] != NULL)
			{
				// 更新処理
				m_apScene[nCntPriority][nCnt]->Draw();
			}
		}
	}

	//カメラの処理
	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetCamera(1);
}

//=================================================
//使われている所の破棄処理
//=================================================
void CScene::Release(void)
{
	//シーンの破棄
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
//タイプ分岐処理
//=================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=================================================
//タイプ分岐処理
//=================================================
CScene * CScene::GetScene(int nPriority, int nIndex)
{
	return m_apScene[nPriority][nIndex];
}

//=================================================
//エネミーのダメージ処理
//=================================================
void CScene::SetDamage(int nDamage)
{
	//体力からダメージをひく
	m_nLife -= nDamage;
}

