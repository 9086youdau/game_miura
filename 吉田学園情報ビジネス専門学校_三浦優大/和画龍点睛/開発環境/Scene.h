//*****************************************************************************
//
//シーン処理[Scene.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//シーンクラスの定義
//*****************************************************************************
class CScene
{
public:
	//列挙型(オブジェクトの数、優先順位)
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_ENEMY_BULLET,
		OBJTYPE_ENEMY_BOSS,
		OBJTYPE_PLAYER_BULLET,
		OBJTYPE_PLAYER_ATTACK,
		OBJTYPE_EFFECT,
		OBJTYPE_SIRCLEHP,
		OBJTYPE_BG,
		OBJTYPE_UI,
		OBJTYPE_SCORE,
		OBJTYPE_GRAZE,
		OBJTYPE_LIFE,
		OBJTYPE_BOMB,
		OBJTYPE_DIRECTING,
		OBJTYPE_FADE,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = OBJTYPE_BG);                                      //コンストラクタ
	virtual ~CScene();                                                      //デストラクタ

	virtual HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth) = 0;         //初期化処理
	virtual void Uninit(void) = 0;                                     //終了処理
	virtual void Update(void) = 0;									   //更新処理
	virtual void Draw(void) = 0;                                       //描画処理

	static void ReleaseAll(void);                                           //全ての廃棄処理
	static void UpdateAll(void);                                            //全ての更新処理
	static void DrawAll(void);												//全ての描画処理
	void Release(void);														//一つだけの破棄

	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void) { return m_objType; }
	static CScene *GetScene(int nPriority,int nIndex);
	int GetDamage(void) { return m_nLife; }
	void SetDamage(int nDamage);
	bool GetboolState(void) { return m_state; }
	virtual D3DXVECTOR3 GetPosition(void) = 0;
	virtual float GetSizeX(void) = 0;
	virtual float GetSizeY(void) = 0;

private:
	static CScene *m_apScene[OBJTYPE_MAX][LOOP_POLYGON];
	static int m_nNumAll;
	int m_nID;//格納先の番号
	OBJTYPE m_objType;//オブジェクトの種類
	int m_nLife;
	static int m_statecount;//状態のカウント
	static bool m_state;
	static int m_nIndex;
	int m_nPriority;									//優先順番の番号
};
#endif
