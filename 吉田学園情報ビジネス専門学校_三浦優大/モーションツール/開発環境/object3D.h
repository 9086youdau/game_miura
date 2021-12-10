//=============================================================================
//
// メイン処理 [object3D.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "player3D.h"

//=============================================================================
// オブジェクトクラスの定義
//=============================================================================

//継承先描画クラス
class CObject3D : public CScene
{
public:
	CObject3D(int nPriority = 0, int nType = 0);								//コンストラクタ
	virtual ~CObject3D();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nType);			//生成処理
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	void CObject3D::LoadObject(void);			//テキストの読み込み

	D3DXMATRIX GetMtxWorld(void);

private:
	LPD3DXMESH		m_pMesh;				//メッシュ
	LPD3DXBUFFER	m_pBuffMat;				//バッファ
	DWORD			m_nNumMat;				//マトリックス
	static D3DXMATRIX		m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3		m_pos;					//位置
	D3DXVECTOR3		m_posold;					//位置
	D3DXVECTOR3		m_CollisionVecPos[4];
	D3DXVECTOR3		m_move;					//位置
	D3DXVECTOR3		m_rot;					//位置
	int				m_nNumVtx;				//頂点数
	float			m_fRot;					//
	static char		*m_pTextName;
	D3DXVECTOR3     m_vtx;
	D3DXVECTOR3		m_MaxStone;				//最大値
	D3DXVECTOR3		m_MinStone;				//最小値
	D3DXVECTOR3		m_aPos[18][4];
	D3DXVECTOR3		m_Vec[18][4];
	float			m_fLength;				//対角線の長さ
	float			m_fAngle;				//対角線の角度
	float			m_fLength2;				//対角線の長さ
	D3DXVECTOR3		m_posMaxBase;
	D3DXVECTOR3		m_posMinBase;

	static D3DXVECTOR3	m_rotStatic;
	static D3DXVECTOR3	m_moveStatic;
	static int m_nType;
	float m_ffRot;
	int g_FailCntDateObjectVector;
	bool m_fUseScene3D;
};

#endif