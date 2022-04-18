//============================
//
// �t�@�C���ǂݍ��ݏ���
// Author:hamada ryuuga
//
//============================

//=====================================
// �C���N���[�h
//=====================================
#include <stdio.h>
#include "main.h"
#include "file.h"
#include "file.h"
#include "player.h"

//=====================================
// �}�N��
//=====================================
#define MAXMODEL (12)
#define MAX_MODELPARTS (9)
#define MAX_KEY2  (5)
//=====================================
// �v���g�^�C�v�錾
//=====================================
void	LoadKeySetFile(FILE*pFile, MODELDATAPLAYER* pSet);
void	motionSet(FILE* pFile);
void	InitFile(void);
//=====================================
// �O���[�o���ϐ�
//=====================================

static char				s_aString[256];//
static MODELDATAPLAYER	s_ModelData[MAX_MOVE];
static int	nSetModel = 0;//���݂̃Z�b�g���Ă�ԍ�
static int a = 0;
static int b = 0;
static int nSetKey = 0;
static int nMoveSet = 0;

void InitFile(void)
{
	 nSetModel = 0;//���݂̃Z�b�g���Ă�ԍ�
	 a = 0;
	 b = 0;
	 nSetKey = 0;
	 nMoveSet = 0;
}
//---------------------------------------------------------------------------
// �t�@�C���ǂݍ��ݏ���
//---------------------------------------------------------------------------
void SetFile(char *Filename)
{

	char modelFile[256];

	// �t�@�C���|�C���^�̐錾
	FILE* pFile;
								//�t�@�C�����J��
	pFile = fopen(Filename, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", &s_aString[0]);
			if (strcmp(&s_aString[0], "MOTION_FILENAME") == 0)
			{// �����񂪈�v�����ꍇ
				fscanf(pFile, "%s", &modelFile);
				fscanf(pFile, "%s", &modelFile);
				LoadSetFile(modelFile);
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}

//---------------------------------------------------------------------------
// �t�@�C���ǂݍ��ݏ���
//---------------------------------------------------------------------------
void LoadSetFile(char *Filename)
{

	int	 nIndex = 0;
	int	 nParent = 0;
	int  nType = 0;
	int	 nModel = 0;
	char modelFilename[MAXMODEL][256];
	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(Filename, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", &s_aString[0]);
			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//����̂��ƃR�����g
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}
			if (strcmp(&s_aString[0], "NUM_MODEL") == 0)
			{

				fscanf(pFile, "%s", &s_aString[0]);
	
			}
			if (strcmp(&s_aString[0], "MODEL_FILENAME") == 0)
			{// �����񂪈�v�����ꍇ

				fscanf(pFile, "%s", &s_aString[0]);
				fscanf(pFile, "%s", &modelFilename[nModel][0]);

				nModel++;
			}
			if (strcmp(&s_aString[0], "MAINPOS") == 0)
			{// �����񂪈�v�����ꍇ

				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%f", &s_modelMainpos.x);
				fscanf(pFile, "%f", &s_modelMainpos.y);
				fscanf(pFile, "%f", &s_modelMainpos.z);
			}
			if (strcmp(&s_aString[0], "MOTIONSET") == 0)
			{
				motionSet(pFile);
			}
			if (strcmp(&s_aString[0], "PARTSSET") == 0 )
			{// �����񂪈�v�����ꍇ
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3	s_modelrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				while (1)
				{
					// ������̏������Ɠǂݍ���
					s_aString[0] = {};
					fscanf(pFile, "%s", &s_aString[0]);

					if (strncmp(&s_aString[0], "#", 1) == 0)
					{//����̂��ƃR�����g
						fgets(&s_aString[0], sizeof(s_aString), pFile);
						continue;
					}

					if (strcmp(&s_aString[0], "POS") == 0)
					{// �����񂪈�v�����ꍇ

						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&s_aString[0], "ROT") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_modelrot.x);
						fscanf(pFile, "%f", &s_modelrot.y);
						fscanf(pFile, "%f", &s_modelrot.z);
					}
					if (strcmp(&s_aString[0], "PARENT") == 0)
					{// �����񂪈�v�����ꍇ//�ǂ̃p�[�c�Ɛe�q�֌W��-�P�͐e�Ȃ�
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nParent);

					}
					if (strcmp(&s_aString[0], "INDEX") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nIndex);

					}
					if (strcmp(&s_aString[0], "END_PARTSSET") == 0)
					{//�v���C���[�̏ꍇ
						//SetPlayer(s_modelpos, s_modelrot, modelFilename[nIndex], nParent, nIndex);
						break;
					}
				}			
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}
void motionSet(FILE* pFile)
{
	while (1)
	{
		// ������̏������Ɠǂݍ���
		s_aString[0] = {};
		fscanf(pFile, "%s", &s_aString[0]);
		if (strncmp(&s_aString[0], "#", 1) == 0)
		{//����̂��ƃR�����g
			fgets(&s_aString[0], sizeof(s_aString), pFile);
		}
		if (strcmp(&s_aString[0], "LOOP") == 0)
		{// �����񂪈�v�����ꍇ//���[�v���邩���Ȃ���1����O���Ȃ�
			fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
			fscanf(pFile, "%d", &s_ModelData[b].loop);

		}
		if (strcmp(&s_aString[0], "NUM_KEY") == 0)
		{// �����񂪈�v�����ꍇ//�L�[�̍ő吔
			fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
			fscanf(pFile, "%d", &s_ModelData[b].num_key);

		}
		if (strcmp(&s_aString[0], "KEYSET") == 0)
		{// �����񂪈�v�����ꍇ//�A�j���[�V�����̃t�@�C���������Ƃ�
			LoadKeySetFile(pFile, &s_ModelData[b]);

		}
		if (strcmp(&s_aString[0], "END_MOTIONSET") == 0)
		{//����motion�ǂݍ��ݐ؂����玟��motion�̃Z�b�g�ɍs�����߂ɃJ�E���g���������ăf�[�^�����Z����
			Loadmotion(&s_ModelData[b], nMoveSet);
			b++;
			nSetModel = 0;
			break;
		}
	}

}
void LoadKeySetFile(FILE * pFile, MODELDATAPLAYER* set)
{
		while (1)
		{	
			// ������̏������Ɠǂݍ���
			s_aString[0] = {};

			fscanf(pFile, "%s", &s_aString[0]);
			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//�R�����g�΍�
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "FRAME") == 0)
			{// �����񂪈�v�����ꍇ
				fscanf(pFile, "%s", &s_aString[0]);//�C�R�[���ǂݍ���
				fscanf(pFile, "%d", &set->KeySet[nSetModel].keyFrame);
			}	
			if (strcmp(&s_aString[0], "KEY") == 0)
			{
				while (1)
				{// ������̏������Ɠǂݍ���
					s_aString[0] = {};
					fscanf(pFile, "%s", &s_aString[0]);
					if (strncmp(&s_aString[0], "#", 1) == 0)
					{
						fgets(&s_aString[0], sizeof(s_aString), pFile);//�R�����g�ǂݍ���
						continue;
					}
					if (strcmp(&s_aString[0], "POS") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &s_aString[0]);//�C�R�[���ǂݍ���
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].pos.x);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].pos.y);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].pos.z);
					}
					if (strcmp(&s_aString[0], "ROT") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &s_aString[0]);//�C�R�[���ǂݍ���
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].rot.x);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].rot.y);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].rot.z);
					}
					if (strcmp(&s_aString[0], "END_KEY") == 0)
					{
						nSetKey++;//�p�[�c�̐����ǂ�ǂ񑝂���
						break;
					}
				}
			}
			if (strcmp(&s_aString[0], "END_KEYSET") == 0)
			{// �����񂪈�v�����ꍇ
				nSetModel++;//���݂̃Z�b�g���Ă�ԍ��̍X�V
				nSetKey = 0;//�p�[�c�̐�������
				break;
			}
		}

}



