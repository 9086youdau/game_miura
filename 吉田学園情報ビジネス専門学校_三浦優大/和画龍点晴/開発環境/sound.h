//*****************************************************************************
//
//�T�E���h����[sound.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//�o���b�g�N���X�̒�`
//*****************************************************************************
class CSound
{
public:
	CSound();//�R���X�g���N�^
	~CSound();//�f�X�g���N�^
	//�T�E���h�t�@�C��
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

	HRESULT Init(HWND hWnd);//����������
	void Uninit(void);//�I������
	HRESULT Play(SOUND_LABEL label);//��������
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	//�p�����[�^�[�\����
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	}SOUNDPARAM;

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
																
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];				// �e���f�ނ̃p�����[�^
};
#endif
		
