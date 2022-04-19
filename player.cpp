//============================
//
// プレイヤー設定
// Author:hamada ryuuga
//
//============================

#include <stdio.h>
#include "Player.h"
#include "input.h"
#include "camera.h"
#include "utility.h"
#include "title.h"
#include "sound.h"
#include "motion.h"

//------------------------------------
// static変数
//------------------------------------
static PLAYER s_Player;	// ポリゴンの構造体
static MODELDATAPLAYER s_ModelData[MAX_MOVE];

static int s_time, s_parts;//タイマーとパーツの最大数
static int s_pow;//ジャンプパワー
static int nMotion;//

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void SetCopy(void);		// コピーの設定
static void Collision(void);	// 当たり判定まとめ
static void SizSet(void);//当たり判定取得
static void AnimationSet(int animation);//アニメーションの計算
static void MoveSet(void);	//ムーブセット
static MODELDATAPLAYER *GetModelData(void);//motionデータのゲット

//=========================================
// 初期化処理
//=========================================
void InitPlayer(void)
{
	//カメラのデータ取得
	Camera *pCamera;
	pCamera = GetCamera();

	s_pow = 0;

	// 初期化処理
	memset(&s_Player, NULL, sizeof(s_Player));

	// 初期化処理
	//縦　　　　　　　　　　//横
	s_Player.rotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
	s_Player.damege = DAMEGE_NORMAL;
	s_Player.MotionType = ANIME_NORMAL;				//いま使ってるmotioの番号
	s_Player.MotionTypeOld = s_Player.MotionType;
	s_parts = 0;

	s_Player.modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_Player.modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	s_Player.isUse = true;

	s_time = 0;

	SetPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=========================================
// 終了処理
//=========================================
void UninitPlayer(void)
{
	StopSound();

	for (int i = 0; i <= MAX_MODELPARTS; i++)
	{
		// 頂点バッファーの解放
		if (s_Player.Parts[i].pBuffer != NULL)
		{
			s_Player.Parts[i].pBuffer->Release();
			s_Player.Parts[i].pBuffer = NULL;
		}
		if (s_Player.Parts[i].pMesh != NULL)
		{
			s_Player.Parts[i].pMesh->Release();
			s_Player.Parts[i].pMesh = NULL;
		}
	}
}

//=========================================
// 更新処理
//=========================================
void UpdatePlayer(void)
{
	// 現在のモーション番号の保管
	s_Player.MotionTypeOld = s_Player.MotionType;

	if (!s_Player.bMotion)
	{// 使用してるモーションがない場合
		s_Player.MotionType = ANIME_NORMAL;
	}

	MoveSet();	//動きセット

	Collision();//床

	//			//アニメーションや足音の設定
	if (s_Player.notLoop == false)
	{
		s_Player.MotionType = ANIME_NORMAL;
	}
	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_S) ||
		GetJoypadPress(JOYKEY_UP, 0) || GetJoypadPress(JOYKEY_DOWN, 0) || GetJoypadPress(JOYKEY_LEFT, 0) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{
		s_Player.MotionType = ANIME_RUN;//歩く

	}


	if (GetKeyboardPress(DIK_SPACE) || GetJoypadPress(JOYKEY_A, 0) || GetJoypadPress(JOYKEY_UP, 0))
	{//SPACEキーが押された
		s_Player.pos.y = s_Player.pos.y + 5.0f;
		s_pow++;
		s_Player.MotionType = ANIME_JUMP;

	}
	else
	{
		s_pow = 0;
	}
	if (s_pow >= 1 && s_pow <= 10)
	{//ジャンプシステム
		s_pow++;
		s_Player.move.y = 1.00f* s_pow;
	}


	s_Player.move.y -= 1.0f;
	if (GetKeyboardPress(DIK_B))
	{
		s_Player.MotionType = ANIME_ATTACK;//攻撃

		s_Player.notLoop = true;
	}
	if (GetKeyboardPress(DIK_N))
	{
		s_Player.notLoop = true;
	}
	if (GetKeyboardPress(DIK_J))
	{

		s_Player.copy = COPY_SWORD;
		// ファイルの読み込み
		SetCopy("Data/system/sword.txt", &s_Player.PartsFile[7], &s_Player.Parts[7], &s_Player.motion[0], &s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_H))
	{
		s_Player.copy = COPY_FIRE;
		// ファイルの読み込み
		SetCopy("Data/system/flare.txt", &s_Player.PartsFile[7], 
					&s_Player.Parts[7], &s_Player.motion[0],
					&s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_F))
	{
		s_Player.copy = COPY_LASER;
		// ファイルの読み込み	
		SetCopy("Data/system/Laser.txt", &s_Player.PartsFile[7], &s_Player.Parts[7], &s_Player.motion[0], &s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_G))
	{
		s_Player.copy = COPY_CUTTER;
		// ファイルの読み込み
		SetCopy("Data/system/Cutter.txt", &s_Player.PartsFile[7], &s_Player.Parts[7], &s_Player.motion[0], &s_Player.nMaxModelParts);
	}
	if (GetKeyboardPress(DIK_K))
	{
		s_Player.copy = COPY_NORMAL;
		s_Player.nMaxModelParts = 7;
	}
	////影更新
	////SetposShadow(s_Player.nShadow, D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y, s_Player.pos.z));


	//AnimationSet((int)s_Player.motion);//アニメーション再生

	if (s_Player.MotionTypeOld != s_Player.MotionType)
	{// モーションタイプが変更された時
		s_Player.motion[s_Player.MotionTypeOld].nCntFrame = 0;
		s_Player.motion[s_Player.MotionTypeOld].nCntKeySet = 0;
		s_Player.bMotionBlend = true;
	}

	if (s_Player.bMotionBlend)
	{// モーションブレンドを使用してる
		s_Player.bMotionBlend = MotionBlend((int)(s_Player.MotionType),				// モーションの配列番号
			&s_Player.Parts[0],														// パーツ情報
			s_Player.nMaxModelParts,												// パーツ数
			&s_Player.motion[0]);													// モーション情報	
	}
	else if (!s_Player.bMotionBlend)
	{// モーションブレンドを使用してない場合
		s_Player.bMotion = PlayMotion(s_Player.nMaxModelParts,						// パーツ数
			&s_Player.Parts[0],														// パーツ情報
			&s_Player.motion[(int)(s_Player.MotionType)]);							// モーション情報
	}
}

