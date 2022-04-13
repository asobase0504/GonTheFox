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

#include "file.h"

#include "title.h"

#include "sound.h"

//------------------------------------
// マクロ
//------------------------------------

#define Attenuation	(0.5f)		//減衰係数
#define Speed	(1.0f)			//スピード
#define WIDTH (10.0f)			//モデルの半径
#define MAX_PRAYER (16)			//最大数
#define MAX_MOVE (5)			//アニメーションの最大数
#define INVINCIBLE (300)		//無敵時間
//------------------------------------
// static変数
//------------------------------------
static PLAYER s_Player;	// ポリゴンの構造体
static MODELDATAPLAYER s_ModelData[MAX_MOVE];
static int s_nTime, s_nParts;//タイマーとパーツの最大数
static D3DXVECTOR3 mystery;
static int s_nPow;//ジャンプパワー

				  //=========================================
				  // 初期化処理
				  //=========================================
void InitPlayer(void)
{
	//カメラのデータ取得
	Camera *pCamera;
	pCamera = GetCamera();


	s_nPow = 0;
	// 初期化処理
	//縦　　　　　　　　　　//横
	s_Player.RotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);
	s_Player.consumption = 0.0f;
	s_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 頂点座標
	s_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転座標
	s_Player.nShadow = 0;							//影番号
	s_Player.Damege = DAMEGE_NORMAL;
	s_Player.nMotion = ANIME_NORMAL;							//いま使ってるmotioの番号
	s_Player.bUse = false;							//使ってるか使ってないか
	s_Player.bMystery = false;						//大魔法の発動スイッチ
	s_nParts = 0;
	s_Player.ModelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	s_Player.ModelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int nSet = 0; nSet < MAX_MODELPARTS; nSet++)
	{
		s_Player.Parts[nSet].nIdxModelParent = 0;
	}
	s_nTime = 0;

	//ファイル
	LoadSetFile(LOOD_FILE_NAME_000);//0がプレイヤー

//ファイル
	LoadSetFile(LOOD_FILE_NAME_002);//0がプレイヤー
}

//=========================================
// 終了処理
//=========================================
void UninitPlayer(void)
{
	StopSound();

	for (int i = 0; i < s_nParts; i++)
	{
		// 頂点バッファーの解放
		if (s_Player.Parts[i].pBuffMat != NULL)
		{
			s_Player.Parts[i].pBuffMat->Release();
			s_Player.Parts[i].pBuffMat = NULL;
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
	MoveSet();	//動きセット

	//Collision();//床

	//			//アニメーションや足音の設定
	if (s_Player.NotLoop == false)
	{
		s_Player.nMotion = ANIME_NORMAL;
	}
	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_S) ||
		GetJoypadPress(JOYKEY_UP, 0) || GetJoypadPress(JOYKEY_DOWN, 0) || GetJoypadPress(JOYKEY_LEFT, 0) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{
		s_Player.nMotion = ANIME_RUN;//歩く

	}
	
	if (GetKeyboardTrigger(DIK_B))
	{
		s_Player.nMotion = ANIME_ATTACK;//攻撃
		s_Player.NotLoop = true;
	}
	////影更新
	////SetposShadow(s_Player.nShadow, D3DXVECTOR3(s_Player.pos.x, s_Player.pos.y, s_Player.pos.z));


	AnimationSet((int)s_Player.nMotion);//アニメーション再生


}

//=========================================
// 描画処理
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// 計算用マトリックス
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat;
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	if (s_Player.bUse)//使ってるやつ出す
	{
		// ワールドマトリックスの初期化
		// 行列初期化関数(第1引数の行列を単位行列に初期化)
		D3DXMatrixIdentity(&s_Player.MtxWorld);

		// 拡縮を反映
		// 行列拡縮関数
		D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&s_Player.MtxWorld, &s_Player.MtxWorld, &mtxScale);

		// 向きを反映
		// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Player.rot.y, s_Player.rot.x, s_Player.rot.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&s_Player.MtxWorld, &s_Player.MtxWorld, &mtxRot);

		// 位置を反映
		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixTranslation(&mtxTrans, s_Player.pos.x, s_Player.pos.y, s_Player.pos.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&s_Player.MtxWorld, &s_Player.MtxWorld, &mtxTrans);

		for (int i = 0; i < s_nParts; i++)
		{
			// ワールドマトリックスの初期化
			// 行列初期化関数(第1引数の行列を単位行列に初期化)
			D3DXMatrixIdentity(&s_Player.Parts[i].MtxWorld);

			//// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			//D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxScale);

			// 向きを反映
			// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, s_Player.Parts[i].rot.y, s_Player.Parts[i].rot.x, s_Player.Parts[i].rot.z);
			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxRot);

			// 位置を反映
			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
			D3DXMatrixTranslation(&mtxTrans, s_Player.Parts[i].pos.x, s_Player.Parts[i].pos.y, s_Player.Parts[i].pos.z);
			// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
			D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxTrans);

			D3DXMATRIX mtxParent;

			//現在のマテリアルを獲得
			pDevice->GetMaterial(&marDef);

			if (s_Player.Parts[i].nIdxModelParent == -1)
			{//親
				mtxParent = s_Player.MtxWorld;
			}
			else
			{//子
				mtxParent = s_Player.Parts[s_Player.Parts[i].nIdxModelParent].MtxWorld;
			}

			//親と子の融合
			D3DXMatrixMultiply(&s_Player.Parts[i].MtxWorld, &s_Player.Parts[i].MtxWorld, &mtxParent);

			// ワールド座標行列の設定
			pDevice->SetTransform(D3DTS_WORLD, &s_Player.Parts[i].MtxWorld);
			//マテリアルデータのポインタを取得
			pMat = (D3DXMATERIAL*)s_Player.Parts[i].pBuffMat->GetBufferPointer();

			for (int j = 0; j < (int)s_Player.Parts[i].nNumMat; j++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, NULL);
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[j].MatD3D);
				//モデルパーツの描画
				s_Player.Parts[i].pMesh->DrawSubset(j);
			}
		}
		//現在のマテリアルを元に戻す
		pDevice->SetMaterial(&marDef);
	}
}



