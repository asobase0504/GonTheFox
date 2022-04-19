//**************************************************************************************************
//
// ���[�V��������(motion.cpp)
// Auther�F�������l
//
//**************************************************************************************************

//***************************************************************************
// �C���N���[�h
//***************************************************************************
#include <stdio.h>
#include <assert.h>
#include "main.h"
#include "motion.h"
#include "utility.h"

//***************************************************************************
// �}�N����`
//***************************************************************************
#define MOTION_BLEND_FRAM			(12)				// ���[�V�����u�����h�̃t���[����		

//---------------------------------------------------------------------------
// �p�[�c�ݒu����
//---------------------------------------------------------------------------
void SetParts(int nMaxParts,		// �p�[�c��
	PARTS *Parts,					// �p�[�c���
	D3DXMATRIX mtxWorld,			// ���[���h�}�g���b�N�X
	D3DXMATRIX mtxRot,				// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxTrans,			// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 *matDef,			// �}�e���A���ۑ��ϐ�
	D3DXMATERIAL *pMat)				// �}�e���A���f�[�^
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	for (int i = 0; i < nMaxParts; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&(Parts + i)->mtxWorld);				// �s�񏉊����֐�

		// �����̔��f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			(Parts + i)->rot.y,
			(Parts + i)->rot.x,
			(Parts + i)->rot.z);								// �s���]�֐�
		D3DXMatrixMultiply(&(Parts + i)->mtxWorld,
			&(Parts + i)->mtxWorld,
			&mtxRot);											// �s��|���Z�֐� 

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			(Parts + i)->pos.x,
			(Parts + i)->pos.y,
			(Parts + i)->pos.z);								// �s��ړ��֐�
		D3DXMatrixMultiply(&(Parts + i)->mtxWorld,
			&(Parts + i)->mtxWorld,
			&mtxTrans);											// �s��|���Z�֐�

		// �e�p�[�c�̃��[���h�}�g���b�N�X��ێ�
		D3DXMATRIX mtxParent;

		if ((Parts + i)->nIdxModelParent == -1)
		{// �e���f���̃C���f�b�N�X����-1�̎�
			mtxParent = mtxWorld;
		}
		else
		{
			mtxParent = (Parts + (Parts + i)->nIdxModelParent)->mtxWorld;
		}

		// �����̐e�}�g���b�N�X�Ƃ̊|���Z
		D3DXMatrixMultiply(&(Parts + i)->mtxWorld, &(Parts + i)->mtxWorld, &mtxParent);

		//// �T�C�Y�̔��f
		//D3DXMatrixScaling()

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &(Parts + i)->mtxWorld);

		// ���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&*matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)(Parts + i)->pBuffer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)(Parts + i)->nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �v���C���[�p�[�c�̕`��
			(Parts + i)->pMesh->DrawSubset(nCntMat);
		}

		// �ۂ��Ă����}�e���A����߂�
		pDevice->SetMaterial(&*matDef);
	}
}

