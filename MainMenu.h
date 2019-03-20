//=============================================================================
//
// メインメニューヘッダー [MainMenu.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _MAINMENU_H_
#define _MAINMENU_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ゲームオーバー選択肢状態
enum MainMenuSelectState
{
	IteStage,		// いて座
	FutagoStage,	// ふたご座
	OushiStage,		// おうし座
	ShishiStage,	// しし座
	StageCheck,		// ステージ確認画面
};

enum StageClearFlag
{
	ClearFlag_Ite,
	ClearFlag_Futago,
	ClearFlag_Oushi,
	ClearFlag_Shishi,
	ClearFlag_Max,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitMainMenu(bool FirstInit);
// 終了処理
void UninitMainMenu(void);
// 更新処理
void UpdateMainMenu(void);
// 描画処理
void DrawMainMenu(void);
// 選択したステージを取得する
int GetSelectStage(void);
// ステージクリアフラグを設置
void SetClearFlag(int StageClearFlag);

#endif
