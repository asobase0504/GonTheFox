//============================
//
// �v���C���[�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "motion.h"

//------------------------------------
// �}�N��
//------------------------------------

#define Attenuation	(0.5f)		//�����W��
#define Speed	(1.0f)			//�X�s�[�h
#define WIDTH (10.0f)			//���f���̔��a
#define MAX_PRAYER (16)			//�ő吔
#define MAX_MOVE (9)			//�A�j���[�V�����̍ő吔
#define INVINCIBLE (300)		//���G����
#define MAX_MODELPARTS (9)
//#define MAX_KEY  (6)
#define MAX_COPY  (4)

typedef enum
{
	ANIME_NORMAL = 0,	//�j���[�g����
	ANIME_RUN,			//����
	ANIME_ATTACK,		//�U��
	ANIME_JUMP,			//�W�����v
	ANIME_LANDING,		//���n
	ANIME_MAX
}ANIME;


typedef enum
{
	STATUS_NORMAL = 0,	//�j���[�g����
	STATUS_RUN,		//����
	STATUS_ATTACK,	//�U��
	STATUS_JUMPUP,		//�W�����v
	STATUS_JUMPDOWN,		//�W�����v
	STATUS_LANDING,	//���n
	STATUS_MAX
}STATUS;

typedef enum
{
	DAMEGE_NORMAL = 0,	//�j���[�g����
	DAMEGE_NOU,			//�_���[�W������Ă�
	DAMEGE_MAX
}DAMEGE;

typedef enum
{
	COPY_NORMAL = 0,//�j���[�g����
	COPY_SWORD,		//�\�[�h
	COPY_FIRE,		//�t�@�C�A
	COPY_LASER,		//���[�U�[
	COPY_CUTTER,	//�J�b�^�[
	COPY_MAX
}COPY;

//model�f�[�^�̍\����//
typedef struct
{
	int key;		// ���ԊǗ�
	int nowKey;		// ���̃L�[
	int loop;		// ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1 : ���[�v����]
	int num_key;  	// �L�[��
	MyKeySet KeySet[MAX_KEY];
}MODELDATAPLAYER;

typedef struct
{
	D3DXVECTOR3		pos;						// �ʒu
	D3DXVECTOR3		posOld;						// �ʒu�ߋ�
	D3DXVECTOR3		move;						// ���[�u
	D3DXVECTOR3		rot;						// ��]	
	D3DXVECTOR3		rotMove;					// ��]���[�u
	D3DXVECTOR3		modelMin;					// �T�C�Y�ŏ�
	D3DXVECTOR3		modelMax;					// �T�C�Y�ő�
	D3DXMATRIX		mtxWorld;					// �}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J

	STATUS			status;						// ���̃X�e�[�^�X
	DAMEGE			damege;						// �_���[�W������Ă邩������ĂȂ���
	COPY			copy;						// �R�s�[

	Parts			parts[MAX_MODELPARTS];		// ���f���p�[�c
	PartsFile		partsFile[MAX_MODELPARTS];	// �p�[�c�t�@�C��
	MyMotion		motion[ANIME_MAX];			// ���[�V����
	ANIME			motionType;					// ���[�V�����^�C�v(����)
	ANIME			motionTypeOld;				// ���[�V�����^�C�v(�ߋ�)
	int				nMaxModelType;				// ���f���̃^�C�v��
	int				nMaxModelParts;				// �������f���p�[�c��
	int				nMaxMotion;					// ���[�V������

	int				type;						// �^�C�v
	int				shadow;						// �e�ԍ�
	int				invincible;					// ���G����
	float			consumption;				// �v�Z�p

	bool			bMotionBlend;				// ���[�V�����u�����h
	bool			bMotion;					// ���[�V�������g�p��

	bool			isUse;						// �g���Ă邩�g���ĂȂ���
	bool			notLoop;					// ���[�v���邩���Ȃ���

	 char			aFirename[256];//
}PLAYER;



//�v���g�^�C�v�錾
void InitPlayer(void);//������
void UninitPlayer(void);//�j��
void UpdatePlayer(void);//�X�V
void DrawPlayer(void);//�`��

void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);//�Z�b�g�������W�Ɠǂݍ��ރt�@�C����

void MoveSet(void);	//���[�u�Z�b�g
void Collision(void);	//�����蔻��܂Ƃ�
void SetCopy(char *pFileName, PartsFile *partsFile, Parts *parts, MyMotion *Motion, int *nMaxParts);
PLAYER *GetPlayer(void);
#endif
