//==================================================
//
// プレイヤー設定
// Author:hamada ryuuga
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include <stdio.h>
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "utility.h"
#include "title.h"
#include "sound.h"
#include "motion.h"

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_ENEMY	(100)

//--------------------------------------------------
// 静的変数
//--------------------------------------------------
static Enemy s_Enemy[MAX_ENEMY];	// ポリゴンの構造体
static MODELDATAPLAYER s_ModelData[MAX_MOVE];

static int s_parts;	// パーツの最大数
static int s_pow;	// ジャンプパワー
static int nMotion; //

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Loadmotion(MODELDATAPLAYER* set, int Setnumber);	// モーションの読込
static void AnimationSet(int animation);						// アニメーションの計算
static void MoveSet(void);										// ムーブセット
static void Collision(void);									// 当たり判定まとめ
static void SetCopy(void);

//==================================================
// 初期化処理
//==================================================
void InitEnemy(void)
{
	// カメラのデータ取得
	Camera *pCamera;
	pCamera = GetCamera();

	s_pow = 0;

	// 初期化処理
	memset(&s_Enemy, NULL, sizeof(s_Enemy));

	// 初期化処理
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		pEnemy->rotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
		pEnemy->damege = DAMEGE_NORMAL;
		pEnemy->MotionType = ANIME_NORMAL;		//いま使ってるmotioの番号
		pEnemy->MotionTypeOld = pEnemy->MotionType;
		s_parts = 0;

		pEnemy->modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		pEnemy->modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		pEnemy->use = true;
	}

	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=========================================
// 終了処理
//=========================================
void UninitEnemy(void)
{
	StopSound();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		for (int i = 0; i <= s_parts; i++)
		{
			// 頂点バッファーの解放
			if (pEnemy->Parts[i].pBuffer != NULL)
			{
				pEnemy->Parts[i].pBuffer->Release();
				pEnemy->Parts[i].pBuffer = NULL;
			}
			if (pEnemy->Parts[i].pMesh != NULL)
			{
				pEnemy->Parts[i].pMesh->Release();
				pEnemy->Parts[i].pMesh = NULL;
			}
		}
	}
}

//=========================================
// 更新処理
//=========================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->use)
		{
			continue;
		}

		// 現在のモーション番号の保管
		pEnemy->MotionTypeOld = pEnemy->MotionType;

		if (!pEnemy->bMotion)
		{// 使用してるモーションがない場合
			pEnemy->MotionType = ANIME_NORMAL;
		}

		MoveSet();		// 動きセット

		Collision();	// 床

		// アニメーションや足音の設定
		if (!pEnemy->notLoop)
		{
			pEnemy->MotionType = ANIME_NORMAL;
		}
		if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_S) ||
			GetJoypadPress(JOYKEY_UP, 0) || GetJoypadPress(JOYKEY_DOWN, 0) || GetJoypadPress(JOYKEY_LEFT, 0) || GetJoypadPress(JOYKEY_RIGHT, 0))
		{
			pEnemy->MotionType = ANIME_RUN;	// 歩く

		}


		if (GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A, 0) || GetJoypadPress(JOYKEY_UP, 0))
		{//SPACEキーが押された
			pEnemy->pos.y = pEnemy->pos.y + 5.0f;
			s_pow++;
			pEnemy->MotionType = ANIME_JUMP;

		}
		else
		{
			s_pow = 0;
		}
		if (s_pow >= 1 && s_pow <= 10)
		{//ジャンプシステム
			s_pow++;
			pEnemy->move.y = 1.00f* s_pow;
		}


		pEnemy->move.y -= 1.0f;
		if (GetKeyboardPress(DIK_B))
		{
			pEnemy->MotionType = ANIME_ATTACK;//攻撃

			pEnemy->notLoop = true;
		}
		if (GetKeyboardPress(DIK_N))
		{
			pEnemy->notLoop = true;
		}

		////影更新
		////SetposShadow(pEnemy->nShadow, D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z));


		//AnimationSet((int)pEnemy->motion);//アニメーション再生

		if (pEnemy->MotionTypeOld != pEnemy->MotionType)
		{// モーションタイプが変更された時
			pEnemy->motion[pEnemy->MotionTypeOld].nCntFrame = 0;
			pEnemy->motion[pEnemy->MotionTypeOld].nCntKeySet = 0;
			pEnemy->bMotionBlend = true;
		}

		if (pEnemy->bMotionBlend)
		{// モーションブレンドを使用してる
			pEnemy->bMotionBlend = MotionBlend((int)(pEnemy->MotionType),				// モーションの配列番号
				&pEnemy->Parts[0],														// パーツ情報
				pEnemy->nMaxModelParts,												// パーツ数
				&pEnemy->motion[0]);													// モーション情報	
		}
		else if (!pEnemy->bMotionBlend)
		{// モーションブレンドを使用してない場合
			pEnemy->bMotion = PlayMotion(pEnemy->nMaxModelParts,						// パーツ数
				&pEnemy->Parts[0],														// パーツ情報
				&pEnemy->motion[(int)(pEnemy->MotionType)]);							// モーション情報
		}
	}
}

