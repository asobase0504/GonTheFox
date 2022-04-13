//=========================================
// 
// メインのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _MAIN_H_	// このマクロ定義がされてなかったら
#define _MAIN_H_	// 2重インクルード防止のマクロ定義


//------------------------------------
// include
//------------------------------------
#include <windows.h>
#include "d3dx9.h"						// 描画処理に必要
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"						// 入力処理に必要
#include "xaudio2.h"					// 音楽処理に必要
#include "Xinput.h"						// ジョイパット処理に必要

//------------------------------------
// ライブラリのリンク
//------------------------------------
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要
#pragma comment(lib,"xinput.lib")	// ジョイパット処理に必要

#define SCREEN_WIDTH	(80 * 16)			// ウィンドウの幅
#define SCREEN_HEIGHT	(80 * 9)			// ウインドウの高さ

#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// 座標・カラー・テクスチャ座標
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 座標・法線・カラー・テクスチャ座標
#define FVF_VERTEX_LINE		(D3DFVF_XYZ | D3DFVF_DIFFUSE)							// 頂点フォーマット[線] 位置・カラー

#define ZERO_VECTOR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// ゼロベクトル
#define ZERO_QUATERNION	(D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f))

// デバッグ用
#define _GETMODEL_POP

#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_PRINT(...) _RPT_BASE(_CRT_WARN, __FILE__, __LINE__, NULL, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#endif

//------------------------------------
// 頂点情報[2D]の構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数（1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャの座標
} VERTEX_2D;

//------------------------------------
// 頂点情報[3D]の構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャの座標
} VERTEX_3D;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);	// デバイスの取得
int GetFPS(void);

#endif	// !_MAIN_H_