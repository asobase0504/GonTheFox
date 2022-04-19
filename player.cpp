//============================
//
// �v���C���[�ݒ�
// Author:hamada ryuuga
//
//============================

#include <stdio.h>
#include "Player.h"
#include "input.h"
#include "camera.h"
#include "utility.h"
#include "title.h"
#include "sound.h"
#include "motion.h"

//------------------------------------
// static�ϐ�
//------------------------------------
static PLAYER s_Player;	// �|���S���̍\����
static MODELDATAPLAYER s_ModelData[MAX_MOVE];

static int s_time, s_parts;//�^�C�}�[�ƃp�[�c�̍ő吔
static int s_pow;//�W�����v�p���[
static int nMotion;//

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void SetCopy(void);		// �R�s�[�̐ݒ�
static void Collision(void);	// �����蔻��܂Ƃ�
static void SizSet(void);//�����蔻��擾
static void AnimationSet(int animation);//�A�j���[�V�����̌v�Z
static void MoveSet(void);	//���[�u�Z�b�g
static MODELDATAPLAYER *GetModelData(void);//motion�f�[�^�̃Q�b�g

//=========================================
// ����������
//=========================================
void InitPlayer(void)
{
	//�J�����̃f�[�^�擾
	Camera *pCamera;
	pCamera = GetCamera();

	s_pow = 0;

	// ����������
	memset(&s_Player, NULL, sizeof(s_Player));

	// ����������
	//�c�@�@�@�@�@�@�@�@�@�@//��
	s_Player.rotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
	s_Player.damege = DAMEGE_NORMAL;
	s_Player.MotionType = ANIME_NORMAL;				//���܎g���Ă�motio�̔ԍ�
	s_Player.MotionTypeOld = s_Player.MotionType;
	s_parts = 0;

	s_Player.modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_Player.modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	s_Player.isUse = true;

	s_time = 0;

	SetPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=========================================
// �I������
//=========================================
void UninitPlayer(void)
{
	StopSound();

	for (int i = 0; i <= MAX_MODELPARTS; i++)
	{
		// ���_�o�b�t�@�[�̉��
		if (s_Player.Parts[i].pBuffer != NULL)
		{
			s_Player.Parts[i].pBuffer->Release();
			s_Player.Parts[i].pBuffer = NULL;
		}
		if (s_Player.Parts[i].pMesh != NULL)
		{
			s_Player.Parts[i].pMesh->Release();
			s_Player.Parts[i].pMesh = NULL;
		}
	}
}

//=========================================
// �X�V����
//=========================================
void UpdatePlayer(void)
{
	// ���݂̃��[�V�����ԍ��̕ۊ�
	s_Player.MotionTypeOld = s_Player.MotionType;

	if (!s_Player.bMotion)
	{// �g�p���Ă郂�[�V�������Ȃ��ꍇ
		s_Player.MotionType = ANIME_NORMAL;
	}

	MoveSet();	//�����Z�b�g

	Collision();//��

	//			//�A�j���[�V�����⑫���̐ݒ�
	if (s_Player.notLoop == false)
	{
		s_Player.MotionType = ANIME_NORMAL;
	}
	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_S) ||
		GetJoypadPress(JOYKEY_UP, 0) || GetJoypadPress(JOYKEY_DOWN, 0) || GetJoypadPress(JOYKEY_LEFT, 0) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{
		s_Player.MotionType = ANIME_RUN;//����

	}


	if (GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A, 0) || GetJoypadPress(JOYKEY_UP, 0))
	{//SPACE�L�[�������ꂽ
		s_Player.pos.y = s_Player.pos.y + 5.0f;
		s_pow++;
		s_Player.MotionType = ANIME_JUMP;

	}
	else
	{
		s_pow = 0;
	}
	if (s_pow >= 1 && s_pow <= 10)
	{//�W�����v�V�X�e��
		s_pow++;
		s_Player.move.y = 1.00f* s_pow;
	}


	s_Player.move.y -= 1.0f;
	if (GetKeyboardPress(DIK_B))
	{
		s_Player.MotionType = ANIME_ATTACK;//�U��

		s_Player.notLoop = true;
	}
	if (GetKeyboardPress(DIK_N))
	{
		s_Player.notLoop = true;
	}
	if (GetKeyboardPress(DIK_J))
	{

		s_Player.copy = COPY_SWORD;
		// �t�@�C���̓ǂݍ���
		SetCopy("Data/system/sword.txt", &s_Player.PartsFile[7], &s_Player.Parts[7], &s_Player.motion[0], &s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_H))
	{
		s_Player.copy = COPY_FIRE;
		// �t�@�C���̓ǂݍ���
		SetCopy("Data/system/flare.txt", &s_Player.PartsFile[7], 
					&s_Player.Parts[7], &s_Player.motion[0],
					&s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_F))
	{
		s_Player.copy = COPY_LASER;
		// �t�@�C���̓ǂݍ���	
		SetCopy("Data/system/Laser.txt", &s_Player.PartsFile[7], &s_Player.Parts[7], &s_Player.motion[0], &s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_G))
	{
		s_Player.copy = COPY_CUTTER;
		// �t�@�C���̓ǂݍ���
		SetCopy("Data/system/Cutter.txt", &s_Player.PartsFile[7], &s_Player.Parts[7], &s_Player.motion[0], &s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_K))
	{
		s_Player.copy = COPY_NORMAL;
		s_Player.nMaxModelParts = 7;
	}
	////�e�X�V
	////SetposShadow(s_Player.nShadow, D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y, s_Player.pos.z));


	//AnimationSet((int)s_Player.motion);//�A�j���[�V�����Đ�

	if (s_Player.MotionTypeOld != s_Player.MotionType)
	{// ���[�V�����^�C�v���ύX���ꂽ��
		s_Player.motion[s_Player.MotionTypeOld].nCntFrame = 0;
		s_Player.motion[s_Player.MotionTypeOld].nCntKeySet = 0;
		s_Player.bMotionBlend = true;
	}

	if (s_Player.bMotionBlend)
	{// ���[�V�����u�����h���g�p���Ă�
		s_Player.bMotionBlend = MotionBlend((int)(s_Player.MotionType),				// ���[�V�����̔z��ԍ�
			&s_Player.Parts[0],														// �p�[�c���
			s_Player.nMaxModelParts,												// �p�[�c��
			&s_Player.motion[0]);													// ���[�V�������	
	}
	else if (!s_Player.bMotionBlend)
	{// ���[�V�����u�����h���g�p���ĂȂ��ꍇ
		s_Player.bMotion = PlayMotion(s_Player.nMaxModelParts,						// �p�[�c��
			&s_Player.Parts[0],														// �p�[�c���
			&s_Player.motion[(int)(s_Player.MotionType)]);							// ���[�V�������
	}
}

