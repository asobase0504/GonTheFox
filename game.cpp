//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
// ��v�w�b�_�[
#include "game.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "mode.h"
// �����w�b�_�[
#include "player.h"
#include "enemy.h"

#include <stdio.h>
// �`��w�b�_�[
#include "camera.h"
#include "light.h"
#include "rectangle3D.h"
#include "rectangle2D.h"
#include "color.h"
// DEBUG�w�b�_�[
#include <assert.h>
#include "lnhale.h"

//------------------------------------
// ��`
//------------------------------------
namespace
{

}

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
namespace
{

}

//=========================================
// ������
//=========================================
void InitGame(void)
{


	InitLight();
	InitCamera();
	InitPlayer();
	InitEnemy();
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
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 15.0f, SCREEN_HEIGHT * 0.75f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, -SCREEN_HEIGHT * 0.75f);

		// �ݒ�
		SetPosRectangle3D(hoge, pos);						// �ʒu
		SetSizeRectangle3D(hoge, size);						// �傫��
		SetColorRectangle3D(hoge, GetColor(COLOR_BLUE));	// �F
		SetIsLightRectangle3D(hoge,false);					// ���C�g�L��
	}
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	UninitLight();
	UninitCamera();
	//UninitPlayer();
	UninitEnemy();
	


}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{

	UpdateCamera();	// �J����
	UpdatePlayer();	// �v���C���[

	UpdateEnemy();	// �G�l�~�[
}

//=========================================
// �`��
//=========================================
void DrawGame()
{
	SetCamera();	// �J����

	DrawPlayer();		// �v���C���[
	DrawEnemy();		// �G�l�~�[
	DrawRectangle3D();	// ��`(3D)
	DrawRectangle();	// ��`(2D)

	// 2D�̑O��3D��u��
	GetDevice()->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	/* ��2D�̏�ɒu������3D�`�恫 */
}

//=========================================
// �|�[�Y�̐ݒ菈��
//=========================================
void SetEnablePause(bool bUse)
{

}

//=========================================
// �|�[�Y�̏�Ԃ��擾
//=========================================
bool GetPause(void)
{
	return false;
}
