//*****************************************************************************
//
// エフェクト処理[effect.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//エフェクトクラスの定義
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	//列挙型（エフェクトの種類）
	typedef enum
	{
		EFFECTTYPE_NONE = 0,
		EFFECTTYPE_PLAYER_MOVE,
		EFFECTTYPE_BOSS_END,
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	CEffect(int nPriority = OBJTYPE_EFFECT);//コンストラクタ
	~CEffect();//デストラクタ
	static CEffect *Create(D3DXVECTOR3 pos,float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,D3DXCOLOR col,EFFECTTYPE effect);//初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理

	void SetEffectMove(void);					        //エフェクトの移動処理

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

private:
	D3DXVECTOR3 m_move;//弾の移動量
	D3DXVECTOR3 m_pos;//弾の位置
	static LPDIRECT3DTEXTURE9 m_pTexture;				 //共有テクスチャのポインタ
	int m_nLife;//エフェクトの寿命
	float m_fAngle;//角度保存用
	EFFECTTYPE m_effecttype;//エフェクト保存用
};
#endif
