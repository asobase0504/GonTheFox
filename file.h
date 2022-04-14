//============================
//
// ファイル設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_			// このマクロ定義がされてなかったら
#define _FILE_H_			// 二重インクルード防止のマクロ定義

//=====================================
// インクルード
//=====================================
#include "main.h"

//=====================================
//　ファイルパスまとめ
//=====================================
#define FILE_3D_PLAYER  "Data/Model/"
#define FILE_3D_MODEL  "Data/Model/"
#define FILE_3D_SYSTEM "Data/system/"
#define LOOD_FILE_NAME_000	"Prayer.txt"
#define LOOD_FILE_NAME_EZ	"ez/model.txt"
#define LOOD_FILE_NAME_NO	"no/model.txt"
#define LOOD_FILE_NAME_HD	"hd/model.txt"
#define LOOD_FILE_NAME_002 "motion.txt"
#define LOOD_FILE_EZ_ENEMY "ez/enemy.txt"	
#define LOOD_FILE_NO_ENEMY "no/enemy.txt"
#define LOOD_FILE_HD_ENEMY "hd/enemy.txt"
#define LOOD_FILE_NOME_UI "ui.txt"
#define LOOD_FILE_NOME_AITEMEZ "ez/item.txt"
#define LOOD_FILE_NOME_AITEMNO "no/item.txt"
#define LOOD_FILE_NOME_AITEMHD "hd/item.txt"
#define MAX_MODELPARTS (9)
#define MAX_KEY2  (5)

//キーの構造体//
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3  rot;
}kEY;

//キーセットの構造体//
typedef struct
{
	int KeyFrame;
	kEY key[MAX_MODELPARTS];
}kEYSET;

//modelデータの構造体//
typedef struct
{
	int  LOOP;		// ループするかどうか[0:ループしない / 1 : ループする]
	int NUM_KEY;  	// キー数
	kEYSET KeySet[MAX_KEY2];
}ModelData;

//=====================================
// プロトタイプ宣言
//=====================================
// 制御関数
void	LoadSetFile(char *Filename);
void	OutputSetFile(void);

#endif

