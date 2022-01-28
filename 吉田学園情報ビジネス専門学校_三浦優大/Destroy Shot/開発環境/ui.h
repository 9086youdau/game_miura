//=============================================================================
//
// ui処理 [ui.h]
// Author : Miura　Yudai
//
//=============================================================================

#ifndef _UI_H_
#define _UI_H_

//=============================================================================
// インクルードファイル読み込み
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CSound;

//=============================================================================
// uiクラスの定義
//=============================================================================

//継承先描画クラス
class CUi : public CScene2D
{
public:
	//uiの詳細情報
	typedef enum
	{
		UITYPE_FREAM = 0,
		UITYPE_BG,
		UITYPE_START,
		UITYPE_TUTORIAL,
		UITYPE_EXIT,
		UITYPE_POINTER,
		UITYPE_GAMEMESSAGE,
		UITYPE_RESULTMENU,
		UITYPE_ENEMYREMAINING,
		UITYPE_RESULTHP,
		UITYPE_RESULTADD,
		UITYPE_RESULTEQUAL,
		RANKING_FIRST,
		RANKING_SECOND,
		RANKING_THIRD,
		RANKING_FOUR,
		RANKING_FIVE,
		TITLE_LOGO,
		TUTORIAL_MENU,
		PAUSE_FASE,
		PAUSE_MENU,
		PAUSE_TITLE,
		PAUSE_RESTART,
		PAUSE_FREAM,
		UITYPE_MAX
	}UITYPE;
	CUi(int nPriority = 1, int nType = 0);								//コンストラクタ
	virtual ~CUi();														//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth,int nType);			//初期化処理
	void Uninit(void);														//終了処理
	void Update(void);														//更新処理
	void Draw(void);														//描画処理

	static void Load(void);
	static void Unload(void);

	void TitleUi(void);//タイトルのUI設定
	static void MoveUi(D3DXVECTOR3 pos, int nType);

	static CUi *Create(D3DXVECTOR3 pos, float fHight, float fWidth, int nType);//生成処理

	static int GetPointCount(void) { return m_PointerCount; }

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[UITYPE_MAX];			//テクスチャ情報
	D3DXVECTOR3 m_pos;                                          //位置情報
	static D3DXVECTOR3 m_posPause;                              //位置情報
	D3DXVECTOR3 m_firstpos;                                     //最初の位置
	D3DXVECTOR3 m_finalpos;                                     //最後の位置
	D3DXVECTOR3 m_move;                                         //移動量
	int m_nType;                                                //uiのタイプ
	static int m_nPauseType;                                    //uiのタイプ
	static int m_PointerCount;                                  //タイトルのポインタの処理用
	int m_BlinkingCount;                                        //点滅のカウント用
	CSound		*m_pSound;			          					//サウンドポインタ
};
//=============================================================================
// uiクラスの定義(Pause)
//=============================================================================
class CUiPause : public CUi
{
public:
	CUiPause(int nPriority = 0, int nType = 2);				//コンストラクタ
	~CUiPause();												//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);				//初期化処理
	void Uninit(void);																	//終了処理
	void Update(void);																	//更新処理
	void Draw(void);																	//描画処理

	static void Unload(void);                                                            //テクスチャのアンロード

	void PauseUi(void);//ポーズのUI設定

	static CUiPause *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType);	//生成処理
};

#endif

