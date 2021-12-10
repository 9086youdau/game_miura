//=============================================================================
//
// シーン処理 [scene.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_3D;

//=============================================================================
// 描画クラスの定義
//=============================================================================
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,				// プレイヤー
		OBJTYPE_ENEMY,				// 敵
		OBJTYPE_ENEMY2,				// 中ボス
		OBJTYPE_ENEMY3,				// ボス
		OBJTYPE_BULET_PLAYER,		// プレイヤーから発射された弾
		OBJTYPE_BULET_ENEMY,		// 敵から発射された弾
		OBJTYPE_BULET_ENEMY2,		// 敵から発射された弾
		OBJTYPE_BULET_ENEMY3,		// 敵から発射された弾
		OBJECT_MAX
	}OBJTYPE;

	typedef enum
	{
		ENEMY_01 = 0,
		ENEMY_02,
		ENEMY_03,
		ENEMY_04,
		ENEMY_05,
		ENEMY_06,
		ENEMY_07,
		ENEMY_99,
		ENEMY_BOSS_01,
		ENEMY_BOSS_02,
		ENEMY_BOSS_03,
		ENEMY_MAX
	}ENEMY_TYPE;

	typedef enum
	{
		BULLET_NORMAL = 0,
		BULLET_AIMING,				// 弾のホーミング
		BULLET_MAX
	}BULLET;

	typedef enum
	{
		FRAME = 0,

		BG01,
		BG02,
		BG03,

		TITLE_UI,
		TITLE_UI_PRESS_ENTER,
		TITLE_UI_GAMESTART,
		TITLE_UI_TUTORIAL,
		TITLE_UI_END,

		PLAYER_COLLISION_PINTER,

		PLAYER01,
		PLAYER02,
		PLAYER03,
		PLAYER04,
		PLAYER05,

		SELECT_PLAYER01_STATUS,
		SELECT_PLAYER02_STATUS,
		SELECT_PLAYER03_STATUS,
		SELECT_PLAYER04_STATUS,
		SELECT_PLAYER05_STATUS,

		SELECT_PLAYER01_STATUS2,
		SELECT_PLAYER02_STATUS2,
		SELECT_PLAYER03_STATUS2,
		SELECT_PLAYER04_STATUS2,
		SELECT_PLAYER05_STATUS2,

		TUTORIAL_KEYBOARD,
		TUTORIAL_GAMEPAD,

		GAMECLEAR01,

		GAMEOVER01,
		GAMEOVER_RESTART,
		GAMEOVER_START,
		GAMEOVER_END,

		PAUSE01,
	}UI;

	CScene(int nPriority = 1, int nType = 0);				//コンストラクタ
	virtual ~CScene();										//デストラクタ

	virtual HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY) = 0;	//初期化処理
	virtual void Uninit(void) = 0;											//終了処理
	virtual void Update(void) = 0;											//更新処理
	virtual void Draw(void) = 0;											//描画処理

	static void ReleaseAll(void);											//すべての終了処理
	static void UpdateAll(void);											//すべての更新処理
	static void DrawAll(void);												//すべての描画処理

	static CScene *GetScene(int nIdx, int nPriority);
	
	//ライフ管理
	void SetLife(int nLife);
	void Damage(int nDamage);
	int GetLife(void);

	//タイプ管理
	OBJTYPE GetObjType(void) { return m_objType; }
	void SetObjType(OBJTYPE objType) { m_objType = objType; }

	//サイズ
	float GetSizeX(void) { return m_fSizeX; }
	void SetSizeXScene(float nSize) { m_fSizeX = nSize; }
	float GetSizeY(void) { return m_fSizeY; }
	void SetSizeYScene(float nSize) { m_fSizeY = nSize; }

	//色情報
	D3DXCOLOR GetCollar(void) { return m_col; }
	void SetCollar(D3DXCOLOR col) { m_col = col; }

	//位置管理
	virtual D3DXVECTOR3 GetPosition(void);

protected:
	void Release(void);

private:
	static int		m_NumAll;								//生成番号
	int				m_nID;									//格納先の番号
	OBJTYPE			m_objType;								//オブジェクトの種類
	float			m_fSizeX;								//サイズ
	float			m_fSizeY;								//サイズ
	int				m_nPriority;							// 優先順位
	int				m_nType;								//タイプ
	int				m_nLife;								//ライフ

	//色情報
	D3DXCOLOR		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//リスト化用変数
	static CScene *m_pTop[6][3];	//最前列
	static CScene *m_pCur[6][3];	//最後尾
	CScene *m_pPrev;				//前のポインター
	CScene *m_pNext;				//後のポインター
	bool m_bDeath  = false;			//死亡フラグ
};

#endif
