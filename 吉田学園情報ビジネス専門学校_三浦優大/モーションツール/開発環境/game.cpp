//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルードファイル読み込み
//=============================================================================
#include "game.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "object3D.h"
#include "player3D.h"
#include "meshfield.h"
#include "scene3D.h"
#include "passerby.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CObject3D *CGame::m_pObject3D = NULL;

//==============================================================================
// コンストラクタ
//==============================================================================
CGame::CGame()
{
}

//==============================================================================
// デストラクタ
//==============================================================================
CGame::~CGame()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// フォントオブジェクトの生成
	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFontGame);	

	//メッシュフィールド
	CMeshfield::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	//3Dプレイヤーの生成
	CPlayer3D::Create();

	//通行人の生成
	CPasserby::Create(D3DXVECTOR3(-4903.0f, 0.0f, -4867.0f), PASSERBY_TYPE_MEN, 0);
	CPasserby::Create(D3DXVECTOR3(-4903.0f, 0.0f, 4867.0f), PASSERBY_TYPE_MEN, 1);
	CPasserby::Create(D3DXVECTOR3(4903.0f, 0.0f, 4867.0f), PASSERBY_TYPE_MEN, 2);
	CPasserby::Create(D3DXVECTOR3(4903.0f, 0.0f, -4867.0f), PASSERBY_TYPE_MEN, 3);

	//猫の生成
	CPasserby::Create(D3DXVECTOR3(-4978.0f, 0.0f, 5575.0f), PASSERBY_TYPE_CAT, 4);
	CPasserby::Create(D3DXVECTOR3(-4778.0f, 0.0f, 5775.0f), PASSERBY_TYPE_CAT, 4);
	CPasserby::Create(D3DXVECTOR3(-4578.0f, 0.0f, 5975.0f), PASSERBY_TYPE_CAT, 4);
	CPasserby::Create(D3DXVECTOR3(-4378.0f, 0.0f, 6175.0f), PASSERBY_TYPE_CAT, 4);
	CPasserby::Create(D3DXVECTOR3(-4178.0f, 0.0f, 6375.0f), PASSERBY_TYPE_CAT, 4);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CGame::Uninit(void)
{
	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// オブジェクト破棄
	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void CGame::Update(void)
{
}

//==============================================================================
// 描画処理
//==============================================================================
void CGame::Draw(void)
{

}

//==============================================================================
// 生成処理
//==============================================================================
CGame * CGame::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ポインタ変数の生成
	CGame *pGame;
	pGame = new CGame;

	// NULLチェック
	if (pGame != NULL)
	{
		pGame->Init(pos, fSizeX, fSizeY);
	}

	return pGame;
}

////---------------------------------------------
////書き出し
////---------------------------------------------
//int CGame::FailObject()
//{
//	FILE *fp;
//
//	fp = fopen("data/TEXT/VectorSet.txt", "w");
//
//	for (int nCnt = 0; nCnt < g_FileCnt + 1; nCnt++)
//	{
//		//書き込む内容
//		fprintf(fp, "VECTORSET\n");
//
//		fprintf(fp, "	Vec1POSX = %.1f\n", g_posfile[nCnt][0].x);
//		fprintf(fp, "	Vec1POSY = %.1f\n", g_posfile[nCnt][0].y);
//		fprintf(fp, "	Vec1POSZ = %.1f\n", g_posfile[nCnt][0].z);
//
//		fprintf(fp, "	Vec2POSX = %.1f\n", g_posfile[nCnt][1].x);
//		fprintf(fp, "	Vec2POSY = %.1f\n", g_posfile[nCnt][1].y);
//		fprintf(fp, "	Vec2POSZ = %.1f\n", g_posfile[nCnt][1].z);
//
//		fprintf(fp, "	Vec3POSX = %.1f\n", g_posfile[nCnt][2].x);
//		fprintf(fp, "	Vec3POSY = %.1f\n", g_posfile[nCnt][2].y);
//		fprintf(fp, "	Vec3POSZ = %.1f\n", g_posfile[nCnt][2].z);
//
//		fprintf(fp, "	Vec4POSX = %.1f\n", g_posfile[nCnt][3].x);
//		fprintf(fp, "	Vec4POSY = %.1f\n", g_posfile[nCnt][3].y);
//		fprintf(fp, "	Vec4POSZ = %.1f\n", g_posfile[nCnt][3].z);
//
//		fprintf(fp, "END_VECTORSET\n\n");
//	}
//
//	//「txt」のファイルを閉じる
//	fclose(fp);
//
//	return 0;
//}