void LoadCopy(char *Filename)
{
	InitFile();
	char aString[256];
	int	 nIndex = 0;
	int	 nParent = 0;
	int  nType = 0;
	int	 nModel = 0;
	char modelFilename[256];

	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	//�t�@�C�����J��
	pFile = fopen(Filename, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			aString[0] = {};
			fscanf(pFile, "%s", &aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���

			fscanf(pFile, "%s", &aString[0]);
			if (strncmp(&aString[0], "#", 1) == 0)
			{//����̂��ƃR�����g
				fgets(&aString[0], sizeof(aString), pFile);
				continue;
			}
			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{

				fscanf(pFile, "%s", &aString[0]);

			}
			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{// �����񂪈�v�����ꍇ

				fscanf(pFile, "%s", &aString[0]);
				fscanf(pFile, "%s", &modelFilename[0]);
			}
			if (strcmp(&aString[0], "MAINPOS") == 0)
			{// �����񂪈�v�����ꍇ

				fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%f", &s_modelMainpos.x);
				fscanf(pFile, "%f", &s_modelMainpos.y);
				fscanf(pFile, "%f", &s_modelMainpos.z);
			}
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{
				motionSet(pFile);
			}
			if (strcmp(&aString[0], "PARTSSET") == 0)
			{// �����񂪈�v�����ꍇ
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3	s_modelrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				while (1)
				{
					// ������̏������Ɠǂݍ���
					aString[0] = {};
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{//����̂��ƃR�����g
						fgets(&aString[0], sizeof(aString), pFile);
						continue;
					}

					if (strcmp(&aString[0], "POS") == 0)
					{// �����񂪈�v�����ꍇ

						fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&aString[0], "ROT") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%f", &s_modelrot.x);
						fscanf(pFile, "%f", &s_modelrot.y);
						fscanf(pFile, "%f", &s_modelrot.z);
					}
					if (strcmp(&aString[0], "PARENT") == 0)
					{// �����񂪈�v�����ꍇ//�ǂ̃p�[�c�Ɛe�q�֌W��-�P�͐e�Ȃ�
						fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nParent);

					}
					if (strcmp(&aString[0], "INDEX") == 0)
					{// �����񂪈�v�����ꍇ
						fscanf(pFile, "%s", &aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nIndex);

					}
					if (strcmp(&aString[0], "END_PARTSSET") == 0)
					{//�v���C���[�̏ꍇ
						//SetPlayer(s_modelpos, s_modelrot, modelFilename, nParent, nIndex);
						break;
					}
				}
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}