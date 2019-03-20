//=============================================================================
//
// タイトル画面処理 [Title.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Title.h"
#include "BackGround.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_TeamLogo			_T("data/Texture/BackGround/TeamLogo.png")
#define Texture_TeamLogo_Width		(800)
#define Texture_TeamLogo_Height		(800)
#define TeamLogo_Pos				(D3DXVECTOR3(560.0f,140.0f,0.0f))

#define Texture_TitleBG				_T("data/Texture/BackGround/BackGround_Title.png")
#define Texture_TitleBG_Width		(1920)
#define Texture_TitleBG_Height		(1080)

#define Texture_GameTitle			_T("data/Texture/BackGround/GameTitle.png")
#define Texture_GameTitle_Width		(1200)
#define Texture_GameTitle_Height	(200)

#define Texture_GameStart			_T("data/Texture/UI/GameStart.png")
#define Texture_GameStart_Width		(480)
#define Texture_GameStart_Height	(80)

#define Texture_GameExit			_T("data/Texture/UI/GameExit.png")
#define Texture_GameExit_Width		(480)
#define Texture_GameExit_Height		(80)

// 背景スクロール速度
#define TitleBGSpeed				(0.0003f)
// テキスト矩形の高さ
#define RectHeight					(100)
// テキスト表示の座標
#define GameTitle_Pos				(D3DXVECTOR3(360.0f,100.0f,0.0f))
#define GameStart_Pos				(D3DXVECTOR3(740.0f,600.0f,0.0f))
#define GameEnd_Pos					(D3DXVECTOR3(740.0f,750.0f,0.0f))

// チームロゴの表示速度
#define LogoFadeSpeed				(2)

