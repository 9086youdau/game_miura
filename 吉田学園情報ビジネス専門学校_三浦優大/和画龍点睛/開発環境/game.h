//*****************************************************************************
//
//　ゲーム処理[game.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CPlayer;
class CBg;
class CEnemy;
class CScore;
class CLife;
class CGraze;
class CPause;
class CUi;
class CBomb;
class CDirecting;

//*****************************************************************************
//ゲームクラスの定義
//*****************************************************************************
class CGame : public CScene
{
public:
	CGame();//コンストラクタ
	~CGame();//デストラクタ
	static CGame *Create(void);//ゲームのクリエイト処理

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);//ゲームの初期化処理
	void Uninit(void);//ゲームの終了処理
	void Update(void);//ゲームの更新処理
	void Draw(void);//ゲームの描画処理

	static CBg *GetBg(void) { return m_pBg; }//背景のゲット処理
	static CPlayer *GetPlayer(void) { return m_pPlayer; }//プレイヤーのゲット処理
	static bool GetPause(void) { return m_Pause; }//ポーズのゲット処理
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }

private:
	//ポインタのメンバ変数
	static CBg *m_pBg;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CScore *m_pScore;
	static CLife *m_pLife;
	static CGraze *m_pGraze;
	static CPause *m_pPause;
	static CBomb *m_pBomb;
	static CBomb *m_pBomb2;
	static CBomb *m_pBomb3;
	static CUi *m_pUi;
	static CDirecting *m_Directing;

	//カウントメンバ変数
	int m_nCnt;
	int m_nTen;
	int m_nSen;
	int m_nCnt2;
	int m_lastCnt;
	int m_lastCnt2;
	int m_nCnt3;
	int m_nCnt4;
	int m_DirectingCnt;
	int m_bombCnt;

	//ポーズ画面メンバ変数
	static bool m_Pause;

	float m_fx, m_fy;
};
#endif
