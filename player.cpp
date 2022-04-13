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

#include "file.h"

#include "title.h"

#include "sound.h"

//------------------------------------
// �}�N��
//------------------------------------

#define Attenuation	(0.5f)		//�����W��
#define Speed	(1.0f)			//�X�s�[�h
#define WIDTH (10.0f)			//���f���̔��a
#define MAX_PRAYER (16)			//�ő吔
#define MAX_MOVE (5)			//�A�j���[�V�����̍ő吔
#define INVINCIBLE (300)		//���G����
//------------------------------------
// static�ϐ�
//------------------------------------
static PLAYER s_Player;	// �|���S���̍\����
static MODELDATAPLAYER s_ModelData[MAX_MOVE];
static int s_nTime, s_nParts;//�^�C�}�[�ƃp�[�c�̍ő吔
static D3DXVECTOR3 mystery;
static int s_nPow;//�W�����v�p���[

				  //=========================================
				  // ����������
				  //=========================================
void InitPlayer(void)
{
	//�J�����̃f�[�^�擾
	Camera *pCamera;
	pCamera = GetCamera();


	s_nPow = 0;
	// ����������
	//�c�@�@�@�@�@�@�@�@�@�@//��
	s_Player.RotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
	s_Player.consumption = 0.0f;
	s_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���_���W
	s_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]���W
	s_Player.nShadow = 0;							//�e�ԍ�
	s_Player.Damege = DAMEGE_NORMAL;
	s_Player.nMotion = ANIME_NORMAL;							//���܎g���Ă�motio�̔ԍ�
	s_Player.bUse = false;							//�g���Ă邩�g���ĂȂ���
	s_Player.bMystery = false;						//�喂�@�̔����X�C�b�`
	s_nParts = 0;
	s_Player.ModelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_Player.ModelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int nSet = 0; nSet < MAX_MODELPARTS; nSet++)
	{
		s_Player.Parts[nSet].nIdxModelParent = 0;
	}
	s_nTime = 0;

	//�t�@�C��
	LoadSetFile(LOOD_FILE_NAME_000);//0���v���C���[

//�t�@�C��
	LoadSetFile(LOOD_FILE_NAME_002);//0���v���C���[
}

//=========================================
// �I������
//=========================================
void UninitPlayer(void)
{
	StopSound();

	for (int i = 0; i < s_nParts; i++)
	{
		// ���_�o�b�t�@�[�̉��
		if (s_Player.Parts[i].pBuffMat != NULL)
		{
			s_Player.Parts[i].pBuffMat->Release();
			s_Player.Parts[i].pBuffMat = NULL;
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
	MoveSet();	//�����Z�b�g

	//Collision();//��

	//			//�A�j���[�V�����⑫���̐ݒ�
	if (s_Player.NotLoop == false)
	{
		s_Player.nMotion = ANIME_NORMAL;
	}
	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_S) ||
		GetJoypadPress(JOYKEY_UP, 0) || GetJoypadPress(JOYKEY_DOWN, 0) || GetJoypadPress(JOYKEY_LEFT, 0) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{
		s_Player.nMotion = ANIME_RUN;//����

	}
	
	if (GetKeyboardTrigger(DIK_B))
	{
		s_Player.nMotion = ANIME_ATTACK;//�U��
		s_Player.NotLoop = true;
	}
	////�e�X�V
	////SetposShadow(s_Player.nShadow, D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y, s_Player.pos.z));


	AnimationSet((int)s_Player.nMotion);//�A�j���[�V�����Đ�


}

//=========================================
// �`�揈��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat;
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	if (s_Player.bUse)//�g���Ă��o��
	{
		// ���[���h�}�g���b�N�X�̏�����
		// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)
		D3DXMatrixIdentity(&s_Player.MtxWorld);

		// �g�k�𔽉f
		// �s��g�k�֐�
		D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&s_Player.MtxWorld, &s_Player.MtxWorld, &mtxScale);

		// �����𔽉f
		// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Player.rot.y, s_Player.rot.x, s_Player.rot.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&s_Player.MtxWorld, &s_Player.MtxWorld, &mtxRot);

		// �ʒu�𔽉f
		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixTranslation(&mtxTrans, s_Player.pos.x, s_Player.pos.y, s_Player.pos.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&s_Player.MtxWorld, &s_Player.MtxWorld, &mtxTrans);

		for (int i = 0; i < s_nParts; i++)
		{
			// ���[���h�}�g���b�N�X�̏�����
			// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)
			D3DXMatrixIdentity(&s_Player.Parts[i].MtxWorld);

			//// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
			//D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxScale);

			// �����𔽉f
			// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Player.Parts[i].rot.y, s_Player.Parts[i].rot.x, s_Player.Parts[i].rot.z);
			// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
			D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxRot);

			// �ʒu�𔽉f
			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
			D3DXMatrixTranslation(&mtxTrans, s_Player.Parts[i].pos.x, s_Player.Parts[i].pos.y, s_Player.Parts[i].pos.z);
			// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
			D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxTrans);

			D3DXMATRIX mtxParent;

			//���݂̃}�e���A�����l��
			pDevice->GetMaterial(&marDef);

			if (s_Player.Parts[i].nIdxModelParent == -1)
			{//�e
				mtxParent = s_Player.MtxWorld;
			}
			else
			{//�q
				mtxParent = s_Player.Parts[s_Player.Parts[i].nIdxModelParent].MtxWorld;
			}

			//�e�Ǝq�̗Z��
			D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxParent);

			// ���[���h���W�s��̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &s_Player.Parts[i].MtxWorld);
			//�}�e���A���f�[�^�̃|�C���^���擾
			pMat = (D3DXMATERIAL*)s_Player.Parts[i].pBuffMat->GetBufferPointer();

			for (int j = 0; j < (int)s_Player.Parts[i].nNumMat; j++)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[j].MatD3D);
				//���f���p�[�c�̕`��
				s_Player.Parts[i].pMesh->DrawSubset(j);
			}
		}
		//���݂̃}�e���A�������ɖ߂�
		pDevice->SetMaterial(&marDef);
	}
}



