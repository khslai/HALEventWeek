//=============================================================================
//
// Main処理 [main.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "resource.h"
#include "DebugProcess.h"
#include "Input.h"
#include "BackGround.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "Effect.h"
#include "GameUI.h"
#include "CheckHit.h"
#include "AttackRange.h"
#include "Apple.h"
#include "Wolf.h"
#include "Score.h"
#include "Bull.h"
#include "Timer.h"
#include "Block.h"
#include "Door.h"
#include "GameClear.h"
#include "MainMenu.h"
#include "Transition.h"
#include "Title.h"
#include "Pause.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")			// ウインドウのクラス名
#define WINDOW_NAME			_T("ふたりのSTARGAZING")	// ウインドウのキャプション名
#define FPS					(60)


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;							// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;					// Deviceオブジェクト(描画に必要)
int						GameStage = Stage_Title;				// ゲームステージ
int						GameCount = 0;							// ゲームカウント
int						FPSCount = 0;							// FPSカウント
#if _DEBUG
bool					DisplayDebugMsg = true;					// デバッグ表示ON/OFF
#else
bool					DisplayDebugMsg = false;
#endif


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// ゲームアイコン
	HICON MyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	// 時間計測用
	DWORD dwExecLastTime;	// 前回画面処理の時間
	DWORD dwFPSLastTime;	// 前回FPG計算の時間
	DWORD dwCurrentTime;	// 現在時間
	DWORD dwFrameCount;		// 一秒間何回処理する = FPS

	// タスクバーを除いた領域のサイズを取得
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		MyIcon,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		MyIcon
	};
	HWND		hWnd;
	MSG			msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rt.right,
		rt.bottom,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (hWnd == NULL)
	{
		return false;
	}

	// DirectXの初期化(ウィンドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		// ゲーム終了
		if (GetGameExit() == true)
		{
			break;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// システム時刻を取得
			dwCurrentTime = timeGetTime();

			// 1秒ごとに実行
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{

#ifdef _DEBUG
				// FPS計算
				//if (GameStage == Stage_Game)
				//{
				//	FPSCount = dwFrameCount;
				//}
#endif
				// FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;
				// カウントをクリア
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))	// 1/60秒ごとに実行
			{
				// 処理した時刻を保存
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				// 処理回数のカウントを加算
				dwFrameCount++;
			}
		}
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
			// [ESC]キーが押された
		case VK_ESCAPE:
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = Screen_Width;			// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = Screen_Height;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// バックバッファのフォーマットは現在設定されているものを使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;			// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// デプスバッファとして16bitを使う
	d3dpp.BackBufferFormat = d3ddm.Format;			// カラーモードの指定

	if (bWindow)
	{
		// ウィンドウモード
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;							// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// インターバル
	}
	else
	{
		// フルスクリーンモード
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;							// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		// ディスプレイアダプタ
		D3DDEVTYPE_HAL,										// ディスプレイタイプ
		hWnd,												// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,				// デバイス作成制御の組み合わせ
		&d3dpp,												// デバイスのプレゼンテーションパラメータ
		&g_pD3DDevice)))									// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// ランダムの初期化
	srand((unsigned)time(NULL));

