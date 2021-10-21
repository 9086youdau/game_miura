//*****************************************************************************
//
//円のHP処理[CircleHp.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _CIRCLEHP_H_
#define _CIRCLEHP_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene2D.h"

//*****************************************************************************
//円のHPの定義
//*****************************************************************************
class CCircleHp : public CScene2D
{
public:
	//列挙型（敵の種類）
	typedef enum
	{
		CIRCLEHP_OUT = 0,
		CIRCLEHP_IN,
		ENEMYTYPE_MAX
	}CIRCLEHP;

	CCircleHp(int nPriority = OBJTYPE_SIRCLEHP);//コンストラクタ
	~CCircleHp();//デストラクタ
	static CCircleHp *Create(D3DXVECTOR3 pos, float fHight, float fWidth, CIRCLEHP CircleHp);//サークルHPの作成処理

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,CIRCLEHP CircleHp);//サークルHPの初期化処理
	void Uninit(void);//サークルHPの終了処理
	void Update(void);//サークルHPの更新処理
	void Draw(void);//サークルHPの描画処理

	static HRESULT Load(void);//テクスチャロード
	static void Unload(void);//テクスチャアンロード

	D3DXVECTOR3 GetAllPosition(void) { return D3DXVECTOR3(); }
	static void SetBossLife(int nDamage);//ボスのライフのセット関数
	static int GetBossLife(void) { return m_sirclelife; }//ボスのライフのゲット関数

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2];//テクスチャのメンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//頂点情報のメンバ変数
	D3DXVECTOR3 m_pos;//位置情報
	static int m_sirclelife;//HPのライフ処理
	D3DXVECTOR3 m_bosspos;//ボスの位置処理
	CIRCLEHP m_CircleHp;//HPのタイプ変数
};
#endif
