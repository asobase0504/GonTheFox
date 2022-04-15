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
	ANIME_COPY,		//�R�s�[
	ANIME_FIRE,		//�t�@�C�A
	ANIME_LASER,	//���[�U�[
	ANIME_CUTTER,	//�J�b�^�[
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
	int keyFrame;
	KEYPLAYER key[MAX_MODELPARTS];
}KEYSETPLAYER;

//model�f�[�^�̍\����//
typedef struct
{
	int key;		//���ԊǗ�
	int nowKey;		//���̃L�[
	int loop;		// ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1 : ���[�v����]
	int num_key;  	// �L�[��
	KEYSETPLAYER KeySet[MAX_KEY];
}MODELDATAPLAYER;

//���f���̍\����//
typedef struct
{
	D3DXVECTOR3 modelMin;		//�����蔻��ŏ�
	D3DXVECTOR3 modelMax;		//�����蔻��ő�
	D3DXMATRIX mtxWorld;		//�}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J
	LPD3DXMESH mesh;			//�p�[�c�̃��b�V��
	LPD3DXBUFFER buffMat;		//�p�[�c�̃o�b�t�@
	DWORD numMat;				//�p�[�c�̃}�b�g
	D3DXVECTOR3 pos;			//�p�[�c�̃|�X
	D3DXVECTOR3 posOri;			//�I���W�i��
	D3DXVECTOR3 posdefault;		//�ŏ�
	D3DXVECTOR3 rot;			//�p�[�c�̃��b�g
	D3DXVECTOR3 rotOri;			//�I���W�i��
	D3DXVECTOR3 rotdefault;		//�ŏ�
	int idxModelParent;         //�e�̃C���f�b�N�X       aModel[   ] �̔ԍ�
}ModelParts;

typedef enum
{
	COPY_NORMAL = 0,//�j���[�g����
	COPY_SWORD,		//�U��
	COPY_FIRE,		//�t�@�C�A
	COPY_LASER,		//���[�U�[
	COPY_CUTTER,	//�J�b�^�[
	COPY_MAX
}COPY;

typedef struct
{
	ModelParts Parts[MAX_MODELPARTS];	//model�̐�
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posOld;	//�ʒu�ߋ�
	D3DXVECTOR3 move;	//���[�u
	D3DXVECTOR3 rot;	//��]	
	D3DXVECTOR3 rotMove;//��]���[�u
	D3DXVECTOR3 modelMin;//�T�C�Y�ŏ�
	D3DXVECTOR3 modelMax;//�T�C�Y�ő�
	D3DXMATRIX mtxWorld; //�}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J
	int type;			//�^�C�v
	int shadow;		//�e�ԍ�
	int invincible;		//���G����
	ANIME motion;		//���܎g���Ă�motio�̔ԍ�
	STATUS status;		//���̃X�e�[�^�X
	DAMEGE damege;		//�_���[�W������Ă邩������ĂȂ���
	COPY cipy;			//�R�s�[
	bool use;			//�g���Ă邩�g���ĂȂ���
	bool notLoop;		//���[�v���邩���Ȃ���
	float consumption;	//�v�Z�p
}PLAYER;



//�v���g�^�C�v�錾
void InitPlayer(void);//������
void UninitPlayer(void);//�j��
void UpdatePlayer(void);//�X�V
void DrawPlayer(void);//�`��

void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *filename, int parent,int index, D3DXVECTOR3 modelPos);//�Z�b�g�������W�Ɠǂݍ��ރt�@�C����
void SizSet(void);//�����蔻��擾
void AnimationSet(int animation);//�A�j���[�V�����̌v�Z
void MoveSet(void);	//���[�u�Z�b�g
void Collision(void);	//�����蔻��܂Ƃ�
PLAYER *GetPlayer(void);//�Q�b�g
MODELDATAPLAYER *GetModelData(void);//motion�f�[�^�̃Q�b�g
#endif
