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
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "utility.h"
#include "title.h"
#include "sound.h"
#include "motion.h"

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_ENEMY	(100)

//--------------------------------------------------
// �ÓI�ϐ�
//--------------------------------------------------
static Enemy s_Enemy[MAX_ENEMY];	// �|���S���̍\����
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
	Camera *pCamera;
	pCamera = GetCamera();

	s_pow = 0;

	// ����������
	memset(&s_Enemy, NULL, sizeof(s_Enemy));

	// ����������
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		pEnemy->rotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
		pEnemy->damege = DAMEGE_NORMAL;
		pEnemy->MotionType = ANIME_NORMAL;		//���܎g���Ă�motio�̔ԍ�
		pEnemy->MotionTypeOld = pEnemy->MotionType;
		s_parts = 0;

		pEnemy->modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		pEnemy->modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		pEnemy->use = true;
	}

	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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
			if (pEnemy->Parts[i].pBuffer != NULL)
			{
				pEnemy->Parts[i].pBuffer->Release();
				pEnemy->Parts[i].pBuffer = NULL;
			}
			if (pEnemy->Parts[i].pMesh != NULL)
			{
				pEnemy->Parts[i].pMesh->Release();
				pEnemy->Parts[i].pMesh = NULL;
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

		if (!pEnemy->use)
		{
			continue;
		}

		// ���݂̃��[�V�����ԍ��̕ۊ�
		pEnemy->MotionTypeOld = pEnemy->MotionType;

		if (!pEnemy->bMotion)
		{// �g�p���Ă郂�[�V�������Ȃ��ꍇ
			pEnemy->MotionType = ANIME_NORMAL;
		}

		MoveSet();		// �����Z�b�g

		Collision();	// ��

		// �A�j���[�V�����⑫���̐ݒ�
		if (!pEnemy->notLoop)
		{
			pEnemy->MotionType = ANIME_NORMAL;
		}
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_S) ||
			GetJoypadPress(JOYKEY_UP, 0) || GetJoypadPress(JOYKEY_DOWN, 0) || GetJoypadPress(JOYKEY_LEFT, 0) || GetJoypadPress(JOYKEY_RIGHT, 0))
		{
			pEnemy->MotionType = ANIME_RUN;	// ����

		}


		if (GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A, 0) || GetJoypadPress(JOYKEY_UP, 0))
		{//SPACE�L�[�������ꂽ
			pEnemy->pos.y = pEnemy->pos.y + 5.0f;
			s_pow++;
			pEnemy->MotionType = ANIME_JUMP;

		}
		else
		{
			s_pow = 0;
		}
		if (s_pow >= 1 && s_pow <= 10)
		{//�W�����v�V�X�e��
			s_pow++;
			pEnemy->move.y = 1.00f* s_pow;
		}


		pEnemy->move.y -= 1.0f;
		if (GetKeyboardPress(DIK_B))
		{
			pEnemy->MotionType = ANIME_ATTACK;//�U��

			pEnemy->notLoop = true;
		}
		if (GetKeyboardPress(DIK_N))
		{
			pEnemy->notLoop = true;
		}

		////�e�X�V
		////SetposShadow(pEnemy->nShadow, D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z));


		//AnimationSet((int)pEnemy->motion);//�A�j���[�V�����Đ�

		if (pEnemy->MotionTypeOld != pEnemy->MotionType)
		{// ���[�V�����^�C�v���ύX���ꂽ��
			pEnemy->motion[pEnemy->MotionTypeOld].nCntFrame = 0;
			pEnemy->motion[pEnemy->MotionTypeOld].nCntKeySet = 0;
			pEnemy->bMotionBlend = true;
		}

		if (pEnemy->bMotionBlend)
		{// ���[�V�����u�����h���g�p���Ă�
			pEnemy->bMotionBlend = MotionBlend((int)(pEnemy->MotionType),				// ���[�V�����̔z��ԍ�
				&pEnemy->Parts[0],														// �p�[�c���
				pEnemy->nMaxModelParts,												// �p�[�c��
				&pEnemy->motion[0]);													// ���[�V�������	
		}
		else if (!pEnemy->bMotionBlend)
		{// ���[�V�����u�����h���g�p���ĂȂ��ꍇ
			pEnemy->bMotion = PlayMotion(pEnemy->nMaxModelParts,						// �p�[�c��
				&pEnemy->Parts[0],														// �p�[�c���
				&pEnemy->motion[(int)(pEnemy->MotionType)]);							// ���[�V�������
		}
	}
}

