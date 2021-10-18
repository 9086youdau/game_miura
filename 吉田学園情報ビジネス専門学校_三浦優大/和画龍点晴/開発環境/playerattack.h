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
//マクロ定義
//*****************************************************************************
#define MAX_SIZE_PLAYER_BULLET (30)//バレットのサイズ

//*****************************************************************************
//マネージャークラスの定義
//*****************************************************************************
class CPlayerAttack : public CScene2D
{
public:
	//列挙型（敵の種類）
	typedef enum
	{
		PLAYERATTACK_LEFT = 0,
		PLAYERATTACK_RIGHT,
		PLAYERATTACK_MAX
	}PLAYERATTACK;
	CPlayerAttack(int nPriority = OBJTYPE_PLAYER_ATTACK);
	~CPlayerAttack();
	static CPlayerAttack *Create(D3DXVECTOR3 pos,PLAYERATTACK playerattack);
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,PLAYERATTACK playerattack);     //初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理
	void PlayerAttackMove(void);								 //移動処理
	void InitPlayerAttackMove(void);							 //初期化の動き
	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード
private:
	static D3DXVECTOR3 m_pos[2];//位置情報
	D3DXVECTOR3 m_rot[2];
	static LPDIRECT3DTEXTURE9 m_pTexture;       //テクスチャ情報   
	D3DXVECTOR3 m_move[2];                   //移動情報
	D3DXVECTOR3 m_vec[2];
	float m_homingAngle[2];
	bool m_Pause;
	bool m_bInit;
	int m_shotcount;
	int m_raise[2];
	PLAYERATTACK m_playerattack;
	bool m_toggle;
};
#endif
