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
		OBJTYPE_ENEMY_BOSS,
		OBJTYPE_ENEMY_BULLET,
		OBJTYPE_PLAYER_BULLET,
		OBJTYPE_PLAYER_ATTACK,
		OBJTYPE_BG,
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
	void SetDamage(int nDamage);
	virtual D3DXVECTOR3 GetPosition(void) = 0;
	virtual float GetSizeX(void) = 0;
	virtual float GetSizeY(void) = 0;

private:
	static CScene *m_apScene[OBJTYPE_MAX][LOOP_POLYGON];//シーンの全ての情報
	static int m_nNumAll;                               //カウント用メンバ変数
	int m_nID;                                          //格納先の番号
	OBJTYPE m_objType;                                  //オブジェクトの種類
	static int m_nIndex;                                //番号の順番
	int m_nPriority;									//優先順番の番号
};
#endif
