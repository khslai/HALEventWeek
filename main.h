//=============================================================================
//
// Mainヘッダー [main.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_


//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

#include <time.h>
#include <Windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <tchar.h>


#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include <dsound.h>
#include <dinput.h>
#include <mmsystem.h>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// UI
#define Screen_Width		(1920)					// ウインドウの幅
#define Screen_Height		(1080)					// ウインドウの高さ
#define ScreenCenter_X		(Screen_Width / 2)		// ウインドウの中心座標
#define ScreenCenter_Y		(Screen_Height / 2)		
// 計算用
#define UpVec				(D3DXVECTOR3(0.0f, -1.0f, 0.0f))
#define DownVec				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define LeftVec				(D3DXVECTOR3(-1.0f, 0.0f, 0.0f))
#define RightVec			(D3DXVECTOR3(1.0f, 0.0f, 0.0f))
#define Root2				(1.414214f)
// 頂点数
#define	Num_Vertex			(4)					
// ポリゴン数
#define Num_Polygon			(2)
// 壁の厚さ
#define Wall_Width			(50.0f)
// 選択肢ループカウント
#define RepeatCount			(60)
// 選択肢ループ速度
#define RepeatSpeed			(5)

// 色
#define WHITE(Alpha)		D3DCOLOR_RGBA(255, 255, 255, Alpha)
#define BLACK(Alpha)		D3DCOLOR_RGBA(0, 0, 0, Alpha)
#define GRAY(Alpha)			D3DCOLOR_RGBA(64, 64, 64, Alpha)
#define GREEN(Alpha)		D3DCOLOR_RGBA(0, 255, 0, Alpha)
#define BLUE(Alpha)			D3DCOLOR_RGBA(0, 0, 255, Alpha)
#define YELLOW(Alpha)		D3DCOLOR_RGBA(255, 255, 0, Alpha)
#define RED(Alpha)			D3DCOLOR_RGBA(255, 0, 0, Alpha)
#define SKYBLUE(Alpha)		D3DCOLOR_RGBA(135, 206, 235, Alpha)
#define AZURE(Alpha)		D3DCOLOR_RGBA(0, 127, 255, Alpha)
#define GRASSGREEN(Alpha)	D3DCOLOR_RGBA(153, 230, 77, Alpha)
#define ORANGE(Alpha)		D3DCOLOR_RGBA(255, 165, 0, Alpha)
#define PURPLE(Alpha)		D3DCOLOR_RGBA(128, 0, 128, Alpha)

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// テクスチャのメモリをリリース
// メモリリリース
// new
#define SafeDelete(Object)			{ if (Object) { delete(Object);			(Object) = NULL; } }
#define SafeDeleteArray(Object)		{ if (Object) { delete[](Object);		(Object) = NULL; } }
// malloc,calloc
#define SafeFree(Object)			{ if (Object) { free(Object);			(Object) = NULL; } }
#define SafeRelease(Object)			{ if (Object) { (Object)->Release();	(Object) = NULL; } }

// キャラクターの方向
enum CharacterDirection
{
	Down,
	Left,
	Right,
	Up,
};

// 画面遷移の状態
enum TransitionState
{
	TransitionOver = 0,	// 何もない状態
	Fadein,				// フェードイン処理
	Fadeout,			// フェードアウト処理
};

// ゲームステージ
enum StagePhase
{
	Stage_Title,
	Stage_MainMenu,
	Stage_Ite,			// 射手座
	Stage_Oushi,		// 牡牛座
	Stage_Futago,		// 双子座
	Stage_Shishi,		// 獅子座
	Stage_GameEnd,		// ゲームクリア
};

// テクスチャ色の引数
enum Texturecolor
{
	White,
	Red,
	Red_AlphaHalf,
	Green,
	SkyBlue,
	Yellow,
};

// エフェクト所有者
enum EffectOwner
{
	Player_A = 0,		// プレイヤーA
	Player_B = 1,		// プレイヤーB
	BossEffect,			// ボス
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3		vtx;					// 頂点座標
	float			rhw;					// テクスチャのパースペクティブコレクト用
	D3DCOLOR		diffuse;				// 反射光
	D3DXVECTOR2		tex;					// テクスチャ座標
} VERTEX_2D;

// 画像構造体
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3			pos;					// 座標
	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ
	int					Width;					// 横幅
	int					Height;					// 縦幅
	int					Type;					// 画像の種類
	bool				Use;					// 使用フラグ
}IMAGE;

// 選択肢構造体
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR2			pos;					// 座標
	int					PreState;				// 前の選択状態
	int					State;					// 選択状態
	bool				InYes;					// 答えは"YES"か"No"か
}SELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// デバイス取得関数
LPDIRECT3DDEVICE9 GetDevice(void);
// ゲームステージ取得関数
int GetGameStage(void);
// ゲームステージ設定関数（画面遷移）
void SetGameStage(int Stage);
// ゲームの再初期化
void ReInitialize(void);
// 弾はスクリーンの範囲内かどうかをチェック
bool CheckPosition(D3DXVECTOR3 Pos, int Width, int Height);
// テクスチャ読み込む関数
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc);
// 二つの座標の移動角度を計算する
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos);

#endif