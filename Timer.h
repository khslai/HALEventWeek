//=============================================================================
//
// タイマー処理 [Timer.h]
// Author：
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// タイマー構造体
typedef struct
{
	VERTEX_2D				vertexwk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// 座標
	int						Number;					// 表示する数字
}TIMER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimer(bool FirstInit);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void SetTimerUse(bool Flag);
// ゲームスタートのカウントダウンフラグを取得
bool GetStartCDOver(void);
// ゲーム終了のカウントダウンフラグを取得
bool GetEndCDOver(void);

#endif