void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Filename, int nParent, int nIndex, D3DXVECTOR3 nModelPos)
{
	//�J�����̃f�[�^�擾
	Camera *pCamera = GetCamera();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	char aFile[128] = FILE_3D_PLAYER;

	strcat(aFile, &Filename[0]);//�����@aFile��-�����ɓ����

	D3DXLoadMeshFromX(&aFile[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_Player.Parts[s_nParts].pBuffMat,
		NULL,
		&s_Player.Parts[s_nParts].nNumMat,
		&s_Player.Parts[s_nParts].pMesh
	);

	s_Player.Parts[s_nParts].nIdxModelParent = nParent;
	s_Player.RotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);

	// ����������
	s_Player.pos = nModelPos;		//������W�ݒ�
	s_Player.Parts[s_nParts].pos = pos;	// ���_���W
	s_Player.Parts[s_nParts].posOri = s_Player.Parts[s_nParts].pos;
	s_Player.Parts[s_nParts].posdefault = s_Player.Parts[s_nParts].pos;
	s_Player.Parts[s_nParts].rot = rot;	// ��]���W
	s_Player.Parts[s_nParts].rotOri = s_Player.Parts[s_nParts].rot;
	s_Player.Parts[s_nParts].rotdefault = s_Player.Parts[s_nParts].rot;

	s_Player.bUse = true;

	if (s_Player.Parts[s_nParts].nIdxModelParent == -1)
	{
		SizSet();
		//s_Player.nShadow = SetShadow(s_Player.pos, s_Player.rot, 0);//�ꏊ.��]
	}
	s_nParts++;
}