//=========================================
// �`�揈��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat= {};
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	if (s_Player.isUse)//�g���Ă��o��
	{
		// ���[���h�}�g���b�N�X�̏�����
		// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)
		D3DXMatrixIdentity(&s_Player.mtxWorld);

		// �g�k�𔽉f
		// �s��g�k�֐�
		D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&s_Player.mtxWorld, &s_Player.mtxWorld, &mtxScale);

		// �����𔽉f
		// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Player.rot.y, s_Player.rot.x, s_Player.rot.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&s_Player.mtxWorld, &s_Player.mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixTranslation(&mtxTrans, s_Player.pos.x, s_Player.pos.y, s_Player.pos.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&s_Player.mtxWorld, &s_Player.mtxWorld, &mtxTrans);

		// �p�[�c�̕`��ݒ�
		SetParts(s_Player.nMaxModelParts,					// �p�[�c��
			&s_Player.Parts[0],								// �p�[�c���
			s_Player.mtxWorld,								// ���[���h�}�g���b�N�X
			mtxRot,											// �v�Z�p�}�g���b�N�X
			mtxTrans,										// �v�Z�p�}�g���b�N�X
			&marDef,										// �}�e���A���ۑ��ϐ�
			pMat);											// �}�e���A���f�[�^
	

		//	//// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		pDevice->SetMaterial(&marDef);
	}
}



