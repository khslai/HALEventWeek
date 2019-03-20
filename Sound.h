//=============================================================================
//
// サウンドヘッダー [Sound.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// サウンド番号
enum Sound_No
{
	BGM_Futago,
	BGM_GameEnd,
	BGM_Ite,
	BGM_MainMenu,
	BGM_Oushi,
	BGM_Shishi,
	BGM_Title,
	SE_BossShot_01,
	SE_BossShot_02,
	SE_BossShot_03,
	SE_BulletFreeze,
	SE_Cancel,
	SE_Determine,
	SE_HitApple,
	SE_HitBlock,
	SE_HitBull,
	SE_HPRestore,
	SE_PlayerBeShot,
	SE_Scream_PlayerA,
	SE_Scream_PlayerB,
	SE_SelectMove,
	SE_ShotArrow,
	SE_Slash,
	SE_SpeedDown,
	SE_StageClear,
	Sound_Max,
};

// 再生用フラグ
enum
{
	E_DS8_FLAG_NONE,	// 一回しかプレイしない
	E_DS8_FLAG_LOOP,	// ループ
	E_DS8_FLAG_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT	InitSound(HWND hWnd);
// 再初期化処理
void ReInitSound(void);
// 終了処理
void UninitSound(void);
// 更新処理
void UpdateSound(void);
// BGM、SEを設置する
void SetSound(int Sound_No, int Flag, bool FromStart, bool IsBGM);
// サウンドを停止する
void StopSound(int Sound_No);
// 今鳴らしているBGMの番号を取得する
int GetPlayingBGM_No(void);
// サウンドのフェイドアウト
void SoundFadeOut(int Sound_No);

#endif