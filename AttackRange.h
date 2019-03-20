//=============================================================================
//
// 当たり範囲表示ヘッダー [AttackRange.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _ATTACKRANGE_H_
#define _ATTACKRANGE_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 当たり範囲の種類
enum AttackRangeType
{
	PlayerAR_A,
	PlayerBulletAR_A,
	PlayerAR_B,
	PlayerBulletAR_B,
	BossAR,
	BossBulletAR,
};


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 当たり範囲構造体
typedef struct 
{
	VERTEX_2D		vertexwk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3		pos;					// 座標
	float			Radius;					// 画像中心から頂点までの距離
	int				Owner;					// 所有者
	int				BulletNo;				// 対応するバレット番号
	int				BarrageNo;				// 対応する弾幕番号
	bool			Use;					// 使用フラグ
}ATTACKRANGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitAttackRange(bool FirstInit);
// 終了処理
void UninitAttackRange(void);
// 更新処理
void UpdateAttackRange(void);
// 描画処理
void DrawAttackRange(void);
// キャラクターの当たり範囲の設置
void SetCharacterAR(D3DXVECTOR3 Pos, float HitRadius, int Owner);
// 弾の当たり範囲の設置
void SetBulletAR(D3DXVECTOR3 Pos, float HitRadius, int Bullet_No, int Owner);

#endif
