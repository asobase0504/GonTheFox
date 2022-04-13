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
// �v���g�^�C�v�錾
//=====================================
void	LoadKeySetFile(FILE*pFile, MODELDATAPLAYER* pSet);

//=====================================
// �O���[�o���ϐ�
//=====================================

static char				s_aString[256];//
static ModelData		s_ModelData;
static int	nSetModel = 0;//���݂̃Z�b�g���Ă�ԍ�
int a = 0;
int b = 0;
static int nSetKey = 0;

//---------------------------------------------------------------------------
// �t�@�C���ǂݍ��ݏ���
//---------------------------------------------------------------------------
void LoadSetFile(char *Filename)
{
	MODELDATAPLAYER* ModelData = GetModelData();
	char modelFile[256];
	int	 nIndex = 0;
	int	 nParent = 0;
	int  nType = 0;
	// �t�@�C���|�C���^�̐錾
	FILE* pFile;

	char aFile[128] = FILE_3D_SYSTEM;
	strcat(aFile, &Filename[0]);//�����@aFile��-�����ɓ����

	//�t�@�C�����J��
	pFile = fopen(aFile, "r");

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

			if (strcmp(&s_aString[0], "MODEL_FILENAME") == 0)
			{// �����񂪈�v�����ꍇ
				fscanf(pFile, "%s", &modelFile);
			}
			if (strcmp(&s_aString[0], "MAINPOS") == 0)
			{// �����񂪈�v�����ꍇ

				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%f", &s_modelMainpos.x);
				fscanf(pFile, "%f", &s_modelMainpos.y);
				fscanf(pFile, "%f", &s_modelMainpos.z);
			}
			else if (strcmp(&s_aString[0], "MODELSET") == 0|| strcmp(&s_aString[0], "MOTIONSET") == 0)
			{// �����񂪈�v�����ꍇ
				 D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f,0.0f,0.0f);
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
						fscanf(pFile, "%d", &nParent);

					}
					if (strcmp(&s_aString[0], "INDEX") == 0)
					{// �����񂪈�v�����ꍇ//�ǂ̃p�[�c�Ɛe�q�֌W��-�P�͐e�Ȃ�
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nIndex);

					}
					if (strcmp(&s_aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&s_aString[0], "LOOP") == 0)
					{// �����񂪈�v�����ꍇ//���[�v���邩���Ȃ���1����O���Ȃ�
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &ModelData->LOOP);

					}
					if (strcmp(&s_aString[0], "NUM_KEY") == 0)
					{// �����񂪈�v�����ꍇ//�L�[�̍ő吔
						fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
						fscanf(pFile, "%d", &ModelData->NUM_KEY);

					}
					if (strcmp(&s_aString[0], "KEYSET") == 0)
					{// �����񂪈�v�����ꍇ//�A�j���[�V�����̃t�@�C���������Ƃ�
						LoadKeySetFile(pFile, ModelData);
					
					}
					if (strcmp(&s_aString[0], "END_MOTIONSET")==0)
					{//����motion�ǂݍ��ݐ؂����玟��motion�̃Z�b�g�ɍs�����߂ɃJ�E���g���������ăf�[�^�����Z����
						ModelData++;
						b++;
						nSetModel = 0;
					}	
					if (strcmp(&s_aString[0], "END_PLAYESET") == 0)
					{//�v���C���[�̏ꍇ
						SetPlayer(s_modelpos, s_modelrot, modelFile, nParent, nIndex, s_modelMainpos);
						break;
					}
					if (strcmp(&s_aString[0], "END_SCRIPT") == 0)
					{// �����񂪈�v�����ꍇ
						break;
					}
				}	
			}
		}
	}

	//�t�@�C�������
	fclose(pFile);
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
				fscanf(pFile, "%d", &set->KeySet[nSetModel].KeyFrame);
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
				a++;
				nSetKey = 0;//�p�[�c�̐�������
				break;
			}
		}

}
//---------------------------------------------------------------------------
// �t�@�C���X�V����
//---------------------------------------------------------------------------
void OutputSetFile(void)
{
}