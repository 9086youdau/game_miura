//=============================================================================
//
// �v���C���[3D���� [player3D.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _PLAYER3D_H_
#define _PLAYER3D_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "main.h"
#include "scene.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MODEL_PARTS		(30)	// ���f���p�[�c�̍ő吔
#define TEXT_LENGTH		(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH		(64)	// �f�[�^�ǂݍ��݂̍ő吔
#define MAX_PLAYER_PARTS (17)   // ���f���̃p�[�c��
#define MAX_MOTION       (20)   // ���[�V�����̍ő吔

//==============================================================================
// �O���錾
//==============================================================================
class CHp;
class CPasserby;
class CSound;
class CBullet;
class CModel;

//==============================================================================
// ���[�V�����̎��
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// �j���[�g�������[�V����
	MOTIONTYPE_MOVE,		// �ړ����[�V����
	MOTIONTYPE_SHOTMOVE,	// �����[�V����
	MOTIONTYPE_MAX			// ���[�V�����̍ő吔
} MOTIONTYPE;

//=============================================================================
// �|���S���N���X�̒�`
//=============================================================================
//�p����`��N���X
class CPlayer3D : public CScene
{
public:

	CPlayer3D(int nPriority = 5, int nType = 0);								//�R���X�g���N�^
	virtual ~CPlayer3D();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//����������
	void Uninit(void);															//�I������
	void Update(void);															//�X�V����
	void Draw(void);															//�`�揈��

	static CPlayer3D *Create();													//��������
	void LoadMotion(void);														//�O���ǂݍ���                                                   //�O����������
	void UpdateMotion(int MOTIONTYPE,bool bUse);
	void PlayerMove(void);                                                      //�v���C���[�̈ړ�����
	void CollisionPlayer(void);                                                 //�v���C���[�̓����蔻��

	static D3DXVECTOR3	GetPosition(void);						//�ʒu�擾�p
	static D3DXVECTOR3	GetRotation(void);						//�����擾�p

	static CBullet *GetBullet (void) { return m_pBullet; }
	static bool GetParticle(void) {return m_bReset;}

private:
	static CModel   *m_apModel[MODEL_PARTS];				//���f���ւ̃|�C���^
	char			*m_pTextName[MODEL_PARTS];				//���O�ۑ�
	int				m_aIndexParent[MODEL_PARTS];			//���f���̐e
	float	m_aOffset[MODEL_PARTS][6];				//���f���̈ʒu������ݒu
	float	m_aOffsetWrite[10][MODEL_PARTS][6];		//�������ݗp�̈ʒu������ݒ�p
	float			m_aOldOffset[MODEL_PARTS][6];			//���f���̈ʒu������ݒu�ۑ��p
	float			m_aOffsetMemo[MODEL_PARTS][6];			//�ۑ��p���f���̈ʒu������ݒu

	//���[���h�}�g���b�N�X�֘A
	LPDIRECT3DTEXTURE9	m_pTexture;		//�e�N�X�`���̃|�C���^
	LPD3DXMESH				m_pMesh;			//���b�V��
	LPD3DXBUFFER			m_pBuffMat;			//�o�b�t�@
	DWORD					m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3		        m_pos;				//�ʒu
	static D3DXVECTOR3		m_getpos;				//�󂯓n���p�ʒu���
	static D3DXVECTOR3		m_rot;				//����
	D3DXVECTOR3             m_rotDest;          //�����̊���
	int						m_nNumVtx;			//���_��
	float					m_fRot;				//��]
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_MaxPlayer;//�ő�l
	D3DXVECTOR3 m_MinPlayer;//�ŏ��l
	D3DXVECTOR3 m_posold;
	D3DXVECTOR3 m_posold2;
	int nCntPlayer;
	D3DXVECTOR3	m_RotDest;

