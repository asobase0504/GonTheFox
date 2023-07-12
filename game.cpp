//=========================================
// 
// ゲーム内処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
// 主要ヘッダー
#include "game.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "mode.h"
// 処理ヘッダー
#include "player.h"
#include "enemy.h"

#include <stdio.h>
// 描画ヘッダー
#include "camera.h"
#include "light.h"
#include "rectangle3D.h"
#include "rectangle2D.h"
#include "color.h"
// DEBUGヘッダー
#include <assert.h>
#include "lnhale.h"

//------------------------------------
// 定義
//------------------------------------
namespace
{

}

//------------------------------------
// 静的変数
//------------------------------------
namespace
{

}

//=========================================
// 初期化
//=========================================
void InitGame(void)
{


	InitLight();
	InitCamera();
	InitPlayer();
	InitEnemy();
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
		D3DXVECTOR3 size = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 15.0f, SCREEN_HEIGHT * 0.75f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, -SCREEN_HEIGHT * 0.75f);

		// 設定
		SetPosRectangle3D(hoge, pos);						// 位置
		SetSizeRectangle3D(hoge, size);						// 大きさ
		SetColorRectangle3D(hoge, GetColor(COLOR_BLUE));	// 色
		SetIsLightRectangle3D(hoge,false);					// ライト有無
	}
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitLight();
	UninitCamera();
	//UninitPlayer();
	UninitEnemy();
	


}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{

	UpdateCamera();	// カメラ
	UpdatePlayer();	// プレイヤー

	UpdateEnemy();	// エネミー
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	SetCamera();	// カメラ

	DrawPlayer();		// プレイヤー
	DrawEnemy();		// エネミー
	DrawRectangle3D();	// 矩形(3D)
	DrawRectangle();	// 矩形(2D)

	// 2Dの前に3Dを置く
	GetDevice()->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	/* ↓2Dの上に置きたい3D描画↓ */
}

//=========================================
// ポーズの設定処理
//=========================================
void SetEnablePause(bool bUse)
{

}

//=========================================
// ポーズの状態を取得
//=========================================
bool GetPause(void)
{
	return false;
}
