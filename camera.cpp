//==================================================
// 
// �J�����̍쐬
// Author YudaKaito
// 
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "main.h"
#include "camera.h"
#include "input.h"

//--------------------------------------------------
// ��`
//--------------------------------------------------
#define CAMERA_MOVE			(1.0f);
#define CAMERA_ROT_VOLUME	(0.03f);

//--------------------------------------------------
// �ÓI�ϐ�
//--------------------------------------------------
static Camera s_camera;			// �J�������

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
static void InputCamera(void);	// �J�����̓��͏���

//==================================================
// ������
//==================================================
void InitCamera(void)
{
	ZeroMemory(&s_camera, sizeof(s_camera));

	// ���_�E�����_�E�������ݒ肷��
	s_camera.posV = D3DXVECTOR3(0.0f, 0.0f, -100.0f);	// ���_
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �����_
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// ������x�N�g��
	s_camera.rot = ZERO_VECTOR;	// ����

	s_camera.rot.y = atan2f((s_camera.posR.x - s_camera.posV.x), (s_camera.posR.z - s_camera.posV.z));
	s_camera.rot.x = atan2f((s_camera.posR.z - s_camera.posV.z), (s_camera.posR.y - s_camera.posV.y));

	D3DXVECTOR3 vec = s_camera.posV - s_camera.posR;
	s_camera.fDistance = D3DXVec3Length(&vec);
}

//==================================================
// �I��
//==================================================
void UninitCamera(void)
{

}

//==================================================
// �X�V
//==================================================
void UpdateCamera(void)
{
	//InputCamera();
}

//==================================================
// �ݒ�
//==================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera* camara = &(s_camera);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&camara->mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&camara->mtxView, &camara->posV, &camara->posR, &camara->vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &camara->mtxView);	// �r���[���W�s��̐ݒ�

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&camara->mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixOrthoLH(&camara->mtxProjection, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 10.0f, 820.0f);
	
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//==================================================
// ����
//==================================================
void InputCamera(void)
{
	Camera* pCamera = &(s_camera);

	pCamera->vec = ZERO_VECTOR;

	// ���_�p�x�̉�]
	if (GetKeyboardPress(DIK_Q))
	{
		pCamera->rot.y += -CAMERA_ROT_VOLUME;	// ��]��
	}
	if (GetKeyboardPress(DIK_E))
	{
		pCamera->rot.y += CAMERA_ROT_VOLUME;	// ��]��
	}
	if (GetKeyboardPress(DIK_W))
	{
		pCamera->posR.y += 2.0f;	// ��]��
		pCamera->posV.y += 2.0f;	// ��]��
	}
	if (GetKeyboardPress(DIK_S))
	{
		pCamera->posR.y += -2.0f;	// ��]��
		pCamera->posV.y += -2.0f;	// ��]��
	}
	if (GetKeyboardPress(DIK_A))
	{
		pCamera->posR.x += -2.0f;	// ��]��
		pCamera->posV.x += -2.0f;	// ��]��
	}
	if (GetKeyboardPress(DIK_D))
	{
		pCamera->posR.x += 2.0f;	// ��]��
		pCamera->posV.x += 2.0f;	// ��]��
	}

	D3DXVec3Normalize(&pCamera->vec, &pCamera->vec);	// ���K������(�傫���P�̃x�N�g���ɂ���)
	pCamera->vec *= CAMERA_MOVE;
	pCamera->posV += pCamera->vec;

	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.y = pCamera->posV.y + tanf(-pCamera->rot.x + (D3DX_PI * 0.5f)) * pCamera->fDistance;
}

//==================================================
// �J�����̏��擾
//==================================================
Camera* GetCamera(void)
{
	return &s_camera;
}

//==================================================
// �J�����̊p�x���擾
//==================================================
D3DXVECTOR3* GetRotCamera(void)
{
	return &s_camera.rot;
}