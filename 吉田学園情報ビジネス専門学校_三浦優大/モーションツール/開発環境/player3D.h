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
#include "model.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MODEL_PARTS		(20)	// ���f���p�[�c�̍ő吔
#define TEXT_LENGTH		(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH		(64)	// �f�[�^�ǂݍ��݂̍ő吔
#define MAX_PLAYER_PARTS (17)   // ���f���̃p�[�c��

//==============================================================================
// ���[�V�����̎��
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// �j���[�g�������[�V����
	MOTIONTYPE_MOVE,		// �ړ����[�V����
	MOTIONTYPE_LEFTMOVE,	// ���ړ����[�V����
	MOTIONTYPE_RIGHTMOVE,	// �E�ړ����[�V����
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
	void LoadMotion(void);														//�O���ǂݍ���
	void WriteMotion(void);                                                     //�O����������
	void UpdateMotion(int MOTIONTYPE,bool bUse);
	void MotionTool(void);                                                      //���[�V�����c�[���p����֐�
	void PlayerMove(void);                                                      //�v���C���[�̈ړ�����

	static D3DXVECTOR3	GetPosition(void);						//�ʒu�擾�p
	static D3DXVECTOR3	GetRotation(void);						//�����擾�p
	//���[�V�����c�[���p
	static MOTIONTYPE   GetMotionTypeMax(void) { return m_MotionTypeMax; } //���[�V�����̍ő吔�擾
	static int   GetMotionType(void) { return m_MotionChange; } //���݂̃��[�V�����擾
	static int   GetKeyMax(void) { return m_nNumKeyKeep; } //���[�V�����̍ő吔�擾
	static int   GetKey(void) { return m_GetKey; } //���[�V�����̍ő吔�擾
	static int   GetFrame(void) { return m_nFrame[m_MotionChange][m_GetKey]; }
	static int   GetFrameTotal(void) { return m_nFrameTotal[m_MotionChange]; }
	static int   GetCounter(void) { return m_nCounterMotion; }
	static int   GetNumModel(void) { return m_nNumModel; }
	static D3DXVECTOR3 GetModelParentRot(int nCnt) { return m_apModel[nCnt]->GetRot(); }
	static D3DXVECTOR3 GetModelParentPos(int nCnt) { return m_apModel[nCnt]->GetPos(); }
	static char *GetSelectData(int nCnt) { return m_SelectData[nCnt][256]; }
	static int   GetModelCount(void) { return m_DataCount; }
	static float GetFirstModel(int nCnt) { return m_aOffset[0][nCnt]; }

private:
	static CModel   *m_apModel[MAX_PLAYER_PARTS];				//���f���ւ̃|�C���^
	char			*m_pTextName[MAX_PLAYER_PARTS];				//���O�ۑ�
	int				m_aIndexParent[MAX_PLAYER_PARTS];			//���f���̐e
	static float	m_aOffset[MAX_PLAYER_PARTS][6];				//���f���̈ʒu������ݒu
	float			m_aOldOffset[MAX_PLAYER_PARTS][6];			//���f���̈ʒu������ݒu�ۑ��p
	float			m_aOffsetMemo[MAX_PLAYER_PARTS][6];			//�ۑ��p���f���̈ʒu������ݒu

	//���[���h�}�g���b�N�X�֘A
	LPDIRECT3DTEXTURE9	m_pTexture;		//�e�N�X�`���̃|�C���^
	LPD3DXMESH				m_pMesh;			//���b�V��
	LPD3DXBUFFER			m_pBuffMat;			//�o�b�t�@
	DWORD					m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	static D3DXVECTOR3		m_pos;				//�ʒu
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
	bool					m_Loop[MOTIONTYPE_MAX];
	int						m_NumKey[MOTIONTYPE_MAX];
	int						m_SetParent[MAX_PLAYER_PARTS];
	float					m_Player3D[MOTIONTYPE_MAX][10][MAX_PLAYER_PARTS][6];		//�e���[�V�����̈ʒu�������L���ꏊ
	static int				m_nFrame[MOTIONTYPE_MAX][10];								//�e���[�V�����̃t���[���L��
	static int				m_nFrameTotal[MOTIONTYPE_MAX];								//�e���[�V�����̃t���[���L��
	int			            m_Key;														//���݂̃��[�V�������J�E���g
	static int			    m_nCounterMotion;											//���[�V�����̃J�E���^�[
	static int              m_GetKey;
	static int				m_KeySet[MOTIONTYPE_MAX];									//�ő�t���[�������L��
	static int       	    m_MotionType;
	int      				m_MotionTypeOld;
	int                     m_nMoveCnt;                                                 //���[�V�����p�J�E���g�ϐ�

	static int              m_nNumModel;                                                //���f���̃p�[�c��
	static int              m_nNumKeyKeep;                                              //���f���̃L�[�ۑ��p
	bool                    m_bLoopKeep;                                                //���[�v���邩���Ȃ����ۑ��p
	bool                    m_bMotionChange;                                            //���[�V�����ω��p
	int                     m_nBrendFrame;                                              //�u�����h���[�V�����t���[��
	char                    m_pModelName[MAX_PLAYER_PARTS][128];                        //X�t�@�C���ǂݍ��ݗp
	D3DXVECTOR3             m_posMemo;                                                  //�ۑ��p
	int                     m_nBrendCount;
	static MOTIONTYPE       m_MotionTypeMax;                                            //���[�V�����̍ő�l
	bool                    m_bUse;
	int                     m_nCnt;                                                     //F2�L�[�̏���
	static int              m_MotionChange;                                             //���[�V�����؂�ւ�
	int                     m_AddKey;                                                   //�L�[���𑝂₷
	int                     m_TotalKey;
	int                     m_nCntMat;                                                  //�}�e���A���̐F�ύX�J�E���g
	int                     m_nCntMatOld;                                               //�}�e���A���̐̂̃J�E���g�ۑ��p
	static char             *m_SelectData[MAX_PLAYER_PARTS + 1][256];                   //�f�o�b�N���O�\���p
	bool                    m_bData;                                                    //���O�f�[�^�p�t���O
	bool                    m_bData2;                                                   //���O�f�[�^�p�t���O
	static int              m_DataCount;                                                //�J�E���g�f�[�^
	int                     m_OldDataCount;                                             //�J�E���g�f�[�^�ۑ��p
	bool                    m_bMove;                                                    //�ړ����邽�߂̃t���O
};

#endif