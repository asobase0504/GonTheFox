//============================
//
// 吸い込み処理
// Author  :Hamada Ryuuga
//
//============================

#include "main.h"
#include "lnhale.h"
#include "color.h"
#include "input.h"
#include "player.h"
//--------------------------
//マクロ
//--------------------------
#define LNHALESIZE (D3DXVECTOR3(100.0f,50.0f,0.0f))
#define LNHALEPOS (D3DXVECTOR3(0.0f,0.0f,0.0f))
//--------------------------
//初期化関数
//--------------------------
void CLnhale::Init(void)
{
	bUse = false; 						// 使用しているかどうか

	// 頂点バッファの生成
	GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = pos + D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[1].pos = pos + D3DXVECTOR3(+size.x, -size.y, 0.0f);
	pVtx[2].pos = pos + D3DXVECTOR3(-size.x, +size.y, 0.0f);
	pVtx[3].pos = pos + D3DXVECTOR3(+size.x, +size.y, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = GetColor(COLOR_RED);
	pVtx[1].col = GetColor(COLOR_RED);
	pVtx[2].col = GetColor(COLOR_RED);
	pVtx[3].col = GetColor(COLOR_RED);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

}

//--------------------------
//破棄関数
//--------------------------
void CLnhale::Uninit(void)
{

}

//--------------------------
//更新関数
//--------------------------
void CLnhale::Update(void)
{
	if (bUse)
	{
		if (!GetKeyboardPress(DIK_B))
		{
			bUse = false;
		}

		//Playerの座標付近に常に変更する
		//PLAYER* pPlayer = GetPlayer();
		//pos = pPlayer->pos;

		VERTEX_2D *pVtx = NULL;		// 頂点情報へのポインタ
		// 頂点情報をロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = pos + D3DXVECTOR3(-size.x, -size.y, 0.0f);
		pVtx[1].pos = pos + D3DXVECTOR3(+size.x, -size.y, 0.0f);
		pVtx[2].pos = pos + D3DXVECTOR3(-size.x, +size.y, 0.0f);
		pVtx[3].pos = pos + D3DXVECTOR3(+size.x, +size.y, 0.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//--------------------------
//描画関数
//--------------------------
void CLnhale::Draw(void)
{	
	
		/*↓ 使用している、描画する ↓*/
		// デバイスへのポインタの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		 // レンダーステートの設定
	/*	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		//pDevice->SetTexture(0,pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,		// プリミティブの種類
			0,							// 描画する最初の頂点インデックス
			2);				// プリミティブ(ポリゴン)数

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

		// レンダーステートを元に戻す
		//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
}

//--------------------------
//セット関数
//--------------------------
void CLnhale::Set(D3DXVECTOR3 setpos)
{
	if (!bUse)
	{
		pos = setpos;						//座標
		bUse = true; 						// 使用しているかどうか
	}
 }

//--------------------------
//当たり判定関数
//--------------------------
bool CLnhale::Hit(D3DXVECTOR3 nowpos, D3DXVECTOR3 hitsize)
{
	bool Hit = false;

	if (bUse)
	{	//吸い込む物との当たり判定
		if ((nowpos.x - hitsize.x >= pos.x + size.x|| nowpos.x + hitsize.x <= pos.x - size.x)&& 
			(nowpos.y - hitsize.y >= pos.y + size.y || nowpos.y + hitsize.y <= pos.y - size.y))
		{
			Hit = true;
		}
	}
	return Hit;
}

//-------------------------------------------------
//コンストラクタ（インスタンス生成時に通ります）
//-------------------------------------------------
CLnhale::CLnhale()
{
	pos = LNHALEPOS;					//座標
	size = LNHALESIZE;					//サイズ

	pTexture = NULL;					// テクスチャ
	pVtxBuff = NULL;					// 頂点バッファ
	
}

//-----------------------------------------------
//デストラクタ（インスタンス破棄時に通ります）
//-----------------------------------------------
CLnhale::~CLnhale()
{
		if (pTexture != NULL)
		{// 頂点バッファの解放
			pTexture->Release();
			pTexture = NULL;
		}

		if (pVtxBuff != NULL)
		{// 頂点バッファの解放
			pVtxBuff->Release();
			pVtxBuff = NULL;
		}
}