//=============================================================================
//
// �e�N�X�`������ [texture.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _TECTURE_H_
#define _TECTURE_H_

//=============================================================================
//�C���N���[�h�t�@�C���̓ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
//�e�N�X�`���N���X�̒�`
//=============================================================================
//�p����`��N���X
class CTexture
{
public:

	//�e�L�X�`���̎�ށi�񋓌^�j
	typedef enum
	{
		TEX_PLAYER = 0,
		TEX_MAX,
	}TEX;

	CTexture(int nPriority = 0);				//�R���X�g���N�^
	~CTexture();								//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);				//����������
	void Uninit(void);														//�I������
	void Update(void);														//�X�V����
	void Draw(void);														//�`�揈��

	HRESULT Load(void);												//�e�N�X�`���ǂݍ���
	void Unload(void);												//�e�N�X�`���j��

	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);						//�A�h���X���̎󂯓n��

private:
	LPDIRECT3DTEXTURE9 m_pTexture;						// �|�C���^�z��i�[�|�C���^�@�擪�A�h���X
};

#endif