//=============================================================================
//
// タイマー処理 [Timer.cpp]
// Author：
//
//=============================================================================
#include "main.h"
#include "Timer.h"
#include "Score.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_TimerBox		_T("data/Texture/UI/TimerBox.png")
#define Texture_TimerBox_Width	(250)
#define Texture_TimerBox_Height	(120)
// タイマーの枠の表示座標
#define TimerBox_Pos_X			(850)
#define TimerBox_Pos_Y			(0)

// タイマーの数字の幅
#define Texture_Number_Width	(70)
// タイマーの数字の高さ
#define Texture_Number_Height	(70)
// タイマーの数字の表示間隔
#define	Timer_Interval			(30)							
// タイマーの桁数
#define	TimerDigit				(2)			
// タイマーの表示座標
#define Timer_Pos_X				(990)
#define Timer_Pos_Y				(40)

// カウントダウンの表示座標
#define CountDown_Pos_X			(900)
#define CountDown_Pos_Y			(80)
// 数字拡大
#define	NumZoom					(50)

// 一回プレイの時間
#define OnePlayTime_Oushi		(30)
#define OnePlayTime_Futago		(60)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeTimerVertex(int Digit, int Num);
HRESULT MakeTimerBoxVertex(void);
// 頂点座標の設定
void SetTimerVertex(int Digit);
void SetTimerBoxVertex(void);
// テクスチャ座標の設定
void SetTimerTexture(int Digit, int Num);
void SetTimerBoxTexture(void);
// タイマーのカウントダウンを設置する
void SetTimerCountDown(void);