//=========================================
// 描画処理
//=========================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// 計算用マトリックス
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat= {};
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (pEnemy->use)//使ってるやつ出す
		{
			// ワールドマトリックスの初期化
			// 行列初期化関数(第1引数の行列を単位行列に初期化)
			D3DXMatrixIdentity(&pEnemy->mtxWorld);

			// 拡縮を反映
			// 行列拡縮関数
			D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxScale);

			// 向きを反映
			// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEnemy->rot.y, pEnemy->rot.x, pEnemy->rot.z);
			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxRot);

			// 位置を反映
			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
			D3DXMatrixTranslation(&mtxTrans, pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);
			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxTrans);

			// パーツの描画設定
			SetParts(pEnemy->nMaxModelParts,					// パーツ数
				&pEnemy->Parts[0],								// パーツ情報
				pEnemy->mtxWorld,								// ワールドマトリックス
				mtxRot,											// 計算用マトリックス
				mtxTrans,										// 計算用マトリックス
				&marDef,										// マテリアル保存変数
				pMat);											// マテリアルデータ

			//現在のマテリアルを元に戻す
			pDevice->SetMaterial(&marDef);
		}
	}
}

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (pEnemy->use)
		{
			continue;
		}

		// プレイヤー情報の初期化
		pEnemy->pos = pos;									// 位置の初期化
		pEnemy->posOld = pEnemy->pos;						// 過去位置の初期化
		pEnemy->posOld = pEnemy->pos;						// 過去位置の初期化
		pEnemy->rot = rot;									// 向きの初期化
		pEnemy->modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);		// 頂点座標の最小値
		pEnemy->modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// 頂点座標の最大値
		pEnemy->mtxWorld = {};								// ワールドマトリックス
		//pEnemy->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目的の向き
		pEnemy->MotionType = ANIME_NORMAL;					// ニュートラルモーション
		pEnemy->MotionTypeOld = pEnemy->MotionType;			// ニュートラルモーション
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		pEnemy->bMotionBlend = false;						// モーションブレンドの使用状況
		pEnemy->use = true;									// 使用状況
		pEnemy->bMotionBlend = false;						// プレイヤーがディスクを持っていない
		//g_nIdxShadow[nCntEnemy] = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//影の設定処理の初期化

		// ファイルの読み込み
		LoodSetMotion("Data/system/Fox.txt", &pEnemy->PartsFile[0], &pEnemy->Parts[0], &pEnemy->motion[0], &pEnemy->nMaxModelParts);

		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		for (int i = 0; i < pEnemy->nMaxModelParts; i++)
		{
			PARTS* pParts = &pEnemy->Parts[i];

			// 位置と向きの初期値を保存
			pParts->posOrigin = pParts->pos;
			pParts->rotOrigin = pParts->rot;

			// パーツ情報の初期化
			pParts->mtxWorld = {};																// ワールドマトリックス
			pParts->vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);		// 頂点座標の最小値
			pParts->vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// 頂点座標の最大値

			// Xファイルの読み込み
			D3DXLoadMeshFromX(&pEnemy->PartsFile[pParts->nType].aName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&pParts->pBuffer,
				NULL,
				&pParts->nNumMat,
				&pParts->pMesh);

			// 頂点座標の最小値・最大値の算出
			int			nNumVtx;	// 頂点数
			DWORD		sizeFVF;	// 頂点フォーマットのサイズ
			BYTE		*pVtxBuff;	// 頂点バッファへのポインタ

			// 頂点数の取得
			nNumVtx = pParts->pMesh->GetNumVertices();

			// 頂点フォーマットのサイズの取得
			sizeFVF = D3DXGetFVFVertexSize(pParts->pMesh->GetFVF());

			// 頂点バッファのロック
			pParts->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				// 頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				if (vtx.x < pParts->vtxMin.x)
				{// 比較対象が現在の頂点座標(X)の最小値より小さい
					pParts->vtxMin.x = vtx.x;
				}
				if (vtx.y < pParts->vtxMin.y)
				{// 比較対象が現在の頂点座標(Y)の最小値より小さい
					pParts->vtxMin.y = vtx.y;
				}
				if (vtx.z < pParts->vtxMin.z)
				{// 比較対象が現在の頂点座標(Z)の最小値より小さい
					pParts->vtxMin.z = vtx.z;
				}

				if (vtx.x > pParts->vtxMax.x)
				{// 比較対象が現在の頂点座標(X)の最大値より大きい
					pParts->vtxMax.x = vtx.x;
				}
				if (vtx.y > pParts->vtxMax.y)
				{// 比較対象が現在の頂点座標(Y)の最大値より大きい
					pParts->vtxMax.y = vtx.y;
				}
				if (vtx.z > pParts->vtxMax.z)
				{// 比較対象が現在の頂点座標(Z)の最大値より大きい
					pParts->vtxMax.z = vtx.z;
				}

				// 頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;
			}

			if (pParts->vtxMin.x < pEnemy->modelMin.x)
			{// 比較対象が現在の頂点座標(X)の最小値より小さい
				pEnemy->modelMin.x = pParts->vtxMin.x;
			}
			if (pParts->vtxMin.y < pEnemy->modelMin.y)
			{// 比較対象が現在の頂点座標(Y)の最小値より小さい
				pEnemy->modelMin.y = pParts->vtxMin.y;
			}
			if (pParts->vtxMin.z < pEnemy->modelMin.z)
			{// 比較対象が現在の頂点座標(Z)の最小値より小さい
				pEnemy->modelMin.z = pParts->vtxMin.z;
			}

			if (pParts->vtxMax.x > pEnemy->modelMax.x)
			{// 比較対象が現在の頂点座標(X)の最大値より大きい
				pEnemy->modelMax.x = pParts->vtxMax.x;
			}
			if (pParts->vtxMax.y > pEnemy->modelMax.y)
			{// 比較対象が現在の頂点座標(Y)の最大値より大きい
				pEnemy->modelMax.y = pParts->vtxMax.y;
			}
			if (pParts->vtxMax.z > pEnemy->modelMax.z)
			{// 比較対象が現在の頂点座標(Z)の最大値より大きい
				pEnemy->modelMax.z = pParts->vtxMax.z;
			}

			// 頂点バッファのアンロック
			pParts->pMesh->UnlockVertexBuffer();
		}
	}
}