void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Filename, int nParent, int nIndex, D3DXVECTOR3 nModelPos)
{
	//カメラのデータ取得
	Camera *pCamera = GetCamera();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	char aFile[128] = FILE_3D_PLAYER;

	strcat(aFile, &Filename[0]);//合成　aFile＜-こいつに入れる

	D3DXLoadMeshFromX(&aFile[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_Player.Parts[s_nParts].pBuffMat,
		NULL,
		&s_Player.Parts[s_nParts].nNumMat,
		&s_Player.Parts[s_nParts].pMesh
	);

	s_Player.Parts[s_nParts].nIdxModelParent = nParent;
	s_Player.RotMove = D3DXVECTOR3(D3DX_PI + pCamera->rot.y, D3DX_PI*0.5f + pCamera->rot.y, 0.0f);

	// 初期化処理
	s_Player.pos = nModelPos;		//初回座標設定
	s_Player.Parts[s_nParts].pos = pos;	// 頂点座標
	s_Player.Parts[s_nParts].posOri = s_Player.Parts[s_nParts].pos;
	s_Player.Parts[s_nParts].posdefault = s_Player.Parts[s_nParts].pos;
	s_Player.Parts[s_nParts].rot = rot;	// 回転座標
	s_Player.Parts[s_nParts].rotOri = s_Player.Parts[s_nParts].rot;
	s_Player.Parts[s_nParts].rotdefault = s_Player.Parts[s_nParts].rot;

	s_Player.bUse = true;

	if (s_Player.Parts[s_nParts].nIdxModelParent == -1)
	{
		SizSet();
		//s_Player.nShadow = SetShadow(s_Player.pos, s_Player.rot, 0);//場所.回転
	}
	s_nParts++;
}

//--------------------------
//当たり判定のサイズせってい
//--------------------------
void SizSet(void)
{
	//すべての頂点数を保存する関数を作る
	int NumVpx;	//頂点数
				//一つの頂点のサイズを入れる関数
	DWORD sizeFVF;
	//バイトがたのポインターchar
	BYTE *pVtxBuff;

	//ここで頂点数を参照
	NumVpx = s_Player.Parts[s_nParts].pMesh->GetNumVertices();

	//ここでサイズを入れる
	sizeFVF = D3DXGetFVFVertexSize(s_Player.Parts[s_nParts].pMesh->GetFVF());

	//頂点バッファのロックしましょう
	s_Player.Parts[s_nParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int i = 0; i < NumVpx; i++)
	{
		D3DXVECTOR3 pVtx = *(D3DXVECTOR3*)pVtxBuff;
		//それを使ってモデルの最大値と最小値を使って求める
		//XYZ全部求める
		if (pVtx.x < s_Player.ModelMin.x)
		{
			s_Player.ModelMin.x = pVtx.x;
		}
		if (pVtx.x > s_Player.ModelMax.x)
		{
			s_Player.ModelMax.x = pVtx.x;
		}
		if (pVtx.y < s_Player.ModelMin.y)
		{
			s_Player.ModelMin.y = pVtx.y;
		}
		if (pVtx.y > s_Player.ModelMax.y)
		{
			s_Player.ModelMax.y = pVtx.y;
		}
		if (pVtx.z < s_Player.ModelMin.z)
		{
			s_Player.ModelMin.z = pVtx.z;
		}
		if (pVtx.z > s_Player.ModelMax.z)
		{
			s_Player.ModelMax.z = pVtx.z;
		}

		//一個分求めたら次に生きたいので
		pVtxBuff += sizeFVF;
		//これをやれば最大最小を求められる
	}
	s_Player.ModelMax.z *= 3;
	s_Player.ModelMax.x *= 3;
	//頂点バッファのロックしましょう
	s_Player.Parts[s_nParts].pMesh->UnlockVertexBuffer();
}

//------------------------------
//アニメーションセット
//-------------------------------
void AnimationSet(int Animation)
{
	s_ModelData[Animation].Key++;//カウント加算

	if (s_ModelData[Animation].LOOP == 1)
	{//YESループ
		if (s_ModelData[Animation].Key <= s_ModelData[Animation].KeySet[0].KeyFrame)
		{//キーフレーム加算入った

			for (int i = 0; i < MAX_MODELPARTS; i++)
			{//パーツ全部に位置の移動などを入れる

				D3DXVECTOR3 RotDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].rot - s_Player.Parts[i].rotOri);
				D3DXVECTOR3 PosDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].pos - s_Player.Parts[i].posOri);

				if (RotDiff.x > D3DX_PI)
				{
					RotDiff.x += D3DX_PI * 2;
				}
				if (RotDiff.x < -D3DX_PI)
				{
					RotDiff.x += -D3DX_PI * 2;
				}
				//正規化
				if (RotDiff.z > D3DX_PI)
				{
					RotDiff.z += D3DX_PI * 2;
				}
				if (RotDiff.z < -D3DX_PI)
				{
					RotDiff.z += -D3DX_PI * 2;
				}
				//正規化
				if (RotDiff.y > D3DX_PI)
				{
					RotDiff.y += D3DX_PI * 2;
				}
				if (RotDiff.y < -D3DX_PI)
				{
					RotDiff.y += -D3DX_PI * 2;
				}

				D3DXVECTOR3 RotOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);
				D3DXVECTOR3 PosOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);

				D3DXVECTOR3 RotFrame = RotOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].rotOri;
				D3DXVECTOR3 PosFrame = PosOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].posOri;

				//バグったら正規化しなよ上のBy過去の浜田
				s_Player.Parts[i].rot = RotFrame;
				s_Player.Parts[i].pos = PosFrame;

				//正規化
				if (s_Player.Parts[i].rot.x > D3DX_PI)
				{
					s_Player.Parts[i].rot.x += D3DX_PI * 2;
				}
				if (s_Player.Parts[i].rot.x < -D3DX_PI)
				{
					s_Player.Parts[i].rot.x += -D3DX_PI * 2;
				}
				//正規化
				if (s_Player.Parts[i].rot.z > D3DX_PI)
				{
					s_Player.Parts[i].rot.z += D3DX_PI * 2;
				}
				if (s_Player.Parts[i].rot.z < -D3DX_PI)
				{
					s_Player.Parts[i].rot.z += -D3DX_PI * 2;
				}
				//正規化
				if (s_Player.Parts[i].rot.y > D3DX_PI)
				{
					s_Player.Parts[i].rot.y += D3DX_PI * 2;
				}
				if (s_Player.Parts[i].rot.y < -D3DX_PI)
				{
					s_Player.Parts[i].rot.y += -D3DX_PI * 2;
				}
			}
		}

		if (s_ModelData[Animation].Key >= s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame)
		{//アニメーションの更新してキーをゼロに戻します
			for (int i = 0; i < MAX_MODELPARTS; i++)
			{//パーツ全部に位置の移動などを入れる
				s_Player.Parts[i].rotOri = s_Player.Parts[i].rot;
				s_Player.Parts[i].posOri = s_Player.Parts[i].pos;
			}
			s_ModelData[Animation].NowKey++;									//次に更新
			s_ModelData[Animation].Key = 0;										//キーをゼロにする
			s_ModelData[Animation].NowKey %= s_ModelData[Animation].NUM_KEY;	//アニメーションの最大を超えたらゼロに戻す
			s_Player.nMotion = ANIME_NORMAL;
		}
	}
	else
	{//NOTループ
		for (int i = 0; i < MAX_MODELPARTS; i++)
		{//パーツ全部に位置の移動などを入れる

			D3DXVECTOR3 RotDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].rot - s_Player.Parts[i].rotOri);
			D3DXVECTOR3 PosDiff = (s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].key[i].pos - s_Player.Parts[i].posOri);

			D3DXVECTOR3 RotOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);
			D3DXVECTOR3 PosOneFrame = (RotDiff / (float)s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame);

			D3DXVECTOR3 RotFrame = RotOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].rotOri;
			D3DXVECTOR3 PosFrame = PosOneFrame * (float)s_ModelData[Animation].Key + s_Player.Parts[i].posOri;

			//バグったら正規化しなよ上のBy過去の浜田
			s_Player.Parts[i].rot = RotFrame;
			s_Player.Parts[i].pos = PosFrame;

			//正規化
			if (s_Player.Parts[i].rot.x > D3DX_PI)
			{
				s_Player.Parts[i].rot.x += D3DX_PI * 2;
			}
			if (s_Player.Parts[i].rot.x < -D3DX_PI)
			{
				s_Player.Parts[i].rot.x += -D3DX_PI * 2;
			}
			//正規化
			if (s_Player.Parts[i].rot.z > D3DX_PI)
			{
				s_Player.Parts[i].rot.z += D3DX_PI * 2;
			}
			if (s_Player.Parts[i].rot.z < -D3DX_PI)
			{
				s_Player.Parts[i].rot.z += -D3DX_PI * 2;
			}
			//正規化
			if (s_Player.Parts[i].rot.y > D3DX_PI)
			{
				s_Player.Parts[i].rot.y += D3DX_PI * 2;
			}
			if (s_Player.Parts[i].rot.y < -D3DX_PI)
			{
				s_Player.Parts[i].rot.y += -D3DX_PI * 2;
			}
		}
	}

	if (s_ModelData[Animation].Key >= s_ModelData[Animation].KeySet[s_ModelData[Animation].NowKey].KeyFrame)
	{//アニメーションの更新してキーをゼロに戻します

		for (int i = 0; i < MAX_MODELPARTS; i++)
		{//パーツ全部に位置の移動などを入れる
			s_Player.Parts[i].rotOri = s_Player.Parts[i].rot;
			s_Player.Parts[i].posOri = s_Player.Parts[i].pos;
		}
		s_ModelData[Animation].NowKey++;									//次に更新
		s_ModelData[Animation].Key = 0;										//キーをゼロにする
		if (s_ModelData[Animation].NowKey == s_ModelData[Animation].NUM_KEY)
		{
			for (int i = 0; i < MAX_MODELPARTS; i++)
			{//パーツ全部に位置の移動などを入れる
				s_Player.Parts[i].rotOri = s_Player.Parts[i].rotdefault;
				s_Player.Parts[i].posOri = s_Player.Parts[i].posdefault;
			}
			s_ModelData[Animation].NowKey = 0;
			s_Player.NotLoop = false;

		}
	}
}