//=========================================
// 描画処理
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// 計算用マトリックス
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat= {};
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	if (s_Player.isUse)//使ってるやつ出す
	{
		// ワールドマトリックスの初期化
		// 行列初期化関数(第1引数の行列を単位行列に初期化)
		D3DXMatrixIdentity(&s_Player.mtxWorld);

		// 拡縮を反映
		// 行列拡縮関数
		D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&s_Player.mtxWorld, &s_Player.mtxWorld, &mtxScale);

		// 向きを反映
		// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Player.rot.y, s_Player.rot.x, s_Player.rot.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&s_Player.mtxWorld, &s_Player.mtxWorld, &mtxRot);

		// 位置を反映
		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixTranslation(&mtxTrans, s_Player.pos.x, s_Player.pos.y, s_Player.pos.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&s_Player.mtxWorld, &s_Player.mtxWorld, &mtxTrans);

		// パーツの描画設定
		SetParts(s_Player.nMaxModelParts,					// パーツ数
			&s_Player.Parts[0],								// パーツ情報
			s_Player.mtxWorld,								// ワールドマトリックス
			mtxRot,											// 計算用マトリックス
			mtxTrans,										// 計算用マトリックス
			&marDef,										// マテリアル保存変数
			pMat);											// マテリアルデータ
	

		//	//// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		pDevice->SetMaterial(&marDef);
	}
}