//---------------------------------------------------------------------------
// ���[�V��������
//---------------------------------------------------------------------------
bool PlayMotion(int nMaxParts,			// �p�[�c��
	PARTS *Parts,						// �p�[�c���
	MOTION *motion)						// ���[�V�������
{
	// �ϐ��錾
	bool bMotion = true;

	for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
	{
		if (motion->nCntFrame == 0)
		{// �t���[���J�E���g��0�̎�
			// �ړI�̈ʒu�ƌ����̎Z�o
			(Parts + nCntParts)->posDest = ((Parts + nCntParts)->posOrigin + motion->keySet[motion->nCntKeySet].key[nCntParts].pos) - (Parts + nCntParts)->pos;
			(Parts + nCntParts)->rotDest = ((Parts + nCntParts)->rotOrigin + motion->keySet[motion->nCntKeySet].key[nCntParts].rot) - (Parts + nCntParts)->rot;

			// �p�x�̐��K��
			NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
			
		}

		// �ϐ��錾
		D3DXVECTOR3 addPos = D3DXVECTOR3((Parts + nCntParts)->posDest / (float)(motion->keySet[motion->nCntKeySet].nFrame));
		D3DXVECTOR3 addRot = D3DXVECTOR3((Parts + nCntParts)->rotDest / (float)(motion->keySet[motion->nCntKeySet].nFrame));

		// �ʒu�̉��Z
		(Parts + nCntParts)->pos += addPos;

		//	�����̉��Z
		(Parts + nCntParts)->rot += addRot;

		// �p�x�̐��K��
		NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
	}

	// �t���[���J�E���g�̉��Z
	motion->nCntFrame++;
	
	if (motion->nCntFrame >= motion->keySet[motion->nCntKeySet].nFrame)
	{// �t���[���J�E���g���w��̃t���[�����𒴂����ꍇ
		// �t���[�����̏�����
		motion->nCntFrame = 0;

		// �Đ����̃L�[�ԍ����̉��Z
		motion->nCntKeySet++;

		if (motion->nCntKeySet >= motion->nNumKey && motion->bLoop)
		{// �Đ����̃L�[���J�E���g���L�[���̍ő�l�𒴂����Ƃ��A���̃��[�V���������[�v���g�p���Ă���
			// �Đ����̃L�[���J�E���g��������
			motion->nCntKeySet = 0;

		}
		else if (motion->nCntKeySet >= motion->nNumKey)
		{
			motion->nCntKeySet = 0;
			bMotion = false;
		}
	}

	return bMotion;
}

//---------------------------------------------------------------------------
// ���[�V�����u�����h����
//---------------------------------------------------------------------------
bool MotionBlend(int nCntMotionSet,						// ���[�V�����̔z��ԍ�
	PARTS *Parts,										// �p�[�c���
	int nMaxParts,										// �p�[�c��
	MOTION *motion)										// ���[�V�������			
{
	// �ϐ��錾
	bool bMotionBlend = true;

	for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
	{
		if ((motion + nCntMotionSet)->nCntFrame == 0)
		{// �t���[���J�E���g��0�̎�
			// �ړI�̈ʒu�ƌ����̎Z�o
			(Parts + nCntParts)->posDest = ((Parts + nCntParts)->posOrigin + (motion + nCntMotionSet)->keySet[(motion + nCntMotionSet)->nCntKeySet].key[nCntParts].pos) - (Parts + nCntParts)->pos;
			(Parts + nCntParts)->rotDest = ((Parts + nCntParts)->rotOrigin + (motion + nCntMotionSet)->keySet[(motion + nCntMotionSet)->nCntKeySet].key[nCntParts].rot) - (Parts + nCntParts)->rot;

			// �p�x�̐��K��
			NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
		}

		// �ϐ��錾
		D3DXVECTOR3 addPos = D3DXVECTOR3((Parts + nCntParts)->posDest / (float)(MOTION_BLEND_FRAM));
		D3DXVECTOR3 addRot = D3DXVECTOR3((Parts + nCntParts)->rotDest / (float)(MOTION_BLEND_FRAM));

		// �ʒu�̉��Z
		(Parts + nCntParts)->pos += addPos;

		//	�����̉��Z
		(Parts + nCntParts)->rot += addRot;

		// �p�x�̐��K��
		NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
	}

	// �t���[���J�E���g�̉��Z
	(motion + nCntMotionSet)->nCntFrame++;

	if ((motion + nCntMotionSet)->nCntFrame >= MOTION_BLEND_FRAM)
	{// �t���[���J�E���g���w��̃t���[�����𒴂����ꍇ
		// �t���[�����̏�����
		(motion + nCntMotionSet)->nCntFrame = 0;

		// �Đ����̃L�[�ԍ����̉��Z
		(motion + nCntMotionSet)->nCntKeySet++;

		bMotionBlend = false;
	}

	return bMotionBlend;
}

