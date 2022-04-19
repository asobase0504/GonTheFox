//=========================================
// 
// モデルの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

#define MODEL_MAX				(1024)

//------------------------------------
// モデルの構造体定義
//------------------------------------
typedef struct
{
	// 基本情報
	int nType;						// モデルタイプ
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 pos_old;			// 前回位置
	D3DXVECTOR3 pos_world;			// ワールド位置
	D3DXVECTOR3 rot;				// 角度

	// 当たり判定情報
	D3DXVECTOR3 pos_Collision;		// 当たり判定の基準値
	D3DXVECTOR3 size;				// 直方体の当たり判定の大きさ
	float fLength;					// 球の当たり判定の大きさ

	// モデル情報
	LPD3DXMESH pMesh;				// メッシュ情報へのポインタ		// 頂点の集まりのこと
	LPD3DXBUFFER pBuffMat;			// マテリアル情報へのポインタ	// 1つのXファイルに複数のマテリアルが入っている
	LPDIRECT3DTEXTURE9 *pTexture;	// テクスチャへのポインタ
	DWORD nNumMat;					// マテリアル情報の数
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	float scale;					// モデルスケール

	// クオータニオン情報
	D3DXQUATERNION quaternion;		// クオータニオン
	bool isQuaternion;				// クオータニオンを使用するかどうか。

	D3DXVECTOR3 movevec;			// ベクトル
	float moverot;					// 移動時の回転量
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 MinVtx;				// 頂点の最小値
	D3DXVECTOR3 MaxVtx;				// 頂点の最大値
	int nIdxModelParent;			// 親モデルのインデックス
	bool bDraw;						// 描画状況
	bool bUse;						// 使用状況
}Model;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitModel(void);		// モデルの初期化処理
void UninitModel(void);		// モデルの終了処理
void UpdateModel(void);		// モデルの更新処理
void DrawModel(void);		// モデルの描画処理
void CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3* pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max);		// モデルの衝突処理
Model* GetModel(void);		// モデルの取得処理
void LoadModel(void);		// モデルの読み込み処理
void LoadMap(void);			// モデルマップの読み込み処理
Model* SetModel(int nType);	// モデルの設定(読み込みされたモデル参照)
Model* SetJustModel(void);
void ModelSize(D3DXVECTOR3* minOut, D3DXVECTOR3* maxOut, const LPD3DXMESH mesh);	// モデルのサイズを算出

#endif // !_MODEL_H_
