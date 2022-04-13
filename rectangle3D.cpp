//==================================================
// 
// ポリゴンの作成
// Author YudaKaito
// 
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include "main.h"
#include "rectangle3D.h"
#include "camera.h"
#include "color.h"
#include "texture.h"
#include <assert.h>

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_POLYGON	(1024)	// ポリゴンの最大数
#define NUM_VERTEX	(4)		// 頂点数
#define NUM_POLYGON	(2)		// ポリゴン数

//--------------------------------------------------
// ポリゴンの構造体を定義
//--------------------------------------------------
typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3				pos;		// 頂点座標
	D3DXVECTOR3				rot;		// 回転座標
	D3DXMATRIX				mtxWorld;	// ワールドマトリックス
	bool					bUse;		// 使用フラグ
	bool					bDraw;		// 描画フラグ
} MyRectangle3D;

//--------------------------------------------------
// 静的変数
//--------------------------------------------------
static MyRectangle3D s_aRectangle3D[MAX_POLYGON];		// ポリゴンの構造体

//==================================================
// 初期化
//==================================================
void InitRectangle3D(void)
{
	ZeroMemory(s_aRectangle3D, sizeof(s_aRectangle3D));	// 初期化
}

//==================================================
// 終了
//==================================================
void UninitRectangle3D(void)
{
	for (int i = 0; i < MAX_POLYGON; i++)
	{
		// 頂点バッファーの破棄
		if (s_aRectangle3D[i].pVtxBuff != NULL)
		{
			s_aRectangle3D[i].pVtxBuff->Release();
			s_aRectangle3D[i].pVtxBuff = NULL;
		}
	}

}

