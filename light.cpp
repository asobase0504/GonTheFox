//=========================================
// 
// ライトの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "light.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define LIGTH_MAX	(5)

//------------------------------------
// 静的変数
//------------------------------------
static D3DLIGHT9 s_light[LIGTH_MAX];	//ライト情報

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void SetLight(D3DLIGHTTYPE type, D3DXCOLOR col, D3DXVECTOR3 vec, int nNum);

//=========================================
// 初期化
//=========================================
void InitLight(void)
{
	// ライトをクリアする
	ZeroMemory(s_light, sizeof(s_light));

	// ライトの設定
	SetLight(D3DLIGHT_DIRECTIONAL,D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR3(0.2f, -0.8f, 0.4f), 0);
	SetLight(D3DLIGHT_DIRECTIONAL,D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR3(0.2f, -0.8f, -0.4f), 1);
	SetLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR3(-0.2f, 0.8f, 0.4f), 2);
	//SetLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR3(-0.2f, 0.8f, 0.4f), 3);

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_AMBIENT, 0xff363333);
}

//=========================================
// 終了
//=========================================
void UninitLight(void)
{
}

//=========================================
// 更新
//=========================================
void UpdateLight(void)
{
}

//=========================================
// 設定
//=========================================
static void SetLight(D3DLIGHTTYPE type,D3DXCOLOR col, D3DXVECTOR3 vec, int nNum)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 vecDir;		// ライトの方向ベクトル

	s_light[nNum].Type = D3DLIGHT_DIRECTIONAL;	// ライトの種類を設定 ( 平行光源 )
	s_light[nNum].Diffuse = col;	// ライトの拡散光を設定 ( ライトの色 )

	vecDir = vec;	// ライトの方向を設定

	// 正規化する ( 大きさ 1 のベクトルにする )
	D3DXVec3Normalize(&vecDir, &vecDir);
	s_light[nNum].Direction = vecDir;

	// ライトを設定する
	pDevice->SetLight(nNum, &s_light[nNum]);

	// ライトを有効にする
	pDevice->LightEnable(nNum, TRUE);

}
