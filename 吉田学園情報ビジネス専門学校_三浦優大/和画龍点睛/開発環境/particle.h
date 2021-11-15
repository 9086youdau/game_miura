//*****************************************************************************
//
//パーティクル処理[effect.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "effect.h"

//*****************************************************************************
//パーティクルクラスの定義
//*****************************************************************************
class CParticle : public CEffect
{
public:
	//列挙型（パーティクルの種類）
	typedef enum
	{
		PARTICLE_PLAYER,
		PARTICLE_ENEMY,
		PARTICLE_MAX
	}PARTICLE;

	CParticle();
	~CParticle();
	static CParticle *Create(D3DXVECTOR3 pos,D3DXCOLOR col,PARTICLE particle);

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,D3DXCOLOR col,PARTICLE particle);	 //初期化処理
	void Uninit(void);									 //終了処理
	void Update(void);									 //更新処理
	void Draw(void);									 //描画処理

	void SetParticleMove(void);					 //弾の移動処理

	static HRESULT Load();								 //テクスチャのロード
	static void Unload(void);							 //テクスチャのアンロード

private:
	D3DXVECTOR3 m_move;//弾の移動量
	D3DXVECTOR3 m_pos;//弾の位置
	static LPDIRECT3DTEXTURE9 m_pTexture;				 //共有テクスチャのポインタ
	int m_nLife;//エフェクトの寿命
	float m_fAngle;//角度計算用
};
#endif
