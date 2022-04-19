//**************************************************************************************************
//
// モーション処理(motion.cpp)
// Auther：唐﨑結斗
//
//**************************************************************************************************

//***************************************************************************
// インクルード
//***************************************************************************
#include <stdio.h>
#include <assert.h>
#include "main.h"
#include "motion.h"
#include "utility.h"

//***************************************************************************
// マクロ定義
//***************************************************************************
#define MOTION_BLEND_FRAM			(12)				// モーションブレンドのフレーム数		

//---------------------------------------------------------------------------
// パーツ設置処理
//---------------------------------------------------------------------------
void SetParts(int nMaxParts,		// パーツ数
	PARTS *Parts,					// パーツ情報
	D3DXMATRIX mtxWorld,			// ワールドマトリックス
	D3DXMATRIX mtxRot,				// 計算用マトリックス
	D3DXMATRIX mtxTrans,			// 計算用マトリックス
	D3DMATERIAL9 *matDef,			// マテリアル保存変数
	D3DXMATERIAL *pMat)				// マテリアルデータ
{
	// デバイスの取得
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	for (int i = 0; i < nMaxParts; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&(Parts + i)->mtxWorld);				// 行列初期化関数

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			(Parts + i)->rot.y,
			(Parts + i)->rot.x,
			(Parts + i)->rot.z);								// 行列回転関数
		D3DXMatrixMultiply(&(Parts + i)->mtxWorld,
			&(Parts + i)->mtxWorld,
			&mtxRot);											// 行列掛け算関数 

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			(Parts + i)->pos.x,
			(Parts + i)->pos.y,
			(Parts + i)->pos.z);								// 行列移動関数
		D3DXMatrixMultiply(&(Parts + i)->mtxWorld,
			&(Parts + i)->mtxWorld,
			&mtxTrans);											// 行列掛け算関数

		// 親パーツのワールドマトリックスを保持
		D3DXMATRIX mtxParent;

		if ((Parts + i)->nIdxModelParent == -1)
		{// 親モデルのインデックス数が-1の時
			mtxParent = mtxWorld;
		}
		else
		{
			mtxParent = (Parts + (Parts + i)->nIdxModelParent)->mtxWorld;
		}

		// 自分の親マトリックスとの掛け算
		D3DXMatrixMultiply(&(Parts + i)->mtxWorld, &(Parts + i)->mtxWorld, &mtxParent);

		//// サイズの反映
		//D3DXMatrixScaling()

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &(Parts + i)->mtxWorld);

		// 現在のマテリアルを保持
		pDevice->GetMaterial(&*matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)(Parts + i)->pBuffer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)(Parts + i)->nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// プレイヤーパーツの描画
			(Parts + i)->pMesh->DrawSubset(nCntMat);
		}

		// 保していたマテリアルを戻す
		pDevice->SetMaterial(&*matDef);
	}
}

//---------------------------------------------------------------------------
// モーション処理
//---------------------------------------------------------------------------
bool PlayMotion(int nMaxParts,			// パーツ数
	PARTS *Parts,						// パーツ情報
	MOTION *motion)						// モーション情報
{
	// 変数宣言
	bool bMotion = true;

	for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
	{
		if (motion->nCntFrame == 0)
		{// フレームカウントが0の時
			// 目的の位置と向きの算出
			(Parts + nCntParts)->posDest = ((Parts + nCntParts)->posOrigin + motion->keySet[motion->nCntKeySet].key[nCntParts].pos) - (Parts + nCntParts)->pos;
			(Parts + nCntParts)->rotDest = ((Parts + nCntParts)->rotOrigin + motion->keySet[motion->nCntKeySet].key[nCntParts].rot) - (Parts + nCntParts)->rot;

			// 角度の正規化
			NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
			
		}

		// 変数宣言
		D3DXVECTOR3 addPos = D3DXVECTOR3((Parts + nCntParts)->posDest / (float)(motion->keySet[motion->nCntKeySet].nFrame));
		D3DXVECTOR3 addRot = D3DXVECTOR3((Parts + nCntParts)->rotDest / (float)(motion->keySet[motion->nCntKeySet].nFrame));

		// 位置の加算
		(Parts + nCntParts)->pos += addPos;

		//	向きの加算
		(Parts + nCntParts)->rot += addRot;

		// 角度の正規化
		NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
	}

	// フレームカウントの加算
	motion->nCntFrame++;
	
	if (motion->nCntFrame >= motion->keySet[motion->nCntKeySet].nFrame)
	{// フレームカウントが指定のフレーム数を超えた場合
		// フレーム数の初期化
		motion->nCntFrame = 0;

		// 再生中のキー番号数の加算
		motion->nCntKeySet++;

		if (motion->nCntKeySet >= motion->nNumKey && motion->bLoop)
		{// 再生中のキー数カウントがキー数の最大値を超えたとき、そのモーションがループを使用している
			// 再生中のキー数カウントを初期化
			motion->nCntKeySet = 0;

		}
		else if (motion->nCntKeySet >= motion->nNumKey)
		{
			motion->nCntKeySet = 0;
			bMotion = false;
		}
	}

	return bMotion;
}

