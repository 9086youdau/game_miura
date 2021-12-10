//=============================================================================
//
// シーンX処理 [sceneX.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _SCENEX_H_
#define _SCENEX_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// インクルードファイルの読み込み
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// シーンXクラスの定義
//=============================================================================
//継承先描画クラス
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority = 5, int nType = 0);								//コンストラクタ
	virtual ~CSceneX();					//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);		//初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	static CSceneX *Create();										//生成処理
																	
private:															
	LPD3DXMESH		m_pMesh;			//メッシュ
	LPD3DXBUFFER	m_pBuffMat;			//バッファ
	DWORD			m_nNumMat;			//マトリックス
	D3DXMATRIX		m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_rot;				//向き
	int				m_nNumVtx;			//頂点数
	float			m_fRot;

};

#endif