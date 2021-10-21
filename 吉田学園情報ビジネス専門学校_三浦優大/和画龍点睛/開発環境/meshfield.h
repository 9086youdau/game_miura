//*****************************************************************************
//
//メッシュフィールド処理[meshfield.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define WIDTHNUM	(10)		// 列数(幅)
#define DEPTHNUM	(60)		// 行数(奥行)
#define WIDTH		(40.0f)	// 幅の大きさ
#define DEPTH		(40.0f)	// 奥行の大きさ
#define FIELD_MAX	(32)		// フィールドの最大数

//*****************************************************************************
//メッシュフィールドクラスの定義
//*****************************************************************************
class CMeshfield
{
public:
	CMeshfield(); //コンストラクタ
	~CMeshfield();//デストラクタ

	HRESULT InitMeshfield(void);	// 3Dポリゴンの初期化処理
	void UninitMeshfield(void);		// 3Dポリゴンの終了処理
	void UpdateMeshfield(void);		// 3Dポリゴンの更新処理
	void DrawMeshfield(void);		// 3Dポリゴンの描画処理

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshfield = NULL;	// バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshfield = NULL;	// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureMeshfield = NULL;		// テクスチャへのポインタ
	D3DXVECTOR3 m_posMeshfield;							// 位置
	D3DXVECTOR3 m_rotMeshfield;							// 向き
	D3DXMATRIX m_mtxWorldMeshfield;						// ワールドマトリックス
	int m_nAllPoint;									// 総頂点数
	int m_nPolygon;										// ポリゴン数
	int m_nIdxPoint;									// インデックスバッファの必要な確保数
	float m_fWidth = WIDTH;				// 横幅の端
	float m_fDepth = DEPTH;				// 奥行の端
	int m_nWidth = WIDTHNUM + 1;		// 横幅の頂点数
	int m_nDepth = DEPTHNUM + 1;		// 奥行の頂点数
	float m_meshcount = 0.0f;
	float m_fWidthMax = 0.0f;
	float m_fDepthMax = 0.0f;
};
#endif

