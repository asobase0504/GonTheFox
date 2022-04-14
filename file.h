//============================
//
// �t�@�C���ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_			// ���̃}�N����`������ĂȂ�������
#define _FILE_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//=====================================
// �C���N���[�h
//=====================================
#include "main.h"

//=====================================
//�@�t�@�C���p�X�܂Ƃ�
//=====================================
#define FILE_3D_PLAYER  "Data/Model/"
#define FILE_3D_MODEL  "Data/Model/"
#define FILE_3D_SYSTEM "Data/system/"
#define LOOD_FILE_NAME_000	"Prayer.txt"
#define LOOD_FILE_NAME_EZ	"ez/model.txt"
#define LOOD_FILE_NAME_NO	"no/model.txt"
#define LOOD_FILE_NAME_HD	"hd/model.txt"
#define LOOD_FILE_NAME_002 "motion.txt"
#define LOOD_FILE_EZ_ENEMY "ez/enemy.txt"	
#define LOOD_FILE_NO_ENEMY "no/enemy.txt"
#define LOOD_FILE_HD_ENEMY "hd/enemy.txt"
#define LOOD_FILE_NOME_UI "ui.txt"
#define LOOD_FILE_NOME_AITEMEZ "ez/item.txt"
#define LOOD_FILE_NOME_AITEMNO "no/item.txt"
#define LOOD_FILE_NOME_AITEMHD "hd/item.txt"
#define MAX_MODELPARTS (9)
#define MAX_KEY2  (5)

//�L�[�̍\����//
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3  rot;
}kEY;

//�L�[�Z�b�g�̍\����//
typedef struct
{
	int KeyFrame;
	kEY key[MAX_MODELPARTS];
}kEYSET;

//model�f�[�^�̍\����//
typedef struct
{
	int  LOOP;		// ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1 : ���[�v����]
	int NUM_KEY;  	// �L�[��
	kEYSET KeySet[MAX_KEY2];
}ModelData;

//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�
void	LoadSetFile(char *Filename);
void	OutputSetFile(void);

#endif