//--------------------------
//�����蔻��̃T�C�Y�����Ă�
//--------------------------
void SizSet(void)
{
	//���ׂĂ̒��_����ۑ�����֐������
	int NumVpx;	//���_��
				//��̒��_�̃T�C�Y������֐�
	DWORD sizeFVF;
	//�o�C�g�����̃|�C���^�[char
	BYTE *pVtxBuff;

	//�����Œ��_�����Q��
	NumVpx = s_Player.Parts[s_nParts].pMesh->GetNumVertices();

	//�����ŃT�C�Y������
	sizeFVF = D3DXGetFVFVertexSize(s_Player.Parts[s_nParts].pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N���܂��傤
	s_Player.Parts[s_nParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int i = 0; i < NumVpx; i++)
	{
		D3DXVECTOR3 pVtx = *(D3DXVECTOR3*)pVtxBuff;
		//������g���ă��f���̍ő�l�ƍŏ��l���g���ċ��߂�
		//XYZ�S�����߂�
		if (pVtx.x < s_Player.ModelMin.x)
		{
			s_Player.ModelMin.x = pVtx.x;
		}
		if (pVtx.x > s_Player.ModelMax.x)
		{
			s_Player.ModelMax.x = pVtx.x;
		}
		if (pVtx.y < s_Player.ModelMin.y)
		{
			s_Player.ModelMin.y = pVtx.y;
		}
		if (pVtx.y > s_Player.ModelMax.y)
		{
			s_Player.ModelMax.y = pVtx.y;
		}
		if (pVtx.z < s_Player.ModelMin.z)
		{
			s_Player.ModelMin.z = pVtx.z;
		}
		if (pVtx.z > s_Player.ModelMax.z)
		{
			s_Player.ModelMax.z = pVtx.z;
		}

		//������߂��玟�ɐ��������̂�
		pVtxBuff += sizeFVF;
		//��������΍ő�ŏ������߂���
	}
	s_Player.ModelMax.z *= 3;
	s_Player.ModelMax.x *= 3;
	//���_�o�b�t�@�̃��b�N���܂��傤
	s_Player.Parts[s_nParts].pMesh->UnlockVertexBuffer();
}

//------------------------------
//�A�j���[�V�����Z�b�g
//-------------------------------
void AnimationSet(int Animation)
{
	s_ModelData[Animation].Key++;//�J�E���g���Z

	if (s_ModelData[Animation].LOOP == 1)
	{//YES���[�v
		if (s_ModelData[Animation].Key <= s_ModelData[Animation].KeySet[0].KeyFrame)
		{//�L�[�t���[�����Z������

			for (int i = 0; i < MAX_MODELPARTS; i++)
			{//�p�[�c�S���Ɉʒu�̈ړ��Ȃǂ�����

				D3DXVECTOR3 RotDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].rot - s_Player.Parts[i].rotOri);
				D3DXVECTOR3 PosDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].pos - s_Player.Parts[i].posOri);

				if (RotDiff.x > D3DX_PI)
				{
					RotDiff.x += D3DX_PI * 2;
				}
				if (RotDiff.x < -D3DX_PI)
				{
					RotDiff.x += -D3DX_PI * 2;
				}
				//���K��
				if (RotDiff.z > D3DX_PI)
				{
					RotDiff.z += D3DX_PI * 2;
				}
				if (RotDiff.z < -D3DX_PI)
				{
					RotDiff.z += -D3DX_PI * 2;
				}
				//���K��
				if (RotDiff.y > D3DX_PI)
				{
					RotDiff.y += D3DX_PI * 2;
				}
				if (RotDiff.y < -D3DX_PI)
				{
					RotDiff.y += -D3DX_PI * 2;
				}

				D3DXVECTOR3 RotOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);
				D3DXVECTOR3 PosOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);

				D3DXVECTOR3 RotFrame = RotOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].rotOri;
				D3DXVECTOR3 PosFrame = PosOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].posOri;

				//�o�O�����琳�K�����Ȃ���By�ߋ��̕l�c
				s_Player.Parts[i].rot = RotFrame;
				s_Player.Parts[i].pos = PosFrame;

				//���K��
				if (s_Player.Parts[i].rot.x > D3DX_PI)
				{
					s_Player.Parts[i].rot.x += D3DX_PI * 2;
				}
				if (s_Player.Parts[i].rot.x < -D3DX_PI)
				{
					s_Player.Parts[i].rot.x += -D3DX_PI * 2;
				}
				//���K��
				if (s_Player.Parts[i].rot.z > D3DX_PI)
				{
					s_Player.Parts[i].rot.z += D3DX_PI * 2;
				}
				if (s_Player.Parts[i].rot.z < -D3DX_PI)
				{
					s_Player.Parts[i].rot.z += -D3DX_PI * 2;
				}
				//���K��
				if (s_Player.Parts[i].rot.y > D3DX_PI)
				{
					s_Player.Parts[i].rot.y += D3DX_PI * 2;
				}
				if (s_Player.Parts[i].rot.y < -D3DX_PI)
				{
					s_Player.Parts[i].rot.y += -D3DX_PI * 2;
				}
			}
		}

		if (s_ModelData[Animation].Key >= s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame)
		{//�A�j���[�V�����̍X�V���ăL�[���[���ɖ߂��܂�
			for (int i = 0; i < MAX_MODELPARTS; i++)
			{//�p�[�c�S���Ɉʒu�̈ړ��Ȃǂ�����
				s_Player.Parts[i].rotOri = s_Player.Parts[i].rot;
				s_Player.Parts[i].posOri = s_Player.Parts[i].pos;
			}
			s_ModelData[Animation].NowKey++;									//���ɍX�V
			s_ModelData[Animation].Key = 0;										//�L�[���[���ɂ���
			s_ModelData[Animation].NowKey %= s_ModelData[Animation].NUM_KEY;	//�A�j���[�V�����̍ő�𒴂�����[���ɖ߂�
			s_Player.nMotion = ANIME_NORMAL;
		}
	}
	else
	{//NOT���[�v
		for (int i = 0; i < MAX_MODELPARTS; i++)
		{//�p�[�c�S���Ɉʒu�̈ړ��Ȃǂ�����

			D3DXVECTOR3 RotDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].rot - s_Player.Parts[i].rotOri);
			D3DXVECTOR3 PosDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].pos - s_Player.Parts[i].posOri);

			D3DXVECTOR3 RotOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);
			D3DXVECTOR3 PosOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);

			D3DXVECTOR3 RotFrame = RotOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].rotOri;
			D3DXVECTOR3 PosFrame = PosOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].posOri;

			//�o�O�����琳�K�����Ȃ���By�ߋ��̕l�c
			s_Player.Parts[i].rot = RotFrame;
			s_Player.Parts[i].pos = PosFrame;

			//���K��
			if (s_Player.Parts[i].rot.x > D3DX_PI)
			{
				s_Player.Parts[i].rot.x += D3DX_PI * 2;
			}
			if (s_Player.Parts[i].rot.x < -D3DX_PI)
			{
				s_Player.Parts[i].rot.x += -D3DX_PI * 2;
			}
			//���K��
			if (s_Player.Parts[i].rot.z > D3DX_PI)
			{
				s_Player.Parts[i].rot.z += D3DX_PI * 2;
			}
			if (s_Player.Parts[i].rot.z < -D3DX_PI)
			{
				s_Player.Parts[i].rot.z += -D3DX_PI * 2;
			}
			//���K��
			if (s_Player.Parts[i].rot.y > D3DX_PI)
			{
				s_Player.Parts[i].rot.y += D3DX_PI * 2;
			}
			if (s_Player.Parts[i].rot.y < -D3DX_PI)
			{
				s_Player.Parts[i].rot.y += -D3DX_PI * 2;
			}
		}
	}

	if (s_ModelData[Animation].Key >= s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame)
	{//�A�j���[�V�����̍X�V���ăL�[���[���ɖ߂��܂�

		for (int i = 0; i < MAX_MODELPARTS; i++)
		{//�p�[�c�S���Ɉʒu�̈ړ��Ȃǂ�����
			s_Player.Parts[i].rotOri = s_Player.Parts[i].rot;
			s_Player.Parts[i].posOri = s_Player.Parts[i].pos;
		}
		s_ModelData[Animation].NowKey++;									//���ɍX�V
		s_ModelData[Animation].Key = 0;										//�L�[���[���ɂ���
		if (s_ModelData[Animation].NowKey == s_ModelData[Animation].NUM_KEY)
		{
			for (int i = 0; i < MAX_MODELPARTS; i++)
			{//�p�[�c�S���Ɉʒu�̈ړ��Ȃǂ�����
				s_Player.Parts[i].rotOri = s_Player.Parts[i].rotdefault;
				s_Player.Parts[i].posOri = s_Player.Parts[i].posdefault;
			}
			s_ModelData[Animation].NowKey = 0;
			s_Player.NotLoop = false;

		}
	}
}

