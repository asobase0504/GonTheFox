//==================================================
//
// �v���C���[�ݒ�
// Author:hamada ryuuga
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "utility.h"
#include "title.h"
#include "sound.h"
#include "motion.h"
#include "game.h"
//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_ENEMY	(100)	// �ő�G�l�~�[��
#define SIZE_ENEMY (D3DXVECTOR3(50.0f,50.0f,0.0f))
//--------------------------------------------------
// �ÓI�ϐ�
//--------------------------------------------------
static Enemy s_Enemy[MAX_ENEMY];		// �G�l�~�[�̍\����
static Enemy s_EnemyType[ENEMY_TYPE_MAX];	// �G�l�~�[��ʂ̍\����
static MODELDATAPLAYER s_ModelData[MAX_MOVE];

static int s_parts;	// �p�[�c�̍ő吔
static int s_pow;	// �W�����v�p���[
static int nMotion; //

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void Loadmotion(MODELDATAPLAYER* set, int Setnumber);	// ���[�V�����̓Ǎ�
static void AnimationSet(int animation);						// �A�j���[�V�����̌v�Z
static void MoveSet(void);										// ���[�u�Z�b�g
static void Collision(void);									// �����蔻��܂Ƃ�
static void SetCopy(void);

//==================================================
// ����������
//==================================================
void InitEnemy(void)
{
	// �J�����̃f�[�^�擾
	Camera *pCamera = GetCamera();

	s_pow = 0;

	// ����������
	memset(&s_Enemy, NULL, sizeof(s_Enemy));

	// �Ǎ�
	LoadEnemy();

	// �G�l�~�[�̐ݒu
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),ENEMY_TYPE_HUMANSOUL);
	SetEnemy(D3DXVECTOR3(20.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMY_TYPE_SKELETON);
}

//=========================================
// �I������
//=========================================
void UninitEnemy(void)
{
	StopSound();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		for (int i = 0; i <= s_parts; i++)
		{
			// ���_�o�b�t�@�[�̉��
			if (pEnemy->parts[i].pBuffer != NULL)
			{
				pEnemy->parts[i].pBuffer->Release();
				pEnemy->parts[i].pBuffer = NULL;
			}
			if (pEnemy->parts[i].pMesh != NULL)
			{
				pEnemy->parts[i].pMesh->Release();
				pEnemy->parts[i].pMesh = NULL;
			}
		}
	}
}

//=========================================
// �X�V����
//=========================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		// ���݂̃��[�V�����ԍ��̕ۊ�
		pEnemy->motionTypeOld = pEnemy->motionType;

		if (!pEnemy->bMotion)
		{// �g�p���Ă郂�[�V�������Ȃ��ꍇ
			pEnemy->motionType = ANIME_NORMAL;
		}

		MoveSet();	// �����Z�b�g

		Collision();	// ��

		// �A�j���[�V�����⑫���̐ݒ�
		if (!pEnemy->notLoop)
		{
			pEnemy->motionType = ANIME_NORMAL;
		}

		if (s_pow >= 1 && s_pow <= 10)
		{// �W�����v�V�X�e��
			s_pow++;
			pEnemy->move.y = 1.00f * s_pow;
		}

		pEnemy->move.y -= 1.0f;
		if (GetKeyboardPress(DIK_B))
		{
			pEnemy->motionType = ANIME_ATTACK;//�U��

			pEnemy->notLoop = true;
		}
		if (GetKeyboardPress(DIK_N))
		{
			pEnemy->notLoop = true;
		}

		if (pEnemy->motionTypeOld != pEnemy->motionType)
		{// ���[�V�����^�C�v���ύX���ꂽ��
			pEnemy->motion[pEnemy->motionTypeOld].nCntFrame = 0;
			pEnemy->motion[pEnemy->motionTypeOld].nCntKeySet = 0;
			pEnemy->bMotionBlend = true;
		}

		if (pEnemy->bMotionBlend)
		{// ���[�V�����u�����h���g�p���Ă�
			pEnemy->bMotionBlend = MotionBlend((int)(pEnemy->motionType),	// ���[�V�����̔z��ԍ�
				&pEnemy->parts[0],											// �p�[�c���
				pEnemy->nMaxModelParts,										// �p�[�c��
				&pEnemy->motion[0]);										// ���[�V�������	
		}
		else if (!pEnemy->bMotionBlend)
		{// ���[�V�����u�����h���g�p���ĂȂ��ꍇ
			pEnemy->bMotion = PlayMotion(pEnemy->nMaxModelParts,			// �p�[�c��
				&pEnemy->parts[0],											// �p�[�c���
				&pEnemy->motion[(int)(pEnemy->motionType)]);				// ���[�V�������
		}
	}
}

