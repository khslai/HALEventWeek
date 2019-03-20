//=============================================================================
//
// 一時停止画面処理 [Pause.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Pause.h"
#include "Transition.h"
#include "Player.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_BlackScreen			_T("data/Texture/UI/BlackScreen.png")
#define Texture_Pause				_T("data/Texture/UI/Pause.png")
#define Texture_Pause_Width			(640)
#define Texture_Pause_Height		(160)
#define Pause_Pos					(D3DXVECTOR3(640.0f,140.0f,0.0f))

#define Texture_Resume				_T("data/Texture/UI/Resume.png")
#define Texture_ToMainMenu			_T("data/Texture/UI/ToMainMenu.png")
#define Texture_ToTitle				_T("data/Texture/UI/ToTitle.png")
#define Texture_Text_Width			(480)
#define Texture_Text_Height			(80)
#define Resume_Pos					(D3DXVECTOR3(720.0f,540.0f,0.0f))
#define ToMainMenu_Pos				(D3DXVECTOR3(720.0f,690.0f,0.0f))
#define ToTitle_Pos					(D3DXVECTOR3(720.0f,840.0f,0.0f))


enum PauseImageType
{
	Img_PauseBG,
	Img_Pause,
	Img_Resume,
	Img_ToMainMenu,
	Img_ToTitle,
	PauseImg_Max,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakePauseVertex(IMAGE *PauseImg);
// 頂点座標の設定
void SetPauseVertex(IMAGE *PauseImg);
// テクスチャ座標の設定
void SetPauseTexture(IMAGE *PauseImg);
// 色、透明度の設定
void SetPauseColor(IMAGE *PauseImg);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		PauseTexture[PauseImg_Max] = { NULL };
// タイトル画像構造体
IMAGE					PauseImg[PauseImg_Max];
// タイトル選択肢
SELECT					PauseSelect;
// 一時停止中フラグ
bool					InPause = false;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(bool FirstInit)
{
	int Image_No = 0;

	// 選択肢初期化
	PauseSelect.State = Select_Resume;
	PauseSelect.PreState = Select_Resume;
	PauseSelect.InYes = false;

	// フラグ初期化
	InPause = false;

	for (Image_No = 0; Image_No < PauseImg_Max; Image_No++)
	{
		PauseImg[Image_No].Use = true;
	}

	if (FirstInit == true)
	{
		PauseImg[Img_PauseBG].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		PauseImg[Img_PauseBG].Width = Screen_Width;
		PauseImg[Img_PauseBG].Height = Screen_Height;
		PauseImg[Img_PauseBG].Type = Img_PauseBG;

		PauseImg[Img_Pause].pos = Pause_Pos;
		PauseImg[Img_Pause].Width = Texture_Pause_Width;
		PauseImg[Img_Pause].Height = Texture_Pause_Height;
		PauseImg[Img_Pause].Type = Img_Pause;

		PauseImg[Img_Resume].pos = Resume_Pos;
		PauseImg[Img_Resume].Width = Texture_Text_Width;
		PauseImg[Img_Resume].Height = Texture_Text_Height;
		PauseImg[Img_Resume].Type = Img_Resume;

		PauseImg[Img_ToMainMenu].pos = ToMainMenu_Pos;
		PauseImg[Img_ToMainMenu].Width = Texture_Text_Width;
		PauseImg[Img_ToMainMenu].Height = Texture_Text_Height;
		PauseImg[Img_ToMainMenu].Type = Img_ToMainMenu;

		PauseImg[Img_ToTitle].pos = ToTitle_Pos;
		PauseImg[Img_ToTitle].Width = Texture_Text_Width;
		PauseImg[Img_ToTitle].Height = Texture_Text_Height;
		PauseImg[Img_ToTitle].Type = Img_ToTitle;

		// 頂点情報の作成
		for (Image_No = 0; Image_No < PauseImg_Max; Image_No++)
		{
			MakePauseVertex(&PauseImg[Image_No]);
		}

		// テクスチャの読み込み
		if (SafeLoad(Texture_BlackScreen, &PauseImg[Img_PauseBG].Texture, "Pause") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Pause, &PauseImg[Img_Pause].Texture, "Pause") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Resume, &PauseImg[Img_Resume].Texture, "Pause") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ToMainMenu, &PauseImg[Img_ToMainMenu].Texture, "Pause") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ToTitle, &PauseImg[Img_ToTitle].Texture, "Pause") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	for (int Image_No = 0; Image_No < PauseImg_Max; Image_No++)
	{
		SafeRelease(PauseImg[Image_No].Texture);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	int Image_No = 0;
	static int PressCount = 0;

	if (InPause == true)
	{
		// Enter
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
		{
			SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
			switch (PauseSelect.State)
			{
			case Select_Resume:

				SetPauseFlag(false);
				break;

			case Select_ToMainMenu:
			case Select_ToTitle:

				SetTransition(Fadein);
				break;

			default:
				break;
			}
		}

		// 選択肢移動
		// 上下
		if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) ||
			IsButtonTriggered(1, BUTTON_UP))
		{
			SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
			switch (PauseSelect.State)
			{
			case Select_Resume:
				PauseSelect.State = Select_ToTitle;
				break;
			case Select_ToMainMenu:
				PauseSelect.State = Select_Resume;
				break;
			case Select_ToTitle:
				PauseSelect.State = Select_ToMainMenu;
				break;
			default:
				break;
			}
		}
		else if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN) ||
			IsButtonTriggered(1, BUTTON_DOWN))
		{
			SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
			switch (PauseSelect.State)
			{
			case Select_Resume:
				PauseSelect.State = Select_ToMainMenu;
				break;
			case Select_ToMainMenu:
				PauseSelect.State = Select_ToTitle;
				break;
			case Select_ToTitle:
				PauseSelect.State = Select_Resume;
				break;
			default:
				break;
			}
		}

		// 選択肢ループ
		if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP) || IsButtonRepeat(1, BUTTON_UP))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
				switch (PauseSelect.State)
				{
				case Select_Resume:
					PauseSelect.State = Select_ToTitle;
					break;
				case Select_ToMainMenu:
					PauseSelect.State = Select_Resume;
					break;
				case Select_ToTitle:
					PauseSelect.State = Select_ToMainMenu;
					break;
				default:
					break;
				}
			}
		}
		else if (GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN) || IsButtonRepeat(1, BUTTON_DOWN))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
				switch (PauseSelect.State)
				{
				case Select_Resume:
					PauseSelect.State = Select_ToMainMenu;
					break;
				case Select_ToMainMenu:
					PauseSelect.State = Select_ToTitle;
					break;
				case Select_ToTitle:
					PauseSelect.State = Select_Resume;
					break;
				default:
					break;
				}
			}
		}

		// プレスカウント初期化
		if (GetKeyboardRelease(DIK_UP) || IsButtonReleased(0, BUTTON_UP) || IsButtonRepeat(1, BUTTON_UP) ||
			GetKeyboardRelease(DIK_DOWN) || IsButtonReleased(0, BUTTON_DOWN) || IsButtonRepeat(1, BUTTON_DOWN))
		{
			PressCount = 0;
		}

		for (Image_No = 0; Image_No < PauseImg_Max; Image_No++)
		{
			if (PauseImg[Image_No].Use == true)
			{
				// 色、透明度の設定
				SetPauseColor(&PauseImg[Image_No]);
			}
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *PlayerA = GetPlayer(Player_A);
	PLAYER *PlayerB = GetPlayer(Player_B);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (InPause == true)
	{
		for (int Image_No = 0; Image_No < PauseImg_Max; Image_No++)
		{
			if (PauseImg[Image_No].Use == true)
			{
				// テクスチャの設定
				Device->SetTexture(0, PauseImg[Image_No].Texture);

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PauseImg[Image_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakePauseVertex(IMAGE *PauseImg)
{
	// テクスチャ座標の設定
	SetPauseTexture(PauseImg);

	// rhwの設定
	PauseImg->vertexwk[0].rhw = 1.0f;
	PauseImg->vertexwk[1].rhw = 1.0f;
	PauseImg->vertexwk[2].rhw = 1.0f;
	PauseImg->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	if (PauseImg->Type != Img_PauseBG)
	{
		PauseImg->vertexwk[0].diffuse = WHITE(255);
		PauseImg->vertexwk[1].diffuse = WHITE(255);
		PauseImg->vertexwk[2].diffuse = WHITE(255);
		PauseImg->vertexwk[3].diffuse = WHITE(255);
	}
	else
	{
		PauseImg->vertexwk[0].diffuse = WHITE(128);
		PauseImg->vertexwk[1].diffuse = WHITE(128);
		PauseImg->vertexwk[2].diffuse = WHITE(128);
		PauseImg->vertexwk[3].diffuse = WHITE(128);
	}

	// 頂点座標の設定
	SetPauseVertex(PauseImg);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetPauseTexture(IMAGE *PauseImg)
{
	PauseImg->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	PauseImg->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	PauseImg->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	PauseImg->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetPauseVertex(IMAGE *PauseImg)
{
	PauseImg->vertexwk[0].vtx = D3DXVECTOR3(PauseImg->pos.x, PauseImg->pos.y, 0.0f);
	PauseImg->vertexwk[1].vtx = D3DXVECTOR3(PauseImg->pos.x + PauseImg->Width, PauseImg->pos.y, 0.0f);
	PauseImg->vertexwk[2].vtx = D3DXVECTOR3(PauseImg->pos.x, PauseImg->pos.y + PauseImg->Height, 0.0f);
	PauseImg->vertexwk[3].vtx = D3DXVECTOR3(PauseImg->pos.x + PauseImg->Width, PauseImg->pos.y + PauseImg->Height, 0.0f);

	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetPauseColor(IMAGE *PauseImg)
{
	switch (PauseImg->Type)
	{
	case Img_Resume:
		if (PauseSelect.State == Select_Resume)
		{
			PauseImg->vertexwk[0].diffuse = GREEN(255);
			PauseImg->vertexwk[1].diffuse = GREEN(255);
			PauseImg->vertexwk[2].diffuse = GREEN(255);
			PauseImg->vertexwk[3].diffuse = GREEN(255);
		}
		else
		{
			PauseImg->vertexwk[0].diffuse = WHITE(255);
			PauseImg->vertexwk[1].diffuse = WHITE(255);
			PauseImg->vertexwk[2].diffuse = WHITE(255);
			PauseImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;
	case Img_ToMainMenu:
		if (PauseSelect.State == Select_ToMainMenu)
		{
			PauseImg->vertexwk[0].diffuse = GREEN(255);
			PauseImg->vertexwk[1].diffuse = GREEN(255);
			PauseImg->vertexwk[2].diffuse = GREEN(255);
			PauseImg->vertexwk[3].diffuse = GREEN(255);
		}
		else
		{
			PauseImg->vertexwk[0].diffuse = WHITE(255);
			PauseImg->vertexwk[1].diffuse = WHITE(255);
			PauseImg->vertexwk[2].diffuse = WHITE(255);
			PauseImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;
	case Img_ToTitle:
		if (PauseSelect.State == Select_ToTitle)
		{
			PauseImg->vertexwk[0].diffuse = GREEN(255);
			PauseImg->vertexwk[1].diffuse = GREEN(255);
			PauseImg->vertexwk[2].diffuse = GREEN(255);
			PauseImg->vertexwk[3].diffuse = GREEN(255);
		}
		else
		{
			PauseImg->vertexwk[0].diffuse = WHITE(255);
			PauseImg->vertexwk[1].diffuse = WHITE(255);
			PauseImg->vertexwk[2].diffuse = WHITE(255);
			PauseImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 一時停止状態を設置
//=============================================================================
void SetPauseFlag(bool Flag)
{
	InPause = Flag;

	return;
}

//=============================================================================
// 一時停止フラグを取得
//=============================================================================
bool GetPauseFlag(void)
{
	return InPause;
}

//=============================================================================
// 選択状態を取得
//=============================================================================
int GetPauseSelect(void)
{
	return PauseSelect.State;
}