	//���[�V�����g�p
	bool					m_Loop[MAX_MOTION];
	int						m_NumKey[MAX_MOTION];
	int						m_SetParent[MODEL_PARTS];
	float					m_Player3D[MAX_MOTION][10][MODEL_PARTS][6];		//�e���[�V�����̈ʒu�������L���ꏊ
	int				m_nFrame[MAX_MOTION][10];								//�e���[�V�����̃t���[���L��
	int				m_nFrameTotal[MAX_MOTION];								//�e���[�V�����̃t���[���L��
	int			            m_Key;														//���݂̃��[�V�������J�E���g
	int			    m_nCounterMotion;											//���[�V�����̃J�E���^�[
	int              m_GetKey;
	int				m_KeySet[MAX_MOTION];									//�ő�t���[�������L��
	int       	    m_MotionType;
	int      				m_MotionTypeOld;
	int                     m_nMoveCnt;                                                 //���[�V�����p�J�E���g�ϐ�

	int              m_nNumModel;                                                //���f���̃p�[�c��
	int              m_nNumKeyKeep;                                              //���f���̃L�[�ۑ��p
	bool                    m_bLoopKeep;                                                //���[�v���邩���Ȃ����ۑ��p
	bool                    m_bMotionChange;                                            //���[�V�����ω��p
	int                     m_nBrendFrame;                                              //�u�����h���[�V�����t���[��
	char                    m_pModelName[MODEL_PARTS][128];                        //X�t�@�C���ǂݍ��ݗp
	D3DXVECTOR3             m_posMemo;                                                  //�ۑ��p
	int                     m_nBrendCount;
	int              m_MotionTypeMax;                                            //���[�V�����̍ő�l
	bool                    m_bUse;
	int                     m_nCnt;                                                     //F2�L�[�̏���
	int              m_MotionChange;                                             //���[�V�����؂�ւ�
	int                     m_AddKey;                                                   //�L�[���𑝂₷
	int                     m_TotalKey;
	int                     m_nCntMat;                                                  //�}�e���A���̐F�ύX�J�E���g
	int                     m_nCntMatOld;                                               //�}�e���A���̐̂̃J�E���g�ۑ��p
	char             *m_SelectData[MODEL_PARTS + 1][256];                   //�f�o�b�N���O�\���p
	bool                    m_bData;                                                    //���O�f�[�^�p�t���O
	bool                    m_bData2;                                                   //���O�f�[�^�p�t���O
	int                     m_DataCount;                                                //�J�E���g�f�[�^
	int                     m_OldDataCount;                                             //�J�E���g�f�[�^�ۑ��p
	bool                    m_bMove;                                                    //�ړ����邽�߂̃t���O
	int                     m_nCounter;
	bool                    m_nNextKey;
	int                     m_MotionMax;
	bool                    m_bColision;
	static CBullet *m_pBullet;

	//�����蔻��p�����o�ϐ�
	D3DXVECTOR3 m_aPosColision[4];//�����蔻��̋�`�p
	D3DXVECTOR3 m_aVec[4];        //�x�N�g���p
	D3DXVECTOR3 m_bVec[4];        //�x�N�g���p2
	float m_fPlayerVec[4];        //�v���C���[�̃x�N�g���ۑ��p

	CHp *m_pHp[4];                //HP�p�|�C���^�ϐ�
	CPasserby *m_pPasserby;       //�G�̃|�C���^�ۑ��p
	int m_nHpCount;               //HP�̃J�E���g�p
	static bool m_bReset;         //�v���C���[�����񂾂Ƃ��̃t���O
	int m_nResetCount;            //�v���C���[�����񂾂Ƃ��̃J�E���g
	int m_nBulletCount;           //�e�̔��ˑ��x
	int m_nParticlecount;         //�p�[�e�B�N������������J�E���g   
	bool m_bParticle;             //�p�[�e�B�N������������t���O
	CSound		*m_pSound;		  //�T�E���h�|�C���^
};

#endif