//=========================================
// 
// ���f�������֐�
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "model.h"
#include "utility.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include <stdio.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define MODEL_NUM				(100)
#define PLAYER_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)
#define MODEL_LOAD_FILE			("data/model.txt")
#define MAP_LOAD_FILE			("data/map05.txt")

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static Model s_ModelType[MODEL_NUM];	// ���f���̎�ނ�ۊ�
static Model s_Model[MODEL_MAX];		// ���f���̍\����
static int s_nShadowCnt;				// �e�̊��蓖��

//=========================================
// ������
//=========================================
void InitModel(void)
{
	ZeroMemory(s_ModelType, sizeof(s_ModelType));
	ZeroMemory(s_Model, sizeof(s_Model));
}

//=========================================
// �I��
//=========================================
void UninitModel(void)
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		Model* model = &(s_ModelType[i]);

		if (model->pTexture != NULL)
		{
			for (int j = 0; j < (int)model->nNumMat; j++)
			{
				if (model->pTexture[j] != NULL)
				{// �e�N�X�`���̉��
					model->pTexture[j]->Release();
					model->pTexture[j] = NULL;
				}
			}

			delete[]model->pTexture;
			model->pTexture = NULL;
		}

		// ���b�V���̉��
		if (model->pMesh != NULL)
		{
			model->pMesh->Release();
			model->pMesh = NULL;
		}
		// �}�e���A���̉��
		if (model->pBuffMat != NULL)
		{
			model->pBuffMat->Release();
			model->pBuffMat = NULL;
		}
	}
}

//=========================================
// �X�V
//=========================================
void UpdateModel(void)
{

}

//=========================================
// �`��
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���ۑ��p

	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* model = s_Model + i;

		if (!model->bUse)
		{
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&model->mtxWorld);

		// �X�P�[���̔��f
		D3DXMatrixScaling(&mtxScale, model->scale, model->scale, model->scale);
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxScale);			// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �`����@
		if (model->isQuaternion)
		{
			// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
			D3DXMatrixRotationQuaternion(&mtxRot, &model->quaternion);			// �N�I�[�^�j�I���ɂ��s���]
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);	// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		}
		else
		{
			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		}

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �v���C���[�Ƃ������Ă����Ԃ̃��f���̓v���C���[�Ƃ̍s��v�Z
		if (model->nIdxModelParent != -1)
		{
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &s_Model[model->nIdxModelParent].mtxWorld);
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

		// ���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			D3DMATERIAL9 matBak = pMat[j].MatD3D;

			// �A���r�G���g���C�g�̔��f
			pMat[j].MatD3D.Ambient = pMat[j].MatD3D.Diffuse;

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, model->pTexture[j]);

			// ���f���p�[�c�̕`��
			model->pMesh->DrawSubset(j);

			pMat[j].MatD3D = matBak;
		}
		// �ێ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=========================================
// �����蔻��
//=========================================
void CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3* pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		Model* model = &(s_ModelType[i]);

		if (pos->x + max.x > model->pos.x + model->MinVtx.x && pos->x + min.x < model->pos.x + model->MaxVtx.x)
		{
			// ��
			if (pos->z + max.z >= model->pos.z + model->MinVtx.z && pos_old->z + max.z <= model->pos.z + model->MinVtx.z)
			{
				pos->z = model->pos.z + model->MinVtx.z - max.z;
			}
			// ��O
			else if (pos->z + min.z <= model->pos.z + model->MaxVtx.z && pos_old->z + min.z >= model->pos.z + model->MaxVtx.z)
			{
				pos->z = model->pos.z + model->MaxVtx.z - min.z;
			}
		}
		if (pos->z + max.z > model->pos.z + model->MinVtx.z && pos->z + min.z < model->pos.z + model->MaxVtx.z)
		{
			// ��
			if (pos->x + max.x >= model->pos.x + model->MinVtx.x && pos_old->x + max.x <= model->pos.x + model->MinVtx.x)
			{
				pos->x = model->pos.x + model->MinVtx.x - max.x;
			}
			// �E
			else if (pos->x + min.x <= model->pos.x + model->MaxVtx.x && pos_old->x + min.x >= model->pos.x + model->MaxVtx.x)
			{
				pos->x = model->pos.x + model->MaxVtx.x - min.x;
			}
		}
	}
}

