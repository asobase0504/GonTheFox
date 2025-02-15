//**************************************************
// 
// Hackathon ( rectangle.h )
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _RECTANGLE2D_H_	//このマクロ定義がされてなかったら
#define _RECTANGLE2D_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "main.h"
#include "texture.h"

//==================================================
// プロトタイプ宣言
//==================================================
//--------------------------------------------------
// 初期化
//--------------------------------------------------
void InitRectangle(void);

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitRectangle(void);

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawRectangle(void);

//--------------------------------------------------
// 設定
// 引数  : TEXTURE texture / 列挙型 種類
//--------------------------------------------------
int SetRectangle(TEXTURE texture);

//--------------------------------------------------
// 設定 [ テクスチャあり ]
// 引数  : LPDIRECT3DTEXTURE9 pTexture / テクスチャ
//--------------------------------------------------
int SetRectangleWithTex(LPDIRECT3DTEXTURE9 pTexture);

//--------------------------------------------------
// 使うのを止める
// 引数  : int nIdx / インデックス
//--------------------------------------------------
void StopUseRectangle(int nIdx);

//--------------------------------------------------
// 位置の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 &pos / 中心の位置
// 引数3  : D3DXVECTOR3 &size / サイズ
//--------------------------------------------------
void SetPosRectangle(int nIdx, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size);

//--------------------------------------------------
// 回転する位置の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR3 &pos / 中心の位置
// 引数3  : float fRot / 向き
// 引数4  : float fWidth / 幅
// 引数5  : float fHeight / 高さ
//--------------------------------------------------
void SetRotationPosRectangle(int nIdx, const D3DXVECTOR3 &pos, float fRot, float fWidth, float fHeight);

//--------------------------------------------------
// 色の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXCOLOR &color / 色
//--------------------------------------------------
void SetColorRectangle(int nIdx, const D3DXCOLOR &color);

//--------------------------------------------------
// テクスチャ座標の設定
// 引数1  : int nIdx / インデックス
// 引数2  : D3DXVECTOR2 &texU / x = 左端, y = 右端
// 引数3  : D3DXVECTOR2 &texV / x = 上端, y = 下端
//--------------------------------------------------
void SetTexRectangle(int nIdx, const D3DXVECTOR2 &texU, const D3DXVECTOR2 &texV);

//--------------------------------------------------
// 描画するかどうか
// 引数1  : int nIdx / インデックス
// 引数2  : bool bDraw / 描画するかどうか
//--------------------------------------------------
void SetDrawRectangle(int nIdx, bool bDraw);

//--------------------------------------------------
// 加算合成するかどうか
// 引数1  : int nIdx / インデックス
// 引数2  : bool bDraw / 加算合成するかどうか
//--------------------------------------------------
void SetAddRectangle(int nIdx, bool bAdd);

//--------------------------------------------------
// テクスチャの変更
// 引数1  : int nIdx / インデックス
// 引数2  : TEXTURE texture / 列挙型 種類
//--------------------------------------------------
void ChangeTextureRectangle(int nIdx, TEXTURE texture);

//--------------------------------------------------
// テクスチャの変更 [ テクスチャあり ]
// 引数1  : int nIdx / インデックス
// 引数2  : TEXTURE texture / 列挙型 種類
//--------------------------------------------------
void ChangeTextureRectangleWithTex(int nIdx, LPDIRECT3DTEXTURE9 pTexture);

//--------------------------------------------------
// 頂点バッファの取得
// 引数  : int nIdx / インデックス
// 返値  : LPDIRECT3DVERTEXBUFFER9 / 頂点バッファ
//--------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 GetVtxBuffRectangle(int nIdx);

#endif // !_RECTANGLE2D_H_
