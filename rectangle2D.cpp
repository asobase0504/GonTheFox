//**************************************************
// 
// Hackathon ( rectangle.cpp )
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "rectangle2D.h"
#include "color.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
#define	MAX_RECTANGLE	(1024)	// 矩形の最大数
#define	NUM_VERTEX		(4)			// 頂点の数
#define	NUM_POLYGON		(2)			// ポリゴンの数

typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture;	// テクスチャ
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// 頂点バッファ
	bool					bUse;		// 使用しているかどうか
	bool					bDraw;		// 描画するかどうか
	bool					bAdd;		// 加算合成するかどうか
}MyRectangle2D;

//==================================================
// スタティック変数
//==================================================
static MyRectangle2D	s_aRectangle2D[MAX_RECTANGLE];	// 矩形の情報

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRectangle(void)
{
	// メモリのクリア
	memset(s_aRectangle2D, 0, sizeof(s_aRectangle2D));
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectangle(void)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		MyRectangle2D *pRectangle = &s_aRectangle2D[i];

		if (pRectangle->pVtxBuff != NULL)
		{// 頂点バッファの解放
			pRectangle->pVtxBuff->Release();
			pRectangle->pVtxBuff = NULL;
		}
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectangle(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		MyRectangle2D *pRectangle = &s_aRectangle2D[i];

		if (!pRectangle->bUse || !pRectangle->bDraw)
		{// 使用していない、描画するしない
			continue;
		}

		/*↓ 使用している、描画する ↓*/

		if (pRectangle->bAdd)
		{// 加算合成する
			// レンダーステートの設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pRectangle->pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pRectangle->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			NUM_POLYGON);				// プリミティブ(ポリゴン)数

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

		if (pRectangle->bAdd)
		{// 加算合成する
			// レンダーステートを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
int SetRectangle(TEXTURE texture)
{
	// 設定 [ テクスチャあり ]
	return SetRectangleWithTex(GetTexture(texture));
}

//--------------------------------------------------
// 設定 [ テクスチャあり ]
//--------------------------------------------------
int SetRectangleWithTex(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i < MAX_RECTANGLE; i++)
	{
		MyRectangle2D *pRectangle = &s_aRectangle2D[i];

		if (pRectangle->bUse)
		{// 使用している
			continue;
		}

		/*↓ 使用していない ↓*/

		pRectangle->pTexture = pTexture;
		pRectangle->bUse = true;
		pRectangle->bDraw = true;
		pRectangle->bAdd = false;

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(
			sizeof(VERTEX_2D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pRectangle->pVtxBuff,
			NULL);

		VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

		// 頂点情報をロックし、頂点情報へのポインタを取得
		pRectangle->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点バッファをアンロックする
		pRectangle->pVtxBuff->Unlock();

		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 位置の設定
		SetPosRectangle(i, pos, size);

		//色の設定
		SetColorRectangle(i, GetColor(COLOR_WHITE));

		D3DXVECTOR2 texU = D3DXVECTOR2(0.0f, 1.0f);
		D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

		// テクスチャ座標の設定
		SetTexRectangle(i, texU, texV);

		return i;
	}

	assert(false);
	return -1;
}

//--------------------------------------------------
// 使用をやめる
//--------------------------------------------------
void StopUseRectangle(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	pRectangle->bUse = false;

	if (pRectangle->pVtxBuff != NULL)
	{// 頂点バッファの解放
		pRectangle->pVtxBuff->Release();
		pRectangle->pVtxBuff = NULL;
	}
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void SetPosRectangle(int nIdx, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = size.x * 0.5f;
	float fHeight = size.y * 0.5f;

	// 頂点座標の設定
	pVtx[0].pos = pos + D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(+fWidth, -fHeight, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-fWidth, +fHeight, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(+fWidth, +fHeight, 0.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 回転する位置の設定
//--------------------------------------------------
void SetRotationPosRectangle(int nIdx, const D3DXVECTOR3 &pos, float fRot, float fWidth, float fHeight)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXMATRIX mtx, mtxTrans;

	// 回転の反映
	D3DXMatrixRotationZ(&mtx, -fRot);

	// 位置の反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	D3DXVECTOR3 pVtxpos[NUM_VERTEX];

	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;

	pVtxpos[0] = D3DXVECTOR3(-fHalfWidth, -fHalfHeight, 0.0f);
	pVtxpos[1] = D3DXVECTOR3(+fHalfWidth, -fHalfHeight, 0.0f);
	pVtxpos[2] = D3DXVECTOR3(-fHalfWidth, +fHalfHeight, 0.0f);
	pVtxpos[3] = D3DXVECTOR3(+fHalfWidth, +fHalfHeight, 0.0f);
	
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		D3DXVec3TransformCoord(&pVtx[i].pos, &pVtxpos[i], &mtx);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 色の設定
//--------------------------------------------------
void SetColorRectangle(int nIdx, const D3DXCOLOR &color)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// テクスチャ座標の設定
//--------------------------------------------------
void SetTexRectangle(int nIdx, const D3DXVECTOR2 &texU, const D3DXVECTOR2 &texV)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(texU.x, texV.x);
	pVtx[1].tex = D3DXVECTOR2(texU.y, texV.x);
	pVtx[2].tex = D3DXVECTOR2(texU.x, texV.y);
	pVtx[3].tex = D3DXVECTOR2(texU.y, texV.y);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//--------------------------------------------------
// 描画するかどうか
//--------------------------------------------------
void SetDrawRectangle(int nIdx, bool bDraw)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle->bDraw = bDraw;
}

//--------------------------------------------------
// 加算合成するかどうか
//--------------------------------------------------
void SetAddRectangle(int nIdx, bool bAdd)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle->bAdd = bAdd;
}

//--------------------------------------------------
// テクスチャの変更
//--------------------------------------------------
void ChangeTextureRectangle(int nIdx, TEXTURE texture)
{
	// テクスチャの変更 [ テクスチャあり ]
	ChangeTextureRectangleWithTex(nIdx, GetTexture(texture));
}

//--------------------------------------------------
// テクスチャの変更 [ テクスチャあり ]
//--------------------------------------------------
void ChangeTextureRectangleWithTex(int nIdx, LPDIRECT3DTEXTURE9 pTexture)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectangle->pTexture = pTexture;
}

//--------------------------------------------------
// 頂点バッファを取得
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_RECTANGLE);

	MyRectangle2D *pRectangle = &s_aRectangle2D[nIdx];

	if (!pRectangle->bUse)
	{// 使用していない
		return NULL;
	}

	/*↓ 使用している ↓*/

	return s_aRectangle2D[nIdx].pVtxBuff;
}
