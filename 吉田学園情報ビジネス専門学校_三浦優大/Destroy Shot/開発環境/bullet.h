//=============================================================================
//
// バレット処理 [bullet.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
// バレットクラスの定義
//=============================================================================
//継承先描画クラス
class CBullet: public CScene3D
{
public:
	CBullet(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CBullet();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, float fHight, float fWidth);		//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife);   //生成処理

	void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife);           //引数から代入用

	static void Load(void);                                                 //テクスチャロード用
	static void Unload(void);                                               //テクスチャアンロード

	static D3DXVECTOR3 GetPos(void) { return m_getpos; }                    //位置情報受け渡し用

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャ情報
	D3DXVECTOR3 m_pos;                              //位置保存
	static D3DXVECTOR3 m_getpos;                    //受け渡し用位置
	D3DXVECTOR3 m_posold;                           //昔の位置保存
	D3DXVECTOR3 m_move;                             //移動量保存
	int m_nLife;                                    //寿命保存用
};

#endif

