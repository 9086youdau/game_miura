//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

//=============================================================================
//インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "manager.h"

//=============================================================================
// カメラクラスの定義
//=============================================================================
//継承先描画クラス
class CCamera
{
public:
	CCamera();								// コンストラクタ
	virtual ~CCamera();						// デストラクタ

	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理

	void SetCamera(void);					// 生成処理
	void Mouseoperation(void);				// マウス操作処理


	static D3DXVECTOR3 GetRot(void) { return m_rot; }		// 向き取得用
	static D3DXVECTOR3 GetposV(void) { return m_posV; }		// 視点取得
	static D3DXVECTOR3 GetposR(void) { return m_posR; }		// 注視点取得

private:

	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		// ビューマトリックス	
	D3DXVECTOR3 m_posVDest;		// 目的の視点	
	D3DXVECTOR3 m_posRDest;		// 目的の注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	int m_Rpos;					// モデルの前
	float m_fDistance;			// カメラとの距離
	float m_fMouseSpeed;		// カメラ回転速度（マウス操作）
	POINT m_Mousepoint;			// マウス座標
	float m_fhorizontalAngle;	// 水平
	float m_fverticalAngle;		// 垂直

	static D3DXVECTOR3 m_posV;	// 現在の視点
	static D3DXVECTOR3 m_posR;	// 現在の注視点
	static D3DXVECTOR3 m_rot;	// カメラの角度
};

#endif