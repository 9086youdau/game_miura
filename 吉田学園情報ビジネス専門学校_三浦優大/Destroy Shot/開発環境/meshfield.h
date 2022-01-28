//=============================================================================
//
//メッシュフィールド処理[meshfield.h]
// Author; Miura　Yudai
//
//=============================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
//インクルード読み込み
//=============================================================================
#include "main.h"
#include "Scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define WIDTHNUM	(40)		// 列数(幅)
#define DEPTHNUM	(35)		// 行数(奥行)
#define WIDTH		(100.0f)	// 幅の大きさ
#define DEPTH		(100.0f)	// 奥行の大きさ
#define FIELD_MAX	(1)	// フィールドの最大数

//=============================================================================
//シーン3Dクラスの定義
//=============================================================================
class CMeshfield : public CScene
{
public:
	CMeshfield(); //コンストラクタ
	~CMeshfield();//デストラクタ

	static CMeshfield *Create(D3DXVECTOR3 pos,float fHight, float fWidth);

	HRESULT Init(D3DXVECTOR3 pos,float fHight, float fWidth);       //初期化処理
	void Uninit(void);												//終了処理
	void Update(void);												//更新処理
	void Draw(void);												//描画処理

	static void Load(void);
	static void Unload(void);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	// インデックスバッファへのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nAllPoint;							// 総頂点数
	int m_nPolygon;								// ポリゴン数
	int m_nIdxPoint;							// インデックスバッファの必要な確保数
	float m_fWidth = WIDTH;				// 横幅の端
	float m_fDepth = DEPTH;				// 奥行の端
	int m_nWidth = WIDTHNUM + 1;		// 横幅の頂点数
	int m_nDepth = DEPTHNUM + 1;		// 奥行の頂点数
};
#endif