//=========================================
// �`�揈��
//=========================================
void DrawEnemy(void)
{
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat = {};
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)//�g���Ă��o��
		{
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)
		D3DXMatrixIdentity(&pEnemy->mtxWorld);

		// �g�k�𔽉f
		// �s��g�k�֐�
		D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxScale);

		// �����𔽉f
		// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pEnemy->rot.y, pEnemy->rot.x, pEnemy->rot.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixTranslation(&mtxTrans, pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxTrans);

		// �p�[�c�̕`��ݒ�
		SetParts(pEnemy->nMaxModelParts,	// �p�[�c��
			&pEnemy->parts[0],				// �p�[�c���
			pEnemy->mtxWorld,				// ���[���h�}�g���b�N�X
			mtxRot,							// �v�Z�p�}�g���b�N�X
			mtxTrans,						// �v�Z�p�}�g���b�N�X
			&marDef,						// �}�e���A���ۑ��ϐ�
			pMat);							// �}�e���A���f�[�^

		//���݂̃}�e���A�������ɖ߂�
		GetDevice()->SetMaterial(&marDef);
	}
}

//=========================================
// �ݒ�
//=========================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot,ENEMY_TYPE type)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (pEnemy->isUse)
		{
			continue;
		}

		*pEnemy = s_EnemyType[type];
		// �v���C���[���̏�����
		pEnemy->pos = pos;									// �ʒu�̏�����
		pEnemy->posOld = pEnemy->pos;						// �ߋ��ʒu�̏�����
		pEnemy->rot = rot;									// �����̏�����
		pEnemy->mtxWorld = {};								// ���[���h�}�g���b�N�X
		pEnemy->motionType = ANIME_NORMAL;					// �j���[�g�������[�V����
		pEnemy->motionTypeOld = pEnemy->motionType;			// �O��̃��[�V����
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		pEnemy->bMotionBlend = false;						// ���[�V�����u�����h�̎g�p��
		pEnemy->isUse = true;								// �g�p��

		break;
	}
}

//--------------------------
//�����蔻��̃T�C�Y�����Ă�
//--------------------------
void SizeSet(void)
{

}

//------------------------------
//�A�j���[�V�����Z�b�g
//-------------------------------
void AnimationSet(int animation)
{

}

//------------------------------
//�����Z�b�g
//-------------------------------
void MoveSet(void)
{

	
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		
	//	bool bHit = cLnhale[0].Hit(pEnemy->pos, SIZE_ENEMY);

		/*if (bHit)
		{
			pEnemy->isUse = false;
		}*/
	}
}
//-------------------------------
//�����蔻��܂Ƃ�
//-------------------------------
void Collision(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		if (pEnemy->pos.y <= 0.0f)
		{
			pEnemy->pos.y = 0.0f;
		}
	}
}

//-------------------------------
//���[�V���������[�h���鏈��
//-------------------------------
void  Loadmotion(MODELDATAPLAYER* set, int Setnumber)
{
	s_ModelData[nMotion] = *set;
	nMotion++;
}

//-------------------------------
//�R�s�[������
//-------------------------------
void SetCopy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		nMotion = 0;
		if (s_parts >= 8)
		{
			s_parts = 7;
		}

		switch (pEnemy->copy)
		{
		case COPY_SWORD:
			break;
		case COPY_FIRE:
			break;
		case COPY_LASER:
			break;
		case COPY_CUTTER:
			break;
		default:
			break;
		}
	}
}

//----------------------
// �Q�b�g(�\����)
//----------------------
Enemy *GetEnemy(void)
{
	return s_Enemy;
}

