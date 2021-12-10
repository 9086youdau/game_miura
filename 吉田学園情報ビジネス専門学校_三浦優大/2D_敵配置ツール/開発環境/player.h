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
//プレイヤークラスの定義
//*****************************************************************************
class CPlayer : public CScene2D
{
public:

	CPlayer(int nPriority = OBJTYPE_PLAYER);
	~CPlayer();
	static CPlayer *Create(D3DXVECTOR3 pos);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);     //初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理

	void PlayerMove(void);								 //移動処理
	void InitPlayerMove(void);							 //初期化の動き

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

	static D3DXVECTOR3 GetPlayer(void) { return m_pos; }
	static D3DXVECTOR3 GetOldPlayer(void) { return m_oldpos; }
	static bool GetPlayerAttack(void) { return m_playerattack; }

private:
	static D3DXVECTOR3 m_pos;              //位置情報
	static D3DXVECTOR3 m_oldpos;           //昔の位置情報
	static LPDIRECT3DTEXTURE9 m_pTexture;  //テクスチャ情報   
	D3DXVECTOR3 m_move;                    //移動情報
	float  m_DiagonalMove;                 //斜め移動用
	float m_Counter;                       //カウント保存用
	bool m_Pause;                          //ポーズ情報
	bool m_bInit;                          //管理情報
	static bool m_playerattack;            //プレイヤーアタックの情報
	D3DXVECTOR3 m_Rot;                     //回転情報
	float  m_fLength;                      //線の長さ
	float m_fAngle;                        //角度保存用
};
#endif
