//=============================================================================
//
// �ʍs�l���� [passerby.h]
// Author : Miura�@Yudai
//
//=============================================================================

#ifndef _PASSERBY_H_
#define _PASSERBY_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//==============================================================================
//�C���N���[�h�t�@�C���ǂݍ���
//==============================================================================
#include "main.h"
#include "scene.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MODEL_PASSERBY_PARTS (30)	// ���f���p�[�c�̍ő吔
#define TEXT_LENGTH		(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH		(64)	// �f�[�^�ǂݍ��݂̍ő吔
#define MAX_PASSERBY_PARTS (13)

//=============================================================================
// �O���錾
//=============================================================================
class CSound;
class CHp;
class CModel;

//==============================================================================
// ���[�V�����̎��
//==============================================================================
typedef enum
{
	MOTIONTYPE_PASSERBY_NEUTRAL = 0,	// �j���[�g�������[�V����
	MOTIONTYPE_PASSERBY_MOVE,		// �ړ����[�V����
	MOTIONTYPE_PASSERBY_LEFTMOVE,	// ���ړ����[�V����
	MOTIONTYPE_PASSERBY_RIGHTMOVE,	// �E�ړ����[�V����
	MOTIONTYPE_PASSERBY_JUMP,		// �W�����v���[�V����
	MOTIONTYPE_PASSERBY_MAX			// ���[�V�����̍ő吔
} MOTIONTYPE_PASSERBY;

//==============================================================================
// ���s�҂̎��
//==============================================================================
typedef enum
{
	PASSERBY_TYPE_NEUTRAL = 0,  // �����Ȃ�
	PASSERBY_TYPE_MEN,		    // �j�̐l
	PASSERBY_TYPE_WOMEN,	    // ���̐l
	PASSERBY_TYPE_CAT,	        // �L
	PASSERBY_TYPE_MAX			// ���[�V�����̍ő吔
} PASSERBY_TYPE;

//=============================================================================
// ���s�҃N���X�̒�`
//=============================================================================
//�p����`��N���X
class CPasserby : public CScene
{
public:

	CPasserby(int nPriority = OBJTYPE_ENEMY, int nType = 0);								//�R���X�g���N�^
	virtual ~CPasserby();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//����������
	void Uninit(void);															//�I������
	void Update(void);															//�X�V����
	void Draw(void);															//�`�揈��

	static CPasserby *Create(D3DXVECTOR3 pos,PASSERBY_TYPE passerbytype,int nType);//��������
	
	void LoadMotion(PASSERBY_TYPE passerbytype);														//�O���ǂݍ���
	void UpdateMotion(MOTIONTYPE_PASSERBY passerby);
	void PasserbyMove(void);
	bool CollisionVec(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

	void CollisionPlayer(void);
	D3DXVECTOR3 GetPasserbyPos(void) { return m_pos; }

private:
	CModel			*m_apModel[MODEL_PASSERBY_PARTS];				//���f���ւ̃|�C���^
	char			*m_pTextName[MODEL_PASSERBY_PARTS];				//���O�ۑ�
	int				m_aIndexParent[MODEL_PASSERBY_PARTS];			//���f���̐e
	float			m_aOffset[MODEL_PASSERBY_PARTS][6];				//���f���̈ʒu������ݒu
	float			m_aOffsetMemo[MODEL_PASSERBY_PARTS][6];				//�ۑ��p���f���̈ʒu������ݒu

	//���[���h�}�g���b�N�X�֘A
	LPDIRECT3DTEXTURE9	m_pTexture;		//�e�N�X�`���̃|�C���^
	LPD3DXMESH				m_pMesh;			//���b�V��
	LPD3DXBUFFER			m_pBuffMat;			//�o�b�t�@
	DWORD					m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;				        //�ʒu
	D3DXVECTOR3     m_poskeep;                  //�ʒu���L������ϐ�
	static D3DXVECTOR3   m_posHoming;           //������ǔ�����
	D3DXVECTOR3		m_rot;				//����
	D3DXVECTOR3             m_rotDest;          //�����̊���
	int						m_nNumVtx;			//���_��
	float					m_fRot;				//��]
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_MaxPasserby;//�ő�l
	D3DXVECTOR3 m_MinPasserby;//�ŏ��l
	D3DXVECTOR3 m_posold;
	D3DXVECTOR3 m_posold2;
	int nCntPlayer;
	D3DXVECTOR3	m_RotDest;
	D3DXVECTOR3 m_startpos;

	//���[�V�����g�p
	bool					m_Loop[MOTIONTYPE_PASSERBY_MAX];
	int						m_NumKey[MOTIONTYPE_PASSERBY_MAX];
	int						m_SetParent[MODEL_PASSERBY_PARTS];
	float					m_Passerby[MOTIONTYPE_PASSERBY_MAX][10][MODEL_PASSERBY_PARTS][6];		//�e���[�V�����̈ʒu�������L���ꏊ
	int						m_nFrame[MOTIONTYPE_PASSERBY_MAX][10];								//�e���[�V�����̃t���[���L��
	int						m_nFrameTotal[MOTIONTYPE_PASSERBY_MAX];								//�e���[�V�����̃t���[���L��
	int						m_Key;														//���݂̃��[�V�������J�E���g
	float					m_fCounterMotion;											//���[�V�����̃J�E���^�[
	int						m_KeySet[MOTIONTYPE_PASSERBY_MAX];									//�ő�t���[�������L��
	MOTIONTYPE_PASSERBY		m_MotionType;
	MOTIONTYPE_PASSERBY		m_MotionTypeOld;
	int                     m_nMoveCnt;                                                 //���[�V�����p�J�E���g�ϐ�

	int                     m_nNumModel;                                                //���f���̃p�[�c��
	int                     m_nNumKeyKeep;                                              //���f���̃L�[�ۑ��p
	bool                    m_bLoopKeep;                                                //���[�v���邩���Ȃ����ۑ��p
	bool                    m_bMotionChange;                                            //���[�V�����ω��p
	int                     m_nBrendFrame;                                              //�u�����h���[�V�����t���[��
	char                    m_pModelName[MODEL_PASSERBY_PARTS][128];                      //X�t�@�C���ǂݍ��ݗp
	D3DXVECTOR3             m_posMemo;                                                  //�ۑ��p
	bool                    m_bGoal[5];                                                 //�ړI�n����p
	PASSERBY_TYPE           m_passerbytype;                                             //���s�҂̎��
	int                     m_nPasserbyCount;                                           //���s�҂̕ʁX�̓����ϐ�
	int                     m_nRamdom[2];                                               //�����_������������
	D3DXVECTOR3          	m_aPos[10][4];                                              //�����蔻��̊O�ϗp
	bool                    m_fUseScene3D;
	bool                    m_EnemyMove;

	//�����蔻��p�����o�ϐ�
	D3DXVECTOR3 m_aPosColision[4];
	D3DXVECTOR3 m_aVec[4];
	D3DXVECTOR3 m_bVec[4];
	float m_fPlayerVec[4];

	CHp *m_pHp[4];

	bool m_bParticle;
	static bool m_bParticle2;
	bool m_bParticle3;
	bool m_bReset;
	int m_nParticlecount;
	CSound		*m_pSound;								//�T�E���h�|�C���^
};

#endif