//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		TimerBox_Texture = NULL;
// タイマーの時間
int						TimerNumber = 0;
// タイマー動作ON/OFF
bool					TimerUse = true;
// カウントダウンフラグ
bool					CountDown = false;
// タイマー構造体
TIMER					Timer[TimerDigit];
// タイマーの枠構造体
TIMER					TimerBox;
// ゲームスタートのカウントダウン終了フラグ
bool					StartCDOver = false;
// ゲーム終了のカウントダウン終了フラグ
bool					EndCDOver = false;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimer(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// タイマーの初期化
	StartCDOver = false;
	EndCDOver = false;
	TimerUse = false;
	// ゲーム開始のカウントダウン(3秒)
	TimerNumber = 3;
	CountDown = true;

	for (int Digit = 0; Digit < TimerDigit; Digit++)
	{
		Timer[Digit].pos = D3DXVECTOR3((float)Timer_Pos_X - Digit * (Texture_Number_Width + Timer_Interval), Timer_Pos_Y, 0.0f);
		Timer[Digit].Number = 0;

		// 頂点情報の作成
		MakeTimerVertex(Digit, Timer[Digit].Number);
	}

	if (FirstInit == true)
	{
		TimerBox.pos = D3DXVECTOR3(TimerBox_Pos_X, TimerBox_Pos_Y, 0.0f);

		// 頂点情報の作成
		MakeTimerBoxVertex();

		// テクスチャの読み込み
		if (SafeLoad(Texture_TimerBox, &TimerBox_Texture, "Timer") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	// テクスチャの開放
	SafeRelease(TimerBox_Texture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	int Digit = 0;
	int Timertemp = 0;
	int GameStage = GetGameStage();
	static int Count = 0;

	if (TimerUse == true)
	{
		// カウントダウン開始
		if (CountDown == true)
		{
			SetTimerCountDown();
		}

		Count++;

		// 一秒経過
		if (Count != 0 && Count % 60 == 0)
		{
			TimerNumber--;
			if (TimerNumber <= 0)
			{
				TimerNumber = 0;
			}
		}

		// カウントダウン終了
		if (CountDown == true && TimerNumber <= 0)
		{
			if (StartCDOver == false)
			{
				// ゲームスタートのカウントダウン終了
				StartCDOver = true;
				CountDown = false;
				// タイマーの時間設置
				if (GameStage == Stage_Oushi)
				{

					TimerNumber = OnePlayTime_Oushi;
				}
				else if (GameStage == Stage_Futago)
				{

					TimerNumber = OnePlayTime_Futago;
				}

				// 数字の座標をリセット
				for (Digit = 0; Digit < TimerDigit; Digit++)
				{
					Timer[Digit].pos = D3DXVECTOR3((float)Timer_Pos_X - Digit * (Texture_Number_Width + Timer_Interval), Timer_Pos_Y, 0.0f);
					Timer[Digit].Number = 0;

					// 頂点情報の作成
					MakeTimerVertex(Digit, Timer[Digit].Number);
				}
			}
			else if (EndCDOver == false)
			{
				// ゲーム終了のカウントダウン終了
				EndCDOver = true;
			}
		}

		// ゲーム終了のカウントダウン開始
		if (EndCDOver == false && TimerNumber < 10)
		{
			CountDown = true;
		}

		// 数字計算
		Timertemp = TimerNumber;
		for (Digit = 0; Digit < TimerDigit; Digit++)
		{
			Timer[Digit].Number = Timertemp % 10;
			Timertemp /= 10;

			// テクスチャ座標を設定
			SetTimerTexture(Digit, Timer[Digit].Number);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	int Digit = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 NumberTex = GetNumberTex();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (TimerUse == true)
	{
		// 最後10秒以外は普通で表示
		if (CountDown == false)
		{
			// テクスチャの設定
			Device->SetTexture(0, TimerBox_Texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TimerBox.vertexwk, sizeof(VERTEX_2D));

			// テクスチャの設定
			Device->SetTexture(0, NumberTex);

			// ポリゴンの描画
			for (Digit = 0; Digit < TimerDigit; Digit++)
			{
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Timer[Digit].vertexwk, sizeof(VERTEX_2D));
			}
		}
		// 最後10秒の表示
		else
		{
			// テクスチャの設定
			Device->SetTexture(0, NumberTex);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Timer[0].vertexwk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeTimerVertex(int Digit, int TimerNum)
{
	// 頂点座標の設定	
	SetTimerVertex(Digit);

	// rhwの設定
	Timer[Digit].vertexwk[0].rhw = 1.0f;
	Timer[Digit].vertexwk[1].rhw = 1.0f;
	Timer[Digit].vertexwk[2].rhw = 1.0f;
	Timer[Digit].vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Timer[Digit].vertexwk[0].diffuse = D3DCOLOR_RGBA(0, 0, 255, 255);
	Timer[Digit].vertexwk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	Timer[Digit].vertexwk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	Timer[Digit].vertexwk[3].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);

	// テクスチャ座標の設定
	SetTimerTexture(Digit, TimerNum);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTimerTexture(int Digit, int TimerNum)
{
	Timer[Digit].vertexwk[0].tex = D3DXVECTOR2(TimerNum * 0.1f, 0.0f);
	Timer[Digit].vertexwk[1].tex = D3DXVECTOR2((TimerNum + 1) * 0.1f, 0.0f);
	Timer[Digit].vertexwk[2].tex = D3DXVECTOR2(TimerNum * 0.1f, 1.0f);
	Timer[Digit].vertexwk[3].tex = D3DXVECTOR2((TimerNum + 1) * 0.1f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTimerVertex(int Digit)
{
	Timer[Digit].vertexwk[0].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x, Timer[Digit].pos.y, 0.0f);
	Timer[Digit].vertexwk[1].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x + Texture_Number_Width, Timer[Digit].pos.y, 0.0f);
	Timer[Digit].vertexwk[2].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x, Timer[Digit].pos.y + Texture_Number_Height, 0.0f);
	Timer[Digit].vertexwk[3].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x + Texture_Number_Width, Timer[Digit].pos.y + Texture_Number_Height, 0.0f);

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeTimerBoxVertex(void)
{
	// 頂点座標の設定	
	SetTimerBoxVertex();

	// rhwの設定
	TimerBox.vertexwk[0].rhw = 1.0f;
	TimerBox.vertexwk[1].rhw = 1.0f;
	TimerBox.vertexwk[2].rhw = 1.0f;
	TimerBox.vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	TimerBox.vertexwk[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	TimerBox.vertexwk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	TimerBox.vertexwk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	TimerBox.vertexwk[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

	// テクスチャ座標の設定
	SetTimerBoxTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTimerBoxTexture(void)
{
	TimerBox.vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TimerBox.vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TimerBox.vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TimerBox.vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTimerBoxVertex(void)
{
	TimerBox.vertexwk[0].vtx = D3DXVECTOR3(TimerBox.pos.x, TimerBox.pos.y, 0.0f);
	TimerBox.vertexwk[1].vtx = D3DXVECTOR3(TimerBox.pos.x + Texture_TimerBox_Width, TimerBox.pos.y, 0.0f);
	TimerBox.vertexwk[2].vtx = D3DXVECTOR3(TimerBox.pos.x, TimerBox.pos.y + Texture_TimerBox_Height, 0.0f);
	TimerBox.vertexwk[3].vtx = D3DXVECTOR3(TimerBox.pos.x + Texture_TimerBox_Width, TimerBox.pos.y + Texture_TimerBox_Height, 0.0f);

	return;
}

//=============================================================================
// タイマーの使用を設置
//=============================================================================
void SetTimerUse(bool Flag)
{
	TimerUse = Flag;
}

//=============================================================================
// タイマーのカウントダウンを設置する
//=============================================================================
void SetTimerCountDown(void)
{
	// 頂点座標の設定
	Timer[0].vertexwk[0].vtx = D3DXVECTOR3(CountDown_Pos_X, CountDown_Pos_Y, 0.0f);
	Timer[0].vertexwk[1].vtx = D3DXVECTOR3(CountDown_Pos_X + Texture_Number_Width + NumZoom, CountDown_Pos_Y, 0.0f);
	Timer[0].vertexwk[2].vtx = D3DXVECTOR3(CountDown_Pos_X, CountDown_Pos_Y + Texture_Number_Height + NumZoom, 0.0f);
	Timer[0].vertexwk[3].vtx = D3DXVECTOR3(CountDown_Pos_X + Texture_Number_Width + NumZoom, CountDown_Pos_Y + Texture_Number_Height + NumZoom, 0.0f);
}

//=============================================================================
// ゲームスタートのカウントダウンフラグを取得
//=============================================================================
bool GetStartCDOver(void)
{
	return StartCDOver;
}

//=============================================================================
// ゲーム終了のカウントダウンフラグを取得
//=============================================================================
bool GetEndCDOver(void)
{
	return EndCDOver;
}
