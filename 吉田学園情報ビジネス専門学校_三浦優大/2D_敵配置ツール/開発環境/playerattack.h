//*****************************************************************************
//
//プレイヤーアタック処理[playerattack.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PLAYERATTACK_H_
#define _PLAYERATTACK_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//プレイヤーアタッククラスの定義
//*****************************************************************************
class CPlayerAttack : public CScene2D
{
public:
	//列挙型（プレイヤーアタックの左右）
	typedef enum
	{
		PLAYERATTACK_LEFT = 0,
		PLAYERATTACK_RIGHT,
		PLAYERATTACK_MAX
	}PLAYERATTACK;

	CPlayerAttack(int nPriority = OBJTYPE_PLAYER_ATTACK); //コンストラクタ
	~CPlayerAttack();                                     //デストラクタ
	static CPlayerAttack *Create(D3DXVECTOR3 pos,PLAYERATTACK playerattack);             //生成処理

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,PLAYERATTACK playerattack); //初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理

	void PlayerAttackMove(void);						 //移動処理
	void InitPlayerAttackMove(void);					 //初期化の動き

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

private:
	static D3DXVECTOR3 m_pos[2];               //位置情報
	D3DXVECTOR3 m_rot[2];                      //角度情報
	static LPDIRECT3DTEXTURE9 m_pTexture;      //テクスチャ情報   
	D3DXVECTOR3 m_move[2];                     //移動情報
	D3DXVECTOR3 m_vec[2];                      //ベクトル情報
	float m_homingAngle[2];                    //角度計算情報
	bool m_Pause;                              //ポーズ情報
	int m_raise[2];                            //角度計算
	PLAYERATTACK m_playerattack;               //プレイヤーアタックの情報       
	bool m_toggle;                             //使われているかいないか
	int m_shotcount;                           //弾の間隔カウント
};
#endif
