//*****************************************************************************
//
//　フェード処理[fade.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"
#include "Scene.h"
#include "manager.h"

//*****************************************************************************
//フェードクラスの定義
//*****************************************************************************
class CFade : public CScene
{
public:
	//フェードの状態
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	}FADE;

	CFade(int nPriority = OBJTYPE_FADE); //コンストラクタ
	~CFade();//デストラクタ
	static CFade *Create(CManager::MODE modenext);//フェードのクリエイト

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);  //初期化処理
	void Uninit(void);                       //終了処理
	void Update(void);						 //更新処理
	void Draw(void);                         //描画処理

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); };
	float GetSizeX(void) { return m_fx; };
	float GetSizeY(void) { return m_fy; };
	static void SetFade(FADE fade, CManager::MODE modeNext);//フェードのセット処理
	FADE GetFade(void) { return m_fade; }//フェードのゲット処理

private:
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;  //頂点情報
	static FADE m_fade;                         //フェードの状態
	static CManager::MODE m_modenext;           //次のモード
	D3DXCOLOR m_col;                            //頂点カラー
	D3DXVECTOR3 m_pos;                          //位置
	float m_fx, m_fy;
};
#endif
