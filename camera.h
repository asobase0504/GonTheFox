//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//------------------------------------
// カメラの構造体
//------------------------------------
typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 posRDest;		// 視点の目的値
	D3DXVECTOR3 posVDest;		// 注視点の目的値
	D3DXVECTOR3 vec;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 rotDest;		// 向きの目的地
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクトマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	float fDistance;			// 視点から注視点の距離
	float fHeight;
}Camera;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitCamera(void);			// カメラの初期化処理
void UninitCamera(void);		// カメラの終了処理
void UpdateCamera(void);		// カメラの更新処理
void SetCamera();				// カメラの設定処理
Camera* GetCamera();			// カメラの情報取得

#endif // !_CAMERA_H_