void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �v���C���[���̏�����
	s_Player.pos = pos;																		// �ʒu�̏�����
	s_Player.posOld = s_Player.pos;															// �ߋ��ʒu�̏�����
	s_Player.posOld = s_Player.pos;															// �ߋ��ʒu�̏�����
	s_Player.rot = rot;																		// �����̏�����
	s_Player.modelMin = D3DXVECTOR3(100, 100, 100);											// ���_���W�̍ŏ��l
	s_Player.modelMax = D3DXVECTOR3(-100, -100, -100);										// ���_���W�̍ő�l
	s_Player.mtxWorld = {};																	// ���[���h�}�g���b�N�X
	//s_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);										// �ړI�̌���
	s_Player.MotionType = ANIME_NORMAL;														// �j���[�g�������[�V����
	s_Player.MotionTypeOld = s_Player.MotionType;											// �j���[�g�������[�V����
	s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);											// �ړ���
	s_Player.bMotionBlend = false;															// ���[�V�����u�����h�̎g�p��
	s_Player.isUse = true;																	// �g�p��
	//g_nIdxShadow[nCntPlayer] = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//�e�̐ݒ菈���̏�����


	// �t�@�C���̓ǂݍ���
	LoodSetMotion("Data/system/Fox.txt", s_Player.PartsFile, s_Player.Parts, s_Player.motion, &s_Player.nMaxModelParts);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_Player.nMaxModelParts; i++)
	{
		// �ʒu�ƌ����̏����l��ۑ�
		s_Player.Parts[i].posOrigin = s_Player.Parts[i].pos;
		s_Player.Parts[i].rotOrigin = s_Player.Parts[i].rot;

		// �p�[�c���̏�����
		s_Player.Parts[i].mtxWorld = {};																// ���[���h�}�g���b�N�X
		s_Player.Parts[i].vtxMin = D3DXVECTOR3(1000, 1000, 1000);		// ���_���W�̍ŏ��l
		s_Player.Parts[i].vtxMax = D3DXVECTOR3(-1000, -1000, -1000);	// ���_���W�̍ő�l

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&s_Player.PartsFile[s_Player.Parts[i].nType].aName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&s_Player.Parts[i].pBuffer,
			NULL,
			&s_Player.Parts[i].nNumMat,
			&s_Player.Parts[i].pMesh);

		// ���_���W�̍ŏ��l�E�ő�l�̎Z�o
		int			nNumVtx;	// ���_��
		DWORD		sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE		*pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

								// ���_���̎擾
		nNumVtx = s_Player.Parts[i].pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
		sizeFVF = D3DXGetFVFVertexSize(s_Player.Parts[i].pMesh->GetFVF());

		// ���_�o�b�t�@�̃��b�N
		s_Player.Parts[i].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// ���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x < s_Player.Parts[i].vtxMin.x)
			{// ��r�Ώۂ����݂̒��_���W(X)�̍ŏ��l��菬����
				s_Player.Parts[i].vtxMin.x = vtx.x;
			}
			if (vtx.y < s_Player.Parts[i].vtxMin.y)
			{// ��r�Ώۂ����݂̒��_���W(Y)�̍ŏ��l��菬����
				s_Player.Parts[i].vtxMin.y = vtx.y;
			}
			if (vtx.z < s_Player.Parts[i].vtxMin.z)
			{// ��r�Ώۂ����݂̒��_���W(Z)�̍ŏ��l��菬����
				s_Player.Parts[i].vtxMin.z = vtx.z;
			}

			if (vtx.x > s_Player.Parts[i].vtxMax.x)
			{// ��r�Ώۂ����݂̒��_���W(X)�̍ő�l���傫��
				s_Player.Parts[i].vtxMax.x = vtx.x;
			}
			if (vtx.y > s_Player.Parts[i].vtxMax.y)
			{// ��r�Ώۂ����݂̒��_���W(Y)�̍ő�l���傫��
				s_Player.Parts[i].vtxMax.y = vtx.y;
			}
			if (vtx.z > s_Player.Parts[i].vtxMax.z)
			{// ��r�Ώۂ����݂̒��_���W(Z)�̍ő�l���傫��
				s_Player.Parts[i].vtxMax.z = vtx.z;
			}

			// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		if (s_Player.Parts[i].vtxMin.x < s_Player.modelMin.x)
		{// ��r�Ώۂ����݂̒��_���W(X)�̍ŏ��l��菬����
			s_Player.modelMin.x = s_Player.Parts[i].vtxMin.x;
		}
		if (s_Player.Parts[i].vtxMin.y < s_Player.modelMin.y)
		{// ��r�Ώۂ����݂̒��_���W(Y)�̍ŏ��l��菬����
			s_Player.modelMin.y = s_Player.Parts[i].vtxMin.y;
		}
		if (s_Player.Parts[i].vtxMin.z < s_Player.modelMin.z)
		{// ��r�Ώۂ����݂̒��_���W(Z)�̍ŏ��l��菬����
			s_Player.modelMin.z = s_Player.Parts[i].vtxMin.z;
		}

		if (s_Player.Parts[i].vtxMax.x > s_Player.modelMax.x)
		{// ��r�Ώۂ����݂̒��_���W(X)�̍ő�l���傫��
			s_Player.modelMax.x = s_Player.Parts[i].vtxMax.x;
		}
		if (s_Player.Parts[i].vtxMax.y > s_Player.modelMax.y)
		{// ��r�Ώۂ����݂̒��_���W(Y)�̍ő�l���傫��
			s_Player.modelMax.y = s_Player.Parts[i].vtxMax.y;
		}
		if (s_Player.Parts[i].vtxMax.z > s_Player.modelMax.z)
		{// ��r�Ώۂ����݂̒��_���W(Z)�̍ő�l���傫��
			s_Player.modelMax.z = s_Player.Parts[i].vtxMax.z;
		}

		// ���_�o�b�t�@�̃A�����b�N
		s_Player.Parts[i].pMesh->UnlockVertexBuffer();
	}
}

