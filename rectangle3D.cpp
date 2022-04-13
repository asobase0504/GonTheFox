//==================================================
// 
// �|���S���̍쐬
// Author YudaKaito
// 
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "main.h"
#include "rectangle3D.h"
#include "camera.h"
#include "color.h"
#include "texture.h"
#include <assert.h>

//--------------------------------------------------
// �}�N����`
//--------------------------------------------------
#define MAX_POLYGON	(1024)	// �|���S���̍ő吔
#define NUM_VERTEX	(4)		// ���_��
#define NUM_POLYGON	(2)		// �|���S����

//--------------------------------------------------
// �|���S���̍\���̂��`
//--------------------------------------------------
typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// ���_�o�b�t�@
	D3DXVECTOR3				pos;		// ���_���W
	D3DXVECTOR3				rot;		// ��]���W
	D3DXMATRIX				mtxWorld;	// ���[���h�}�g���b�N�X
	bool					bUse;		// �g�p�t���O
	bool					bDraw;		// �`��t���O
} MyRectangle3D;

//--------------------------------------------------
// �ÓI�ϐ�
//--------------------------------------------------
static MyRectangle3D s_aRectangle3D[MAX_POLYGON];		// �|���S���̍\����

//==================================================
// ������
//==================================================
void InitRectangle3D(void)
{
	ZeroMemory(s_aRectangle3D, sizeof(s_aRectangle3D));	// ������
}

//==================================================
// �I��
//==================================================
void UninitRectangle3D(void)
{
	for (int i = 0; i < MAX_POLYGON; i++)
	{
		// ���_�o�b�t�@�[�̔j��
		if (s_aRectangle3D[i].pVtxBuff != NULL)
		{
			s_aRectangle3D[i].pVtxBuff->Release();
			s_aRectangle3D[i].pVtxBuff = NULL;
		}
	}

}

