//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
// エフェクトクラスの定義
//=============================================================================

//継承先描画クラス
class CEffect : public CScene3D
{
public:
	CEffect(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CEffect();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, int nLife, D3DXCOLOR col, float fHight, float fWidth);;			//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	void SetEffect(D3DXVECTOR3 pos,int nLife, D3DXCOLOR col);        //引数から変数に代入する用の関数

	static CEffect *Create(D3DXVECTOR3 pos,D3DXCOLOR col, int nLife, float fHight, float fWidth);//生成処理

	static void Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャ情報
	D3DXVECTOR3 m_pos;                              //位置情報
	D3DXVECTOR3 m_move;                             //移動量
	int m_nLife;                                    //寿命
	D3DXCOLOR m_col;                                //カラー情報
};

#endif