//---------------------------------------------------------------------------
// モーションブレンド処理
//---------------------------------------------------------------------------
bool MotionBlend(int nCntMotionSet,						// モーションの配列番号
	PARTS *Parts,										// パーツ情報
	int nMaxParts,										// パーツ数
	MOTION *motion)										// モーション情報			
{
	// 変数宣言
	bool bMotionBlend = true;

	for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
	{
		if ((motion + nCntMotionSet)->nCntFrame == 0)
		{// フレームカウントが0の時
			// 目的の位置と向きの算出
			(Parts + nCntParts)->posDest = ((Parts + nCntParts)->posOrigin + (motion + nCntMotionSet)->keySet[(motion + nCntMotionSet)->nCntKeySet].key[nCntParts].pos) - (Parts + nCntParts)->pos;
			(Parts + nCntParts)->rotDest = ((Parts + nCntParts)->rotOrigin + (motion + nCntMotionSet)->keySet[(motion + nCntMotionSet)->nCntKeySet].key[nCntParts].rot) - (Parts + nCntParts)->rot;

			// 角度の正規化
			NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
			NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
		}

		// 変数宣言
		D3DXVECTOR3 addPos = D3DXVECTOR3((Parts + nCntParts)->posDest / (float)(MOTION_BLEND_FRAM));
		D3DXVECTOR3 addRot = D3DXVECTOR3((Parts + nCntParts)->rotDest / (float)(MOTION_BLEND_FRAM));

		// 位置の加算
		(Parts + nCntParts)->pos += addPos;

		//	向きの加算
		(Parts + nCntParts)->rot += addRot;

		// 角度の正規化
		NormalizeAngle(&(Parts + nCntParts)->rotDest.x);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.y);
		NormalizeAngle(&(Parts + nCntParts)->rotDest.z);
	}

	// フレームカウントの加算
	(motion + nCntMotionSet)->nCntFrame++;

	if ((motion + nCntMotionSet)->nCntFrame >= MOTION_BLEND_FRAM)
	{// フレームカウントが指定のフレーム数を超えた場合
		// フレーム数の初期化
		(motion + nCntMotionSet)->nCntFrame = 0;

		// 再生中のキー番号数の加算
		(motion + nCntMotionSet)->nCntKeySet++;

		bMotionBlend = false;
	}

	return bMotionBlend;
}

//---------------------------------------------------------------------------
// モーション読み込み処理
//---------------------------------------------------------------------------
void LoodSetMotion(char *pFileName, PARTSFILE *PartsFile, PARTS *Parts, MOTION *Motion,int *nMaxParts)
{
	// 変数宣言
	char aString[128] = {};			// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数
	int	nCntKeySet = 0;				// KeySetカウント
	int	nCntKey = 0;				// Keyカウント

	// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{// 文字列が一致した場合
			aString[0] = {};
			// 文字列の読み込み
			fscanf(pFile, "%s", &aString[0]);
		}
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strncmp(&aString[0], "#", 1) == 0)
			{// 一列読み込む
				fgets(&aString[0], sizeof(aString), pFile);
			}

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{// ファイル名の読み込み
				fscanf(pFile, "%s", &g_aEqual[0]);
				fscanf(pFile, "%s", &PartsFile->aName[0]);
				PartsFile++;
			}

			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{// キャラクターの読み込み
				while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "NUM_PARTS") == 0)
					{// 読み込むパーツ数
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &*nMaxParts);
					}

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{// パーツの読み込み
						while (strcmp(&aString[0], "END_PARTSSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// 一列読み込む
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "INDEX") == 0)
							{// タイプの読み込み
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &Parts->nType);
							}
							if (strcmp(&aString[0], "PARENT") == 0)
							{// 親の読み込み
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &Parts->nIdxModelParent);
							}
							if (strcmp(&aString[0], "POS") == 0)
							{// 位置の読み込み
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &Parts->pos.x);
								fscanf(pFile, "%f", &Parts->pos.y);
								fscanf(pFile, "%f", &Parts->pos.z);
							}
							if (strcmp(&aString[0], "ROT") == 0)
							{// 向きの読み込み
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &Parts->rot.x);
								fscanf(pFile, "%f", &Parts->rot.y);
								fscanf(pFile, "%f", &Parts->rot.z);
							}
						}

						// パーツカウントのインクリメント
						Parts++;
					}
				}
			}
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{// モーションの読み込み
				while (strcmp(&aString[0], "END_MOTIONSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "LOOP") == 0)
					{// ループ有無の読み込み
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", (int*)(&Motion->bLoop));
					}
					if (strcmp(&aString[0], "NUM_KEY") == 0)
					{// キー数の読み込み
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &Motion->nNumKey);
					}
					if (strcmp(&aString[0], "KEYSET") == 0)
					{// キーセットの読み込み
						while (strcmp(&aString[0], "END_KEYSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// 一列読み込む
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "FRAME") == 0)
							{// フレーム数の読み込み
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &Motion->keySet[nCntKeySet].nFrame);
							}
							if (strcmp(&aString[0], "KEY") == 0)
							{// キーの読み込み
								while (strcmp(&aString[0], "END_KEY") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									if (strncmp(&aString[0], "#", 1) == 0)
									{// 一列読み込む
										fgets(&aString[0], sizeof(aString), pFile);
									}

									if (strcmp(&aString[0], "POS") == 0)
									{// 位置の読み込み
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].pos.x);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].pos.y);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].pos.z);
									}
									if (strcmp(&aString[0], "ROT") == 0)
									{// 向きの読み込み
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].rot.x);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].rot.y);
										fscanf(pFile, "%f", &Motion->keySet[nCntKeySet].key[nCntKey].rot.z);
									}
								}

								// キーカウントのインクリメント
								nCntKey++;
							}
						}

						// キーカウントの初期化
						nCntKey = 0;

						// キーセットカウントのインクリメント
						nCntKeySet++;
					}
				}
				// キーセットカウントの初期化
				nCntKeySet = 0;

				// パーツ情報のインクリメント
				Motion++;
			}

		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けない場合
		assert(false);
	}
}