//=========================================
// �擾
//=========================================
Model *GetModel(void)
{
	return s_ModelType;
}

//=========================================
// �ǂݍ��ݏ���
//=========================================
void LoadModel(void)
{
	FILE* pFile;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	bool isType = false;
	char modelFile[255][255] = {};
	int nModelFileCnt = 0;

	pFile = fopen(MODEL_LOAD_FILE, "r");

	char read[255] = {};
	while (1)
	{
		fscanf(pFile, "%s", &read);

		//# �����o����ƈ�s�ǂݍ���
		if (strncmp(&read[0], "#", 1) == 0)
		{
			fgets(read, 255, pFile);
			continue;
		}

		if (strcmp(&read[0], "END_SCRIPT") == 0)
		{
			break;
		}

		if (strcmp(&read[0], "TYPESET") == 0)
		{
			isType = true;
		}

		if (strcmp(&read[0], "END_TYPESET") == 0)
		{
			nModelFileCnt++;
			isType = false;
		}

		if (isType)
		{
			if (strcmp(&read[0], "FILENAME") == 0)
			{
				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%s", &modelFile[nModelFileCnt][0]);

				Model* modelType = &(s_ModelType[nModelFileCnt]);

				modelType->scale = 1.0f;

				// X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(&modelFile[nModelFileCnt][0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&modelType->pBuffMat,
					NULL,
					&modelType->nNumMat,
					&modelType->pMesh);

				// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
				modelType->pTexture = new LPDIRECT3DTEXTURE9[modelType->nNumMat];

				// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
				D3DXMATERIAL *pMat = (D3DXMATERIAL*)modelType->pBuffMat->GetBufferPointer();

				// �e���b�V���̃}�e���A�������擾����
				for (int i = 0; i < (int)modelType->nNumMat; i++)
				{
					modelType->pTexture[i] = NULL;

					if (pMat[i].pTextureFilename != NULL)
					{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
						D3DXCreateTextureFromFileA(pDevice,
							pMat[i].pTextureFilename,
							&modelType->pTexture[i]);
					}
				}

				// ���f���̃T�C�Y�v��
				ModelSize(&modelType->MinVtx, &modelType->MaxVtx, modelType->pMesh);
			}
			if (strcmp(&read[0], "SCALE") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f", &modelType->scale);
			}
		}
	}
	fclose(pFile);
}