//==================================================
// �`��
//==================================================
void DrawRectangle3D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	for (int i = 0; i < MAX_POLYGON; i++)
	{
		MyRectangle3D *pRectAngle3D = &s_aRectangle3D[i];

		if (!pRectAngle3D->bUse || !pRectAngle3D->bDraw)
		{
			continue;
		}

		/*�� �g�p���Ă���A�`�悷�� ��*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pRectAngle3D->mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pRectAngle3D->pos.x, pRectAngle3D->pos.y, pRectAngle3D->pos.z);	// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pRectAngle3D->rot.y, pRectAngle3D->rot.x, pRectAngle3D->rot.z);		// ���[�s�b�`���[���ɂ��s���]
		D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxRot);							// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pRectAngle3D->mtxWorld);	// ���[���h���W�s��̐ݒ�

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pRectAngle3D->pVtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->SetRenderState(D3DRS_CULLMODE, 1);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NOTEQUAL);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, pRectAngle3D->pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		pDevice->SetRenderState(D3DRS_CULLMODE, 3);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);

	}
}

//==================================================
// �ݒ�
//==================================================
int SetRectangle3D(TEXTURE texture)
{
	// �ݒ� [ �e�N�X�`������ ]
	return SetRectangle3DWithTex(GetTexture(texture));
}

//==================================================
// �ݒ� [ �e�N�X�`������ ]
//==================================================
int SetRectangle3DWithTex(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i <= MAX_POLYGON; i++)
	{
		MyRectangle3D* polygon = &s_aRectangle3D[i];

		if (polygon->bUse)
		{
			continue;
		}

		polygon->pTexture = pTexture;
		polygon->bUse = true;
		polygon->bDraw = true;

		// ���_�o�b�t�@�̐���
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&polygon->pVtxBuff,
			NULL);

		// �e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
		D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(-(D3DX_PI / 2.0f), 0.0f, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���̐ݒ�
		SetNormalRectangle3D(i, nor);

		// �ʒu�̐ݒ�
		SetPosRectangle3D(i, pos);

		// �ʒu�̐ݒ�
		SetRotationRectangle3D(i, rot);

		// �傫���̐ݒ�
		SetSizeRectangle3D(i, size);

		// ���_�J���[�̐ݒ�
		SetColorRectangle3D(i, GetColor(COLOR_WHITE));

		D3DXVECTOR2 texU = D3DXVECTOR2(0.0f, 1.0f);
		D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		SetTexRectangle3D(i, texU, texV);

		return i;
	}

	assert(false);
	return -1;
}

//==================================================
// �g���̂��~�߂�
// ����  : int nIdx / �C���f�b�N�X
//==================================================
void StopUseRectangle3D(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	pRectAngle3D->bUse = false;

	if (pRectAngle3D->pVtxBuff != NULL)
	{// ���_�o�b�t�@�̉��
		pRectAngle3D->pVtxBuff->Release();
		pRectAngle3D->pVtxBuff = NULL;
	}
}

//==================================================
// �ʒu�̐ݒ�
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : D3DXVECTOR3 &pos / �ړ��ʒu
//==================================================
void SetPosRectangle3D(int nIdx, const D3DXVECTOR3 &pos)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X

	pRectAngle3D->pos = pos;

	//// ���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&pRectAngle3D->mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	//// �ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);	// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	//D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
}

//==================================================
// ��]�̐ݒ�
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : D3DXVECTOR3 &rot / ����
//==================================================
void SetRotationRectangle3D(int nIdx, const D3DXVECTOR3 &rot)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	D3DXMATRIX mtxRot;	// �v�Z�p�}�g���b�N�X

	pRectAngle3D->rot = rot;

	//// ���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&pRectAngle3D->mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

	//// �����𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// ���[�s�b�`���[���ɂ��s���]
	//D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxRot);							// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
}

//==================================================
// �傫���̐ݒ�
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : D3DXVECTOR3 &size / �傫��
//==================================================
void SetSizeRectangle3D(int nIdx, const D3DXVECTOR3 &size)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

}

//==================================================
// �@���̐ݒ�
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : D3DXVECTOR3 &nor / �@��
//==================================================
void SetNormalRectangle3D(int nIdx, const D3DXVECTOR3 &nor)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//==================================================
// �F�̐ݒ�
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : D3DXCOLOR &color / �F
//==================================================
void SetColorRectangle3D(int nIdx, const D3DXCOLOR &color)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

}

//==================================================
// �e�N�X�`�����W�̐ݒ�
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : D3DXVECTOR2 &texU / x = ���[, y = �E�[
// ����3  : D3DXVECTOR2 &texV / x = ��[, y = ���[
//==================================================
void SetTexRectangle3D(int nIdx, const D3DXVECTOR2 &texU, const D3DXVECTOR2 &texV)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(texU.x, texV.x);
	pVtx[1].tex = D3DXVECTOR2(texU.y, texV.x);
	pVtx[2].tex = D3DXVECTOR2(texU.x, texV.y);
	pVtx[3].tex = D3DXVECTOR2(texU.y, texV.y);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

}

//==================================================
// �`�悷�邩�ǂ���
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : bool bDraw / �`�悷�邩�ǂ���
//==================================================
void SetDrawRectangle3D(int nIdx, bool bDraw)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	pRectAngle3D->bDraw = bDraw;

}

//==================================================
// �e�N�X�`���̕ύX
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : TEXTURE texture / �񋓌^ ���
//==================================================
void ChangeTextureRectangle3D(int nIdx, TEXTURE texture)
{
	// �e�N�X�`���̕ύX [ �e�N�X�`������ ]
	ChangeTextureRectangle3DWithTex(nIdx, GetTexture(texture));
}

//==================================================
// �e�N�X�`���̕ύX [ �e�N�X�`������ ]
// ����1  : int nIdx / �C���f�b�N�X
// ����2  : TEXTURE texture / �񋓌^ ���
//==================================================
void ChangeTextureRectangle3DWithTex(int nIdx, LPDIRECT3DTEXTURE9 pTexture)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return;
	}

	/*�� �g�p���Ă��� ��*/

	pRectAngle3D->pTexture = pTexture;
}

//==================================================
// ���_�o�b�t�@�̎擾
// ����  : int nIdx / �C���f�b�N�X
// �Ԓl  : LPDIRECT3DVERTEXBUFFER9 / ���_�o�b�t�@
//==================================================
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle3D(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// �g�p���Ă��Ȃ�
		return NULL;
	}

	/*�� �g�p���Ă��� ��*/

	return s_aRectangle3D[nIdx].pVtxBuff;
}