//------------------------------
//�����Z�b�g
//-------------------------------
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

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -0.5f)
	{//���ɔ�����葽���|������
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > 0.5f)
	{//��ɔ�����葽���|������
		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.RotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -0.5f)
	{//���ɔ�����葽���|������
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP, 0))
	{//W�������Ƃ������

		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN, 0))
	{//S�������Ɖ�����

		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.RotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{//D�������Ƃ��E����

		s_Player.move.x += sinf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;


		s_Player.consumption = s_Player.RotMove.x + (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;
	}

	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT, 0))
	{//A�������ƍ�����

		s_Player.move.x += sinf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;

	}
	s_Player.posOld = s_Player.pos;//�ߋ��̈ړ��ʂ�ۑ�

	s_Player.move.x += (0.0f - s_Player.move.x)*0.5f;//�i�ړI�̒l-���݂̒l�j�������W��
	s_Player.move.z += (0.0f - s_Player.move.z)*0.5f;
	s_Player.pos += s_Player.move;//�ړ������Z

	if (s_Player.invincible <= 0)
	{//���G���Ԃ��[���ɂȂ�����_���[�W���炤�悤�ɂ���
		s_Player.Damege = DAMEGE_NORMAL;
	}

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

	//�Q�[���ȊO�̎��̎��̈ړ����@
}
//------------------------------
//�����蔻��܂Ƃ�
//-------------------------------
void Collision(void)
{

	
	

}

//----------------------
//�Q�b�g(�\����)
//----------------------
PLAYER *GetPlayer(void)
{
	return &s_Player;
}

//----------------------
//�Q�b�g(motion)
//----------------------
MODELDATAPLAYER *GetModelData(void)
{
	return &s_ModelData[0];
}
