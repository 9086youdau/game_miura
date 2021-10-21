//*****************************************************************************
//
//シーンX処理[SceneX.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//シーンXクラスの定義
//*****************************************************************************
class CSceneX : public CScene
{
public:
	//列挙型（モデルの種類）
	typedef enum
	{
		SCENEXTYPE_NONE = 0,
		SCENEXTYPE_RIGHT,
		SCENEXTYPE_LEFT,
		SCENEXTYPE_MAX
	}SCENEXTYPE;

	CSceneX(); //コンストラクタ
	~CSceneX();//デストラクタ
	static CSceneX *Create(D3DXVECTOR3 pos,SCENEXTYPE scenextype);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);       //初期化処理
	void Uninit(void);                       //終了処理
	void Update(void);						 //更新処理
	void Draw(void);                         //描画処理

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };

private:
	LPD3DXMESH m_pMesh;// メッシュへのポインタ
	LPD3DXBUFFER m_pBuffMat;// マテリアルへのポインタ
	DWORD m_nNumMat;// マテリアルの数
	D3DXMATRIX m_mtxWorld;// ワールドマトリックス
	D3DXVECTOR3 m_rot;//回転処理
	D3DXVECTOR3 m_pos;//位置処理
	float m_fx, m_fy;
	SCENEXTYPE m_scenetype;//モデルのタイプ
};
#endif
