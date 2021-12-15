//*****************************************************************************
//
//シーン2D処理[Scene2D.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;
//*****************************************************************************
//シーン2Dクラスの定義
//*****************************************************************************
class CScene2D : public CScene 
{
public:
	CScene2D(int nPriority = OBJTYPE_ENEMY_BOSS); //コンストラクタ
	~CScene2D();//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos,float fHight,float fWidth);       //初期化処理
	void Uninit(void);                       //終了処理
	void Update(void);						 //更新処理
	void Draw(void);                         //描画処理

	bool Collision(D3DXVECTOR3 pos, float fsizeX, float fsizeY, OBJTYPE Type, CScene **pColScene);

	void SetPosition(D3DXVECTOR3 pos,D3DXVECTOR3 rot); //位置情報用関数
	D3DXVECTOR3 GetPosition(void) { return m_pos; }    //位置情報の受け渡し
	D3DXVECTOR3 GetAllPosition(void) { return m_pos; } //全ての位置情報の受け渡し
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);     //テクスチャ情報
	void SetTex(float tex, int nCnt);                  //テクスチャの設定
	void SetBg(float size, float nCnt);                //背景の設定
	void SetCol(D3DXCOLOR col);                        //色の設定
	D3DXCOLOR SetEnemyCol(D3DXCOLOR col);
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };

private:
	LPDIRECT3DTEXTURE9 m_pTexture;		 //テクスチャ情報
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  //頂点情報
	D3DXVECTOR3 m_pos;                   //位置情報
	D3DXVECTOR3 m_Rot;                   //回転情報
	D3DXCOLOR m_col;                     //カラー情報
	float  m_fLength;                    //線の長さ
	float m_fAngle;                      //角度保存用
	float m_fx, m_fy;

	//フィードバックエフェクト用メンバ変数
	LPDIRECT3DTEXTURE9 m_apTextureMT[2];
	LPDIRECT3DSURFACE9 m_apRendererMT[2];
	LPDIRECT3DSURFACE9 m_apBuffMT[2];
	D3DVIEWPORT9 m_viewportMT;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;
};
#endif