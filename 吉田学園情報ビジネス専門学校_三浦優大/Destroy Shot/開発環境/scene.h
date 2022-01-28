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
		OBJTYPE_BULLET,				// 中ボス
		OBJTYPE_PARTICLE,				// 中ボス
		OBJTYPE_ENEMY3,				// ボス
		OBJTYPE_BULET_PLAYER,		// プレイヤーから発射された弾
		OBJECT_MAX
	}OBJTYPE;

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
	static CScene *m_pTop[OBJECT_MAX][3];	//最前列
	static CScene *m_pCur[OBJECT_MAX][3];	//最後尾
	CScene *m_pPrev;				//前のポインター
	CScene *m_pNext;				//後のポインター
	bool m_bDeath  = false;			//死亡フラグ
};

#endif
