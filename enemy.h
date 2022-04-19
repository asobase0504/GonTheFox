//============================
//
// プレイヤー設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "motion.h"
#include "player.h"

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
//#define MAX_KEY  (6)
#define MAX_COPY  (4)

//------------------------------------
// 種別の列挙型
//------------------------------------
enum ENEMY_TYPE
{
	HUMANSOUL = 0,	// 人魂
	SKELETON,		// がいこつ
	UNGAIKYO,		// 雲外鏡
	KAMAITACHI,		// かまいたち
	BUDDHA,			// ブツー
	MAX
};

////------------------------------------
//// 挙動の列挙型
////------------------------------------
//enum ANIME
//{
//	ANIME_NORMAL = 0,	//ニュートラル
//	ANIME_RUN,			//歩き
//	ANIME_ATTACK,		//攻撃
//	ANIME_JUMP,			//ジャンプ
//	ANIME_LANDING,		//着地
//	ANIME_MAX
//};
//
////------------------------------------
//// 状態の列挙型
////------------------------------------
//enum STATUS
//{
//	STATUS_NORMAL = 0,	//ニュートラル
//	STATUS_RUN,		//歩き
//	STATUS_ATTACK,	//攻撃
//	STATUS_JUMPUP,		//ジャンプ
//	STATUS_JUMPDOWN,		//降下
//	STATUS_LANDING,	//着地
//	STATUS_MAX
//};
//
////------------------------------------
//// ダメージ状態の列挙型
////------------------------------------
//typedef enum
//{
//	DAMEGE_NORMAL = 0,	//ニュートラル
//	DAMEGE_NOU,			//ダメージくらってる
//	DAMEGE_MAX
//}DAMEGE;
//
////------------------------------------
//// 能力の列挙型
////------------------------------------
//typedef enum
//{
//	COPY_NORMAL = 0,//ニュートラル
//	COPY_SWORD,		//ソード
//	COPY_FIRE,		//ファイア
//	COPY_LASER,		//レーザー
//	COPY_CUTTER,	//カッター
//	COPY_MAX
//}COPY;
//
////modelデータの構造体//
//typedef struct
//{
//	int key;		//時間管理
//	int nowKey;		//今のキー
//	int loop;		// ループするかどうか[0:ループしない / 1 : ループする]
//	int num_key;  	// キー数
//	MyKeySet KeySet[MAX_KEY];
//}MODELDATAPLAYER;

typedef struct
{
	D3DXVECTOR3	pos;						// 位置
	D3DXVECTOR3	posOld;						// 位置過去
	D3DXVECTOR3	move;						// ムーブ
	D3DXVECTOR3	rot;						// 回転	
	D3DXVECTOR3	rotMove;					// 回転ムーブ
	D3DXVECTOR3	modelMin;					// サイズ最小
	D3DXVECTOR3	modelMax;					// サイズ最大
	D3DXMATRIX	mtxWorld;					// マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ

	STATUS		status;						// 今のステータス
	DAMEGE		damege;						// ダメージくらってるかくらってないか
	COPY		copy;						// コピー

	Parts		parts[MAX_MODELPARTS];		// モデルパーツ
	PartsFile	partsFile[MAX_MODELPARTS];	// パーツファイル
	MyMotion	motion[ANIME_MAX];			// モーション
	ANIME		motionType;					// モーションタイプ(現在)
	ANIME		motionTypeOld;				// モーションタイプ(過去)
	int			nMaxModelType;				// モデルのタイプ数
	int			nMaxModelParts;				// 扱うモデルパーツ数
	int			nMaxMotion;					// モーション数

	int			type;						// タイプ
	int			shadow;						// 影番号
	int			invincible;					// 無敵時間
	float		consumption;				// 計算用

	bool		bMotionBlend;				// モーションブレンド
	bool		bMotion;					// モーションを使用状況

	bool		isUse;						// 使ってるか使ってないか
	bool		notLoop;					// ループするかしないか
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);	// 初期化
void UninitEnemy(void);	// 破棄
void UpdateEnemy(void);	// 更新
void DrawEnemy(void);	// 描画

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);//セット引数座標と読み込むファイル名
void SizeSet(void);//当たり判定取得
Enemy *GetEnemy(void);//ゲット
MODELDATAPLAYER *GetModelDataEnemy(void);//motionデータのゲット
#endif
