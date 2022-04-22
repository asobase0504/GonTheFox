//**************************************************************************************************
//
// �v���C���[����(motion.h)
// Auther�F�������l
//
//**************************************************************************************************
#ifndef _MOTION_H_			// ���̃}�N����`������ĂȂ�������
#define _MOTION_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//***************************************************************************
// �C���N���[�h
//***************************************************************************
#include"main.h"

//***************************************************************************
// �}�N����`
//***************************************************************************
#define	MAX_MODEL_PARTS		(128)		// ���f�����̍ő吔
#define MAX_KEY				(64)		// �L�[�̍ő吔
#define MAX_KEYSET			(64)		// �L�[�ݒ�̍ő吔

//***************************************************************
// �p�[�c�t�@�C���\���̂��`
//***************************************************************
typedef struct
{
	char	aName[MAX_MODEL_PARTS];		// ���O
}PartsFile;

//***************************************************************
// �L�[�\���̂��`
//***************************************************************
typedef struct
{
	D3DXVECTOR3		pos;	// ���݈ʒu
	D3DXVECTOR3		rot;	// ���݂̌���
}MyKey;

//***************************************************************
// �L�[�ݒ�\���̂��`
//***************************************************************
typedef struct
{
	int		nFrame;			// �t���[����
	MyKey	key[MAX_KEY];	// �L�[���
}MyKeySet;

//***************************************************************
// ���[�V�����ݒ�\���̂��`
//***************************************************************
typedef struct
{
	int			nNumKey;				// �L�[��
	int			nCntKeySet;				// �L�[�Z�b�g�J�E���g
	int			nCntFrame;				// �t���[���J�E���g
	bool		bLoop;					// ���[�v�g�p��
	MyKeySet		keySet[MAX_KEYSET];		// �L�[�ݒ���
}MyMotion;

//***************************************************************
// ���f���p�[�c�\���̂��`
//***************************************************************
typedef struct
{
	LPD3DXMESH		pMesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER	pBuffer;				// �}�e���A�����ւ̃|�C���^
	DWORD			nNumMat;				// �}�e���A�����̐�
	D3DXMATRIX		mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3		posOrigin;				// ���̈ʒu
	D3DXVECTOR3		rotOrigin;				// ���̌���
	D3DXVECTOR3		pos;					// ���݂̈ʒu
	D3DXVECTOR3		rot;					// ���݂̌���
	D3DXVECTOR3		posDest;				// �ړI�̈ʒu
	D3DXVECTOR3		rotDest;				// �ړI�̌���
	D3DXVECTOR3		vtxMin;					// ���_���W�̍ŏ��l
	D3DXVECTOR3		vtxMax;					// ���_���W�̍ő�l
	int				nIdxModelParent;		// �e���f���̃C���f�b�N�X��
	int				nType;					// �p�[�c�̃^�C�v
}Parts;

//***************************************************************
// �v���g�^�C�v�錾
//***************************************************************
// ���͊֐�
void SetParts(int nMaxParts,							// �p�[�c��
	Parts *parts,										// �p�[�c���
	D3DXMATRIX mtxWorld,								// ���[���h�}�g���b�N�X
	D3DXMATRIX mtxRot,									// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans,								// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 *matDef,								// �}�e���A���ۑ��ϐ�
	D3DXMATERIAL *pMat);								// �}�e���A���f�[�^
bool PlayMotion(int nMaxParts,							// �p�[�c��
	Parts *parts,										// �p�[�c���
	MyMotion *motion);									// ���[�V�������
bool MotionBlend(int nCntMotionSet,						// ���[�V�����̔z��ԍ�
	Parts *parts,										// �p�[�c���
	int nMaxParts,										// �p�[�c��
	MyMotion *motion);									// ���[�V�������						
void LoodSetMotion(char *pFileName, PartsFile *partsFile, Parts *parts, MyMotion *Motion, int *nMaxParts);

#endif

