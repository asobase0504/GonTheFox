//============================
//
// プレイヤー設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define MAX_MODELPARTS (9)
#define MAX_KEY  (6)

typedef enum
{
	ANIME_NORMAL = 0,	//ニュートラル
	ANIME_RUN,		//歩き
	ANIME_ATTACK,	//攻撃
	ANIME_JUMP,		//ジャンプ
	ANIME_LANDING,	//着地

	ANIME_MAX
}ANIME;

typedef enum
{
	STATUS_NORMAL = 0,	//ニュートラル
	STATUS_RUN,		//歩き
	STATUS_ATTACK,	//攻撃
	STATUS_JUMPUP,		//ジャンプ
	STATUS_JUMPDOWN,		//ジャンプ
	STATUS_LANDING,	//着地
	STATUS_MAX
}STATUS;

typedef enum
{
	DAMEGE_NORMAL = 0,	//ニュートラル
	DAMEGE_NOU,			//ダメージくらってる
	DAMEGE_MAX
}DAMEGE;


//キーの構造体//
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3  rot;
}KEYPLAYER;

//キーセットの構造体//
typedef struct
{
	int KeyFrame;
	KEYPLAYER key[MAX_MODELPARTS];
}KEYSETPLAYER;

//modelデータの構造体//
typedef struct
{
	int Key;		//時間管理
	int NowKey;		//今のキー
	int LOOP;		// ループするかどうか[0:ループしない / 1 : ループする]
	int NUM_KEY;  	// キー数
	KEYSETPLAYER KeySet[MAX_KEY];
}MODELDATAPLAYER;

//モデルの構造体//
typedef struct
{
	D3DXVECTOR3 ModelMin;		//当たり判定最小
	D3DXVECTOR3 ModelMax;		//当たり判定最大
	D3DXMATRIX MtxWorld;		//マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ
	LPD3DXMESH pMesh;			//パーツのメッシュ
	LPD3DXBUFFER pBuffMat;		//パーツのバッファ
	DWORD nNumMat;				//パーツのマット
	D3DXVECTOR3 pos;			//パーツのポス
	D3DXVECTOR3 posOri;			//オリジナル
	D3DXVECTOR3 posdefault;		//最初
	D3DXVECTOR3 rot;			//パーツのロット
	D3DXVECTOR3 rotOri;			//オリジナル
	D3DXVECTOR3 rotdefault;		//最初
	int nIdxModelParent;         //親のインデックス       aModel[   ] の番号
}ModelParts;

typedef struct
{
	ModelParts Parts[MAX_MODELPARTS];	//modelの数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOld;	//位置過去
	D3DXVECTOR3 move;	//ムーブ
	D3DXVECTOR3 rot;	//回転	
	D3DXVECTOR3 RotMove;//回転ムーブ
	D3DXVECTOR3 ModelMin;//サイズ最小
	D3DXVECTOR3 ModelMax;//サイズ最大
	D3DXMATRIX MtxWorld; //マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ
	int nType;			//タイプ
	int nShadow;		//影番号
	int invincible;		//無敵時間
	ANIME nMotion;		//いま使ってるmotioの番号
	STATUS Status;		//今のステータス
	DAMEGE Damege;		//ダメージくらってるかくらってないか
	bool bUse;			//使ってるか使ってないか
	bool bMystery;		//まほう使うか使わないか
	bool NotLoop;		//ループするかしないか
	float consumption;	//計算用
}PLAYER;


//プロトタイプ宣言
void InitPlayer(void);//初期化
void UninitPlayer(void);//破棄
void UpdatePlayer(void);//更新
void DrawPlayer(void);//描画

void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Filename, int nParent,int nIndex, D3DXVECTOR3 nModelPos);//セット引数座標と読み込むファイル名
void SizSet(void);//当たり判定取得
void AnimationSet(int Animation);//アニメーションの計算
void MoveSet(void);	//ムーブセット
void Collision(void);	//当たり判定まとめ
PLAYER *GetPlayer(void);//ゲット
MODELDATAPLAYER *GetModelData(void);//motionデータのゲット
#endif
