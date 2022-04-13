//=========================================
// 
// �f�o�b�O����
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "debug.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include <stdio.h>

//-----------------------------------------
// �}�N���錾
//-----------------------------------------
#define DEBUG_NUM	(9)

//-----------------------------------------
// �ÓI�ϐ��錾
//-----------------------------------------
static LPD3DXFONT s_pFont = NULL;	// �t�H���g�ւ̃|�C���^

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------

//=========================================
// FPS�\���̏�����
//=========================================
void InitDebug(void)
{
	// �f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(GetDevice(), 24, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "HGP����E", &s_pFont);
}

//=========================================
// FPS�\���̏I������
//=========================================
void UninitDebug(void)
{
	// �f�o�b�O�\���p�t�H���g�̔j��
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}
}

//=========================================
// FPS�̕\��
//=========================================
void DrawDebug(void)
{
	char aStr[DEBUG_NUM][256];	// �\������

	// ���̎擾
	Camera* camera = GetCamera();
	D3DXVECTOR3 stickL = GetJoypadStick(JOYKEY_LEFT_STICK, 0);
	D3DXVECTOR3 stickR = GetJoypadStick(JOYKEY_RIGHT_STICK, 0);

	// ������ɑ��
	wsprintf(&aStr[0][0],	"FPS : %d\n", GetFPS());
	sprintf(&aStr[1][0],	"rot : %f\n", camera->rot.y);
	sprintf(&aStr[2][0],	"posV     : %.3f | %.3f | %.3f\n", camera->posV.x, camera->posV.y, camera->posV.z);
	sprintf(&aStr[3][0],	"posR     : %.3f | %.3f | %.3f\n", camera->posR.x, camera->posR.y, camera->posR.z);
	sprintf(&aStr[4][0],	"posVDest : %.3f | %.3f | %.3f\n", camera->posVDest.x, camera->posVDest.y, camera->posVDest.z);
	sprintf(&aStr[5][0],	"posRDest : %.3f | %.3f | %.3f\n", camera->posRDest.x, camera->posRDest.y, camera->posRDest.z);
	sprintf(&aStr[6][0],	"\n");
	sprintf(&aStr[7][0],	"stickL   : %.3f | %.3f | %.3f\n", stickL.x, stickL.y, stickL.z);
	sprintf(&aStr[8][0],	"stickR   : %.3f | %.3f | %.3f\n", stickR.x, stickR.y, stickR.z);

	// �\���̈�̍쐬
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	for (int i = 0; i < DEBUG_NUM; i++)
	{
		// �e�L�X�g�̕`��
		rect = { 0,i * 30,SCREEN_WIDTH,SCREEN_HEIGHT };

		s_pFont->DrawText(NULL, &aStr[i][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	}
}