//=========================================
// �`�揈��
//=========================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat= {};
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (pEnemy->use)//�g���Ă��o��
		{
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
			SetParts(pEnemy->nMaxModelParts,					// �p�[�c��
				&pEnemy->Parts[0],								// �p�[�c���
				pEnemy->mtxWorld,								// ���[���h�}�g���b�N�X
				mtxRot,											// �v�Z�p�}�g���b�N�X
				mtxTrans,										// �v�Z�p�}�g���b�N�X
				&marDef,										// �}�e���A���ۑ��ϐ�
				pMat);											// �}�e���A���f�[�^

			//���݂̃}�e���A�������ɖ߂�
			pDevice->SetMaterial(&marDef);
		}
	}
}

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (pEnemy->use)
		{
			continue;
		}

		// �v���C���[���̏�����
		pEnemy->pos = pos;									// �ʒu�̏�����
		pEnemy->posOld = pEnemy->pos;						// �ߋ��ʒu�̏�����
		pEnemy->posOld = pEnemy->pos;						// �ߋ��ʒu�̏�����
		pEnemy->rot = rot;									// �����̏�����
		pEnemy->modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);		// ���_���W�̍ŏ��l
		pEnemy->modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// ���_���W�̍ő�l
		pEnemy->mtxWorld = {};								// ���[���h�}�g���b�N�X
		//pEnemy->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړI�̌���
		pEnemy->MotionType = ANIME_NORMAL;					// �j���[�g�������[�V����
		pEnemy->MotionTypeOld = pEnemy->MotionType;			// �j���[�g�������[�V����
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		pEnemy->bMotionBlend = false;						// ���[�V�����u�����h�̎g�p��
		pEnemy->use = true;									// �g�p��
		pEnemy->bMotionBlend = false;						// �v���C���[���f�B�X�N�������Ă��Ȃ�
		//g_nIdxShadow[nCntEnemy] = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//�e�̐ݒ菈���̏�����

		// �t�@�C���̓ǂݍ���
		LoodSetMotion("Data/system/Fox.txt", &pEnemy->PartsFile[0], &pEnemy->Parts[0], &pEnemy->motion[0], &pEnemy->nMaxModelParts);

		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		for (int i = 0; i < pEnemy->nMaxModelParts; i++)
		{
			PARTS* pParts = &pEnemy->Parts[i];

			// �ʒu�ƌ����̏����l��ۑ�
			pParts->posOrigin = pParts->pos;
			pParts->rotOrigin = pParts->rot;

			// �p�[�c���̏�����
			pParts->mtxWorld = {};																// ���[���h�}�g���b�N�X
			pParts->vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);		// ���_���W�̍ŏ��l
			pParts->vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// ���_���W�̍ő�l

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(&pEnemy->PartsFile[pParts->nType].aName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&pParts->pBuffer,
				NULL,
				&pParts->nNumMat,
				&pParts->pMesh);

			// ���_���W�̍ŏ��l�E�ő�l�̎Z�o
			int			nNumVtx;	// ���_��
			DWORD		sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE		*pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

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

		if (!pEnemy->use)
		{
			continue;
		}

		Camera *pCamera = GetCamera();

		pEnemy->consumption = 0.0f;
		int nDash = 1;
		if (GetKeyboardPress(DIK_LSHIFT) || GetKeyboardPress(DIK_RSHIFT) || GetJoypadPress(JOYKEY_A, 0))
		{//�V�t�g�_�b�V��
			nDash = 3;
		}
		//�Q�[���̎��̈ړ����@
		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > 0.5f)
		{//�E�ɔ�����葽���|������
			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -0.5f)
		{//���ɔ�����葽���|������
			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > 0.5f)
		{//��ɔ�����葽���|������
			pEnemy->move.x += sinf(pCamera->rot.y - D3DX_PI) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y - D3DX_PI) * Speed * nDash;

			pEnemy->consumption = (pEnemy->rotMove.x -
				D3DX_PI - pEnemy->rot.y
				+ pCamera->rot.y);
		}
		else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -0.5f)
		{//���ɔ�����葽���|������
			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP, 0))
		{//W�������Ƃ������

			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN, 0))
		{//S�������Ɖ�����

			pEnemy->move.x += sinf(pCamera->rot.y - D3DX_PI) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y - D3DX_PI) * Speed * nDash;

			pEnemy->consumption = (pEnemy->rotMove.x -
				D3DX_PI - pEnemy->rot.y
				+ pCamera->rot.y);
		}
		if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT, 0))
		{//D�������Ƃ��E����

			pEnemy->move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;


			pEnemy->consumption = pEnemy->rotMove.x + (D3DX_PI*0.5f) - pEnemy->rot.y + pCamera->rot.y;
		}

		if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT, 0))
		{//A�������ƍ�����

			pEnemy->move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - (D3DX_PI*0.5f) - pEnemy->rot.y + pCamera->rot.y;

		}
		pEnemy->posOld = pEnemy->pos;	// �ߋ��̈ړ��ʂ�ۑ�

		if (pEnemy->invincible <= 0)
		{// ���G���Ԃ��[���ɂȂ�����_���[�W���炤�悤�ɂ���
			pEnemy->damege = DAMEGE_NORMAL;
		}

		pEnemy->move.x += (0.0f - pEnemy->move.x) * 0.5f;	//�i�ړI�̒l-���݂̒l�j�������W��
		pEnemy->move.z += (0.0f - pEnemy->move.z) * 0.5f;
		pEnemy->pos += pEnemy->move;// �ړ������Z

		// ���K��
		if (pEnemy->consumption > D3DX_PI)
		{
			pEnemy->consumption += D3DX_PI * 2;
		}
		if (pEnemy->consumption < -D3DX_PI)
		{
			pEnemy->consumption += -D3DX_PI * 2;
		}

		//���Z�ݒ�i�����j
		pEnemy->rot.y += (pEnemy->consumption)* Attenuation;	// �ړI�̒l-���݂̒l�j�������W��

		//���K��
		if (pEnemy->rot.y > D3DX_PI)
		{
			pEnemy->rot.y += -D3DX_PI * 2;
		}
		if (pEnemy->rot.y <= -D3DX_PI)
		{
			pEnemy->rot.y += D3DX_PI * 2;
		}
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

		if (!pEnemy->use)
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

		if (!pEnemy->use)
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
//�Q�b�g(�\����)
//----------------------
Enemy *GetEnemy(void)
{
	return s_Enemy;
}