//---------------------------------------------------------------------------
// ���[�V�����ǂݍ��ݏ���
//---------------------------------------------------------------------------
void LoodSetMotion(char *pFileName, PARTSFILE *PartsFile, PARTS *Parts, MOTION *Motion,int *nMaxParts)
{
	// �ϐ��錾
	char aString[128] = {};			// �������r�p�̕ϐ�
	char g_aEqual[128] = {};		// ���ǂݍ��ݗp�ϐ�
	int	nCntKeySet = 0;				// KeySet�J�E���g
	int	nCntKey = 0;				// Key�J�E���g

	// �t�@�C���|�C���^�̐錾
	FILE * pFile;

	//�t�@�C�����J��
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{// �����񂪈�v�����ꍇ
			aString[0] = {};
			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aString[0]);
		}
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strncmp(&aString[0], "#", 1) == 0)
			{// ���ǂݍ���
				fgets(&aString[0], sizeof(aString), pFile);
			}

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{// �t�@�C�����̓ǂݍ���
				fscanf(pFile, "%s", &g_aEqual[0]);
				fscanf(pFile, "%s", &PartsFile->aName[0]);
				PartsFile++;
			}

			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{// �L�����N�^�[�̓ǂݍ���
				while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// ���ǂݍ���
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "NUM_PARTS") == 0)
					{// �ǂݍ��ރp�[�c��
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &*nMaxParts);
					}

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{// �p�[�c�̓ǂݍ���
						while (strcmp(&aString[0], "END_PARTSSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// ���ǂݍ���
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "INDEX") == 0)
							{// �^�C�v�̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &Parts->nType);
							}
							if (strcmp(&aString[0], "PARENT") == 0)
							{// �e�̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &Parts->nIdxModelParent);
							}
							if (strcmp(&aString[0], "POS") == 0)
							{// �ʒu�̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &Parts->pos.x);
								fscanf(pFile, "%f", &Parts->pos.y);
								fscanf(pFile, "%f", &Parts->pos.z);
							}
							if (strcmp(&aString[0], "ROT") == 0)
							{// �����̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &Parts->rot.x);
								fscanf(pFile, "%f", &Parts->rot.y);
								fscanf(pFile, "%f", &Parts->rot.z);
							}
						}

						// �p�[�c�J�E���g�̃C���N�������g
						Parts++;
					}
				}
			}
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{// ���[�V�����̓ǂݍ���
				while (strcmp(&aString[0], "END_MOTIONSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// ���ǂݍ���
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "LOOP") == 0)
					{// ���[�v�L���̓ǂݍ���
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", (int*)(&Motion->bLoop));
					}
					if (strcmp(&aString[0], "NUM_KEY") == 0)
					{// �L�[���̓ǂݍ���
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &Motion->nNumKey);
					}
					if (strcmp(&aString[0], "KEYSET") == 0)
					{// �L�[�Z�b�g�̓ǂݍ���
						while (strcmp(&aString[0], "END_KEYSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// ���ǂݍ���
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "FRAME") == 0)
							{// �t���[�����̓ǂݍ���
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &Motion->keySet[nCntKeySet].nFrame);
							}
							if (strcmp(&aString[0], "KEY") == 0)
							{// �L�[�̓ǂݍ���
								while (strcmp(&aString[0], "END_KEY") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									if (strncmp(&aString[0], "#", 1) == 0)
									{// ���ǂݍ���
										fgets(&aString[0], sizeof(aString), pFile);
									}

									if (strcmp(&aString[0], "POS") == 0)
									{// �ʒu�̓ǂݍ���
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].pos.x);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].pos.y);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].pos.z);
									}
									if (strcmp(&aString[0], "ROT") == 0)
									{// �����̓ǂݍ���
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].rot.x);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].rot.y);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].rot.z);
									}
								}

								// �L�[�J�E���g�̃C���N�������g
								nCntKey++;
							}
						}

						// �L�[�J�E���g�̏�����
						nCntKey = 0;

						// �L�[�Z�b�g�J�E���g�̃C���N�������g
						nCntKeySet++;
					}
				}
				// �L�[�Z�b�g�J�E���g�̏�����
				nCntKeySet = 0;

				// �p�[�c���̃C���N�������g
				Motion++;
			}

		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}