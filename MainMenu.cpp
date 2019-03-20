//=============================================================================
//
// メインメニュー処理 [MainMenu.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "MainMenu.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_MainMenu				_T("data/Texture/BackGround/BackGround_MainMenu.png")
#define Texture_StageImgBox_Ite			_T("data/Texture/UI/StageImgBox_Ite.png")
#define Texture_StageImgBox_Futago		_T("data/Texture/UI/StageImgBox_Futago.png")
#define Texture_StageImgBox_Oushi		_T("data/Texture/UI/StageImgBox_Oushi.png")
#define Texture_StageImgBox_Shishi		_T("data/Texture/UI/StageImgBox_Shishi.png")
#define Texture_StageImg_Ite			_T("data/Texture/UI/StageImg_Ite.png")
#define Texture_StageImg_Futago			_T("data/Texture/UI/StageImg_Futago.png")
#define Texture_StageImg_Oushi			_T("data/Texture/UI/StageImg_Oushi.png")
#define Texture_StageImg_Shishi			_T("data/Texture/UI/StageImg_Shishi.png")
#define Texture_StageTitleImg_Ite		_T("data/Texture/UI/StageTitleImg_Ite.png")
#define Texture_StageTitleImg_Futago	_T("data/Texture/UI/StageTitleImg_Futago.png")
#define Texture_StageTitleImg_Oushi		_T("data/Texture/UI/StageTitleImg_Oushi.png")
#define Texture_StageTitleImg_Shishi	_T("data/Texture/UI/StageTitleImg_Shishi.png")
#define Texture_Select					_T("data/Texture/UI/Select.png")
#define Texture_BlackScreen				_T("data/Texture/UI/BlackScreen.png")
#define Texture_Question				_T("data/Texture/UI/Question.png")
#define Texture_Option_Yes				_T("data/Texture/UI/Option_Yes.png")
#define Texture_Option_No				_T("data/Texture/UI/Option_No.png")

// ステージスクリーンショット
#define StageImg_Width				(300)
#define StageImg_Height				(300)
#define StageImg_Pos_Ite			(D3DXVECTOR3(280.0f,600.0f,0.0f))
#define StageImg_Pos_Futago			(D3DXVECTOR3(780.0f,600.0f,0.0f))
#define StageImg_Pos_Oushi			(D3DXVECTOR3(1280.0f,600.0f,0.0f))
#define StageImg_Pos_Shishi			(D3DXVECTOR3(780.0f,100.0f,0.0f))

// ステージ説明
#define StageTitleImg_Width			(280)
#define StageTitleImg_Height		(90)
#define StageTitleImg_Pos_Ite		(D3DXVECTOR3(290.0f,710.0f,0.0f))
#define StageTitleImg_Pos_Futago	(D3DXVECTOR3(790.0f,710.0f,0.0f))
#define StageTitleImg_Pos_Oushi		(D3DXVECTOR3(1290.0f,710.0f,0.0f))
#define StageTitleImg_Pos_Shishi	(D3DXVECTOR3(790.0f,210.0f,0.0f))

// 選択肢画像
#define Select_Width				(500)
#define Select_Height				(500)
#define Select_Pos_Ite				(D3DXVECTOR3(180.0f,500.0f,0.0f))
#define Select_Pos_Futago			(D3DXVECTOR3(680.0f,500.0f,0.0f))
#define Select_Pos_Oushi			(D3DXVECTOR3(1180.0f,500.0f,0.0f))
#define Select_Pos_Shishi			(D3DXVECTOR3(680.0f,0.0f,0.0f))

// 文字
// 問題
#define Question_Width				(480)
#define Question_Height				(160)
#define Question_Pos				(D3DXVECTOR3(700.0f,200.0f,0.0f))

// オプション
#define Option_Width				(480)
#define Option_Height				(80)
// いどむ
#define Option_Yes_Pos				(D3DXVECTOR3(700.0f,600.0f,0.0f))
// やめとく
#define Option_No_Pos				(D3DXVECTOR3(700.0f,800.0f,0.0f))

// 背景スクロール速度
#define BGScrollSpeed				(0.0002f)