//------------------------------
//動きセット
//-------------------------------
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

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x < -0.5f)
	{//左に半分より多く倒したら
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y > 0.5f)
	{//上に半分より多く倒したら
		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.RotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	else if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).y < -0.5f)
	{//下に半分より多く倒したら
		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_W) || GetJoypadPress(JOYKEY_UP, 0))
	{//W押したとき上加速

		s_Player.move.x += sinf(pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - s_Player.rot.y + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_S) || GetJoypadPress(JOYKEY_DOWN, 0))
	{//S押したと下加速

		s_Player.move.x += sinf(pCamera->rot.y - D3DX_PI)*Speed*nDash;
		s_Player.move.z += cosf(pCamera->rot.y - D3DX_PI)*Speed*nDash;

		s_Player.consumption = (s_Player.RotMove.x -
			D3DX_PI - s_Player.rot.y
			+ pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_D) || GetJoypadPress(JOYKEY_RIGHT, 0))
	{//D押したとき右加速

		s_Player.move.x += sinf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;


		s_Player.consumption = s_Player.RotMove.x + (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;
	}

	if (GetKeyboardPress(DIK_A) || GetJoypadPress(JOYKEY_LEFT, 0))
	{//A押したと左加速

		s_Player.move.x += sinf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;
		s_Player.move.z += cosf(-D3DX_PI *0.5f + pCamera->rot.y)*Speed*nDash;

		s_Player.consumption = s_Player.RotMove.x - (D3DX_PI*0.5f) - s_Player.rot.y + pCamera->rot.y;

	}
	s_Player.posOld = s_Player.pos;//過去の移動量を保存

	s_Player.move.x += (0.0f - s_Player.move.x)*0.5f;//（目的の値-現在の値）＊減衰係数
	s_Player.move.z += (0.0f - s_Player.move.z)*0.5f;
	s_Player.pos += s_Player.move;//移動を加算

	if (s_Player.invincible <= 0)
	{//無敵時間がゼロになったらダメージくらうようにする
		s_Player.Damege = DAMEGE_NORMAL;
	}

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

	//ゲーム以外の時の時の移動方法
}
//------------------------------
//当たり判定まとめ
//-------------------------------
void Collision(void)
{

	
	

}

//----------------------
//ゲット(構造体)
//----------------------
PLAYER *GetPlayer(void)
{
	return &s_Player;
}

//----------------------
//ゲット(motion)
//----------------------
MODELDATAPLAYER *GetModelData(void)
{
	return &s_ModelData[0];
}
