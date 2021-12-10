//==============================================================================
//
// シーン3D処理[Scene3D.h]
// Author; Miura　Yudai
//
//==============================================================================

#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//==============================================================================
// マクロ定義
//==============================================================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//インクルード読み込み
//==============================================================================
#include "main.h"
#include "Scene.h"

//==============================================================================
//シーン3Dクラスの定義
//==============================================================================
class CScene3D : public CScene
{
public:
	CScene3D(); //コンストラクタ
	~CScene3D();//デストラクタ
	
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);       //初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理
	
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	static CScene3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY);
	static bool GetBool(void) { return m_bCount; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  //頂点情報
	LPDIRECT3DTEXTURE9 m_pTexture;		 //テクスチャ情報
	D3DXMATRIX m_mtxWorld;               //ワールドマトリックス
	D3DXVECTOR3 m_pos;                   //位置情報
	D3DXVECTOR3 m_Rot;                   //回転情報
	D3DXCOLOR m_col;                     //カラー設定
	static D3DXVECTOR3 size[4];
	int m_nLife;                         //可視化の寿命
	static bool m_bCount;
};
#endif
