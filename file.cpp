//============================
//
// ファイル読み込み処理
// Author:hamada ryuuga
//
//============================

//=====================================
// インクルード
//=====================================
#include <stdio.h>
#include "main.h"
#include "file.h"
#include "file.h"
#include "player.h"

//=====================================
// マクロ
//=====================================
#define MAXMODEL (12)
#define MAX_MODELPARTS (9)
#define MAX_KEY2  (5)
//=====================================
// プロトタイプ宣言
//=====================================
void	LoadKeySetFile(FILE*pFile, MODELDATAPLAYER* pSet);
void	motionSet(FILE* pFile);
void	InitFile(void);
//=====================================
// グローバル変数
//=====================================

static char				s_aString[256];//
static MODELDATAPLAYER	s_ModelData[MAX_MOVE];
static int	nSetModel = 0;//現在のセットしてる番号
static int a = 0;
static int b = 0;
static int nSetKey = 0;
static int nMoveSet = 0;

void InitFile(void)
{
	 nSetModel = 0;//現在のセットしてる番号
	 a = 0;
	 b = 0;
	 nSetKey = 0;
	 nMoveSet = 0;
}
//---------------------------------------------------------------------------
// ファイル読み込み処理
//---------------------------------------------------------------------------
void SetFile(char *Filename)
{

	char modelFile[256];

	// ファイルポインタの宣言
	FILE* pFile;
								//ファイルを開く
	pFile = fopen(Filename, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み

			fscanf(pFile, "%s", &s_aString[0]);
			if (strcmp(&s_aString[0], "MOTION_FILENAME") == 0)
			{// 文字列が一致した場合
				fscanf(pFile, "%s", &modelFile);
				fscanf(pFile, "%s", &modelFile);
				LoadSetFile(modelFile);
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}

//---------------------------------------------------------------------------
// ファイル読み込み処理
//---------------------------------------------------------------------------
void LoadSetFile(char *Filename)
{

	int	 nIndex = 0;
	int	 nParent = 0;
	int  nType = 0;
	int	 nModel = 0;
	char modelFilename[MAXMODEL][256];
	// ファイルポインタの宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(Filename, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み

			fscanf(pFile, "%s", &s_aString[0]);
			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//これのあとコメント
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}
			if (strcmp(&s_aString[0], "NUM_MODEL") == 0)
			{

				fscanf(pFile, "%s", &s_aString[0]);
	
			}
			if (strcmp(&s_aString[0], "MODEL_FILENAME") == 0)
			{// 文字列が一致した場合

				fscanf(pFile, "%s", &s_aString[0]);
				fscanf(pFile, "%s", &modelFilename[nModel][0]);

				nModel++;
			}
			if (strcmp(&s_aString[0], "MAINPOS") == 0)
			{// 文字列が一致した場合

				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%f", &s_modelMainpos.x);
				fscanf(pFile, "%f", &s_modelMainpos.y);
				fscanf(pFile, "%f", &s_modelMainpos.z);
			}
			if (strcmp(&s_aString[0], "MOTIONSET") == 0)
			{
				motionSet(pFile);
			}
			if (strcmp(&s_aString[0], "PARTSSET") == 0 )
			{// 文字列が一致した場合
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3	s_modelrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				while (1)
				{
					// 文字列の初期化と読み込み
					s_aString[0] = {};
					fscanf(pFile, "%s", &s_aString[0]);

					if (strncmp(&s_aString[0], "#", 1) == 0)
					{//これのあとコメント
						fgets(&s_aString[0], sizeof(s_aString), pFile);
						continue;
					}

					if (strcmp(&s_aString[0], "POS") == 0)
					{// 文字列が一致した場合

						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&s_aString[0], "ROT") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_modelrot.x);
						fscanf(pFile, "%f", &s_modelrot.y);
						fscanf(pFile, "%f", &s_modelrot.z);
					}
					if (strcmp(&s_aString[0], "PARENT") == 0)
					{// 文字列が一致した場合//どのパーツと親子関係か-１は親なし
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nParent);

					}
					if (strcmp(&s_aString[0], "INDEX") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nIndex);

					}
					if (strcmp(&s_aString[0], "END_PARTSSET") == 0)
					{//プレイヤーの場合
						//SetPlayer(s_modelpos, s_modelrot, modelFilename[nIndex], nParent, nIndex);
						break;
					}
				}			
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}
void motionSet(FILE* pFile)
{
	while (1)
	{
		// 文字列の初期化と読み込み
		s_aString[0] = {};
		fscanf(pFile, "%s", &s_aString[0]);
		if (strncmp(&s_aString[0], "#", 1) == 0)
		{//これのあとコメント
			fgets(&s_aString[0], sizeof(s_aString), pFile);
		}
		if (strcmp(&s_aString[0], "LOOP") == 0)
		{// 文字列が一致した場合//ループするかしないか1する０しない
			fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
			fscanf(pFile, "%d", &s_ModelData[b].loop);

		}
		if (strcmp(&s_aString[0], "NUM_KEY") == 0)
		{// 文字列が一致した場合//キーの最大数
			fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
			fscanf(pFile, "%d", &s_ModelData[b].num_key);

		}
		if (strcmp(&s_aString[0], "KEYSET") == 0)
		{// 文字列が一致した場合//アニメーションのファイルだったとき
			LoadKeySetFile(pFile, &s_ModelData[b]);

		}
		if (strcmp(&s_aString[0], "END_MOTIONSET") == 0)
		{//一回のmotion読み込み切ったら次のmotionのセットに行くためにカウント初期化してデータを加算する
			Loadmotion(&s_ModelData[b], nMoveSet);
			b++;
			nSetModel = 0;
			break;
		}
	}

}
void LoadKeySetFile(FILE * pFile, MODELDATAPLAYER* set)
{
		while (1)
		{	
			// 文字列の初期化と読み込み
			s_aString[0] = {};

			fscanf(pFile, "%s", &s_aString[0]);
			if (strncmp(&s_aString[0], "#", 1) == 0)
			{//コメント対策
				fgets(&s_aString[0], sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(&s_aString[0], "FRAME") == 0)
			{// 文字列が一致した場合
				fscanf(pFile, "%s", &s_aString[0]);//イコール読み込む
				fscanf(pFile, "%d", &set->KeySet[nSetModel].keyFrame);
			}	
			if (strcmp(&s_aString[0], "KEY") == 0)
			{
				while (1)
				{// 文字列の初期化と読み込み
					s_aString[0] = {};
					fscanf(pFile, "%s", &s_aString[0]);
					if (strncmp(&s_aString[0], "#", 1) == 0)
					{
						fgets(&s_aString[0], sizeof(s_aString), pFile);//コメント読み込む
						continue;
					}
					if (strcmp(&s_aString[0], "POS") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &s_aString[0]);//イコール読み込む
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].pos.x);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].pos.y);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].pos.z);
					}
					if (strcmp(&s_aString[0], "ROT") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &s_aString[0]);//イコール読み込む
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].rot.x);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].rot.y);
						fscanf(pFile, "%f", &set->KeySet[nSetModel].key[nSetKey].rot.z);
					}
					if (strcmp(&s_aString[0], "END_KEY") == 0)
					{
						nSetKey++;//パーツの数がどんどん増える
						break;
					}
				}
			}
			if (strcmp(&s_aString[0], "END_KEYSET") == 0)
			{// 文字列が一致した場合
				nSetModel++;//現在のセットしてる番号の更新
				nSetKey = 0;//パーツの数初期化
				break;
			}
		}

}



