//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "passerby.h"

//=============================================================================
// �O���錾
//=============================================================================
class CObject3D;
class CPlayer3D;
class CHp;
class CSound;

//=============================================================================
//�}�N����`
//=============================================================================
#define TEXT_LENGTH (128)
#define DATA_GAME_LENGTH (248)
#define MAX_DATA (224)

//=============================================================================
// �Q�[����ʃN���X�̒�`
//=============================================================================
//�p����`��N���X
class CGame : public CScene
{
public:
	CGame();									//�R���X�g���N�^
	~CGame();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��

	static CObject3D *GetObject(int nCnt) { return m_pObject3D[nCnt]; }
	static CPlayer3D *GetPlayer(void) { return m_pPlayer3D; }
	static CHp *GetHp(int nCnt) { return m_pHp[nCnt]; }

	void LoadEnemy(void);                                       //�G�̃e�L�X�g�ǂݍ���
	void LoadObject(void);                                      //�I�u�W�F�N�g�̈ʒu�ǂݍ���
	void LoadParticle(void);                                    //�p�[�e�B�N������ǂݍ���

	//��������
	static CGame *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }
	static int GetEnemyPurpose(void) { return m_nCountEnemyText; }
	static void SetCount(int nCnt);
	
	static void GameParticle(D3DXVECTOR3 pos,int nType);
	static int GetLife(void) { return m_nHpCount; }
	static void SetLife(int nLife);

private:
	LPD3DXFONT	m_pTimeFontGame;					//�\���p�t�H���g
	static CObject3D *m_pObject3D[256];				//����Get
	static CPlayer3D *m_pPlayer3D;				    //�v���C���[�̃|�C���^���
	static CHp *m_pHp[4];                           //HP�̃|�C���^���
	static float m_fAngle;                          //�p�[�e�B�N���̊p�x�ύX
	static float m_fWidth;                          //�p�[�e�B�N���̉���
	static float m_fHight;                          //�p�[�e�B�N���̏c��
	static float m_fParicleMove[DATA_GAME_LENGTH];       //�p�[�e�B�N���̑���
	static D3DXCOLOR m_col[DATA_GAME_LENGTH];            //�p�[�e�B�N���̐F
	int m_nLife[DATA_GAME_LENGTH];                       //�p�[�e�B�N���̎���
	static int m_nCnt;                              //�G�l�~�[�̃J�E���g�ϐ�

	//�G�̔z�u�p�e�L�X�g�����o�ϐ�
	static int m_nCountEnemyText;                        //���ǂݍ��񂾂��̃J�E���g
	int m_nStartEnemyText[DATA_GAME_LENGTH];                  //���o�������邩�̃J�E���g
	D3DXVECTOR3 m_EnemyPos[DATA_GAME_LENGTH];                 //�G�̈ʒu���
	D3DXVECTOR3 m_EnemySize[DATA_GAME_LENGTH];                //�G�̃T�C�Y���

	//�I�u�W�F�N�g�z�u�p�����o�ϐ�
	int m_nCountObjectText;                         //���ǂݍ��񂾂��̃J�E���g
	D3DXVECTOR3 m_ObjectPos[DATA_GAME_LENGTH];           //�I�u�W�F�N�g�̈ʒu���
	D3DXVECTOR3 m_ObjectRot[DATA_GAME_LENGTH];           //�I�u�W�F�N�g�̈ʒu���
	int m_nObjectCount;                             //�I�u�W�F�N�g�̐�                          
	char m_sObjectName[30][128];                    //�I�u�W�F�N�g��X�t�@�C���ǂݍ���
	char m_sTextureName[30][128];                   //�I�u�W�F�N�g�̃e�N�X�`���ǂݍ���
	int m_nModelType[DATA_GAME_LENGTH];                  //���f���̃^�C�v
	int m_nTextureType[DATA_GAME_LENGTH];                //���f���̃e�N�X�`���^�C�v

	//�p�[�e�B�N���p
	static int m_nGParicleMax[DATA_GAME_LENGTH];               //�p�[�e�B�N���̔�����
	int m_nGParicleText;								  //���ǂݍ��񂾂��̃J�E���g
	int m_nGCountText;									  //�p�[�e�B�N���̐�
	static float m_GSizeX[DATA_GAME_LENGTH];                   //�p�[�e�B�N���̃T�C�Y
	static float m_GSizeY[DATA_GAME_LENGTH];                   //�p�[�e�B�N���̃T�C�Y
	static float m_fGSpeed[DATA_GAME_LENGTH];                  //�p�[�e�B�N���̃X�s�[�h
	static D3DXCOLOR m_GParticlecol[DATA_GAME_LENGTH];         //�p�[�e�B�N���̃J���[
	static int m_nGLife[DATA_GAME_LENGTH];                     //�p�[�e�B�N���̎���
	static float m_fGGravity[DATA_GAME_LENGTH];                //�p�[�e�B�N���̏d��

	static int m_nHpCount;                                //�v���C���[��HP�J�E���g
	CSound		*m_pSound;								//�T�E���h�|�C���^
};

#endif