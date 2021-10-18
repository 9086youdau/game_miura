//*****************************************************************************
//
//プレイヤー処理[player.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_SIZE_PLAYER_BULLET (30)//バレットのサイズ

//*****************************************************************************
//マネージャークラスの定義
//*****************************************************************************
class CPlayer : public CScene2D
{
public:
	//列挙型（敵の種類）
	typedef enum
	{
		PLAYERSTATE_NOMAL = 0,
		PLAYERSTATE_DAMAGE,
		PLAYERSTATE_MAX
	}PLAYERSTATE;
	CPlayer(int nPriority = OBJTYPE_PLAYER);
	~CPlayer();
	static CPlayer *Create(D3DXVECTOR3 pos);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);     //初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理
	void PlayerMove(void);								 //移動処理
	void InitPlayerMove(void);							 //初期化の動き
	static void SetStatePlayer(PLAYERSTATE playerstate); //プレイヤーの状態変化
	void PlayerCol(void);                                //色変更処理
	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード
	static D3DXVECTOR3 GetPlayer(void) { return m_pos; }
	static D3DXVECTOR3 GetOldPlayer(void) { return m_oldpos; }
	static PLAYERSTATE GetState(void) { return m_playerstate; }
	static bool GetPlayerAttack(void) { return m_playerattack; }
private:
	static D3DXVECTOR3 m_pos;//位置情報
	static D3DXVECTOR3 m_oldpos;//昔の位置情報
	static LPDIRECT3DTEXTURE9 m_pTexture;       //テクスチャ情報   
	D3DXVECTOR3 m_move;                   //移動情報
	float  m_DiagonalMove;                //斜め移動用
	float m_Counter;
	bool m_Pause;
	bool m_bInit;
	static PLAYERSTATE m_playerstate;
	static int m_statecount;
	int m_shotcount;
	int m_nLifeCount;
	static bool m_playerattack;
};
#endif
