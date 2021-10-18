//*****************************************************************************
//
//�Q�[������[game.h]
// Author; miura yuudai
//
//*****************************************************************************

#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//�C���N���[�h�ǂݍ���
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CPlayer;
class CBg;
class CEnemy;
class CScore;
class CLife;
class CGraze;
class CPause;
class CUi;
class CBomb;
class CDirecting;

//*****************************************************************************
//�i���o�[�N���X�̒�`
//*****************************************************************************
class CGame : public CScene
{
public:
	CGame();
	~CGame();
	HRESULT Init(D3DXVECTOR3 pos, float fHight, float fWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGame *Create(void);
	static CBg *GetBg(void) { return m_pBg; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }
	static bool GetPause(void) { return m_Pause; }
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	float GetSizeX(void) { return m_fx; }
	float GetSizeY(void) { return m_fy; }
private:
	static CBg *m_pBg;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CScore *m_pScore;
	static CLife *m_pLife;
	static CGraze *m_pGraze;
	static CPause *m_pPause;
	static CBomb *m_pBomb;
	static CBomb *m_pBomb2;
	static CBomb *m_pBomb3;
	static CUi *m_pUi;
	static CDirecting *m_Directing;

	//�J�E���g�����o�ϐ�
	int m_nCnt;
	int m_nTen;
	int m_nSen;
	int m_nCnt2;
	int m_lastCnt;
	int m_lastCnt2;
	int m_nCnt3;
	int m_nCnt4;
	int m_DirectingCnt;
	int m_bombCnt;

	//�|�[�Y��ʃ����o�ϐ�
	static bool m_Pause;
	float m_fx, m_fy;
};
#endif
