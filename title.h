//=========================================
// 
// タイトルの処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _TITLE_H_	// このマクロ定義がされてなかったら
#define _TITLE_H_	// 2重インクルード防止のマクロ定義

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void SelectTitle(void);
void DrawTitle(void);
bool GetExit(void);

#endif // !_POLYGON_H_