void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// プレイヤー情報の初期化
	s_Player.pos = pos;																		// 位置の初期化
	s_Player.posOld = s_Player.pos;															// 過去位置の初期化
	s_Player.posOld = s_Player.pos;															// 過去位置の初期化
	s_Player.rot = rot;																		// 向きの初期化
	s_Player.modelMin = D3DXVECTOR3(100, 100, 100);											// 頂点座標の最小値
	s_Player.modelMax = D3DXVECTOR3(-100, -100, -100);										// 頂点座標の最大値
	s_Player.mtxWorld = {};																	// ワールドマトリックス
	//s_Player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);										// 目的の向き
	s_Player.MotionType = ANIME_NORMAL;														// ニュートラルモーション
	s_Player.MotionTypeOld = s_Player.MotionType;											// ニュートラルモーション
	s_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);											// 移動量
	s_Player.bMotionBlend = false;															// モーションブレンドの使用状況
	s_Player.isUse = true;																	// 使用状況
	//g_nIdxShadow[nCntPlayer] = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//影の設定処理の初期化


	// ファイルの読み込み
	LoodSetMotion("Data/system/Fox.txt", s_Player.PartsFile, s_Player.Parts, s_Player.motion, &s_Player.nMaxModelParts);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < s_Player.nMaxModelParts; i++)
	{
		// 位置と向きの初期値を保存
		s_Player.Parts[i].posOrigin = s_Player.Parts[i].pos;
		s_Player.Parts[i].rotOrigin = s_Player.Parts[i].rot;

		// パーツ情報の初期化
		s_Player.Parts[i].mtxWorld = {};																// ワールドマトリックス
		s_Player.Parts[i].vtxMin = D3DXVECTOR3(1000, 1000, 1000);		// 頂点座標の最小値
		s_Player.Parts[i].vtxMax = D3DXVECTOR3(-1000, -1000, -1000);	// 頂点座標の最大値

		// Xファイルの読み込み
		D3DXLoadMeshFromX(&s_Player.PartsFile[s_Player.Parts[i].nType].aName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&s_Player.Parts[i].pBuffer,
			NULL,
			&s_Player.Parts[i].nNumMat,
			&s_Player.Parts[i].pMesh);

		// 頂点座標の最小値・最大値の算出
		int			nNumVtx;	// 頂点数
		DWORD		sizeFVF;	// 頂点フォーマットのサイズ
		BYTE		*pVtxBuff;	// 頂点バッファへのポインタ

								// 頂点数の取得
		nNumVtx = s_Player.Parts[i].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF = D3DXGetFVFVertexSize(s_Player.Parts[i].pMesh->GetFVF());

		// 頂点バッファのロック
		s_Player.Parts[i].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x < s_Player.Parts[i].vtxMin.x)
			{// 比較対象が現在の頂点座標(X)の最小値より小さい
				s_Player.Parts[i].vtxMin.x = vtx.x;
			}
			if (vtx.y < s_Player.Parts[i].vtxMin.y)
			{// 比較対象が現在の頂点座標(Y)の最小値より小さい
				s_Player.Parts[i].vtxMin.y = vtx.y;
			}
			if (vtx.z < s_Player.Parts[i].vtxMin.z)
			{// 比較対象が現在の頂点座標(Z)の最小値より小さい
				s_Player.Parts[i].vtxMin.z = vtx.z;
			}

			if (vtx.x > s_Player.Parts[i].vtxMax.x)
			{// 比較対象が現在の頂点座標(X)の最大値より大きい
				s_Player.Parts[i].vtxMax.x = vtx.x;
			}
			if (vtx.y > s_Player.Parts[i].vtxMax.y)
			{// 比較対象が現在の頂点座標(Y)の最大値より大きい
				s_Player.Parts[i].vtxMax.y = vtx.y;
			}
			if (vtx.z > s_Player.Parts[i].vtxMax.z)
			{// 比較対象が現在の頂点座標(Z)の最大値より大きい
				s_Player.Parts[i].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		if (s_Player.Parts[i].vtxMin.x < s_Player.modelMin.x)
		{// 比較対象が現在の頂点座標(X)の最小値より小さい
			s_Player.modelMin.x = s_Player.Parts[i].vtxMin.x;
		}
		if (s_Player.Parts[i].vtxMin.y < s_Player.modelMin.y)
		{// 比較対象が現在の頂点座標(Y)の最小値より小さい
			s_Player.modelMin.y = s_Player.Parts[i].vtxMin.y;
		}
		if (s_Player.Parts[i].vtxMin.z < s_Player.modelMin.z)
		{// 比較対象が現在の頂点座標(Z)の最小値より小さい
			s_Player.modelMin.z = s_Player.Parts[i].vtxMin.z;
		}

		if (s_Player.Parts[i].vtxMax.x > s_Player.modelMax.x)
		{// 比較対象が現在の頂点座標(X)の最大値より大きい
			s_Player.modelMax.x = s_Player.Parts[i].vtxMax.x;
		}
		if (s_Player.Parts[i].vtxMax.y > s_Player.modelMax.y)
		{// 比較対象が現在の頂点座標(Y)の最大値より大きい
			s_Player.modelMax.y = s_Player.Parts[i].vtxMax.y;
		}
		if (s_Player.Parts[i].vtxMax.z > s_Player.modelMax.z)
		{// 比較対象が現在の頂点座標(Z)の最大値より大きい
			s_Player.modelMax.z = s_Player.Parts[i].vtxMax.z;
		}

		// 頂点バッファのアンロック
		s_Player.Parts[i].pMesh->UnlockVertexBuffer();
	}
}

