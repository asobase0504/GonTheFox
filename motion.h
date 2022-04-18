//**************************************************************************************************
//
// プレイヤー処理(motion.h)
// Auther：唐﨑結斗
//
//**************************************************************************************************
#ifndef _MOTION_H_			// このマクロ定義がされてなかったら
#define _MOTION_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include"main.h"

//***************************************************************************
// マクロ定義
//***************************************************************************
#define	MAX_MODEL_PARTS		(128)		// モデル数の最大数
#define MAX_KEY				(64)		// キーの最大数
#define MAX_KEYSET			(64)		// キー設定の最大数

//***************************************************************
// パーツファイル構造体を定義
//***************************************************************
typedef struct
{
	char	aName[MAX_MODEL_PARTS];		// 名前
}PARTSFILE;

//***************************************************************
// キー構造体を定義
//***************************************************************
typedef struct
{
	D3DXVECTOR3		pos;	// 現在位置
	D3DXVECTOR3		rot;	// 現在の向き
}Key;

//***************************************************************
// キー設定構造体を定義
//***************************************************************
typedef struct
{
	int		nFrame;				// フレーム数
	Key		key[MAX_KEY];		// キー情報
}KeySet;

//***************************************************************
// モーション設定構造体を定義
//***************************************************************
typedef struct
{
	int			nNumKey;				// キー数
	int			nCntKeySet;				// キーセットカウント
	int			nCntFrame;				// フレームカウント
	bool		bLoop;					// ループ使用状況
	KeySet		keySet[MAX_KEYSET];		// キー設定情報
}MOTION;

//***************************************************************
// モデルパーツ構造体を定義
//***************************************************************
typedef struct
{
	LPD3DXMESH		pMesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER	pBuffer;				// マテリアル情報へのポインタ
	DWORD			nNumMat;				// マテリアル情報の数
	D3DXMATRIX		mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3		posOrigin;				// 元の位置
	D3DXVECTOR3		rotOrigin;				// 元の向き
	D3DXVECTOR3		pos;					// 現在の位置
	D3DXVECTOR3		rot;					// 現在の向き
	D3DXVECTOR3		posDest;				// 目的の位置
	D3DXVECTOR3		rotDest;				// 目的の向き
	D3DXVECTOR3		vtxMin;					// 頂点座標の最小値
	D3DXVECTOR3		vtxMax;					// 頂点座標の最大値
	int				nIdxModelParent;		// 親モデルのインデックス数
	int				nType;					// パーツのタイプ
}PARTS;

//***************************************************************
// プロトタイプ宣言
//***************************************************************
// 入力関数
void SetParts(int nMaxParts,							// パーツ数
	PARTS *Parts,										// パーツ情報
	D3DXMATRIX mtxWorld,								// ワールドマトリックス
	D3DXMATRIX mtxRot,									// 計算用マトリックス
	D3DXMATRIX mtxTrans,								// 計算用マトリックス
	D3DMATERIAL9 *matDef,								// マテリアル保存変数
	D3DXMATERIAL *pMat);								// マテリアルデータ
bool PlayMotion(int nMaxParts,							// パーツ数
	PARTS *Parts,										// パーツ情報
	MOTION *motion);									// モーション情報
bool MotionBlend(int nCntMotionSet,						// モーションの配列番号
	PARTS *Parts,										// パーツ情報
	int nMaxParts,										// パーツ数
	MOTION *motion);									// モーション情報						
void LoodSetMotion(char *pFileName, PARTSFILE *PartsFile, PARTS *Parts, MOTION *Motion, int *nMaxParts);

#endif

