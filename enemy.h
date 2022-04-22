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
#define MAX_COPY  (4)

//------------------------------------
// 種別の列挙型
//------------------------------------
enum ENEMY_TYPE
{
	ENEMY_TYPE_HUMANSOUL = 0,	// 人魂
	ENEMY_TYPE_SKELETON,		// がいこつ
	ENEMY_TYPE_UNGAIKYO,		// 雲外鏡
	ENEMY_TYPE_KAMAITACHI,		// かまいたち
	ENEMY_TYPE_BUDDHA,			// ブツー
	ENEMY_TYPE_MAX
};

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
	ENEMY_TYPE	type;						// エネミーのタイプ

	Parts		parts[MAX_MODELPARTS];		// モデルパーツ
	PartsFile	partsFile[MAX_MODELPARTS];	// パーツファイル
	MyMotion	motion[ANIME_MAX];			// モーション
	ANIME		motionType;					// モーションタイプ(現在)
	ANIME		motionTypeOld;				// モーションタイプ(過去)
	int			nMaxModelType;				// モデルのタイプ数
	int			nMaxModelParts;				// 扱うモデルパーツ数
	int			nMaxMotion;					// モーション数

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

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type);//セット引数座標と読み込むファイル名
void SizeSet(void);//当たり判定取得
Enemy *GetEnemy(void);//ゲット
MODELDATAPLAYER *GetModelDataEnemy(void);//motionデータのゲット
void LoadEnemy(void);	// 読込
#endif
