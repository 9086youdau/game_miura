//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Miura　Yudai
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//==============================================================================
//インクルードファイル読み込み
//==============================================================================
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_MIGRATION,	// 移動音
	SOUND_LABEL_SE_DECISION,	// 決定音
	SOUND_LABEL_SE_STER,		// 星音
	SOUND_LABEL_SE_METEOR,		// 流星音
	SOUND_LABEL_SE_RECOVER,		// 回復音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class CSound
{
public:
	CSound();								//コンストラクタ
	virtual ~CSound();						//デストラクタ

	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0
		SOUND_LABEL_BGM001,			// BGM1
		SOUND_LABEL_BGM002,			// BGM2
		SOUND_LABEL_BGM003,			// BGM3
		SOUND_LABEL_BGM004,			// BGM4
		SOUND_LABEL_SE_SHOT,		// 弾発射音
		SOUND_LABEL_SE_HIT,			// ヒット音
		SOUND_LABEL_SE_EXPLOSION,	// 爆発音
		SOUND_LABEL_SE_MIGRATION,	// 移動音
		SOUND_LABEL_SE_DECISION,	// 決定音
		SOUND_LABEL_SE_STER,		// 星音
		SOUND_LABEL_SE_METEOR,		// 流星音
		SOUND_LABEL_SE_RECOVER,		// 回復音
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void	Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void	Stop(SOUND_LABEL label);

private:
	//パラメータ構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	HRESULT ChekChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	
	IXAudio2 *m_pXAudio2 = NULL;
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/Previous_Life.wav", -1 },						// BGM0
		{ "data/BGM/警察のテーマ.wav", -1 },						// BGM1
		{ "data/BGM/bgm002.wav", -1 },						// BGM2
		{ "data/BGM/BGM_01_たったそれだけの物語.wav", -1 },	// BGM3
		{ "data/BGM/BGM_02_空間を突きし者.wav", -1 },		// BGM4
		{ "data/SE/ショット.wav", 0 },						// 弾発射音
		{ "data/SE/hit000.wav", 0 },						// ヒット音
		{ "data/SE/game_explosion3.wav", 0 },					// 爆発音
		{ "data/SE/カーソル移動1-_1__1.wav", 0 },			// 移動音
		{ "data/SE/決定、ボタン押下1.wav", 0 },						// 決定音
		{ "data/SE/ster.wav", 0 },							// 星音
		{ "data/SE/meteor.wav", 0 },						// 流星音
		{ "data/SE/recovery.wav", 0 },						// 回復音
	};
};

#endif