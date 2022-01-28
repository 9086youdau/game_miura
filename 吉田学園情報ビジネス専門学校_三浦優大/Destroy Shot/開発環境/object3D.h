//=============================================================================
//
// メイン処理 [object3D.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "sceneX.h"

//=============================================================================
// オブジェクトクラスの定義
//=============================================================================

//継承先描画クラス
class CObject3D : public CSceneX
{
public:
	CObject3D(int nPriority = 0, int nType = 0);						    //コンストラクタ
	virtual ~CObject3D();												    //デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char * TextName, char * TextureName);             //生成処理

	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);//当たり判定

private:
	char *m_TextName;                                                //Xデータの読み込み
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_MaxObject;
	D3DXVECTOR3 m_MinObject;

	//当たり判定用メンバ変数
	D3DXVECTOR3 m_aPosColision[4];
	D3DXVECTOR3 m_aVec[4];
	D3DXVECTOR3 m_bVec[4];
	float m_fPlayerVec[4];
	
};

#endif