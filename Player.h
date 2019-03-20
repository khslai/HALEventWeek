//=============================================================================
//
// プレイヤーヘッダー [Player.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "PlayerBullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// グローバルマクロ定義
// プレイヤーの最大人数
#define Player_Max			(2)
// プレイヤーの弾発射間隔
#define ShotInterval		(90)
// プレイヤーの斬撃間隔
#define SlashInterval		(30)
// ダッシュの時間
#define DashTime			(90)
// プレイヤーのダッシュ間隔
#define DashCDInterval		(1800)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 当たりポイント構造体
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];		// 頂点情報格納ワーク
	float				Radius;						// 画像中心から頂点までの距離
	float				BaseAngle;					// 画像中心から頂点までの角度
}HITPOINT;

// プレイヤー構造体
typedef	struct
{
	VERTEX_2D			vertexwk[Num_Vertex];		// 頂点情報格納ワーク
	LPDIRECT3DTEXTURE9	texture;					// テクスチャ
	D3DXVECTOR3			pos;						// プレイヤーの座標
	D3DXVECTOR3			PreviousPos;				// 1フレイム前の座標
	BULLET				Bullet[PlayerBullet_Max];	// プレイヤーの弾
	HITPOINT			HitPoint;					// 当たりポイント
	int					Direction;					// 方向
	int					Count;						// プレイヤーのアニメカウント
	int					ShotCount;					// プレイヤーのショットカウント
	int					DashCDCount;				// ダッシュのクールダウンカウント
	int					InvincibleCount;			// 無敵カウント
	int					AnimPattern;				// プレイヤーのアニメパターン
	int					dirBlock;					// 障害物にぶつかった時、プレイヤーの向き
	float				Speed;						// 移動速度
	float				HP;							// 生命値
	float				HP_Max;						// 最大生命値
	float				Radius;						// 画像中心から頂点までの距離
	float				HitRadius;					// 当たり判定の円の半径
	float				BaseAngle;					// 画像中心から頂点までの角度
	float				RegeneBonus;				// 双子座のパワーアップボーナス
	float				SlowSpeed;					// 低速モードの移動速度
	bool				InDash;						// ダッシュフラグ
	bool				InFlash;					// 無敵状態のフラッシュ
	bool				InSlash;					// 斬撃
	bool				InMove;						// 移動中
	bool				HitBlock;					// 障害物にぶつかるフラグ
	bool				Exist;						// 使用フラグ
	bool				flagOnIce;					// プレイヤーが氷の床の上にいるかどうかを判断するフラグ
	bool				flagSlide;					// プレイヤーが氷の床の上に滑っているかどうかを判断するフラグ
	bool				Use;						// 使用フラグ
}PLAYER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitPlayer(bool FirstInit);
// 終了処理
void UninitPlayer(void);
// 更新処理
void UpdatePlayer(void);
// 描画処理
void DrawPlayer(void);
// プレイヤーの情報を取得する
PLAYER *GetPlayer(int Player_No);
// プレイヤーのテクスチャのサイズを取得する
D3DXVECTOR2 GetPlayerSize(void);

#endif
