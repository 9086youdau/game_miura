//*****************************************************************************
//
//メッシュウォール処理[meshwall.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _MESHFWALL_H_
#define _MESHFWALL_H_

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
//メッシュウォールクラスの定義
//*****************************************************************************
class CMeshwall
{
public:
	CMeshwall(); //コンストラクタ
	~CMeshwall();//デストラクタ
	HRESULT InitMeshwall(void);	// 3Dポリゴンの初期化処理
	void UninitMeshwall(void);		// 3Dポリゴンの終了処理
	void UpdateMeshwall(void);		// 3Dポリゴンの更新処理
	void DrawMeshwall(void);		// 3Dポリゴンの描画処理
	void SetMeshwall(				// メッシュ壁の設定処理
		D3DXVECTOR3 pos,
		D3DXVECTOR3 rot,
		float fWidth,
		float fHeight,
		int nWidth,
		int nHeight);
private:
	// 壁の構造体
	typedef struct
	{
		D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
		D3DXVECTOR3 m_pos;		// 初期位置
		D3DXVECTOR3	m_posMove;	// 初期位置からの移動した位置
		D3DXVECTOR3 m_rot;		// 向き
		float m_fWidth;			// 幅
		float m_fHeight;			// 高さ
		float m_fWidthMax;
		float m_fHeightMax;
		int m_nWidth;				// 列数
		int m_nHeight;			// 行数
		int m_nWidthPoint;		// 幅の頂点数
		int m_nHeightPoint;		// 高さの頂点数
		int m_nAllPoint;
		int m_nPolygon;
		int m_nIdxPoint;
		bool m_bUse;				// 使用状態
	} Meshwall;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshwall = NULL;	// バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshwall = NULL;	// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureMeshwall = NULL;		// テクスチャへのポインタ
	Meshwall m_meshwall[2];
	float anCnt;
};
#endif