enum MainMenuImgType
{
	Img_MainMenuBG,
	Img_Stage_Ite,
	Img_StageBox_Ite,
	Img_StageTitle_Ite,
	Img_Stage_Futago,
	Img_StageBox_Futago,
	Img_StageTitle_Futago,
	Img_Stage_Oushi,
	Img_StageBox_Oushi,
	Img_StageTitle_Oushi,
	Img_Stage_Shishi,
	Img_StageBox_Shishi,
	Img_StageTitle_Shishi,
	Img_Select,
	Img_BlackScreen,
	Img_Question,
	Img_Option_Yes,
	Img_Option_No,
	MainMenuImg_Max,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeMainMenuVertex(IMAGE *MainMenuImg);
// 頂点座標の設定
void SetMainMenuTexture(IMAGE *MainMenuImg);
// テクスチャ座標の設定
void SetMainMenuVertex(IMAGE *MainMenuImg);
// 色、透明度の設定
void SetMainMenuColor(IMAGE *MainMenuImg);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 MainMenu_Texture[MainMenuImg_Max] = { NULL };
// メインメニュー画像構造体
IMAGE MainMenuImg[MainMenuImg_Max];
// 選択肢構造体
SELECT MainMenuSelect;
// 選択したステージ
int SelectStage = 0;
// ステージクリアフラグ
bool StageClear[ClearFlag_Max] = { false };

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMainMenu(bool FirstInit)
{
	int Image_No = 0;

	// 選択肢初期化
	MainMenuSelect.pos = D3DXVECTOR2(0.0f, 0.0f);
	MainMenuSelect.State = IteStage;
	MainMenuSelect.PreState = IteStage;
	MainMenuSelect.InYes = false;
	MainMenuImg[Img_Select].pos = Select_Pos_Ite;

	// ステージ状態、フラグ初期化
	SelectStage = 0;

	for (int i = 0; i < ClearFlag_Max; i++)
	{
		StageClear[i] = false;
	}

	// UI表示初期化
	for (Image_No = 0; Image_No < MainMenuImg_Max; Image_No++)
	{
		MainMenuImg[Image_No].Use = true;
	}
	MainMenuImg[Img_BlackScreen].Use = false;
	MainMenuImg[Img_Question].Use = false;
	MainMenuImg[Img_Option_Yes].Use = false;
	MainMenuImg[Img_Option_No].Use = false;

	if (FirstInit == true)
	{
		// 背景
		MainMenuImg[Img_MainMenuBG].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		MainMenuImg[Img_MainMenuBG].Width = Screen_Width;
		MainMenuImg[Img_MainMenuBG].Height = Screen_Height;
		MainMenuImg[Img_MainMenuBG].Type = Img_MainMenuBG;

		// 射手座
		MainMenuImg[Img_Stage_Ite].pos = StageImg_Pos_Ite;
		MainMenuImg[Img_Stage_Ite].Width = StageImg_Width;
		MainMenuImg[Img_Stage_Ite].Height = StageImg_Height;
		MainMenuImg[Img_Stage_Ite].Type = Img_Stage_Ite;

		MainMenuImg[Img_StageBox_Ite].pos = StageImg_Pos_Ite;
		MainMenuImg[Img_StageBox_Ite].Width = StageImg_Width;
		MainMenuImg[Img_StageBox_Ite].Height = StageImg_Height;
		MainMenuImg[Img_StageBox_Ite].Type = Img_StageBox_Ite;

		MainMenuImg[Img_StageTitle_Ite].pos = StageTitleImg_Pos_Ite;
		MainMenuImg[Img_StageTitle_Ite].Width = StageTitleImg_Width;
		MainMenuImg[Img_StageTitle_Ite].Height = StageTitleImg_Height;
		MainMenuImg[Img_StageTitle_Ite].Type = Img_StageTitle_Ite;

		// 双子座
		MainMenuImg[Img_Stage_Futago].pos = StageImg_Pos_Futago;
		MainMenuImg[Img_Stage_Futago].Width = StageImg_Width;
		MainMenuImg[Img_Stage_Futago].Height = StageImg_Height;
		MainMenuImg[Img_Stage_Futago].Type = Img_Stage_Futago;

		MainMenuImg[Img_StageBox_Futago].pos = StageImg_Pos_Futago;
		MainMenuImg[Img_StageBox_Futago].Width = StageImg_Width;
		MainMenuImg[Img_StageBox_Futago].Height = StageImg_Height;
		MainMenuImg[Img_StageBox_Futago].Type = Img_StageBox_Futago;

		MainMenuImg[Img_StageTitle_Futago].pos = StageTitleImg_Pos_Futago;
		MainMenuImg[Img_StageTitle_Futago].Width = StageTitleImg_Width;
		MainMenuImg[Img_StageTitle_Futago].Height = StageTitleImg_Height;
		MainMenuImg[Img_StageTitle_Futago].Type = Img_StageTitle_Futago;

		// 牡牛座
		MainMenuImg[Img_Stage_Oushi].pos = StageImg_Pos_Oushi;
		MainMenuImg[Img_Stage_Oushi].Width = StageImg_Width;
		MainMenuImg[Img_Stage_Oushi].Height = StageImg_Height;
		MainMenuImg[Img_Stage_Oushi].Type = Img_Stage_Oushi;

		MainMenuImg[Img_StageBox_Oushi].pos = StageImg_Pos_Oushi;
		MainMenuImg[Img_StageBox_Oushi].Width = StageImg_Width;
		MainMenuImg[Img_StageBox_Oushi].Height = StageImg_Height;
		MainMenuImg[Img_StageBox_Oushi].Type = Img_StageBox_Oushi;

		MainMenuImg[Img_StageTitle_Oushi].pos = StageTitleImg_Pos_Oushi;
		MainMenuImg[Img_StageTitle_Oushi].Width = StageTitleImg_Width;
		MainMenuImg[Img_StageTitle_Oushi].Height = StageTitleImg_Height;
		MainMenuImg[Img_StageTitle_Oushi].Type = Img_StageTitle_Oushi;

		// 獅子座
		MainMenuImg[Img_Stage_Shishi].pos = StageImg_Pos_Shishi;
		MainMenuImg[Img_Stage_Shishi].Width = StageImg_Width;
		MainMenuImg[Img_Stage_Shishi].Height = StageImg_Height;
		MainMenuImg[Img_Stage_Shishi].Type = Img_Stage_Shishi;

		MainMenuImg[Img_StageBox_Shishi].pos = StageImg_Pos_Shishi;
		MainMenuImg[Img_StageBox_Shishi].Width = StageImg_Width;
		MainMenuImg[Img_StageBox_Shishi].Height = StageImg_Height;
		MainMenuImg[Img_StageBox_Shishi].Type = Img_StageBox_Shishi;

		MainMenuImg[Img_StageTitle_Shishi].pos = StageTitleImg_Pos_Shishi;
		MainMenuImg[Img_StageTitle_Shishi].Width = StageTitleImg_Width;
		MainMenuImg[Img_StageTitle_Shishi].Height = StageTitleImg_Height;
		MainMenuImg[Img_StageTitle_Shishi].Type = Img_StageTitle_Shishi;

		// 選択肢
		MainMenuImg[Img_Select].pos = Select_Pos_Ite;
		MainMenuImg[Img_Select].Width = Select_Width;
		MainMenuImg[Img_Select].Height = Select_Height;
		MainMenuImg[Img_Select].Type = Img_Select;

		// 黒い画面
		MainMenuImg[Img_BlackScreen].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
		MainMenuImg[Img_BlackScreen].Width = Screen_Width;
		MainMenuImg[Img_BlackScreen].Height = Screen_Height;
		MainMenuImg[Img_BlackScreen].Type = Img_BlackScreen;

		// 文字
		MainMenuImg[Img_Question].pos = Question_Pos;
		MainMenuImg[Img_Question].Width = Question_Width;
		MainMenuImg[Img_Question].Height = Question_Height;
		MainMenuImg[Img_Question].Type = Img_Question;

		MainMenuImg[Img_Option_Yes].pos = Option_Yes_Pos;
		MainMenuImg[Img_Option_Yes].Width = Option_Width;
		MainMenuImg[Img_Option_Yes].Height = Option_Height;
		MainMenuImg[Img_Option_Yes].Type = Img_Option_Yes;

		MainMenuImg[Img_Option_No].pos = Option_No_Pos;
		MainMenuImg[Img_Option_No].Width = Option_Width;
		MainMenuImg[Img_Option_No].Height = Option_Height;
		MainMenuImg[Img_Option_No].Type = Img_Option_No;

		// 頂点情報の作成
		for (Image_No = 0; Image_No < MainMenuImg_Max; Image_No++)
		{
			MakeMainMenuVertex(&MainMenuImg[Image_No]);
		}

		// テクスチャの読み込み
		if (SafeLoad(Texture_MainMenu, &MainMenuImg[Img_MainMenuBG].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}

		if (SafeLoad(Texture_StageImg_Ite, &MainMenuImg[Img_Stage_Ite].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageImgBox_Ite, &MainMenuImg[Img_StageBox_Ite].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageTitleImg_Ite, &MainMenuImg[Img_StageTitle_Ite].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}

		if (SafeLoad(Texture_StageImg_Futago, &MainMenuImg[Img_Stage_Futago].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageImgBox_Futago, &MainMenuImg[Img_StageBox_Futago].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageTitleImg_Futago, &MainMenuImg[Img_StageTitle_Futago].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}

		if (SafeLoad(Texture_StageImg_Oushi, &MainMenuImg[Img_Stage_Oushi].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageImgBox_Oushi, &MainMenuImg[Img_StageBox_Oushi].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageTitleImg_Oushi, &MainMenuImg[Img_StageTitle_Oushi].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}

		if (SafeLoad(Texture_StageImg_Shishi, &MainMenuImg[Img_Stage_Shishi].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageImgBox_Shishi, &MainMenuImg[Img_StageBox_Shishi].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageTitleImg_Shishi, &MainMenuImg[Img_StageTitle_Shishi].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}

		if (SafeLoad(Texture_Select, &MainMenuImg[Img_Select].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}

		if (SafeLoad(Texture_BlackScreen, &MainMenuImg[Img_BlackScreen].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Question, &MainMenuImg[Img_Question].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Option_Yes, &MainMenuImg[Img_Option_Yes].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Option_No, &MainMenuImg[Img_Option_No].Texture, "MainMenu") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMainMenu(void)
{
	for (int Image_No = 0; Image_No < MainMenuImg_Max; Image_No++)
	{
		SafeRelease(MainMenuImg[Image_No].Texture);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMainMenu(void)
{
	static int PressCount = 0;

	// Enter
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
	{
		switch (MainMenuSelect.State)
		{
			// いて座に遷移
		case IteStage:

			if (StageClear[ClearFlag_Ite] == false)
			{
				SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
				SelectStage = Stage_Ite;
				MainMenuSelect.PreState = MainMenuSelect.State;
				MainMenuSelect.State = StageCheck;
			}
			else
			{
				SetSound(SE_Cancel, E_DS8_FLAG_NONE, true, false);
			}
			break;

			//ふたご座に遷移
		case FutagoStage:

			if (StageClear[ClearFlag_Futago] == false)
			{
				SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
				SelectStage = Stage_Futago;
				MainMenuSelect.PreState = MainMenuSelect.State;
				MainMenuSelect.State = StageCheck;
			}
			else
			{
				SetSound(SE_Cancel, E_DS8_FLAG_NONE, true, false);
			}
			break;

			//おうし座に遷移
		case OushiStage:

			if (StageClear[ClearFlag_Oushi] == false)
			{
				SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
				SelectStage = Stage_Oushi;
				MainMenuSelect.PreState = MainMenuSelect.State;
				MainMenuSelect.State = StageCheck;
			}
			else
			{
				SetSound(SE_Cancel, E_DS8_FLAG_NONE, true, false);
			}
			break;

			//しし座に遷移
		case ShishiStage:

			if (StageClear[ClearFlag_Ite] == true && StageClear[ClearFlag_Futago] == true &&
				StageClear[ClearFlag_Oushi] == true)
			{
				SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
				SelectStage = Stage_Shishi;
				MainMenuSelect.PreState = MainMenuSelect.State;
				MainMenuSelect.State = StageCheck;
			}
			else
			{
				SetSound(SE_Cancel, E_DS8_FLAG_NONE, true, false);
			}
			break;

			// 確認画面
		case StageCheck:

			if (MainMenuSelect.InYes == true)
			{
				SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
				MainMenuSelect.InYes = false;
				MainMenuSelect.State = MainMenuSelect.PreState;
				SetTransition(Fadein);
			}
			else
			{
				SetSound(SE_Cancel, E_DS8_FLAG_NONE, true, false);
				MainMenuSelect.State = MainMenuSelect.PreState;
			}
			break;

		default:
			break;
		}
	}

	// 選択肢移動
	// 左右
	if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
	{
		SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
		switch (MainMenuSelect.State)
		{
		case IteStage:
			MainMenuSelect.State = FutagoStage;
			break;
		case FutagoStage:
			MainMenuSelect.State = OushiStage;
			break;
		case OushiStage:
			MainMenuSelect.State = IteStage;
			break;
		default:
			break;
		}
	}
	else if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
	{
		SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
		switch (MainMenuSelect.State)
		{
		case IteStage:
			MainMenuSelect.State = OushiStage;
			break;
		case FutagoStage:
			MainMenuSelect.State = IteStage;
			break;
		case OushiStage:
			MainMenuSelect.State = FutagoStage;
			break;
		default:
			break;
		}
	}

	// 上下
	if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
		switch (MainMenuSelect.State)
		{
		case IteStage:
		case FutagoStage:
		case OushiStage:
			MainMenuSelect.PreState = MainMenuSelect.State;
			MainMenuSelect.State = ShishiStage;
			break;
		case StageCheck:
			if (MainMenuSelect.InYes == true)
			{
				MainMenuSelect.InYes = false;
			}
			else
			{
				MainMenuSelect.InYes = true;
			}
			break;
		default:
			break;
		}
	}
	else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
		switch (MainMenuSelect.State)
		{
		case ShishiStage:
			MainMenuSelect.State = MainMenuSelect.PreState;
			break;
		case StageCheck:
			if (MainMenuSelect.InYes == true)
			{
				MainMenuSelect.InYes = false;
			}
			else
			{
				MainMenuSelect.InYes = true;
			}
			break;
		default:
			break;
		}
	}

	// 選択肢ループ
	if (GetKeyboardRepeat(DIK_LEFT) || IsButtonRepeat(0, BUTTON_LEFT))
	{
		PressCount++;
		if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
		{
			SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
			switch (MainMenuSelect.State)
			{
			case IteStage:
				MainMenuSelect.State = OushiStage;
				break;
			case FutagoStage:
				MainMenuSelect.State = IteStage;
				break;
			case OushiStage:
				MainMenuSelect.State = FutagoStage;
				break;
			default:
				break;
			}
		}
	}
	else if (GetKeyboardRepeat(DIK_RIGHT) || IsButtonRepeat(0, BUTTON_RIGHT))
	{
		PressCount++;
		if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
		{
			SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
			switch (MainMenuSelect.State)
			{
			case IteStage:
				MainMenuSelect.State = FutagoStage;
				break;
			case FutagoStage:
				MainMenuSelect.State = OushiStage;
				break;
			case OushiStage:
				MainMenuSelect.State = IteStage;
				break;
			default:
				break;
			}
		}
	}

	// プレスカウント初期化
	if (GetKeyboardRelease(DIK_LEFT) || IsButtonReleased(0, BUTTON_LEFT) ||
		GetKeyboardRelease(DIK_RIGHT) || IsButtonReleased(0, BUTTON_RIGHT))
	{
		PressCount = 0;
	}

	// 確認画面表示
	if (MainMenuSelect.State == StageCheck)
	{
		MainMenuImg[Img_Question].Use = true;
		MainMenuImg[Img_Option_Yes].Use = true;
		MainMenuImg[Img_Option_No].Use = true;
		MainMenuImg[Img_BlackScreen].Use = true;
	}
	else
	{
		MainMenuImg[Img_Question].Use = false;
		MainMenuImg[Img_Option_Yes].Use = false;
		MainMenuImg[Img_Option_No].Use = false;
		MainMenuImg[Img_BlackScreen].Use = false;
	}

	// スターの位置を制御
	switch (MainMenuSelect.State)
	{
	case IteStage:
		MainMenuImg[Img_Select].pos = Select_Pos_Ite;
		break;
	case FutagoStage:
		MainMenuImg[Img_Select].pos = Select_Pos_Futago;
		break;
	case OushiStage:
		MainMenuImg[Img_Select].pos = Select_Pos_Oushi;
		break;
	case ShishiStage:
		MainMenuImg[Img_Select].pos = Select_Pos_Shishi;
		break;
	}

	// 背景スクロール
	MainMenuImg[Img_MainMenuBG].pos.x += BGScrollSpeed;

	for (int Image_No = 0; Image_No < MainMenuImg_Max; Image_No++)
	{
		if (MainMenuImg[Image_No].Use == true)
		{
			// 頂点座標の更新
			SetMainMenuVertex(&MainMenuImg[Image_No]);
			// テクスチャ座標の設定
			SetMainMenuTexture(&MainMenuImg[Image_No]);
			// 色、透明度の設定
			SetMainMenuColor(&MainMenuImg[Image_No]);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMainMenu(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int GameStage = GetGameStage();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (int Image_No = 0; Image_No < MainMenuImg_Max; Image_No++)
	{
		if (MainMenuImg[Image_No].Use == true)
		{
			// テクスチャの設定
			Device->SetTexture(0, MainMenuImg[Image_No].Texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, MainMenuImg[Image_No].vertexwk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeMainMenuVertex(IMAGE *MainMenuImg)
{
	// 頂点座標の設定	
	SetMainMenuVertex(MainMenuImg);

	// rhwの設定
	MainMenuImg->vertexwk[0].rhw = 1.0f;
	MainMenuImg->vertexwk[1].rhw = 1.0f;
	MainMenuImg->vertexwk[2].rhw = 1.0f;
	MainMenuImg->vertexwk[3].rhw = 1.0f;

	// 色、透明度の設定
	if (MainMenuImg->Type == Img_Stage_Shishi || MainMenuImg->Type == Img_StageBox_Shishi)
	{
		MainMenuImg->vertexwk[0].diffuse = GRAY(255);
		MainMenuImg->vertexwk[1].diffuse = GRAY(255);
		MainMenuImg->vertexwk[2].diffuse = GRAY(255);
		MainMenuImg->vertexwk[3].diffuse = GRAY(255);
	}
	else
	{
		MainMenuImg->vertexwk[0].diffuse = WHITE(255);
		MainMenuImg->vertexwk[1].diffuse = WHITE(255);
		MainMenuImg->vertexwk[2].diffuse = WHITE(255);
		MainMenuImg->vertexwk[3].diffuse = WHITE(255);
	}

	// テクスチャ座標の設定
	SetMainMenuTexture(MainMenuImg);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetMainMenuTexture(IMAGE *MainMenuImg)
{
	if (MainMenuImg->Type != Img_MainMenuBG)
	{
		MainMenuImg->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		MainMenuImg->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		MainMenuImg->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		MainMenuImg->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	else
	{
		MainMenuImg->vertexwk[0].tex = D3DXVECTOR2(MainMenuImg->pos.x, 0.0f);
		MainMenuImg->vertexwk[1].tex = D3DXVECTOR2(MainMenuImg->pos.x + 1.0f, 0.0f);
		MainMenuImg->vertexwk[2].tex = D3DXVECTOR2(MainMenuImg->pos.x, 1.0f);
		MainMenuImg->vertexwk[3].tex = D3DXVECTOR2(MainMenuImg->pos.x + 1.0f, 1.0f);
	}

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetMainMenuVertex(IMAGE *MainMenuImg)
{
	MainMenuImg->vertexwk[0].vtx = D3DXVECTOR3(MainMenuImg->pos.x, MainMenuImg->pos.y, 0.0f);
	MainMenuImg->vertexwk[1].vtx = D3DXVECTOR3(MainMenuImg->pos.x + MainMenuImg->Width, MainMenuImg->pos.y, 0.0f);
	MainMenuImg->vertexwk[2].vtx = D3DXVECTOR3(MainMenuImg->pos.x, MainMenuImg->pos.y + MainMenuImg->Height, 0.0f);
	MainMenuImg->vertexwk[3].vtx = D3DXVECTOR3(MainMenuImg->pos.x + MainMenuImg->Width, MainMenuImg->pos.y + MainMenuImg->Height, 0.0f);

	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetMainMenuColor(IMAGE *MainMenuImg)
{
	switch (MainMenuImg->Type)
	{
	case Img_Stage_Ite:
	case Img_StageBox_Ite:

		if (StageClear[ClearFlag_Ite] == true)
		{
			MainMenuImg->vertexwk[0].diffuse = GRAY(255);
			MainMenuImg->vertexwk[1].diffuse = GRAY(255);
			MainMenuImg->vertexwk[2].diffuse = GRAY(255);
			MainMenuImg->vertexwk[3].diffuse = GRAY(255);
		}
		else
		{
			MainMenuImg->vertexwk[0].diffuse = WHITE(255);
			MainMenuImg->vertexwk[1].diffuse = WHITE(255);
			MainMenuImg->vertexwk[2].diffuse = WHITE(255);
			MainMenuImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;

	case Img_Stage_Futago:
	case Img_StageBox_Futago:

		if (StageClear[ClearFlag_Futago] == true)
		{
			MainMenuImg->vertexwk[0].diffuse = GRAY(255);
			MainMenuImg->vertexwk[1].diffuse = GRAY(255);
			MainMenuImg->vertexwk[2].diffuse = GRAY(255);
			MainMenuImg->vertexwk[3].diffuse = GRAY(255);
		}
		else
		{
			MainMenuImg->vertexwk[0].diffuse = WHITE(255);
			MainMenuImg->vertexwk[1].diffuse = WHITE(255);
			MainMenuImg->vertexwk[2].diffuse = WHITE(255);
			MainMenuImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;

	case Img_Stage_Oushi:
	case Img_StageBox_Oushi:

		if (StageClear[ClearFlag_Oushi] == true)
		{
			MainMenuImg->vertexwk[0].diffuse = GRAY(255);
			MainMenuImg->vertexwk[1].diffuse = GRAY(255);
			MainMenuImg->vertexwk[2].diffuse = GRAY(255);
			MainMenuImg->vertexwk[3].diffuse = GRAY(255);
		}
		else
		{
			MainMenuImg->vertexwk[0].diffuse = WHITE(255);
			MainMenuImg->vertexwk[1].diffuse = WHITE(255);
			MainMenuImg->vertexwk[2].diffuse = WHITE(255);
			MainMenuImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;

	case Img_Stage_Shishi:
	case Img_StageBox_Shishi:

		if (StageClear[ClearFlag_Ite] == true && StageClear[ClearFlag_Futago] == true &&
			StageClear[ClearFlag_Oushi] == true)
		{
			MainMenuImg->vertexwk[0].diffuse = WHITE(255);
			MainMenuImg->vertexwk[1].diffuse = WHITE(255);
			MainMenuImg->vertexwk[2].diffuse = WHITE(255);
			MainMenuImg->vertexwk[3].diffuse = WHITE(255);
		}
		else
		{
			MainMenuImg->vertexwk[0].diffuse = GRAY(255);
			MainMenuImg->vertexwk[1].diffuse = GRAY(255);
			MainMenuImg->vertexwk[2].diffuse = GRAY(255);
			MainMenuImg->vertexwk[3].diffuse = GRAY(255);
		}
		break;

	case Img_BlackScreen:

		MainMenuImg->vertexwk[0].diffuse = WHITE(128);
		MainMenuImg->vertexwk[1].diffuse = WHITE(128);
		MainMenuImg->vertexwk[2].diffuse = WHITE(128);
		MainMenuImg->vertexwk[3].diffuse = WHITE(128);
		break;

	case Img_Option_Yes:

		if (MainMenuSelect.InYes == true)
		{
			MainMenuImg->vertexwk[0].diffuse = GREEN(255);
			MainMenuImg->vertexwk[1].diffuse = GREEN(255);
			MainMenuImg->vertexwk[2].diffuse = GREEN(255);
			MainMenuImg->vertexwk[3].diffuse = GREEN(255);
		}
		else
		{
			MainMenuImg->vertexwk[0].diffuse = WHITE(255);
			MainMenuImg->vertexwk[1].diffuse = WHITE(255);
			MainMenuImg->vertexwk[2].diffuse = WHITE(255);
			MainMenuImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;

	case Img_Option_No:

		if (MainMenuSelect.InYes == true)
		{
			MainMenuImg->vertexwk[0].diffuse = WHITE(255);
			MainMenuImg->vertexwk[1].diffuse = WHITE(255);
			MainMenuImg->vertexwk[2].diffuse = WHITE(255);
			MainMenuImg->vertexwk[3].diffuse = WHITE(255);
		}
		else
		{
			MainMenuImg->vertexwk[0].diffuse = GREEN(255);
			MainMenuImg->vertexwk[1].diffuse = GREEN(255);
			MainMenuImg->vertexwk[2].diffuse = GREEN(255);
			MainMenuImg->vertexwk[3].diffuse = GREEN(255);
		}
		break;

	default:
		break;
	}

	return;
}

//=============================================================================
// 選択したステージを取得する
//=============================================================================
int GetSelectStage(void)
{
	return SelectStage;
}

//=============================================================================
// ステージクリアフラグを設置
//=============================================================================
void SetClearFlag(int StageClearFlag)
{
	StageClear[StageClearFlag] = true;

	return;
}
