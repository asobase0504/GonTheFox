//============================
//
// �z�����ݏ���
// Author  :Hamada Ryuuga
//
//============================

#include "main.h"
#include "lnhale.h"
#include "color.h"
#include "input.h"
#include "player.h"
//--------------------------
//�}�N��
//--------------------------
#define LNHALESIZE (D3DXVECTOR3(100.0f,50.0f,0.0f))
#define LNHALEPOS (D3DXVECTOR3(0.0f,0.0f,0.0f))
//--------------------------
//�������֐�
//--------------------------
void CLnhale::Init(void)
{
	bUse = false; 						// �g�p���Ă��邩�ǂ���

	// ���_�o�b�t�@�̐���
	GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = pos + D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(+size.x, -size.y, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-size.x, +size.y, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(+size.x, +size.y, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = GetColor(COLOR_RED);
	pVtx[1].col = GetColor(COLOR_RED);
	pVtx[2].col = GetColor(COLOR_RED);
	pVtx[3].col = GetColor(COLOR_RED);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

}

//--------------------------
//�j���֐�
//--------------------------
void CLnhale::Uninit(void)
{

}

//--------------------------
//�X�V�֐�
//--------------------------
void CLnhale::Update(void)
{
	if (bUse)
	{
		if (!GetKeyboardPress(DIK_B))
		{
			bUse = false;
		}

		//Player�̍��W�t�߂ɏ�ɕύX����
		//PLAYER* pPlayer = GetPlayer();
		//pos = pPlayer->pos;

		VERTEX_2D *pVtx = NULL;		// ���_���ւ̃|�C���^
		// ���_�������b�N���A���_���ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = pos + D3DXVECTOR3(-size.x, -size.y, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3(+size.x, -size.y, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-size.x, +size.y, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3(+size.x, +size.y, 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//--------------------------
//�`��֐�
//--------------------------
void CLnhale::Draw(void)
{	
	
		/*�� �g�p���Ă���A�`�悷�� ��*/
		// �f�o�C�X�ւ̃|�C���^�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		 // �����_�[�X�e�[�g�̐ݒ�
	/*	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		//pDevice->SetTexture(0,pTexture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);				// �v���~�e�B�u(�|���S��)��

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);

		// �����_�[�X�e�[�g�����ɖ߂�
		//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
}

//--------------------------
//�Z�b�g�֐�
//--------------------------
void CLnhale::Set(D3DXVECTOR3 setpos)
{
	if (!bUse)
	{
		pos = setpos;						//���W
		bUse = true; 						// �g�p���Ă��邩�ǂ���
	}
 }

//--------------------------
//�����蔻��֐�
//--------------------------
bool CLnhale::Hit(D3DXVECTOR3 nowpos, D3DXVECTOR3 hitsize)
{
	bool Hit = false;

	if (bUse)
	{	//�z�����ޕ��Ƃ̓����蔻��
		if ((nowpos.x - hitsize.x >= pos.x + size.x|| nowpos.x + hitsize.x <= pos.x - size.x)&& 
			(nowpos.y - hitsize.y >= pos.y + size.y || nowpos.y + hitsize.y <= pos.y - size.y))
		{
			Hit = true;
		}
	}
	return Hit;
}

//-------------------------------------------------
//�R���X�g���N�^�i�C���X�^���X�������ɒʂ�܂��j
//-------------------------------------------------
CLnhale::CLnhale()
{
	pos = LNHALEPOS;					//���W
	size = LNHALESIZE;					//�T�C�Y

	pTexture = NULL;					// �e�N�X�`��
	pVtxBuff = NULL;					// ���_�o�b�t�@
	
}

//-----------------------------------------------
//�f�X�g���N�^�i�C���X�^���X�j�����ɒʂ�܂��j
//-----------------------------------------------
CLnhale::~CLnhale()
{
		if (pTexture != NULL)
		{// ���_�o�b�t�@�̉��
			pTexture->Release();
			pTexture = NULL;
		}

		if (pVtxBuff != NULL)
		{// ���_�o�b�t�@�̉��
			pVtxBuff->Release();
			pVtxBuff = NULL;
		}
}