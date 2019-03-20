//=============================================================================
//
// プレイヤー弾幕ヘッダー [PlayerBullet.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************

// グローバルマクロ定義
// プレイヤーの弾の最大値
#define PlayerBullet_Max	(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 弾構造体
typedef struct
{
	VERTEX_2D		vertexwk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3		pos;					// 弾の座標
	D3DXVECTOR3		PreviousPos;			// 1フレイム前の弾の座標
	D3DXVECTOR3		rot;					// テクスチャの回転角度
	D3DXVECTOR3		MoveDirection;			// 移動方向を変更する角度
	float			Radius;					// 画像中心から頂点までの距離
	float			HitRadius;				// 当たり判定の円の半径
	float			BaseAngle;				// 画像中心から頂点までの角度
	float			Speed;					// 弾のスピード
	int				Count;					// カウント
	int				Color;					// 色
	int				State;					// 弾の状態
	bool			Use;					// 発射されたかのフラグ
	bool			DelayDisappear;			// 消失が遅れるかのフラグ
}BULLET;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitPlayerBullet(bool FirstInit);
// 終了処理
void UninitPlayerBullet(void);
// 更新処理
void UpdatePlayerBullet(void);
// 描画処理
void DrawPlayerBullet(void);
// プレイヤーの弾発射
void PlayerShot(int Player_No);

#endif
