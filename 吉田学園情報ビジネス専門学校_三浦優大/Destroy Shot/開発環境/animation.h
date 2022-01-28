//=============================================================================
//
// アニメーション処理 [animation.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene3D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CPasserby;

//=============================================================================
// アニメーションクラスの定義
//=============================================================================
//継承先描画クラス
class CAnimation : public CScene3D
{
public:
	CAnimation(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CAnimation();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt);               //初期化処理
	void Uninit(void);														         //終了処理
	void Update(void);														         //更新処理
	void Draw(void);														         //描画処理

	static CAnimation *Create(D3DXVECTOR3 pos,float fHight, float fWidth,int nCnt);  //生成処理
	static bool GetColision(void) { return m_bPasserbyColision; }                    //敵の当たり判定フラグ受け渡し用関数
	static CPasserby *GetPasserby(int nCnt) { return m_pPasserby[nCnt]; }            //敵の保存用ポインタ変数の受け渡し用

	static void Load(void);                                                 //テクスチャロード処理
	static void Unload(void);                                               //テクスチャアンロード

private:

	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャ情報
	static CPasserby *m_pPasserby[256];             //敵のポインタ保存用
	D3DXVECTOR3 m_pos;                              //位置情報
	int m_nCounterAnim;                             //アニメーションのカウント用
	int m_nPatternAnim;                             //アニメーションのパターン用
	static bool m_bPasserbyColision;                //当たり判定のフラグ
	int m_nCnt;                                     //敵のポインタの配列用
};

#endif