//--------------------------
//当たり判定のサイズせってい
//--------------------------

void MoveSet(void)
{
	Camera *pCamera;
	pCamera = GetCamera();

	s_Player.consumption = 0.0f;
	int nDash = 1;
	if (GetKeyboardPress(DIK_LSHIFT) || GetKeyboardPress(DIK_RSHIFT) || GetJoypadPress(JOYKEY_A, 0))
	{//シフトダッシュ
		//SetParticle(s_Player.pos, s_Player.move, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 20.0f, PARTICLETYPE_SMOKE, 20);
		nDash = 3;
	}
	//ゲームの時の移動方法

	if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x > 0.5f)
	{//右に半分より多く倒したら
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -0.5f)
	{//左に半分より多く倒したら
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > 0.5f)
	{//上に半分より多く倒したら
		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.rotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -0.5f)
	{//下に半分より多く倒したら
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP, 0))
	{//W押したとき上加速

		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN, 0))
	{//S押したと下加速

		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.rotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{//D押したとき右加速

		s_Player.move.x += sinf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;


		s_Player.consumption = s_Player.rotMove.x + (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;
	}

	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT, 0))
	{//A押したと左加速

		s_Player.move.x += sinf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.rotMove.x - (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;

	}
	s_Player.posOld = s_Player.pos;//過去の移動量を保存

	

	if (s_Player.invincible <= 0)
	{//無敵時間がゼロになったらダメージくらうようにする
		s_Player.damege = DAMEGE_NORMAL;
	}

	s_Player.move.x += (0.0f - s_Player.move.x)*0.5f;//（目的の値-現在の値）＊減衰係数
	s_Player.move.z += (0.0f - s_Player.move.z)*0.5f;
	s_Player.pos += s_Player.move;//移動を加算

	//正規化
	if (s_Player.consumption > D3DX_PI)
	{
		s_Player.consumption += D3DX_PI * 2;
	}
	if (s_Player.consumption < -D3DX_PI)
	{
		s_Player.consumption += -D3DX_PI * 2;
	}

	//減算設定（感性）
	s_Player.rot.y += (s_Player.consumption)* Attenuation;//目的の値-現在の値）＊減衰係数

	//正規化
	if (s_Player.rot.y > D3DX_PI)
	{
		s_Player.rot.y += -D3DX_PI * 2;
	}
	if (s_Player.rot.y <= -D3DX_PI)
	{
		s_Player.rot.y += D3DX_PI * 2;
	}


}
//-------------------------------
//当たり判定まとめ
//-------------------------------
void Collision(void)
{
	if (s_Player.pos.y <= 0.0f)
	{
		s_Player.pos.y = 0.0f;
	}
}

//-------------------------------
//モーションをロードする処理
//-------------------------------
void SetCopy(char *pFileName, PARTSFILE *PartsFile, PARTS *Parts, MOTION *Motion, int *nMaxParts)
{
	// 変数宣言
	char aString[128] = {};			// 文字列比較用の変数
	char g_aEqual[128] = {};		// ＝読み込み用変数
	int	nCntKeySet = 0;				// KeySetカウント
	int	nCntKey = 0;				// Keyカウント

									// ファイルポインタの宣言
	FILE * pFile;
//-------------------------------
//コピーを処理
	if (s_Player.nMaxModelParts >= 7)
	{
		s_Player.nMaxModelParts = 7;
	}
	
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{
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
		printf("\n * * * ファイルが開けません * * * \n");
	}

	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 位置と向きの初期値を保存
	s_Player.Parts[s_Player.nMaxModelParts].posOrigin = s_Player.Parts[s_Player.nMaxModelParts].pos;
	s_Player.Parts[s_Player.nMaxModelParts].rotOrigin = s_Player.Parts[s_Player.nMaxModelParts].rot;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(&s_Player.PartsFile[s_Player.Parts[s_Player.nMaxModelParts].nType].aName[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_Player.Parts[s_Player.nMaxModelParts].pBuffer,
		NULL,
		&s_Player.Parts[s_Player.nMaxModelParts].nNumMat,
		&s_Player.Parts[s_Player.nMaxModelParts].pMesh);

	
		s_Player.nMaxModelParts++;
	
}

//----------------------
//ゲット(構造体)
//----------------------
PLAYER *GetPlayer(void)
{
	return &s_Player;
}
