//=============================================================================
//
// ボスヘッダー [Boss.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "PlayerBullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// グローバルマクロ定義
// ボスの弾の最大値
#define BossBullet_Max		(500)

// ボスの状態
enum BossState
{
	Idle,		// 待機
	Shot,		// ショット
	BossDead,	// 死亡
};


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ボス構造体
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3			pos;					// 座標
	LPDIRECT3DTEXTURE9	texture;				// テクスチャ
	BULLET				Bullet[BossBullet_Max];	// ボスの弾
	float				Radius;					// 画像中心から頂点までの距離
	float				HitRadius;				// 当たり判定の円の半径
	float				BaseAngle;				// 画像中心から頂点までの角度
	float				HP;						// 生命値
	float				HP_Max;					// 最大生命値
	int					AnimCount;				// アニメ加算カウント
	int					StateCount;				// 状態加算カウント
	int					AnimPattern;			// アニメパターン
	int					State;					// ボスの状態
	int					BarrageType;			// ボスの弾幕の種類
	bool				Exist;					// 存在かどうかのフラグ
}BOSS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitBoss(bool FirstInit);
// 終了処理
void UninitBoss(void);
// 更新処理
void UpdateBoss(void);
// 描画処理
void DrawBoss(void);
// ボスの情報を取得する
BOSS *GetBoss(void);
// ボスの色の設定
void SetBossColor(int Color);

#endif