enum TitleImageType
{
	Img_TeamLogo,
	Img_TitleBG,
	Img_Title,
	Img_GameStart,
	Img_GameExit,
	TitleImg_Max,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeTitleVertex(IMAGE *TitleImg);
// 頂点座標の設定
void SetTitleVertex(IMAGE *TitleImg);
// テクスチャ座標の設定
void SetTitleTexture(IMAGE *TitleImg);
// 色、透明度の設定
void SetTitleColor(IMAGE *TitleImg);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		TitleTexture[TitleImg_Max] = { NULL };
// タイトル画像構造体
IMAGE					TitleImg[TitleImg_Max];
// タイトル選択肢
SELECT					TitleSelect;
// ゲーム終了フラグ
bool					GameExitFlag = false;
// チームロゴの表示フラグ
// ゲーム立ち上がる時、チームロゴ表示
bool					ShowTeamLogo = true;
// チームロゴの透明度
int						LogoAlpha = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(bool FirstInit)
{
	int Image_No = 0;

	// 選択肢初期化
	TitleSelect.State = Img_GameStart;
	TitleSelect.PreState = Img_GameStart;
	TitleSelect.InYes = false;

	// 終了フラグ初期化
	GameExitFlag = false;

	for (Image_No = 0; Image_No < TitleImg_Max; Image_No++)
	{
		TitleImg[Image_No].Use = true;
	}
	if (ShowTeamLogo == false)
	{
		TitleImg[Img_TeamLogo].Use = false;
	}

	if (FirstInit == true)
	{
		TitleImg[Img_TeamLogo].pos = TeamLogo_Pos;
		TitleImg[Img_TeamLogo].Width = Texture_TeamLogo_Width;
		TitleImg[Img_TeamLogo].Height = Texture_TeamLogo_Height;
		TitleImg[Img_TeamLogo].Type = Img_TeamLogo;

		TitleImg[Img_Title].pos = GameTitle_Pos;
		TitleImg[Img_Title].Width = Texture_GameTitle_Width;
		TitleImg[Img_Title].Height = Texture_GameTitle_Height;
		TitleImg[Img_Title].Type = Img_Title;

		TitleImg[Img_TitleBG].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		TitleImg[Img_TitleBG].Width = Texture_TitleBG_Width;
		TitleImg[Img_TitleBG].Height = Texture_TitleBG_Height;
		TitleImg[Img_TitleBG].Type = Img_TitleBG;

		TitleImg[Img_GameStart].pos = GameStart_Pos;
		TitleImg[Img_GameStart].Width = Texture_GameStart_Width;
		TitleImg[Img_GameStart].Height = Texture_GameStart_Height;
		TitleImg[Img_GameStart].Type = Img_GameStart;

		TitleImg[Img_GameExit].pos = GameEnd_Pos;
		TitleImg[Img_GameExit].Width = Texture_GameExit_Width;
		TitleImg[Img_GameExit].Height = Texture_GameExit_Height;
		TitleImg[Img_GameExit].Type = Img_GameExit;

		// 頂点情報の作成
		for (Image_No = 0; Image_No < TitleImg_Max; Image_No++)
		{
			MakeTitleVertex(&TitleImg[Image_No]);
		}

		// テクスチャの読み込み
		if (SafeLoad(Texture_TeamLogo, &TitleImg[Img_TeamLogo].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_TitleBG, &TitleImg[Img_TitleBG].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameTitle, &TitleImg[Img_Title].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameStart, &TitleImg[Img_GameStart].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameExit, &TitleImg[Img_GameExit].Texture, "Title") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	for (int Image_No = 0; Image_No < TitleImg_Max; Image_No++)
	{
		SafeRelease(TitleImg[Image_No].Texture);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	static int PressCount = 0;
	static int StayCount = 0;
	static bool FadeOut = false;

	// タイトル表示処理
	if (ShowTeamLogo == false)
	{
		// Enter
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(Player_A, BUTTON_B))
		{
			SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
			if (TitleSelect.State == Img_GameStart)
			{
				SetTransition(Fadein);
			}
			else
			{
				GameExitFlag = true;
			}
		}

		// 選択肢移動
		// 上下
		if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) ||
			GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
			if (TitleSelect.State == Img_GameStart)
			{
				TitleSelect.State = Img_GameExit;
			}
			else
			{
				TitleSelect.State = Img_GameStart;
			}
		}

		// 選択肢ループ
		if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP) ||
			GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
				if (TitleSelect.State == Img_GameStart)
				{
					TitleSelect.State = Img_GameExit;
				}
				else
				{
					TitleSelect.State = Img_GameStart;
				}
			}
		}

		// プレスカウント初期化
		if (GetKeyboardRelease(DIK_UP) || IsButtonReleased(0, BUTTON_UP) ||
			GetKeyboardRelease(DIK_DOWN) || IsButtonReleased(0, BUTTON_DOWN))
		{
			PressCount = 0;
		}

		for (int Image_No = 0; Image_No < TitleImg_Max; Image_No++)
		{
			// 色、透明度の設定
			SetTitleColor(&TitleImg[Image_No]);
		}
	}
	// チームロゴ表示処理
	else
	{
		if (FadeOut == false)
		{
			LogoAlpha += LogoFadeSpeed;
			if (LogoAlpha >= 255)
			{
				LogoAlpha = 255;
				FadeOut = true;
			}
		}
		else
		{
			if (StayCount >= 60)
			{
				LogoAlpha -= LogoFadeSpeed;
				if (LogoAlpha <= 0)
				{
					LogoAlpha = 0;
					StayCount = 0;
					FadeOut = false;
					ShowTeamLogo = false;
					TitleImg[Img_TeamLogo].Use = false;
				}
			}
			else
			{
				StayCount++;
			}
		}

		// 色、透明度の設定
		SetTitleColor(&TitleImg[Img_TeamLogo]);
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// タイトル表示
	if (ShowTeamLogo == false)
	{
		for (int Image_No = 0; Image_No < TitleImg_Max; Image_No++)
		{
			if (TitleImg[Image_No].Use == true)
			{
				// テクスチャの設定
				Device->SetTexture(0, TitleImg[Image_No].Texture);

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleImg[Image_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}
	// チームロゴ表示
	else
	{
		// テクスチャの設定
		Device->SetTexture(0, TitleImg[Img_TeamLogo].Texture);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleImg[Img_TeamLogo].vertexwk, sizeof(VERTEX_2D));
	}

	return;
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeTitleVertex(IMAGE *TitleImg)
{
	// テクスチャ座標の設定
	SetTitleTexture(TitleImg);

	// rhwの設定
	TitleImg->vertexwk[0].rhw = 1.0f;
	TitleImg->vertexwk[1].rhw = 1.0f;
	TitleImg->vertexwk[2].rhw = 1.0f;
	TitleImg->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	TitleImg->vertexwk[0].diffuse = WHITE(255);
	TitleImg->vertexwk[1].diffuse = WHITE(255);
	TitleImg->vertexwk[2].diffuse = WHITE(255);
	TitleImg->vertexwk[3].diffuse = WHITE(255);

	// 頂点座標の設定
	SetTitleVertex(TitleImg);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTitleTexture(IMAGE *TitleImg)
{
	TitleImg->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TitleImg->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TitleImg->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TitleImg->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTitleVertex(IMAGE *TitleImg)
{
	TitleImg->vertexwk[0].vtx = D3DXVECTOR3(TitleImg->pos.x, TitleImg->pos.y, 0.0f);
	TitleImg->vertexwk[1].vtx = D3DXVECTOR3(TitleImg->pos.x + TitleImg->Width, TitleImg->pos.y, 0.0f);
	TitleImg->vertexwk[2].vtx = D3DXVECTOR3(TitleImg->pos.x, TitleImg->pos.y + TitleImg->Height, 0.0f);
	TitleImg->vertexwk[3].vtx = D3DXVECTOR3(TitleImg->pos.x + TitleImg->Width, TitleImg->pos.y + TitleImg->Height, 0.0f);

	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetTitleColor(IMAGE *TitleImg)
{
	switch (TitleImg->Type)
	{
	case Img_TeamLogo:

		TitleImg->vertexwk[0].diffuse = WHITE(LogoAlpha);
		TitleImg->vertexwk[1].diffuse = WHITE(LogoAlpha);
		TitleImg->vertexwk[2].diffuse = WHITE(LogoAlpha);
		TitleImg->vertexwk[3].diffuse = WHITE(LogoAlpha);
		break;

	case Img_GameStart:

		if (TitleSelect.State == Img_GameStart)
		{
			TitleImg->vertexwk[0].diffuse = GREEN(255);
			TitleImg->vertexwk[1].diffuse = GREEN(255);
			TitleImg->vertexwk[2].diffuse = GREEN(255);
			TitleImg->vertexwk[3].diffuse = GREEN(255);
		}
		else
		{
			TitleImg->vertexwk[0].diffuse = WHITE(255);
			TitleImg->vertexwk[1].diffuse = WHITE(255);
			TitleImg->vertexwk[2].diffuse = WHITE(255);
			TitleImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;

	case Img_GameExit:

		if (TitleSelect.State == Img_GameStart)
		{
			TitleImg->vertexwk[0].diffuse = WHITE(255);
			TitleImg->vertexwk[1].diffuse = WHITE(255);
			TitleImg->vertexwk[2].diffuse = WHITE(255);
			TitleImg->vertexwk[3].diffuse = WHITE(255);
		}
		else
		{
			TitleImg->vertexwk[0].diffuse = GREEN(255);
			TitleImg->vertexwk[1].diffuse = GREEN(255);
			TitleImg->vertexwk[2].diffuse = GREEN(255);
			TitleImg->vertexwk[3].diffuse = GREEN(255);
		}
		break;

	default:
		break;
	}

	return;
}

//=============================================================================
// ゲーム終了フラグを取得
//=============================================================================
bool GetGameExit(void)
{
	return GameExitFlag;
}

//=============================================================================
// ロゴ表示フラグを取得
//=============================================================================
bool GetShowLogo(void)
{
	return ShowTeamLogo;
}