//**************************************************
//
// Hackathon ( mode.cpp )
// Author  : katsuki mizuki
//
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "fade.h"
#include "game.h"
#include "mode.h"
#include "title.h"
#include "rectangle2D.h"
#include "rectangle3D.h"
#include "texture.h"
#include "result.h"
#include "fan.h"
#include <assert.h>
#include <time.h>

//==================================================
// �X�^�e�B�b�N�ϐ�
//==================================================
namespace
{
MODE	s_mode = MODE_NONE;		// ���݂̃��[�h
MODE	s_modeNext = MODE_NONE;	// ���̃��[�h
}// namesapce�͂����܂�

//--------------------------------------------------
// ������
//--------------------------------------------------
void InitMode(void)
{
	// �e�N�X�`���̓ǂݍ���
	srand((unsigned int)time(NULL));

	// �e�N�X�`���̓ǂݍ���
	LoadTexture();

	// ������
	InitRectangle();	// ��`(2D)
	InitRectangle();	// ��`(3D)
	InitFan();			// �~�`
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void UninitMode(void)
{
	// �I������
	UninitTitle();			// �^�C�g��
	UninitGame();			// �Q�[��
	UninitResult();			// ���U���g
	UninitRectangle();		// ��`(2D)
	UninitRectangle3D();	// ��`(3D)
	UninitFan();			// �~�`
	UninitTexture();		// �e�N�X�`��
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void UpdateMode(void)
{
	switch (s_mode)
	{// �ǂ̃��[�h�H
	case MODE_TITLE:	// �^�C�g��
		UpdateTitle();
		break;
	case MODE_GAME:		// �Q�[��
		UpdateGame();
		break;

	case MODE_RESULT:	// ���U���g
		UpdateResult();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void DrawMode(void)
{
	switch (s_mode)
	{// �ǂ̃��[�h�H
	case MODE_TITLE:	// �^�C�g��
		DrawTitle();
		break;

	case MODE_GAME:		// �Q�[��
		DrawGame();
		break;

	case MODE_RESULT:	// ���U���g
		DrawResult();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void SetMode(void)
{
	if (s_modeNext == MODE_NONE)
	{// ���̃��[�h�����܂��ĂȂ�
		return;
	}

	if (GetFade() == FADE_NONE)
	{// �������Ă��Ȃ���ԂȂ�
		StartFadeOut();
	}

	if (GetFade() != FADE_IN)
	{// �t�F�[�h�C����Ԃ���Ȃ�
		return;
	}

	switch (s_mode)
	{// ���݂̃��[�h�̏I��
	case MODE_TITLE:	// �^�C�g��
		UninitTitle();
		break;

	case MODE_GAME:		// �Q�[��
		UninitGame();
		break;

	case MODE_RESULT:	// ���U���g
		UninitResult();
		break;

	case MODE_NONE:
		/* �����Ȃ� */
		break;

	default:
		assert(false);
		break;
	}

	// ��`(2D)�̏I��
	UninitRectangle();

	// ��`(2D)�̏�����
	InitRectangle();

	// ��`(3D)�̏I��
	UninitRectangle3D();

	// ��`(3D)�̏�����
	InitRectangle3D();

	// �~�`�̏I��
	UninitFan();

	// �~�`�̏�����
	InitFan();

	s_mode = s_modeNext;	// ���݂̉��(���[�h)��؂�ւ���
	
	switch (s_modeNext)
	{// ���̃��[�h�̏�����
	case MODE_TITLE:	// �^�C�g��
		InitTitle();
		break;

	case MODE_GAME:		// �Q�[��
		InitGame();
		break;

	case MODE_RESULT:	// ���U���g
		InitResult();
		break;

	case MODE_NONE:
	default:
		assert(false);
		break;
	}

	s_modeNext = MODE_NONE;
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
MODE GetMode(void)
{
	return s_mode;
}

//--------------------------------------------------
// �ύX
//--------------------------------------------------
void ChangeMode(MODE modeNext)
{
	assert(modeNext >= 0 && modeNext < MODE_MAX);

	s_modeNext = modeNext;
}