//==================================================
// 描画
//==================================================
void DrawRectangle3D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int i = 0; i < MAX_POLYGON; i++)
	{
		MyRectangle3D *pRectAngle3D = &s_aRectangle3D[i];

		if (!pRectAngle3D->bUse || !pRectAngle3D->bDraw)
		{
			continue;
		}

		/*↓ 使用している、描画する ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pRectAngle3D->mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pRectAngle3D->pos.x, pRectAngle3D->pos.y, pRectAngle3D->pos.z);	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pRectAngle3D->rot.y, pRectAngle3D->rot.x, pRectAngle3D->rot.z);		// ヨーピッチロールによる行列回転
		D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxRot);							// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pRectAngle3D->mtxWorld);	// ワールド座標行列の設定

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, pRectAngle3D->pVtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->SetRenderState(D3DRS_CULLMODE, 1);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NOTEQUAL);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, pRectAngle3D->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		pDevice->SetRenderState(D3DRS_CULLMODE, 3);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

	}
}

//==================================================
// 設定
//==================================================
int SetRectangle3D(TEXTURE texture)
{
	// 設定 [ テクスチャあり ]
	return SetRectangle3DWithTex(GetTexture(texture));
}

//==================================================
// 設定 [ テクスチャあり ]
//==================================================
int SetRectangle3DWithTex(LPDIRECT3DTEXTURE9 pTexture)
{
	for (int i = 0; i <= MAX_POLYGON; i++)
	{
		MyRectangle3D* polygon = &s_aRectangle3D[i];

		if (polygon->bUse)
		{
			continue;
		}

		polygon->pTexture = pTexture;
		polygon->bUse = true;
		polygon->bDraw = true;

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&polygon->pVtxBuff,
			NULL);

		// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(-(D3DX_PI / 2.0f), 0.0f, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線の設定
		SetNormalRectangle3D(i, nor);

		// 位置の設定
		SetPosRectangle3D(i, pos);

		// 位置の設定
		SetRotationRectangle3D(i, rot);

		// 大きさの設定
		SetSizeRectangle3D(i, size);

		// 頂点カラーの設定
		SetColorRectangle3D(i, GetColor(COLOR_WHITE));

		D3DXVECTOR2 texU = D3DXVECTOR2(0.0f, 1.0f);
		D3DXVECTOR2 texV = D3DXVECTOR2(0.0f, 1.0f);

		// テクスチャ座標の設定
		SetTexRectangle3D(i, texU, texV);

		return i;
	}

	assert(false);
	return -1;
}

//==================================================
// 使うのを止める
// 引数  : int nIdx / インデックス
//==================================================
void StopUseRectangle3D(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	pRectAngle3D->bUse = false;

	if (pRectAngle3D->pVtxBuff != NULL)
	{// 頂点バッファの解放
		pRectAngle3D->pVtxBuff->Release();
		pRectAngle3D->pVtxBuff = NULL;
	}
}

//==================================================
// 位置の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 &pos / 移動位置
//==================================================
void SetPosRectangle3D(int nIdx, const D3DXVECTOR3 &pos)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	D3DXMATRIX mtxTrans;	// 計算用マトリックス

	pRectAngle3D->pos = pos;

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&pRectAngle3D->mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

	//// 位置を反映
	//D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	//D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
}

//==================================================
// 回転の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 &rot / 向き
//==================================================
void SetRotationRectangle3D(int nIdx, const D3DXVECTOR3 &rot)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	D3DXMATRIX mtxRot;	// 計算用マトリックス

	pRectAngle3D->rot = rot;

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&pRectAngle3D->mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

	//// 向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		// ヨーピッチロールによる行列回転
	//D3DXMatrixMultiply(&pRectAngle3D->mtxWorld, &pRectAngle3D->mtxWorld, &mtxRot);							// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
}

//==================================================
// 大きさの設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 &size / 大きさ
//==================================================
void SetSizeRectangle3D(int nIdx, const D3DXVECTOR3 &size)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].pos = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

}

//==================================================
// 法線の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 &nor / 法線
//==================================================
void SetNormalRectangle3D(int nIdx, const D3DXVECTOR3 &nor)
{
	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//==================================================
// 色の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXCOLOR &color / 色
//==================================================
void SetColorRectangle3D(int nIdx, const D3DXCOLOR &color)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

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

//==================================================
// テクスチャ座標の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR2 &texU / x = 左端, y = 右端
// 引数3  : D3DXVECTOR2 &texV / x = 上端, y = 下端
//==================================================
void SetTexRectangle3D(int nIdx, const D3DXVECTOR2 &texU, const D3DXVECTOR2 &texV)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuffRectangle3D(nIdx);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(texU.x, texV.x);
	pVtx[1].tex = D3DXVECTOR2(texU.y, texV.x);
	pVtx[2].tex = D3DXVECTOR2(texU.x, texV.y);
	pVtx[3].tex = D3DXVECTOR2(texU.y, texV.y);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

}

//==================================================
// 描画するかどうか
// 引数1  : int nIdx / インデックス
// 引数2  : bool bDraw / 描画するかどうか
//==================================================
void SetDrawRectangle3D(int nIdx, bool bDraw)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectAngle3D->bDraw = bDraw;

}

//==================================================
// テクスチャの変更
// 引数1  : int nIdx / インデックス
// 引数2  : TEXTURE texture / 列挙型 種類
//==================================================
void ChangeTextureRectangle3D(int nIdx, TEXTURE texture)
{
	// テクスチャの変更 [ テクスチャあり ]
	ChangeTextureRectangle3DWithTex(nIdx, GetTexture(texture));
}

//==================================================
// テクスチャの変更 [ テクスチャあり ]
// 引数1  : int nIdx / インデックス
// 引数2  : TEXTURE texture / 列挙型 種類
//==================================================
void ChangeTextureRectangle3DWithTex(int nIdx, LPDIRECT3DTEXTURE9 pTexture)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return;
	}

	/*↓ 使用している ↓*/

	pRectAngle3D->pTexture = pTexture;
}

//==================================================
// 頂点バッファの取得
// 引数  : int nIdx / インデックス
// 返値  : LPDIRECT3DVERTEXBUFFER9 / 頂点バッファ
//==================================================
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle3D(int nIdx)
{
	assert(nIdx >= 0 && nIdx < MAX_POLYGON);

	MyRectangle3D *pRectAngle3D = &s_aRectangle3D[nIdx];

	if (!pRectAngle3D->bUse)
	{// 使用していない
		return NULL;
	}

	/*↓ 使用している ↓*/

	return s_aRectangle3D[nIdx].pVtxBuff;
}