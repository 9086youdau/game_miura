//=============================================================================
//
// モデル処理 [model.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// モデルクラスの定義
//=============================================================================;
//継承先描画クラス
class CModel
{
public:
	CModel(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CModel();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);		//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName);								//生成処理

	void MotionPos(D3DXVECTOR3 pos);
	void MotionRot(D3DXVECTOR3 rot);

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);//テクスチャ情報を貰ってくる関数
	void SetMaterial(D3DXMATERIAL *pMat);//マテリアル情報を持ってくる関数

	void SetParent(CModel *pModel);
	D3DXMATRIX GetMtxWorld(void);
	LPD3DXMESH GetMash(void) { return m_pMesh; }
	LPD3DXBUFFER GetBuff(void) { return m_pBuffMat; }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATERIAL GetMat(void) { return *m_pMat; }

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		//テクスチャのポインタ
	LPD3DXMESH		m_pMesh;			//メッシュ
	LPD3DXBUFFER	m_pBuffMat;			//バッファ
	DWORD           m_nNumMat;			//マトリックス
	D3DXMATRIX		m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_rot;				//向き
	int				m_nNumVtx;			//頂点数
	float			m_fRot;				//
	CModel			*m_pParent;			//
	static D3DXMATERIAL    *m_pMat;	    //マテリアルへのポインタ

};

#endif