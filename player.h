//============================
//
// プレイヤー設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//------------------------------------
// マクロ
//------------------------------------

#define Attenuation	(0.5f)		//減衰係数
#define Speed	(1.0f)			//スピード
#define WIDTH (10.0f)			//モデルの半径
#define MAX_PRAYER (16)			//最大数
#define MAX_MOVE (9)			//アニメーションの最大数
#define INVINCIBLE (300)		//無敵時間
#define MAX_MODELPARTS (9)
#define MAX_KEY  (6)
#define MAX_COPY  (4)

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

typedef enum
{
	COPY_NORMAL = 0,//ニュートラル
	COPY_SWORD,		//ソード
	COPY_FIRE,		//ファイア
	COPY_LASER,		//レーザー
	COPY_CUTTER,	//カッター
	COPY_MAX
}COPY;

//キーの構造体//
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3  rot;
}KEYPLAYER;

//キーセットの構造体//
typedef struct
{
	int keyFrame;
	KEYPLAYER key[MAX_MODELPARTS];
}KEYSETPLAYER;

//modelデータの構造体//
typedef struct
{
	int key;		//時間管理
	int nowKey;		//今のキー
	int loop;		// ループするかどうか[0:ループしない / 1 : ループする]
	int num_key;  	// キー数
	KEYSETPLAYER KeySet[MAX_KEY];
}MODELDATAPLAYER;

//モデルの構造体//
typedef struct
{
	D3DXVECTOR3 modelMin;		//当たり判定最小
	D3DXVECTOR3 modelMax;		//当たり判定最大
	D3DXMATRIX mtxWorld;		//マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ
	LPD3DXMESH mesh;			//パーツのメッシュ
	LPD3DXBUFFER buffMat;		//パーツのバッファ
	DWORD numMat;				//パーツのマット
	D3DXVECTOR3 pos;			//パーツのポス
	D3DXVECTOR3 posOri;			//オリジナル
	D3DXVECTOR3 posdefault;		//最初
	D3DXVECTOR3 rot;			//パーツのロット
	D3DXVECTOR3 rotOri;			//オリジナル
	D3DXVECTOR3 rotdefault;		//最初
	int idxModelParent;         //親のインデックス       aModel[   ] の番号
}ModelParts;

typedef struct
{
	ModelParts Parts[MAX_MODELPARTS];	//modelの数
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOld;	//位置過去
	D3DXVECTOR3 move;	//ムーブ
	D3DXVECTOR3 rot;	//回転	
	D3DXVECTOR3 rotMove;//回転ムーブ
	D3DXVECTOR3 modelMin;//サイズ最小
	D3DXVECTOR3 modelMax;//サイズ最大
	D3DXMATRIX mtxWorld; //マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ
	int type;			//タイプ
	int shadow;		//影番号
	int invincible;		//無敵時間
	ANIME motion;		//いま使ってるmotioの番号
	STATUS status;		//今のステータス
	DAMEGE damege;		//ダメージくらってるかくらってないか
	COPY copy;			//コピー
	bool use;			//使ってるか使ってないか
	bool notLoop;		//ループするかしないか
	float consumption;	//計算用
}PLAYER;



//プロトタイプ宣言
void InitPlayer(void);//初期化
void UninitPlayer(void);//破棄
void UpdatePlayer(void);//更新
void DrawPlayer(void);//描画

void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *filename, int parent,int index);//セット引数座標と読み込むファイル名
void SizSet(void);//当たり判定取得
void AnimationSet(int animation);//アニメーションの計算
void MoveSet(void);	//ムーブセット
void Collision(void);	//当たり判定まとめ
void Loadmotion(MODELDATAPLAYER* set ,int Setnumber);
void SetCopy(void);
PLAYER *GetPlayer(void);//ゲット
MODELDATAPLAYER *GetModelData(void);//motionデータのゲット
#endif