#ifdef _DEBUG
	// デバッグテストの初期化処理
	if (FAILED(InitDebugProcess(true)))
	{
		MessageBox(0, "DebugProcess Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// 当たり範囲表示の初期化処理
	if (FAILED(InitAttackRange(true)))
	{
		MessageBox(0, "AttackRange Initialize Failed！", "Error", 0);
		return E_FAIL;
	}
#endif

	// 入力処理の初期化
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(0, "DirectInput Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// サウンドの初期化
	if (FAILED(InitSound(hWnd)))
	{
		MessageBox(0, "Sound Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// タイトルの初期化処理
	if (FAILED(InitTitle(true)))
	{
		MessageBox(0, "Title Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// メインメニューの初期化処理
	if (FAILED(InitMainMenu(true)))
	{
		MessageBox(0, "MainMenu Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// 一時停止画面の初期化処理
	if (FAILED(InitPause(true)))
	{
		MessageBox(0, "Pause Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// ステージ遷移画面の初期化
	if (FAILED(InitTransition(true)))
	{
		MessageBox(0, "Transition Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// 背景の初期化処理
	if (FAILED(InitBackGround(true)))
	{
		MessageBox(0, "BackGround Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// ゲームクリアの初期化処理
	if (FAILED(InitGameClear(true)))
	{
		MessageBox(0, "GameClear Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// 障害物の初期化処理
	if (FAILED(InitBlock(true)))
	{
		MessageBox(0, "Block Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// 扉の初期化処理
	if (FAILED(InitDoor(true)))
	{
		MessageBox(0, "Door Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// ゲームUIの初期化処理
	if (FAILED(InitGameUI(true)))
	{
		MessageBox(0, "GameUI Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// スコアの初期化処理
	if (FAILED(InitScore(true)))
	{
		MessageBox(0, "Score Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// タイマーの初期化処理
	if (FAILED(InitTimer(true)))
	{
		MessageBox(0, "Timer Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// プレイヤーの初期化処理
	if (FAILED(InitPlayer(true)))
	{
		MessageBox(0, "Player Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// プレイヤーの弾の初期化処理
	if (FAILED(InitPlayerBullet(true)))
	{
		MessageBox(0, "PlayerBullet Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// アップルの初期化処理
	if (FAILED(InitApple(true)))
	{
		MessageBox(0, "Apple Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// 狼の初期化処理
	if (FAILED(InitWolf(true)))
	{
		MessageBox(0, "Wolf Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// 牡牛の初期化処理
	if (FAILED(InitBull(true)))
	{
		MessageBox(0, "Bull Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// ボスの初期化処理
	if (FAILED(InitBoss(true)))
	{
		MessageBox(0, "Boss Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// ボスの弾幕の初期化処理
	if (FAILED(InitBossBarrage(true)))
	{
		MessageBox(0, "BossBarrage Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	// エフェクトの初期化処理
	if (FAILED(InitEffect(true)))
	{
		MessageBox(0, "Effect Initialize Failed！", "Error", 0);
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	int i = 0;

#ifdef _DEBUG
	// デバッグテストの終了処理
	UninitDebugProcess();

	// 当たり範囲表示の終了処理
	UninitAttackRange();
#endif

	// 入力処理の終了処理
	UninitInput();

	// サウンドの終了処理
	UninitSound();

	// タイトルの終了処理
	UninitTitle();

	// メインメニューの終了処理
	UninitMainMenu();

	// 一時停止画面の終了処理
	UninitPause();

	// ステージ遷移画面の終了処理
	UninitTransition();

	// 背景の終了処理
	UninitBackGround();

	// ゲームクリアの終了処理
	UninitGameClear();

	// 障害物の終了処理
	UninitBlock();

	// 扉の終了処理
	UninitDoor();

	// ゲームUIの終了処理
	UninitGameUI();

	// スコアの終了処理
	UninitScore();

	// タイマーの終了処理
	UninitTimer();

	// プレイヤーの終了処理
	UninitPlayer();

	// プレイヤーの弾の終了処理
	UninitPlayerBullet();

	// アップルの終了処理
	UninitApple();

	// 狼の終了処理
	UninitWolf();

	// 牡牛の終了処理
	UninitBull();

	// ボスの終了処理
	UninitBoss();

	// ボスの弾幕の終了処理
	UninitBossBarrage();

	// エフェクトの終了処理
	UninitEffect();

	// デバイスの開放
	SafeRelease(g_pD3DDevice);

	// Direct3Dオブジェクトの開放
	SafeRelease(g_pD3D);
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	int Transition = GetTransition();

#if _DEBUG
	// デバッグ表示ON/OFF
	if (GetKeyboardTrigger(DIK_F1))
	{
		DisplayDebugMsg = DisplayDebugMsg ? false : true;
	}
#endif

	// 入力の更新処理
	UpdateInput();

	// サウンドの更新処理
	UpdateSound();

	switch (GameStage)
	{
	case Stage_Title:

		// タイトルの更新処理
		UpdateTitle();
		break;

	case Stage_MainMenu:

		// メインメニューの更新処理
		UpdateMainMenu();
		break;

	case Stage_Futago:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// 背景の更新処理
			UpdateBackGround();

			// プレイヤーの更新処理
			UpdatePlayer();

			// 障害物の更新処理
			UpdateBlock();

			// 扉の更新処理
			UpdateDoor();

			// タイマーの更新処理
			UpdateTimer();

			// 当たり判定処理
			CheckHit();
		}

		// 一時停止画面の更新処理
		UpdatePause();

		// ゲームクリアの更新処理
		UpdateGameClear();
		break;

	case Stage_Oushi:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// 背景の更新処理
			UpdateBackGround();

			// プレイヤーの更新処理
			UpdatePlayer();

			if (GetStartCDOver() == true)
			{
				// 牡牛の更新処理
				UpdateBull();
			}

			// ゲームUIの更新処理
			UpdateGameUI();

			// タイマーの更新処理
			UpdateTimer();

			// 当たり判定処理
			CheckHit();
		}

		// 一時停止画面の更新処理
		UpdatePause();

		// ゲームクリアの更新処理
		UpdateGameClear();

		break;

	case Stage_Ite:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// 背景の更新処理
			UpdateBackGround();

			// プレイヤーの更新処理
			UpdatePlayer();

			// プレイヤーの弾の更新処理
			UpdatePlayerBullet();

			// アップルの更新処理
			UpdateApple();

			// 狼の更新処理
			UpdateWolf();

			// エフェクトの更新処理
			UpdateEffect();

			// ゲームUIの更新処理
			UpdateGameUI();

			// 当たり判定処理
			CheckHit();

			// スコアの更新処理
			UpdateScore();
		}

		// 一時停止画面の更新処理
		UpdatePause();

		// ゲームクリアの更新処理
		UpdateGameClear();

		break;

	case Stage_Shishi:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// 背景の更新処理
			UpdateBackGround();

			// プレイヤーの更新処理
			UpdatePlayer();

			// プレイヤーの弾の更新処理
			UpdatePlayerBullet();

			// ボスの更新処理
			UpdateBoss();

			// ボスの弾幕の更新処理
			UpdateBossBarrage();

			// エフェクトの更新処理
			UpdateEffect();

			// ゲームUIの更新処理
			UpdateGameUI();

#if _DEBUG
			// 当たり範囲表示の更新処理
			//UpdateAttackRange();
#endif

				// 当たり判定処理
			CheckHit();
		}

		// 一時停止画面の更新処理
		UpdatePause();

		// ゲームクリアの更新処理
		UpdateGameClear();

		break;

	case Stage_GameEnd:

		// ゲームクリアの更新処理
		UpdateGameClear();
		break;

	default:
		break;
	}

	// ステージ遷移画面の更新処理
	UpdateTransition();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (GameStage)
		{
		case Stage_Title:

			// タイトルの描画処理
			DrawTitle();
			break;

		case Stage_MainMenu:

			// メインメニューの描画処理
			DrawMainMenu();
			break;

		case Stage_Futago:

			// 背景の描画処理
			DrawBackGround();

			// 障害物の描画処理
			DrawBlock();

			// 扉の描画処理
			DrawDoor();

			// プレイヤーの描画処理
			DrawPlayer();

			// タイマーの描画処理
			DrawTimer();

			// 一時停止画面の描画処理
			DrawPause();

			// ゲームクリアの描画処理
			DrawGameClear();
			break;

		case Stage_Oushi:

			// 背景の描画処理
			DrawBackGround();

			// プレイヤーの描画処理
			DrawPlayer();

			// 牡牛の更新処理
			DrawBull();

			// ゲームUIの描画処理
			DrawGameUI();

			// タイマーの描画処理
			DrawTimer();

			// 一時停止画面の描画処理
			DrawPause();

			// ゲームクリアの描画処理
			DrawGameClear();
			break;

		case Stage_Ite:

			// 背景の描画処理
			DrawBackGround();

			// プレイヤーの描画処理
			DrawPlayer();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// アップルの描画処理
			DrawApple();

			// 狼の描画処理
			DrawWolf();

			// エフェクトの描画処理
			DrawEffect();

			// ゲームUIの描画処理
			DrawGameUI();

			// スコアの描画処理
			DrawScore();

			// 一時停止画面の描画処理
			DrawPause();

			// ゲームクリアの描画処理
			DrawGameClear();
			break;

		case Stage_Shishi:

			// 背景の描画処理
			DrawBackGround();

			// プレイヤーの描画処理
			DrawPlayer();

			// プレイヤーの弾の描画処理
			DrawPlayerBullet();

			// ボスの描画処理
			DrawBoss();

			// ボスの弾幕の描画処理
			DrawBossBarrage();

			// エフェクトの描画処理
			DrawEffect();

			// ゲームUIの描画処理
			DrawGameUI();

#if _DEBUG
			// 当たり範囲表示の描画処理
			//DrawAttackRange();
#endif

			// 一時停止画面の描画処理
			DrawPause();

			// ゲームクリアの描画処理
			DrawGameClear();
			break;

		case Stage_GameEnd:

			// ゲームクリアの描画処理
			DrawGameClear();
			break;

		default:
			break;
		}

		// ステージ遷移画面の描画処理
		DrawTransition();

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

#if _DEBUG
	// デバッグ表示の描画
	if (DisplayDebugMsg)
	{
		DrawDebugProcess();
	}
#endif

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイス取得関数
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

//=============================================================================
// テクスチャ読み込む関数
//=============================================================================
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc)
{
	char Message[64];

	D3DXCreateTextureFromFile(g_pD3DDevice, SrcFile, TexturePtr);
	if (*TexturePtr == NULL)
	{
		sprintf(Message, "Load %s Texture Failed！", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return false;
	}

	return true;
}

//=============================================================================
// ゲームステージ取得関数
//=============================================================================
int GetGameStage(void)
{
	return GameStage;
}

//=============================================================================
// ゲームステージ設定関数（画面遷移）
//=============================================================================
void SetGameStage(int Stage)
{
	GameStage = Stage;

	return;
}

//=============================================================================
// 弾はスクリーンの範囲内かどうかをチェック
//=============================================================================
bool CheckPosition(D3DXVECTOR3 Pos, int Width, int Height)
{
	bool Result = false;

	if (Pos.x - Width / 2 >= Screen_Width ||
		Pos.x + Width / 2 <= 0 ||
		Pos.y - Height / 2 >= Screen_Height ||
		Pos.y + Height / 2 <= 0)
	{
		Result = true;
	}

	return Result;
}

//=============================================================================
// 二つの座標の移動角度を計算する
//=============================================================================
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos)
{
	return atan2f(ObjectPos.y - ReferencePos.y, ObjectPos.x - ReferencePos.x);
}

//=============================================================================
// ゲームの再初期化
//=============================================================================
void ReInitialize(void)
{
	int GameStage = GetGameStage();

	// カウントの再初期化
	GameCount = 0;
	FPSCount = 0;

	// サウンドの再初期化処理
	ReInitSound();

	switch (GameStage)
	{
	case Stage_Title:

		// メインメニューの再初期化
		InitMainMenu(false);
		break;

	case Stage_Futago:

		// 背景の再初期化
		InitBackGround(false);

		// 一時停止画面の再初期化
		InitPause(false);

		// ゲームクリアの再初期化
		InitGameClear(false);

		// 障害物の再初期化
		InitBlock(false);

		// 扉の再初期化
		InitDoor(false);

		// タイマーの再初期化
		InitTimer(false);

		// プレイヤーの再初期化
		InitPlayer(false);
		break;

	case Stage_Oushi:

		// 背景の再初期化
		InitBackGround(false);

		// 一時停止画面の再初期化
		InitPause(false);

		// ゲームクリアの再初期化
		InitGameClear(false);

		// タイマーの再初期化
		InitTimer(false);

		// プレイヤーの再初期化
		InitPlayer(false);

		// 牡牛の再初期化
		InitBull(false);

		// ゲームUIの再初期化
		InitGameUI(false);
		break;

	case Stage_Ite:

		// 背景の再初期化
		InitBackGround(false);

		// 一時停止画面の再初期化
		InitPause(false);

		// ゲームクリアの再初期化
		InitGameClear(false);

		// プレイヤーの再初期化
		InitPlayer(false);

		// ゲームUIの再初期化
		InitGameUI(false);

		// スコアの再初期化
		InitScore(false);

		// プレイヤーの弾の再初期化
		InitPlayerBullet(false);

		// アップルの再初期化
		InitApple(false);

		// 狼の再初期化
		InitWolf(false);

		// エフェクトの再初期化
		InitEffect(false);
		break;

	case Stage_Shishi:

		// 背景の再初期化
		InitBackGround(false);

		// 一時停止画面の再初期化
		InitPause(false);

		// ゲームクリアの再初期化
		InitGameClear(false);

		// プレイヤーの再初期化
		InitPlayer(false);

		// プレイヤーの弾の再初期化
		InitPlayerBullet(false);

		// ボスの再初期化
		InitBoss(false);

		// ボスの弾幕の再初期化
		InitBossBarrage(false);

		// ゲームUIの再初期化
		InitGameUI(false);

		// エフェクトの再初期化
		InitEffect(false);
		break;

	default:
		break;
	}

#if _DEBUG
	InitDebugProcess(false);
#endif

	return;
}