//----------------------
// �Ǎ�
//----------------------
void LoadEnemy(void)
{
	FILE *pFile = fopen("Data/system/enemy/enemy.txt","r");

	if (pFile == NULL)
	{//�t�@�C�����J�����ꍇ
		assert(false);
		return;
	}
		
	char aString[128] = {};			// �������r�p�̕ϐ�
	char aEqual[128] = {};		// ���ǂݍ��ݗp�ϐ�
	char fileName[ENEMY_TYPE_MAX][128];
	int fileCnt = 0;

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
			fscanf(pFile, "%s", &aEqual[0]);
			fscanf(pFile, "%s", fileName[fileCnt]);

			Enemy* pEnemy = &s_EnemyType[fileCnt];
			if (pEnemy->isUse)
			{
				continue;
			}

			// �v���C���[���̏�����
			pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏�����
			pEnemy->posOld = pEnemy->pos;					// �ߋ��ʒu�̏�����
			pEnemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;	// �����̏�����
			pEnemy->modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);		// ���_���W�̍ŏ��l
			pEnemy->modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// ���_���W�̍ő�l
			pEnemy->mtxWorld = {};								// ���[���h�}�g���b�N�X
			pEnemy->motionType = ANIME_NORMAL;					// �j���[�g�������[�V����
			pEnemy->motionTypeOld = pEnemy->motionType;			// �j���[�g�������[�V����
			pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
			pEnemy->bMotionBlend = false;						// ���[�V�����u�����h�̎g�p��
			pEnemy->isUse = true;								// �g�p��
			pEnemy->bMotionBlend = false;						// �v���C���[���f�B�X�N�������Ă��Ȃ�

			// �t�@�C���̓ǂݍ���
			LoodSetMotion(fileName[fileCnt], pEnemy->partsFile, pEnemy->parts, pEnemy->motion, &pEnemy->nMaxModelParts);

			for (int i = 0; i < pEnemy->nMaxModelParts; i++)
			{
				Parts* pParts = &pEnemy->parts[i];

				// �ʒu�ƌ����̏����l��ۑ�
				pParts->posOrigin = pParts->pos;
				pParts->rotOrigin = pParts->rot;

				// �p�[�c���̏�����
				pParts->mtxWorld = {};	// ���[���h�}�g���b�N�X
				pParts->vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);	// ���_���W�̍ŏ��l
				pParts->vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// ���_���W�̍ő�l

				// X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(pEnemy->partsFile[pParts->nType].aName,
					D3DXMESH_SYSTEMMEM,
					GetDevice(),
					NULL,
					&pParts->pBuffer,
					NULL,
					&pParts->nNumMat,
					&pParts->pMesh);

				// ���_���W�̍ŏ��l�E�ő�l�̎Z�o
				int nNumVtx;	// ���_��
				DWORD sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
				BYTE *pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

				// ���_���̎擾
				nNumVtx = pParts->pMesh->GetNumVertices();

				// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
				sizeFVF = D3DXGetFVFVertexSize(pParts->pMesh->GetFVF());

				// ���_�o�b�t�@�̃��b�N
				pParts->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					// ���_���W�̑��
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

					if (vtx.x < pParts->vtxMin.x)
					{// ��r�Ώۂ����݂̒��_���W(X)�̍ŏ��l��菬����
						pParts->vtxMin.x = vtx.x;
					}
					if (vtx.y < pParts->vtxMin.y)
					{// ��r�Ώۂ����݂̒��_���W(Y)�̍ŏ��l��菬����
						pParts->vtxMin.y = vtx.y;
					}
					if (vtx.z < pParts->vtxMin.z)
					{// ��r�Ώۂ����݂̒��_���W(Z)�̍ŏ��l��菬����
						pParts->vtxMin.z = vtx.z;
					}

					if (vtx.x > pParts->vtxMax.x)
					{// ��r�Ώۂ����݂̒��_���W(X)�̍ő�l���傫��
						pParts->vtxMax.x = vtx.x;
					}
					if (vtx.y > pParts->vtxMax.y)
					{// ��r�Ώۂ����݂̒��_���W(Y)�̍ő�l���傫��
						pParts->vtxMax.y = vtx.y;
					}
					if (vtx.z > pParts->vtxMax.z)
					{// ��r�Ώۂ����݂̒��_���W(Z)�̍ő�l���傫��
						pParts->vtxMax.z = vtx.z;
					}

					// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
					pVtxBuff += sizeFVF;
				}

				if (pParts->vtxMin.x < pEnemy->modelMin.x)
				{// ��r�Ώۂ����݂̒��_���W(X)�̍ŏ��l��菬����
					pEnemy->modelMin.x = pParts->vtxMin.x;
				}
				if (pParts->vtxMin.y < pEnemy->modelMin.y)
				{// ��r�Ώۂ����݂̒��_���W(Y)�̍ŏ��l��菬����
					pEnemy->modelMin.y = pParts->vtxMin.y;
				}
				if (pParts->vtxMin.z < pEnemy->modelMin.z)
				{// ��r�Ώۂ����݂̒��_���W(Z)�̍ŏ��l��菬����
					pEnemy->modelMin.z = pParts->vtxMin.z;
				}

				if (pParts->vtxMax.x > pEnemy->modelMax.x)
				{// ��r�Ώۂ����݂̒��_���W(X)�̍ő�l���傫��
					pEnemy->modelMax.x = pParts->vtxMax.x;
				}
				if (pParts->vtxMax.y > pEnemy->modelMax.y)
				{// ��r�Ώۂ����݂̒��_���W(Y)�̍ő�l���傫��
					pEnemy->modelMax.y = pParts->vtxMax.y;
				}
				if (pParts->vtxMax.z > pEnemy->modelMax.z)
				{// ��r�Ώۂ����݂̒��_���W(Z)�̍ő�l���傫��
					pEnemy->modelMax.z = pParts->vtxMax.z;
				}

				// ���_�o�b�t�@�̃A�����b�N
				pParts->pMesh->UnlockVertexBuffer();
			}
			fileCnt++;
		}
	}
}
