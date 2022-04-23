//==================================================
// 
// 吸い込み処理
// Author  :Hamada Ryuuga
// 
//==================================================


#ifndef _LNHALE_H_		// このマクロ定義がされなかったら
#define _LNHALE_H_		// 2重インクルード防止のマクロ定義

#include "main.h"

class CLnhale			//Lnhaleクラス
{
public:
	CLnhale();				//コンストラクタ（インスタンス生成時に通ります）
	~CLnhale();				//デストラクタ（インスタンス破棄時に通ります）
	void Init();			//初期化
	void Uninit();			//破棄
	void Update();			//入力
	void Draw();			//出力
	void Set(D3DXVECTOR3 setpos);
	bool Hit(D3DXVECTOR3 nowpos, D3DXVECTOR3 hitsize);
private:
	D3DXVECTOR3 pos;					//座標
	D3DXVECTOR3 size;					//サイズ
	LPDIRECT3DTEXTURE9		pTexture;	// テクスチャ
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// 頂点バッファ
	bool					bUse;		// 使用しているかどうか
};


#endif // !_FAN_H_