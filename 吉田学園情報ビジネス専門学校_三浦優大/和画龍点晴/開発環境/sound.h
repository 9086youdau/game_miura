//*****************************************************************************
//
//サウンド処理[sound.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//インクルード読み込み
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//バレットクラスの定義
//*****************************************************************************
class CSound
{
public:
	CSound();//コンストラクタ
	~CSound();//デストラクタ
	//サウンドファイル
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0
		SOUND_LABEL_BGM001,			// BGM1
		SOUND_LABEL_BGM002,			// BGM2
		SOUND_SHOT_SE000,			// SE1
		SOUND_RETURN_SE001,			// SE2
		SOUND_SELECT_SE002,			// SE3
		SOUND_COLLISION_SE003,			// SE3
		SOUND_LABEL_MAX,
	}SOUND_LABEL;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	HRESULT Init(HWND hWnd);//初期化処理
	void Uninit(void);//終了処理
	HRESULT Play(SOUND_LABEL label);//音声処理
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	//パラメーター構造体
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	}SOUNDPARAM;

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
																
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];				// 各音素材のパラメータ
};
#endif
		
