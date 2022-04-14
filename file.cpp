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
// プロトタイプ宣言
//=====================================
void	LoadKeySetFile(FILE*pFile, MODELDATAPLAYER* pSet);

//=====================================
// グローバル変数
//=====================================

static char				s_aString[256];//
static ModelData		s_ModelData;
static int	nSetModel = 0;//現在のセットしてる番号
int a = 0;
int b = 0;
static int nSetKey = 0;

//---------------------------------------------------------------------------
// ファイル読み込み処理
//---------------------------------------------------------------------------
void LoadSetFile(char *Filename)
{
	MODELDATAPLAYER* ModelData = GetModelData();
	char modelFile[256];
	int	 nIndex = 0;
	int	 nParent = 0;
	int  nType = 0;
	// ファイルポインタの宣言
	FILE* pFile;

	char aFile[128] = FILE_3D_SYSTEM;
	strcat(aFile, &Filename[0]);//合成　aFile＜-こいつに入れる

	//ファイルを開く
	pFile = fopen(aFile, "r");

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

			if (strcmp(&s_aString[0], "MODEL_FILENAME") == 0)
			{// 文字列が一致した場合
				fscanf(pFile, "%s", &modelFile);
			}
			if (strcmp(&s_aString[0], "MAINPOS") == 0)
			{// 文字列が一致した場合

				fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
				fscanf(pFile, "%f", &s_modelMainpos.x);
				fscanf(pFile, "%f", &s_modelMainpos.y);
				fscanf(pFile, "%f", &s_modelMainpos.z);
			}
			else if (strcmp(&s_aString[0], "MODELSET") == 0|| strcmp(&s_aString[0], "MOTIONSET") == 0)
			{// 文字列が一致した場合
				 D3DXVECTOR3	s_modelpos = D3DXVECTOR3(0.0f,0.0f,0.0f);
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
						fscanf(pFile, "%d", &nParent);

					}
					if (strcmp(&s_aString[0], "INDEX") == 0)
					{// 文字列が一致した場合//どのパーツと親子関係か-１は親なし
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nIndex);

					}
					if (strcmp(&s_aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &nType);
					}
					if (strcmp(&s_aString[0], "LOOP") == 0)
					{// 文字列が一致した場合//ループするかしないか1する０しない
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &ModelData->LOOP);

					}
					if (strcmp(&s_aString[0], "NUM_KEY") == 0)
					{// 文字列が一致した場合//キーの最大数
						fscanf(pFile, "%s", &s_aString[0]);//＝読み込むやつ
						fscanf(pFile, "%d", &ModelData->NUM_KEY);

					}
					if (strcmp(&s_aString[0], "KEYSET") == 0)
					{// 文字列が一致した場合//アニメーションのファイルだったとき
						LoadKeySetFile(pFile, ModelData);
					
					}
					if (strcmp(&s_aString[0], "END_MOTIONSET")==0)
					{//一回のmotion読み込み切ったら次のmotionのセットに行くためにカウント初期化してデータを加算する
						ModelData++;
						b++;
						nSetModel = 0;
					}	
					if (strcmp(&s_aString[0], "END_PLAYESET") == 0)
					{//プレイヤーの場合
						SetPlayer(s_modelpos, s_modelrot, modelFile, nParent, nIndex, s_modelMainpos);
						break;
					}
					if (strcmp(&s_aString[0], "END_SCRIPT") == 0)
					{// 文字列が一致した場合
						break;
					}
				}	
			}
		}
	}

	//ファイルを閉じる
	fclose(pFile);
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
				fscanf(pFile, "%d", &set->KeySet[nSetModel].KeyFrame);
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
				a++;
				nSetKey = 0;//パーツの数初期化
				break;
			}
		}

}
//---------------------------------------------------------------------------
// ファイル更新処理
//---------------------------------------------------------------------------
void OutputSetFile(void)
{
}