//--------------------------
//�����蔻��̃T�C�Y�����Ă�
//--------------------------

void MoveSet(void)
{
	Camera *pCamera;
	pCamera = GetCamera();

	s_Player.consumption = 0.0f;
	int nDash = 1;
	if (GetKeyboardPress(DIK_LSHIFT) || GetKeyboardPress(DIK_RSHIFT) || GetJoypadPress(JOYKEY_A, 0))
	{//�V�t�g�_�b�V��
		//SetParticle(s_Player.pos, s_Player.move, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 20.0f, PARTICLETYPE_SMOKE, 20);
		nDash = 3;
	}
	//�Q�[���̎��̈ړ����@

	if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > 0.5f)
	{//�E�ɔ�����葽���|������
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -0.5f)
	{//���ɔ�����葽���|������
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > 0.5f)
	{//��ɔ�����葽���|������
		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.rotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -0.5f)
	{//���ɔ�����葽���|������
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP, 0))
	{//W�������Ƃ������

		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN, 0))
	{//S�������Ɖ�����

		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.rotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{//D�������Ƃ��E����

		s_Player.move.x += sinf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;


		s_Player.consumption = s_Player.rotMove.x + (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;
	}

	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT, 0))
	{//A�������ƍ�����

		s_Player.move.x += sinf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;

	}
	s_Player.posOld = s_Player.pos;//�ߋ��̈ړ��ʂ�ۑ�

	

	if (s_Player.invincible <= 0)
	{//���G���Ԃ��[���ɂȂ�����_���[�W���炤�悤�ɂ���
		s_Player.damege = DAMEGE_NORMAL;
	}

	s_Player.move.x += (0.0f - s_Player.move.x)*0.5f;//�i�ړI�̒l-���݂̒l�j�������W��
	s_Player.move.z += (0.0f - s_Player.move.z)*0.5f;
	s_Player.pos += s_Player.move;//�ړ������Z

	//���K��
	if (s_Player.consumption > D3DX_PI)
	{
		s_Player.consumption += D3DX_PI * 2;
	}
	if (s_Player.consumption < -D3DX_PI)
	{
		s_Player.consumption += -D3DX_PI * 2;
	}

	//���Z�ݒ�i�����j
	s_Player.rot.y += (s_Player.consumption)* Attenuation;//�ړI�̒l-���݂̒l�j�������W��

	//���K��
	if (s_Player.rot.y > D3DX_PI)
	{
		s_Player.rot.y += -D3DX_PI * 2;
	}
	if (s_Player.rot.y <= -D3DX_PI)
	{
		s_Player.rot.y += D3DX_PI * 2;
	}


}
//-------------------------------
//�����蔻��܂Ƃ�
//-------------------------------
void Collision(void)
{
	if (s_Player.pos.y <= 0.0f)
	{
		s_Player.pos.y = 0.0f;
	}
}

//-------------------------------
//���[�V���������[�h���鏈��
//-------------------------------
void SetCopy(char *pFileName, PARTSFILE *PartsFile, PARTS *Parts, MOTION *Motion, int *nMaxParts)
{
	// �ϐ��錾
	char aString[128] = {};			// �������r�p�̕ϐ�
	char g_aEqual[128] = {};		// ���ǂݍ��ݗp�ϐ�
	int	nCntKeySet = 0;				// KeySet�J�E���g
	int	nCntKey = 0;				// Key�J�E���g

									// �t�@�C���|�C���^�̐錾
	FILE * pFile;
//-------------------------------
//�R�s�[������
	if (s_Player.nMaxModelParts >= 7)
	{
		s_Player.nMaxModelParts = 7;
	}
	
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{
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
		printf("\n * * * �t�@�C�����J���܂��� * * * \n");
	}

	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�ƌ����̏����l��ۑ�
	s_Player.Parts[s_Player.nMaxModelParts].posOrigin = s_Player.Parts[s_Player.nMaxModelParts].pos;
	s_Player.Parts[s_Player.nMaxModelParts].rotOrigin = s_Player.Parts[s_Player.nMaxModelParts].rot;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(&s_Player.PartsFile[s_Player.Parts[s_Player.nMaxModelParts].nType].aName[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_Player.Parts[s_Player.nMaxModelParts].pBuffer,
		NULL,
		&s_Player.Parts[s_Player.nMaxModelParts].nNumMat,
		&s_Player.Parts[s_Player.nMaxModelParts].pMesh);

	
		s_Player.nMaxModelParts++;
	
}

//----------------------
//�Q�b�g(�\����)
//----------------------
PLAYER *GetPlayer(void)
{
	return &s_Player;
}
