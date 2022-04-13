//=========================================
// 
// �^�C�g���̏���
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "title.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "rectangle3D.h"
#include "rectangle2D.h"
#include "color.h"
#include <assert.h>

//-----------------------------------------
// �}�N����`
//-----------------------------------------

//-----------------------------------------
// �ÓI�ϐ�
//-----------------------------------------
bool s_bExit = false;	// �Q�[�����̂̏I���t���O

//=========================================
// ����������
//=========================================
void InitTitle(void)
{
	InitLight();
	InitCamera();

	// DEBUG�̂���(��`2D)
	{
		int data = SetRectangle(TEXTURE_NONE);
		D3DXVECTOR3 size = D3DXVECTOR3(300.0f, 300.0f, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.275f, SCREEN_HEIGHT * 0.5f, 0.0f);

		SetColorRectangle(data, GetColor(COLOR_RED));
		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle(data, pos, size);
	}

	// DEBUG�̂���(��`3D)
	{
		int hoge = SetRectangle3D(TEXTURE_NONE);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, SCREEN_HEIGHT * 0.5f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �傫���̐ݒ�
		SetSizeRectangle3D(hoge, size);

		// �F�̐ݒ�
		SetColorRectangle3D(hoge, GetColor(COLOR_BLUE));
		
		// ��`�̈ʒu�̐ݒ�
		SetPosRectangle3D(hoge, pos);
	}
}

//=========================================
// �I������
//=========================================
void UninitTitle(void)
{

}

//=========================================
// �X�V����
//=========================================
void UpdateTitle(void)
{
	UpdateCamera();
}

//=========================================
// �I������
//=========================================
void SelectTitle(void)
{

}

//=========================================
// �`�揈��
//=========================================
void DrawTitle(void)
{
	SetCamera();		// �J����
	DrawRectangle3D();
	DrawRectangle();	// ��`
}

//=========================================
// �I�������擾
//=========================================
bool GetExit(void)
{
	return s_bExit;
}
