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




//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�
void	SetFile(char *Filename);
void	LoadSetFile(char *Filename);
void	OutputSetFile(void);
void	LoadCopy(char *Filename);
#endif

