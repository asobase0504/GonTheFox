//**************************************************
// 
// Hackathon ( fan.cpp )
// Author  : Yuda Kaito
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "fan.h"
#include "color.h"
#include "texture.h"
#include "utility.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int	MAX_FAN = 1024;					// 円形の最大数
const int	NUM_VERTEX = 100;				// 頂点の数
const int	NUM_POLYGON = NUM_VERTEX - 2;	// ポリゴンの数

typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture;	// テクスチャ
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// 頂点バッファ
	int						nMaxBuff;	// 表示する頂点バッファの最大値
	bool					bUse;		// 使用しているかどうか
	bool					bDraw;		// 描画するかどうか
	bool					bAdd;		// 加算合成するかどうか
}MyFan;
}// namespaceはここまで

//==================================================
// スタティック変数
//==================================================
namespace
{
MyFan	s_aFan[MAX_FAN];	// 円形の情報
}// namespaceはここまで

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitFan(void)
{
	// メモリのクリア
	memset(s_aFan, 0, sizeof(s_aFan));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitFan(void)
{
	for (int i = 0; i < MAX_FAN; i++)
	{
		MyFan *pFan = &s_aFan[i];

		if (pFan->pVtxBuff != NULL)
		{// 頂点バッファの解放
			pFan->pVtxBuff->Release();
			pFan->pVtxBuff = NULL;
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawFan(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_FAN; i++)
	{
		MyFan *pFan = &s_aFan[i];

		if (!pFan->bUse || !pFan->bDraw)
		{// 使用していない、描画するしない
			continue;
		}

		/*↓ 使用している、描画する ↓*/

		if (pFan->bAdd)
		{// 加算合成する
			// レンダーステートの設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pFan->pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pFan->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLEFAN,	// プリミティブの種類
			0,					// 描画する最初の頂点インデックス
			pFan->nMaxBuff);	// プリミティブ(ポリゴン)数

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

		if (pFan->bAdd)
		{// 加算合成する
		 // レンダーステートを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetFan(TEXTURE texture)
{
	// 設定 [ テクスチャあり ]
	return SetFanWithTex(GetTexture(texture));
}

//--------------------------------------------------
// 設定 [ テクスチャあり ]
//--------------------------------------------------
int SetFanWithTex(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i < MAX_FAN; i++)
	{
		MyFan *pFan = &s_aFan[i];

		if (pFan->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pFan->pTexture = pTexture;
		pFan->nMaxBuff = NUM_POLYGON;
		pFan->bUse = true;
		pFan->bDraw = true;
		pFan->bAdd = false;

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_2D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pFan->pVtxBuff,
			NULL);

		VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		pFan->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点バッファをアンロックする
		pFan->pVtxBuff->Unlock();

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fLength = 0.0f;

		// 位置の設定
		SetPosFan(i, pos, fLength, true);

		//色の設定
		SetColorFan(i, GetColor(COLOR_WHITE));

		D3DXVECTOR2 texU = D3DXVECTOR2(0.0f, 1.0f);
		D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

		// テクスチャ座標の設定
		SetTexFan(i);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 使用をやめる
//--------------------------------------------------
void StopUseFan(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	pFan->bUse = false;

	if (pFan->pVtxBuff != NULL)
	{// 頂点バッファの解放
		pFan->pVtxBuff->Release();
		pFan->pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void SetPosFan(int nIdx, const D3DXVECTOR3 &pos, const float &fLength, bool bSide)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffFan(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = pos;
	DEBUG_PRINT("%f	%f\n", pVtx[0].pos.x, pVtx[0].pos.y);

	for (int i = 1; i < NUM_VERTEX; i++)
	{
		float fRot = (D3DX_PI * 2.0f / NUM_POLYGON) * (i - 1);

		// 角度の正規化
		NormalizeAngle(&fRot);

		if (bSide)
		{
			// 頂点座標の設定
			pVtx[i].pos.x = pos.x + cosf(fRot) * fLength;
			pVtx[i].pos.y = pos.y + sinf(fRot) * fLength;
			pVtx[i].pos.z = pos.z;

		}
		else
		{
			// 頂点座標の設定
			pVtx[i].pos.x = pos.x + cosf(fRot) * fLength;
			pVtx[i].pos.y = pos.y + sinf(fRot) * fLength;
			pVtx[i].pos.z = pos.z;
		}

		DEBUG_PRINT("%f	%f\n", pVtx[i].pos.x, pVtx[i].pos.y);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 回転する位置の設定
//--------------------------------------------------
void SetRotationPosFan(int nIdx, const D3DXVECTOR3 &pos, float fRot, float fLength, bool bSide)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffFan(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = pos;
	DEBUG_PRINT("%f	%f\n", pVtx[0].pos.x, pVtx[0].pos.y);

	for (int i = 1; i < NUM_VERTEX; i++)
	{
		float fRotData = ((D3DX_PI * 2.0f / NUM_POLYGON) * (i - 1)) - fRot;

		// 角度の正規化
		NormalizeAngle(&fRotData);

		if (bSide)
		{
			// 頂点座標の設定
			pVtx[i].pos.x = pos.x + cosf(fRotData) * fLength;
			pVtx[i].pos.y = pos.y + sinf(fRotData) * fLength;
			pVtx[i].pos.z = pos.z;

		}
		else
		{
			// 頂点座標の設定
			pVtx[i].pos.x = pos.x + sinf(fRotData) * fLength;
			pVtx[i].pos.y = pos.y + cosf(fRotData) * fLength;
			pVtx[i].pos.z = pos.z;
		}

		DEBUG_PRINT("%f	%f\n", pVtx[i].pos.x, pVtx[i].pos.y);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
void SetColorFan(int nIdx, const D3DXCOLOR &color)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffFan(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		float fRot = (D3DX_PI * 2.0f / NUM_POLYGON) * (i - 1);

		// 角度の正規化
		NormalizeAngle(&fRot);

		// 頂点カラーの設定
		pVtx[i].col = color;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
void SetColorCenterFan(int nIdx, const D3DXCOLOR & color)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffFan(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = color;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// テクスチャ座標の設定
//--------------------------------------------------
void SetTexFan(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;	// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffFan(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		// 頂点カラーの設定
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawFan(int nIdx, bool bDraw)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pFan->bDraw = bDraw;
}

//--------------------------------------------------
// 加算合成するかどうか
//--------------------------------------------------
void SetAddFan(int nIdx, bool bAdd)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pFan->bAdd = bAdd;
}

//--------------------------------------------------
// テクスチャの変更
//--------------------------------------------------
void ChangeTextureFan(int nIdx, TEXTURE texture)
{
	// テクスチャの変更 [ テクスチャあり ]
	ChangeTextureFanWithTex(nIdx, GetTexture(texture));
}

//--------------------------------------------------
// テクスチャの変更 [ テクスチャあり ]
//--------------------------------------------------
void ChangeTextureFanWithTex(int nIdx, LPDIRECT3DTEXTURE9 pTexture)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pFan->pTexture = pTexture;
}

//--------------------------------------------------
// 頂点バッファを取得
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffFan(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	if (!pFan->bUse)
	{// 使用していない
		return NULL;
	}

	/*↓ 使用している ↓*/

	return s_aFan[nIdx].pVtxBuff;
}

//--------------------------------------------------
// 描画する円の量を加算
//--------------------------------------------------
void AddDrawFan(int nIdx, int nVolume)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	pFan->nMaxBuff += nVolume;

	if (pFan->nMaxBuff > NUM_POLYGON)
	{
		pFan->nMaxBuff = NUM_POLYGON;
	}
	else if (pFan->nMaxBuff < 0)
	{
		pFan->nMaxBuff = 0;
	}
}

//--------------------------------------------------
// 描画する円の量をリセット
//--------------------------------------------------
void ResetDrawFan(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_FAN);

	MyFan *pFan = &s_aFan[nIdx];

	pFan->nMaxBuff = NUM_POLYGON;
}
