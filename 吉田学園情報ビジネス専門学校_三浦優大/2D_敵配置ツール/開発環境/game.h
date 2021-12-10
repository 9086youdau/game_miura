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
class CPause;

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define TEXT_LENGTH (128)
#define DATA_LENGTH (64)
#define MAX_DATA (224)

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
	void LoadText(void);//テキストファイルから敵の動きを読み込み
	void EnemyTool(void);//ツール操作用関数

	static CBg *GetBg(void) { return m_pBg; }//背景のゲット処理
	static CPlayer *GetPlayer(void) { return m_pPlayer; }//プレイヤーのゲット処理
	static bool GetPause(void) { return m_Pause; }//ポーズのゲット処理
	static int GetEnemyCount(void) { return m_nCnt; } //エネミーカウント
	static int GetEnemySubCount(void) { return m_nSubCnt; } //サブのエネミーカウント
	static int GetEnemyMaxCount(void) { return m_nStartCount[m_nCountText - 1]; }//エネミーカウントの最大数
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }

private:
	//ポインタのメンバ変数
	static CBg *m_pBg;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CPause *m_pPause;

	//カウントメンバ変数
	static int m_nCnt;
	int m_nInterval[MAX_DATA];

	//テキスト読み取り用変数
	static int m_nCountText;
	static int m_nStartCount[MAX_DATA];
	D3DXVECTOR3 m_Pos[MAX_DATA];
	int m_EnemyType[MAX_DATA];
	D3DXVECTOR3 m_Size[MAX_DATA];
	float m_fEnemySpeed[MAX_DATA];
	int   m_nHomingCount[MAX_DATA];

	//ポーズ画面メンバ変数
	static bool m_Pause;

	//ツール用メンバ変数
	bool m_bCount;
	static int m_nSubCnt;

	float m_fx, m_fy;
};
#endif
