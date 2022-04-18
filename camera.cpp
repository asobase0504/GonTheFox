//==================================================
// 
// カメラの作成
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
// 定義
//--------------------------------------------------
#define CAMERA_MOVE			(1.0f);
#define CAMERA_ROT_VOLUME	(0.03f);

//--------------------------------------------------
// 静的変数
//--------------------------------------------------
static Camera s_camera;			// カメラ情報

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void InputCamera(void);	// カメラの入力処理

//==================================================
// 初期化
//==================================================
void InitCamera(void)
{
	ZeroMemory(&s_camera, sizeof(s_camera));

	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f, 0.0f, -100.0f);	// 視点
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 注視点
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// 上方向ベクトル
	s_camera.rot = ZERO_VECTOR;	// 向き

	s_camera.rot.y = atan2f((s_camera.posR.x - s_camera.posV.x), (s_camera.posR.z - s_camera.posV.z));
	s_camera.rot.x = atan2f((s_camera.posR.z - s_camera.posV.z), (s_camera.posR.y - s_camera.posV.y));

	D3DXVECTOR3 vec = s_camera.posV - s_camera.posR;
	s_camera.fDistance = D3DXVec3Length(&vec);
}

//==================================================
// 終了
//==================================================
void UninitCamera(void)
{

}

//==================================================
// 更新
//==================================================
void UpdateCamera(void)
{
	//InputCamera();
}

//==================================================
// 設定
//==================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera* camara = &(s_camera);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camara->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&camara->mtxView, &camara->posV, &camara->posR, &camara->vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camara->mtxView);	// ビュー座標行列の設定

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&camara->mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixOrthoLH(&camara->mtxProjection, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 10.0f, 820.0f);
	
	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//==================================================
// 入力
//==================================================
void InputCamera(void)
{
	Camera* pCamera = &(s_camera);

	pCamera->vec = ZERO_VECTOR;

	// 視点角度の回転
	if (GetKeyboardPress(DIK_Q))
	{
		pCamera->rot.y += -CAMERA_ROT_VOLUME;	// 回転量
	}
	if (GetKeyboardPress(DIK_E))
	{
		pCamera->rot.y += CAMERA_ROT_VOLUME;	// 回転量
	}
	if (GetKeyboardPress(DIK_W))
	{
		pCamera->posR.y += 2.0f;	// 回転量
		pCamera->posV.y += 2.0f;	// 回転量
	}
	if (GetKeyboardPress(DIK_S))
	{
		pCamera->posR.y += -2.0f;	// 回転量
		pCamera->posV.y += -2.0f;	// 回転量
	}
	if (GetKeyboardPress(DIK_A))
	{
		pCamera->posR.x += -2.0f;	// 回転量
		pCamera->posV.x += -2.0f;	// 回転量
	}
	if (GetKeyboardPress(DIK_D))
	{
		pCamera->posR.x += 2.0f;	// 回転量
		pCamera->posV.x += 2.0f;	// 回転量
	}

	D3DXVec3Normalize(&pCamera->vec, &pCamera->vec);	// 正規化する(大きさ１のベクトルにする)
	pCamera->vec *= CAMERA_MOVE;
	pCamera->posV += pCamera->vec;

	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.y = pCamera->posV.y + tanf(-pCamera->rot.x + (D3DX_PI * 0.5f)) * pCamera->fDistance;
}

//==================================================
// カメラの情報取得
//==================================================
Camera* GetCamera(void)
{
	return &s_camera;
}

//==================================================
// カメラの角度情報取得
//==================================================
D3DXVECTOR3* GetRotCamera(void)
{
	return &s_camera.rot;
}