//=========================================
// �ǂݍ��ݏ���
//=========================================
void LoadMap(void)
{
	FILE* pFile;
	bool isModel = false;
	bool isPlayer = false;
	int nModelCnt = 1;
	int nModelData;

	pFile = fopen(MAP_LOAD_FILE, "r");

	char read[255] = {};
	while (1)
	{
		fscanf(pFile, "%s", &read);

		//# �����o����ƈ�s�ǂݍ���
		if (strncmp(&read[0], "#", 1) == 0)
		{
			fgets(read, 255, pFile);
			continue;
		}

		if (strcmp(&read[0], "END_SCRIPT") == 0)
		{
			break;
		}

		if (strcmp(&read[0], "MODELSET") == 0)
		{
			for (nModelData = 0; nModelData < MODEL_MAX; nModelData++)
			{
				if (!s_Model[nModelData].bUse)
				{
					break;
				}
			}
			isModel = true;
		}
		else if (strcmp(&read[0], "PLAYERSET") == 0)
		{
			for (nModelData = 0; nModelData < MODEL_MAX; nModelData++)
			{
				if (!s_Model[nModelData].bUse)
				{
					break;
				}
			}
			isPlayer = true;
		}

		if (strcmp(&read[0], "END_MODELSET") == 0)
		{
			nModelCnt++;
			isModel = false;
		}

		if (isModel)
		{
			if (strcmp(&read[0], "TYPE") == 0)
			{
				int nData;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%d", &nData);

				s_Model[nModelData] = s_ModelType[nData];
				s_Model[nModelData].nType = nData;
				s_Model[nModelData].nIdxModelParent = -2;
				s_Model[nModelData].quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
				s_Model[nModelData].isQuaternion = true;
				s_Model[nModelData].bUse = true;
			}
			if (strcmp(&read[0], "POS") == 0)
			{
				D3DXVECTOR3 pos;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				s_Model[nModelData].pos = pos;
			}
			if (strcmp(&read[0], "ROT") == 0)
			{
				D3DXVECTOR3 rot;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
				D3DXQuaternionRotationYawPitchRoll(&s_Model[nModelData].quaternion, rot.y, rot.x, rot.z);
			}
		}
		if (isPlayer)
		{
			if (strcmp(&read[0], "TYPE") == 0)
			{
				int nData;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%d", &nData);

				s_Model[nModelData] = s_ModelType[nData];

				s_Model[nModelData].nIdxModelParent = -1;
				s_Model[nModelData].quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
				s_Model[nModelData].isQuaternion = true;
				s_Model[nModelData].bUse = true;
			}
			if (strcmp(&read[0], "POS") == 0)
			{
				D3DXVECTOR3 pos;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				s_Model[nModelData].pos = pos;
			}
		}
	}
	fclose(pFile);
}

//=========================================
// �ݒ�
//=========================================
Model* SetModel(int nType)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* pModel = &(s_Model[i]);

		if (pModel->bUse)
		{
			continue;
		}

		s_Model[i] = s_ModelType[nType];

		return pModel;
	}
	return NULL;
}

//=========================================
// �ݒ�
//=========================================
Model* SetModel(char* file)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* pModel = &(s_Model[i]);

		if (pModel->bUse)
		{
			continue;
		}

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(file,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pModel->pBuffMat,
			NULL,
			&pModel->nNumMat,
			&pModel->pMesh);

		// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
		pModel->pTexture = new LPDIRECT3DTEXTURE9[pModel->nNumMat];

		// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

		// �e���b�V���̃}�e���A�������擾����
		for (int j = 0; j < (int)pModel->nNumMat; j++)
		{
			pModel->pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&pModel->pTexture[j]);
			}
		}

		// ���f���̃T�C�Y�v��
		ModelSize(&pModel->MinVtx, &pModel->MaxVtx, pModel->pMesh);

		pModel->bUse = true;

		return pModel;
	}
	return NULL;
}

//=========================================
// ���f���̃T�C�Y���Z�o
// ����1 minOut	���f���̍ŏ����_�̏o��
// ����2 maxOut	���f���̍ő咸�_�̏o��
// ����3 mesh	�T�C�Y���v�����郂�f��
//=========================================
void ModelSize(D3DXVECTOR3* minOut, D3DXVECTOR3* maxOut, LPD3DXMESH mesh)
{
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�[�ւ̃|�C���g

	nNumVtx = mesh->GetNumVertices();	// ���_���̎擾

	sizeFVF = D3DXGetFVFVertexSize(mesh->GetFVF());	// ���_�t�H�[�}�b�g�̃T�C�Y���擾

	// ������
	*minOut = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	*maxOut = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// ���_�o�b�t�@�̃��b�N
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < minOut->x)
		{
			minOut->x = vtx.x;
		}
		if (vtx.y < minOut->y)
		{
			minOut->y = vtx.y;
		}
		if (vtx.z < minOut->z)
		{
			minOut->z = vtx.z;
		}
		if (vtx.x > maxOut->x)
		{
			maxOut->x = vtx.x;
		}
		if (vtx.y > maxOut->y)
		{
			maxOut->y = vtx.y;
		}
		if (vtx.z > maxOut->z)
		{
			maxOut->z = vtx.z;
		}

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	// ���_�o�b�t�@�[�̃A�����b�N
	mesh->UnlockVertexBuffer();
}