void LoadCopy(char *Filename)
{
	InitFile();
	char aString[256];
	int	 nIndex = 0;
	int	 nParent = 0;
	int  nType = 0;
	int	 nModel = 0;
	char modelFilename[256];

	// ファイルポインタの宣言
	FILE* pFile;

	//ファイルを開く
	pFile = fopen(Filename, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			aString[0] = {};
			fscanf(pFile, "%s", &aString[0]);
		}
		D3DXVECTOR3	s_modelMainpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み// 文字列の初期化と読み込み

			fscanf(pFile, "%s", &aString[0]);
			if (strncmp(&aString[0], "#", 1) == 0)
			{//これのあとコメント
				fgets(&aString[0], sizeof(aString), pFile);
				continue;
			}
			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{

				fscanf(pFile, "%s", &aString[0]);

			}
			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{// 文字列が一致した場合

				fscanf(pFile, "%s", &aString[0]);
				fscanf(pFile, "%s", &modelFilename[0]);
			}
			if (strcmp(&aString[0], "MAINPOS") == 0)
			{// 文字列が一致した場合

				fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
				fscanf(pFile, "%f", &s_modelMainpos.x);
				fscanf(pFile, "%f", &s_modelMainpos.y);
				fscanf(pFile, "%f", &s_modelMainpos.z);
			}
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{
				motionSet(pFile);
			}
			if (strcmp(&aString[0], "PARTSSET") == 0)
			{// 文字列が一致した場合
				D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3	s_modelrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				while (1)
				{
					// 文字列の初期化と読み込み
					aString[0] = {};
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{//これのあとコメント
						fgets(&aString[0], sizeof(aString), pFile);
						continue;
					}

					if (strcmp(&aString[0], "POS") == 0)
					{// 文字列が一致した場合

						fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_modelpos.x);
						fscanf(pFile, "%f", &s_modelpos.y);
						fscanf(pFile, "%f", &s_modelpos.z);
					}
					if (strcmp(&aString[0], "ROT") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
						fscanf(pFile, "%f", &s_modelrot.x);
						fscanf(pFile, "%f", &s_modelrot.y);
						fscanf(pFile, "%f", &s_modelrot.z);
					}
					if (strcmp(&aString[0], "PARENT") == 0)
					{// 文字列が一致した場合//どのパーツと親子関係か-１は親なし
						fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nParent);

					}
					if (strcmp(&aString[0], "INDEX") == 0)
					{// 文字列が一致した場合
						fscanf(pFile, "%s", &aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nIndex);

					}
					if (strcmp(&aString[0], "END_PARTSSET") == 0)
					{//プレイヤーの場合
						//SetPlayer(s_modelpos, s_modelrot, modelFilename, nParent, nIndex);
						break;
					}
				}
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}