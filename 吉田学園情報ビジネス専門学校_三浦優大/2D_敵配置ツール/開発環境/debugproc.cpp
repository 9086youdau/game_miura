//=============================================================================
//
//デバック表示処理[debugproc.cpp]
// Author; Miura　Yudai
//
//=============================================================================

//=============================================================================
//インクルードファイル読み込み
//=============================================================================
#include "debugproc.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
LPD3DXFONT CDebugProc::m_pFont = NULL; //フォント用変数
char CDebugProc::m_aStr[MAX_CHAR] = {};//文字格納用変

//=============================================================================
//コンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{
}

//=============================================================================
//デバックの初期化処理
//=============================================================================
void CDebugProc::Init(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

//#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
//#endif

	//バッファのクリア
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}

//=============================================================================
//デバックの終了処理
//=============================================================================
void CDebugProc::Uninit(void)
{
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
//デバックの可変引数処理
//=============================================================================
void CDebugProc::Print(const char * fmt, ...)
{
	char aStar[MAX_CHAR] = {};
	int nCnt = 0;

	va_list ap;

	va_start(ap, fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			//カウントアップ
			fmt++;
			switch (*fmt)
			{
			case 's'://Sの場合
				nCnt += wsprintf(&aStar[nCnt], "%s", va_arg(ap, char *));
				break;
			case 'd'://dの場合
				nCnt += wsprintf(&aStar[nCnt], "%d", va_arg(ap, int));
				break;
			case 'c'://cの場合
				nCnt += wsprintf(&aStar[nCnt], "%c", va_arg(ap, char));
				break;
			case 'f'://fの場合
				nCnt += sprintf(&aStar[nCnt], "%f", va_arg(ap, double));
				break;
			default:
				break;
			}
		}
		else
		{
			nCnt += wsprintf(&aStar[nCnt], "%c", *fmt);
		}

		//カウントアップ
		fmt++;
	}

	//終了
	va_end(ap);

	//テキストの描画
	strcat(m_aStr, aStar);
}

//=============================================================================
//デバックの描画処理
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// テキスト描画
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 255, 255, 15));

	//バッファのクリア
	memset(&m_aStr[0], 0, sizeof(m_aStr));
}
