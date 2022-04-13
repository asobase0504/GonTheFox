//============================
//
// �v���C���[�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define MAX_MODELPARTS (9)
#define MAX_KEY  (6)

typedef enum
{
	ANIME_NORMAL = 0,	//�j���[�g����
	ANIME_RUN,		//����
	ANIME_ATTACK,	//�U��
	ANIME_JUMP,		//�W�����v
	ANIME_LANDING,	//���n
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


//�L�[�̍\����//
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3  rot;
}KEYPLAYER;

//�L�[�Z�b�g�̍\����//
typedef struct
{
	int KeyFrame;
	KEYPLAYER key[MAX_MODELPARTS];
}KEYSETPLAYER;

//model�f�[�^�̍\����//
typedef struct
{
	int Key;		//���ԊǗ�
	int NowKey;		//���̃L�[
	int LOOP;		// ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1 : ���[�v����]
	int NUM_KEY;  	// �L�[��
	KEYSETPLAYER KeySet[MAX_KEY];
}MODELDATAPLAYER;

//���f���̍\����//
typedef struct
{
	D3DXVECTOR3 ModelMin;		//�����蔻��ŏ�
	D3DXVECTOR3 ModelMax;		//�����蔻��ő�
	D3DXMATRIX MtxWorld;		//�}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J
	LPD3DXMESH pMesh;			//�p�[�c�̃��b�V��
	LPD3DXBUFFER pBuffMat;		//�p�[�c�̃o�b�t�@
	DWORD nNumMat;				//�p�[�c�̃}�b�g
	D3DXVECTOR3 pos;			//�p�[�c�̃|�X
	D3DXVECTOR3 posOri;			//�I���W�i��
	D3DXVECTOR3 posdefault;		//�ŏ�
	D3DXVECTOR3 rot;			//�p�[�c�̃��b�g
	D3DXVECTOR3 rotOri;			//�I���W�i��
	D3DXVECTOR3 rotdefault;		//�ŏ�
	int nIdxModelParent;         //�e�̃C���f�b�N�X       aModel[   ] �̔ԍ�
}ModelParts;

typedef struct
{
	ModelParts Parts[MAX_MODELPARTS];	//model�̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posOld;	//�ʒu�ߋ�
	D3DXVECTOR3 move;	//���[�u
	D3DXVECTOR3 rot;	//��]	
	D3DXVECTOR3 RotMove;//��]���[�u
	D3DXVECTOR3 ModelMin;//�T�C�Y�ŏ�
	D3DXVECTOR3 ModelMax;//�T�C�Y�ő�
	D3DXMATRIX MtxWorld; //�}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J
	int nType;			//�^�C�v
	int nShadow;		//�e�ԍ�
	int invincible;		//���G����
	ANIME nMotion;		//���܎g���Ă�motio�̔ԍ�
	STATUS Status;		//���̃X�e�[�^�X
	DAMEGE Damege;		//�_���[�W������Ă邩������ĂȂ���
	bool bUse;			//�g���Ă邩�g���ĂȂ���
	bool bMystery;		//�܂ق��g�����g��Ȃ���
	bool NotLoop;		//���[�v���邩���Ȃ���
	float consumption;	//�v�Z�p
}PLAYER;


//�v���g�^�C�v�錾
void InitPlayer(void);//������
void UninitPlayer(void);//�j��
void UpdatePlayer(void);//�X�V
void DrawPlayer(void);//�`��

void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Filename, int nParent,int nIndex, D3DXVECTOR3 nModelPos);//�Z�b�g�������W�Ɠǂݍ��ރt�@�C����
void SizSet(void);//�����蔻��擾
void AnimationSet(int Animation);//�A�j���[�V�����̌v�Z
void MoveSet(void);	//���[�u�Z�b�g
void Collision(void);	//�����蔻��܂Ƃ�
PLAYER *GetPlayer(void);//�Q�b�g
MODELDATAPLAYER *GetModelData(void);//motion�f�[�^�̃Q�b�g
#endif
