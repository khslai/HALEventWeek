//=============================================================================
//
// ゲームUIヘッダー [GameUI.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// グローバルマクロ定義
// UIの種類
enum UIType
{
	UIType_ShotIcon_PlayerA,
	UIType_ShotCD_PlayerA,
	UIType_DashIcon_PlayerA,
	UIType_DashCD_PlayerA,
	UIType_ShotIcon_PlayerB,
	UIType_ShotCD_PlayerB,
	UIType_DashIcon_PlayerB,
	UIType_DashCD_PlayerB,
	UIType_HPGaugeBox_PlayerA,
	UIType_HPGauge_PlayerA,
	UIType_HPGaugeBox_PlayerB,
	UIType_HPGauge_PlayerB,
	UIType_HPGaugeBox_Boss,
	UIType_HPGauge_Boss,
	UIType_Illust_PlayerA,
	UIType_Illust_PlayerB,
	UIType_Max,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ゲームUI構造体
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3			pos;					// 座標
	int					Width;					// 横幅
	int					Height;					// 縦幅
	int					Alpha;					// 画像の透明度
	int					Type;					// UIの種類
	bool				Use;					// 使用フラグ
}GAMEUI;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitGameUI(bool FirstInit);
// 終了処理
void UninitGameUI(void);
// 更新処理
void UpdateGameUI(void);
// 描画処理
void DrawGameUI(void);
// ゲームUIの情報を取得する
GAMEUI *GetGameUI(int UIType);

#endif
