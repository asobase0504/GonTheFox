//=========================================
// 
// タイトルの処理
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
// マクロ定義
//-----------------------------------------

//-----------------------------------------
// 静的変数
//-----------------------------------------
bool s_bExit = false;	// ゲーム自体の終了フラグ

//=========================================
// 初期化処理
//=========================================
void InitTitle(void)
{
	InitLight();
	InitCamera();

	// DEBUGのため(矩形2D)
	{
		int data = SetRectangle(TEXTURE_NONE);
		D3DXVECTOR3 size = D3DXVECTOR3(300.0f, 300.0f, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.275f, SCREEN_HEIGHT * 0.5f, 0.0f);

		SetColorRectangle(data, GetColor(COLOR_RED));
		// 矩形の位置の設定
		SetPosRectangle(data, pos, size);
	}

	// DEBUGのため(矩形3D)
	{
		int hoge = SetRectangle3D(TEXTURE_NONE);
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, SCREEN_HEIGHT * 0.5f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 大きさの設定
		SetSizeRectangle3D(hoge, size);

		// 色の設定
		SetColorRectangle3D(hoge, GetColor(COLOR_BLUE));
		
		// 矩形の位置の設定
		SetPosRectangle3D(hoge, pos);
	}
}

//=========================================
// 終了処理
//=========================================
void UninitTitle(void)
{

}

//=========================================
// 更新処理
//=========================================
void UpdateTitle(void)
{
	UpdateCamera();
}

//=========================================
// 選択処理
//=========================================
void SelectTitle(void)
{

}

//=========================================
// 描画処理
//=========================================
void DrawTitle(void)
{
	SetCamera();		// カメラ
	DrawRectangle3D();
	DrawRectangle();	// 矩形
}

//=========================================
// 終了情報を取得
//=========================================
bool GetExit(void)
{
	return s_bExit;
}