//--------------------------
//当たり判定のサイズせってい
//--------------------------
void SizeSet(void)
{

}

//------------------------------
//アニメーションセット
//-------------------------------
void AnimationSet(int animation)
{

}

//------------------------------
//動きセット
//-------------------------------
void MoveSet(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->use)
		{
			continue;
		}

		Camera *pCamera = GetCamera();

		pEnemy->consumption = 0.0f;
		int nDash = 1;
		if (GetKeyboardPress(DIK_LSHIFT) || GetKeyboardPress(DIK_RSHIFT) || GetJoypadPress(JOYKEY_A, 0))
		{//シフトダッシュ
			nDash = 3;
		}
		//ゲームの時の移動方法
		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > 0.5f)
		{//右に半分より多く倒したら
			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -0.5f)
		{//左に半分より多く倒したら
			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > 0.5f)
		{//上に半分より多く倒したら
			pEnemy->move.x += sinf(pCamera->rot.y - D3DX_PI) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y - D3DX_PI) * Speed * nDash;

			pEnemy->consumption = (pEnemy->rotMove.x -
				D3DX_PI - pEnemy->rot.y
				+ pCamera->rot.y);
		}
		else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -0.5f)
		{//下に半分より多く倒したら
			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP, 0))
		{//W押したとき上加速

			pEnemy->move.x += sinf(pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - pEnemy->rot.y + pCamera->rot.y;
		}
		if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN, 0))
		{//S押したと下加速

			pEnemy->move.x += sinf(pCamera->rot.y - D3DX_PI) * Speed * nDash;
			pEnemy->move.z += cosf(pCamera->rot.y - D3DX_PI) * Speed * nDash;

			pEnemy->consumption = (pEnemy->rotMove.x -
				D3DX_PI - pEnemy->rot.y
				+ pCamera->rot.y);
		}
		if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT, 0))
		{//D押したとき右加速

			pEnemy->move.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;


			pEnemy->consumption = pEnemy->rotMove.x + (D3DX_PI*0.5f) - pEnemy->rot.y + pCamera->rot.y;
		}

		if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT, 0))
		{//A押したと左加速

			pEnemy->move.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;
			pEnemy->move.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * Speed * nDash;

			pEnemy->consumption = pEnemy->rotMove.x - (D3DX_PI*0.5f) - pEnemy->rot.y + pCamera->rot.y;

		}
		pEnemy->posOld = pEnemy->pos;	// 過去の移動量を保存

		if (pEnemy->invincible <= 0)
		{// 無敵時間がゼロになったらダメージくらうようにする
			pEnemy->damege = DAMEGE_NORMAL;
		}

		pEnemy->move.x += (0.0f - pEnemy->move.x) * 0.5f;	//（目的の値-現在の値）＊減衰係数
		pEnemy->move.z += (0.0f - pEnemy->move.z) * 0.5f;
		pEnemy->pos += pEnemy->move;// 移動を加算

		// 正規化
		if (pEnemy->consumption > D3DX_PI)
		{
			pEnemy->consumption += D3DX_PI * 2;
		}
		if (pEnemy->consumption < -D3DX_PI)
		{
			pEnemy->consumption += -D3DX_PI * 2;
		}

		//減算設定（感性）
		pEnemy->rot.y += (pEnemy->consumption)* Attenuation;	// 目的の値-現在の値）＊減衰係数

		//正規化
		if (pEnemy->rot.y > D3DX_PI)
		{
			pEnemy->rot.y += -D3DX_PI * 2;
		}
		if (pEnemy->rot.y <= -D3DX_PI)
		{
			pEnemy->rot.y += D3DX_PI * 2;
		}
	}
}
//-------------------------------
//当たり判定まとめ
//-------------------------------
void Collision(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->use)
		{
			continue;
		}

		if (pEnemy->pos.y <= 0.0f)
		{
			pEnemy->pos.y = 0.0f;
		}
	}
}

//-------------------------------
//モーションをロードする処理
//-------------------------------
void  Loadmotion(MODELDATAPLAYER* set, int Setnumber)
{
	s_ModelData[nMotion] = *set;
	nMotion++;
}

//-------------------------------
//コピーを処理
//-------------------------------
void SetCopy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->use)
		{
			continue;
		}

		nMotion = 0;
		if (s_parts >= 8)
		{
			s_parts = 7;
		}

		switch (pEnemy->copy)
		{
		case COPY_SWORD:
			break;
		case COPY_FIRE:
			break;
		case COPY_LASER:
			break;
		case COPY_CUTTER:
			break;
		default:
			break;
		}
	}
}

//----------------------
//ゲット(構造体)
//----------------------
Enemy *GetEnemy(void)
{
	return s_Enemy;
}
