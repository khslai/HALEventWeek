//=============================================================================
//
// ゲームUI処理 [GameUI.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "GameUI.h"
#include "Player.h"
#include "Boss.h"
#include "Effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// アイコン
#define Texture_ShotIcon			_T("data/Texture/UI/ShotIcon.png")
#define Texture_DashIcon			_T("data/Texture/UI/DashIcon.png")
#define Texture_CoolDown			_T("data/Texture/UI/CoolDown.png")
#define Texture_Icon_Width			(120)
#define Texture_Icon_Height			(120)

// アイコン表示座標
#define PlayerA_ShotIcon_Pos_X		(50)
#define PlayerB_ShotIcon_Pos_X		(1600)
#define PlayerA_DashIcon_Pos_X		(200)
#define PlayerB_DashIcon_Pos_X		(1750)
#define Player_Icon_Pos_Y			(150)

// HPゲージ(プレイヤー)
#define	Texture_GaugeBox_PlayerA	_T("data/Texture/UI/GaugeBox_PlayerA.png")	
#define	Texture_GaugeBox_PlayerB	_T("data/Texture/UI/GaugeBox_PlayerB.png")		
#define	Texture_Gauge_Player		_T("data/Texture/UI/Gauge_Player.png")
#define PlayerGaugeBox_Width		(600)
#define PlayerGaugeBox_Height		(130)
#define PlayerGauge_Width			(560)
#define PlayerGauge_Height			(60)

// HPゲージ(ボス)
#define Texture_GaugeBox_Boss		_T("data/Texture/UI/GaugeBox_Boss.png")
#define Texture_Gauge_Boss			_T("data/Texture/UI/Gauge_Boss.png")
#define BossGauge_Width				(1500)
#define	BossGauge_Height			(50)

// HPゲージ表示座標(プレイヤー)
#define PlayerA_HPGaugeBox_Pos_X	(100)
#define PlayerB_HPGaugeBox_Pos_X	(1320)
#define Player_HPGaugeBox_Pos_Y		(0)
#define PlayerA_HPGauge_Pos_X		(120)
#define PlayerB_HPGauge_Pos_X		(1340)
#define Player_HPGauge_Pos_Y		(50)

// HPゲージ表示座標(ボス)
#define	Boss_HPGauge_Pos_X			(210)
#define Boss_HPGauge_Pos_Y			(1000)

// プレイヤー立ち絵
#define Texture_Illust_Player_A		_T("data/Texture/UI/Illust_Player_A.png")	
#define Texture_Illust_Player_B		_T("data/Texture/UI/Illust_Player_B.png")	
#define Illust_Width				(100)
#define	Illust_Height				(100)

// プレイヤー立ち絵表示座標
#define PlayerA_Illust_Pos_X		(0)
#define PlayerB_Illust_Pos_X		(1220)
#define Player_Illust_Pos_Y			(15)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeGameUIVertex(GAMEUI *GameUIPtr);
// 頂点座標の設定
void SetGameUIVertex(GAMEUI *GameUIPtr);
// テクスチャ座標の設定
void SetGameUITexture(GAMEUI *GameUIPtr);
// 色、透明度の設定
void SetGameUIDiffuse(GAMEUI *GameUIPtr, int Alpha);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャ
LPDIRECT3DTEXTURE9	ShotIcon_Texture;
LPDIRECT3DTEXTURE9	DashIcon_Texture;
LPDIRECT3DTEXTURE9	CoolDown_Texture;
LPDIRECT3DTEXTURE9	Boss_GaugeBox_Texture;
LPDIRECT3DTEXTURE9	Boss_Gauge_Texture;
LPDIRECT3DTEXTURE9	PlayerA_GaugeBox_Texture;
LPDIRECT3DTEXTURE9	PlayerB_GaugeBox_Texture;
LPDIRECT3DTEXTURE9	Player_Gauge_Texture;
LPDIRECT3DTEXTURE9	PlayerA_Illust_Texture;
LPDIRECT3DTEXTURE9	PlayerB_Illust_Texture;

// ゲームUI構造体
GAMEUI GameUI[UIType_Max];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameUI(bool FirstInit)
{
	int GameStage = GetGameStage();
	int GameUI_No = 0;

	for (GameUI_No = 0; GameUI_No < UIType_Max; GameUI_No++)
	{
		GameUI[GameUI_No].Use = false;
	}

	switch (GameStage)
	{
	case Stage_Oushi:

		GameUI[UIType_HPGaugeBox_PlayerA].Use = true;
		GameUI[UIType_HPGauge_PlayerA].Use = true;
		GameUI[UIType_HPGaugeBox_PlayerB].Use = true;
		GameUI[UIType_HPGauge_PlayerB].Use = true;
		GameUI[UIType_Illust_PlayerA].Use = true;
		GameUI[UIType_Illust_PlayerB].Use = true;
		break;

	case Stage_Ite:

		GameUI[UIType_ShotIcon_PlayerA].Use = true;
		GameUI[UIType_ShotCD_PlayerA].Use = false;
		break;

	case Stage_Shishi:

		// プレイヤーA
		GameUI[UIType_ShotIcon_PlayerA].Use = true;
		GameUI[UIType_ShotCD_PlayerA].Use = false;
		GameUI[UIType_DashIcon_PlayerA].Use = true;
		GameUI[UIType_DashCD_PlayerA].Use = false;
		GameUI[UIType_HPGaugeBox_PlayerA].Use = true;
		GameUI[UIType_HPGauge_PlayerA].Use = true;
		GameUI[UIType_Illust_PlayerA].Use = true;

		// プレイヤーB
		GameUI[UIType_ShotIcon_PlayerB].Use = true;
		GameUI[UIType_ShotCD_PlayerB].Use = false;
		GameUI[UIType_DashIcon_PlayerB].Use = true;
		GameUI[UIType_DashCD_PlayerB].Use = false;
		GameUI[UIType_HPGaugeBox_PlayerB].Use = true;
		GameUI[UIType_HPGauge_PlayerB].Use = true;
		GameUI[UIType_Illust_PlayerB].Use = true;

		// ボス
		GameUI[UIType_HPGauge_Boss].Use = true;
		GameUI[UIType_HPGaugeBox_Boss].Use = true;
		break;

	default:
		break;
	}

	if (FirstInit == false)
	{
		// 頂点情報の作成
		for (GameUI_No = 0; GameUI_No < UIType_Max; GameUI_No++)
		{
			MakeGameUIVertex(&GameUI[GameUI_No]);
		}
	}
	else if (FirstInit == true)
	{
		// =================
		// プレイヤーA
		// =================
		GameUI[UIType_ShotIcon_PlayerA].pos = D3DXVECTOR3(PlayerA_ShotIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_ShotIcon_PlayerA].Width = Texture_Icon_Width;
		GameUI[UIType_ShotIcon_PlayerA].Height = Texture_Icon_Height;
		GameUI[UIType_ShotIcon_PlayerA].Type = UIType_ShotIcon_PlayerA;
		GameUI[UIType_ShotIcon_PlayerA].Alpha = 255;

		GameUI[UIType_ShotCD_PlayerA].pos = D3DXVECTOR3(PlayerA_ShotIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_ShotCD_PlayerA].Width = Texture_Icon_Width;
		GameUI[UIType_ShotCD_PlayerA].Height = Texture_Icon_Height;
		GameUI[UIType_ShotCD_PlayerA].Type = UIType_ShotCD_PlayerA;
		GameUI[UIType_ShotCD_PlayerA].Alpha = 255;

		GameUI[UIType_DashIcon_PlayerA].pos = D3DXVECTOR3(PlayerA_DashIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_DashIcon_PlayerA].Width = Texture_Icon_Width;
		GameUI[UIType_DashIcon_PlayerA].Height = Texture_Icon_Height;
		GameUI[UIType_DashIcon_PlayerA].Type = UIType_DashIcon_PlayerA;
		GameUI[UIType_DashIcon_PlayerA].Alpha = 255;

		GameUI[UIType_DashCD_PlayerA].pos = D3DXVECTOR3(PlayerA_DashIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_DashCD_PlayerA].Width = Texture_Icon_Width;
		GameUI[UIType_DashCD_PlayerA].Height = Texture_Icon_Height;
		GameUI[UIType_DashCD_PlayerA].Type = UIType_DashCD_PlayerA;
		GameUI[UIType_DashCD_PlayerA].Alpha = 255;

		GameUI[UIType_HPGaugeBox_PlayerA].pos = D3DXVECTOR3(PlayerA_HPGaugeBox_Pos_X, Player_HPGaugeBox_Pos_Y, 0.0f);
		GameUI[UIType_HPGaugeBox_PlayerA].Width = PlayerGaugeBox_Width;
		GameUI[UIType_HPGaugeBox_PlayerA].Height = PlayerGaugeBox_Height;
		GameUI[UIType_HPGaugeBox_PlayerA].Type = UIType_HPGaugeBox_PlayerA;
		GameUI[UIType_HPGaugeBox_PlayerA].Alpha = 255;

		GameUI[UIType_HPGauge_PlayerA].pos = D3DXVECTOR3(PlayerA_HPGauge_Pos_X, Player_HPGauge_Pos_Y, 0.0f);
		GameUI[UIType_HPGauge_PlayerA].Width = PlayerGauge_Width;
		GameUI[UIType_HPGauge_PlayerA].Height = PlayerGauge_Height;
		GameUI[UIType_HPGauge_PlayerA].Type = UIType_HPGauge_PlayerA;
		GameUI[UIType_HPGauge_PlayerA].Alpha = 255;

		GameUI[UIType_Illust_PlayerA].pos = D3DXVECTOR3(PlayerA_Illust_Pos_X, Player_Illust_Pos_Y, 0.0f);
		GameUI[UIType_Illust_PlayerA].Width = Illust_Width;
		GameUI[UIType_Illust_PlayerA].Height = Illust_Height;
		GameUI[UIType_Illust_PlayerA].Type = UIType_Illust_PlayerA;
		GameUI[UIType_Illust_PlayerA].Alpha = 255;

		// =================
		// プレイヤーB
		// =================
		GameUI[UIType_ShotIcon_PlayerB].pos = D3DXVECTOR3(PlayerB_ShotIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_ShotIcon_PlayerB].Width = Texture_Icon_Width;
		GameUI[UIType_ShotIcon_PlayerB].Height = Texture_Icon_Height;
		GameUI[UIType_ShotIcon_PlayerB].Type = UIType_ShotIcon_PlayerB;
		GameUI[UIType_ShotIcon_PlayerB].Alpha = 255;

		GameUI[UIType_ShotCD_PlayerB].pos = D3DXVECTOR3(PlayerB_ShotIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_ShotCD_PlayerB].Width = Texture_Icon_Width;
		GameUI[UIType_ShotCD_PlayerB].Height = Texture_Icon_Height;
		GameUI[UIType_ShotCD_PlayerB].Type = UIType_ShotCD_PlayerB;
		GameUI[UIType_ShotCD_PlayerB].Alpha = 255;

		GameUI[UIType_DashIcon_PlayerB].pos = D3DXVECTOR3(PlayerB_DashIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_DashIcon_PlayerB].Width = Texture_Icon_Width;
		GameUI[UIType_DashIcon_PlayerB].Height = Texture_Icon_Height;
		GameUI[UIType_DashIcon_PlayerB].Type = UIType_DashIcon_PlayerB;
		GameUI[UIType_DashIcon_PlayerB].Alpha = 255;

		GameUI[UIType_DashCD_PlayerB].pos = D3DXVECTOR3(PlayerB_DashIcon_Pos_X, Player_Icon_Pos_Y, 0.0f);
		GameUI[UIType_DashCD_PlayerB].Width = Texture_Icon_Width;
		GameUI[UIType_DashCD_PlayerB].Height = Texture_Icon_Height;
		GameUI[UIType_DashCD_PlayerB].Type = UIType_DashCD_PlayerB;
		GameUI[UIType_DashCD_PlayerB].Alpha = 255;

		GameUI[UIType_HPGaugeBox_PlayerB].pos = D3DXVECTOR3(PlayerB_HPGaugeBox_Pos_X, Player_HPGaugeBox_Pos_Y, 0.0f);
		GameUI[UIType_HPGaugeBox_PlayerB].Width = PlayerGaugeBox_Width;
		GameUI[UIType_HPGaugeBox_PlayerB].Height = PlayerGaugeBox_Height;
		GameUI[UIType_HPGaugeBox_PlayerB].Type = UIType_HPGaugeBox_PlayerB;
		GameUI[UIType_HPGaugeBox_PlayerB].Alpha = 255;

		GameUI[UIType_HPGauge_PlayerB].pos = D3DXVECTOR3(PlayerB_HPGauge_Pos_X, Player_HPGauge_Pos_Y, 0.0f);
		GameUI[UIType_HPGauge_PlayerB].Width = PlayerGauge_Width;
		GameUI[UIType_HPGauge_PlayerB].Height = PlayerGauge_Height;
		GameUI[UIType_HPGauge_PlayerB].Type = UIType_HPGauge_PlayerB;
		GameUI[UIType_HPGauge_PlayerB].Alpha = 255;

		GameUI[UIType_Illust_PlayerB].pos = D3DXVECTOR3(PlayerB_Illust_Pos_X, Player_Illust_Pos_Y, 0.0f);
		GameUI[UIType_Illust_PlayerB].Width = Illust_Width;
		GameUI[UIType_Illust_PlayerB].Height = Illust_Height;
		GameUI[UIType_Illust_PlayerB].Type = UIType_Illust_PlayerB;
		GameUI[UIType_Illust_PlayerB].Alpha = 255;

		// =================
		// ボス
		// =================
		GameUI[UIType_HPGaugeBox_Boss].pos = D3DXVECTOR3(Boss_HPGauge_Pos_X, Boss_HPGauge_Pos_Y, 0.0f);
		GameUI[UIType_HPGaugeBox_Boss].Width = BossGauge_Width;
		GameUI[UIType_HPGaugeBox_Boss].Height = BossGauge_Height;
		GameUI[UIType_HPGaugeBox_Boss].Type = UIType_HPGaugeBox_Boss;
		GameUI[UIType_HPGaugeBox_Boss].Alpha = 255;

		GameUI[UIType_HPGauge_Boss].pos = D3DXVECTOR3(Boss_HPGauge_Pos_X, Boss_HPGauge_Pos_Y, 0.0f);
		GameUI[UIType_HPGauge_Boss].Width = BossGauge_Width;
		GameUI[UIType_HPGauge_Boss].Height = BossGauge_Height;
		GameUI[UIType_HPGauge_Boss].Type = UIType_HPGauge_Boss;
		GameUI[UIType_HPGauge_Boss].Alpha = 255;

		// 頂点情報の作成
		for (GameUI_No = 0; GameUI_No < UIType_Max; GameUI_No++)
		{
			MakeGameUIVertex(&GameUI[GameUI_No]);
		}

		// テクスチャの読み込み
		// アイコン
		if (SafeLoad(Texture_ShotIcon, &ShotIcon_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_DashIcon, &DashIcon_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_CoolDown, &CoolDown_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}

		// HPゲージ
		if (SafeLoad(Texture_GaugeBox_PlayerA, &PlayerA_GaugeBox_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_PlayerB, &PlayerB_GaugeBox_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Gauge_Player, &Player_Gauge_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_Boss, &Boss_GaugeBox_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Gauge_Boss, &Boss_Gauge_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}

		// 立ち絵
		if (SafeLoad(Texture_Illust_Player_A, &PlayerA_Illust_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Illust_Player_B, &PlayerB_Illust_Texture, "GameUI") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGameUI(void)
{
	SafeRelease(ShotIcon_Texture);
	SafeRelease(DashIcon_Texture);
	SafeRelease(CoolDown_Texture);
	SafeRelease(PlayerA_GaugeBox_Texture);
	SafeRelease(PlayerB_GaugeBox_Texture);
	SafeRelease(Player_Gauge_Texture);
	SafeRelease(Boss_GaugeBox_Texture);
	SafeRelease(Boss_Gauge_Texture);
	SafeRelease(PlayerA_Illust_Texture);
	SafeRelease(PlayerB_Illust_Texture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameUI(void)
{
	for (int i = 0; i < UIType_Max; i++)
	{
		if (GameUI[i].Use == true)
		{
			// テクスチャ座標更新
			SetGameUITexture(&GameUI[i]);
			// 色、透明度の設定
			SetGameUIDiffuse(&GameUI[i], GameUI[i].Alpha);
			// 頂点座標更新
			SetGameUIVertex(&GameUI[i]);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGameUI(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int GameUI_No = 0;
	int GameStage = GetGameStage();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (GameUI_No = 0; GameUI_No < UIType_Max; GameUI_No++)
	{
		if (GameUI[GameUI_No].Use == true)
		{
			// テクスチャの設定
			switch (GameUI[GameUI_No].Type)
			{
			case UIType_ShotIcon_PlayerA:
			case UIType_ShotIcon_PlayerB:

				Device->SetTexture(0, ShotIcon_Texture);
				break;

			case UIType_DashIcon_PlayerA:
			case UIType_DashIcon_PlayerB:

				Device->SetTexture(0, DashIcon_Texture);
				break;

			case UIType_ShotCD_PlayerA:
			case UIType_ShotCD_PlayerB:
			case UIType_DashCD_PlayerA:
			case UIType_DashCD_PlayerB:

				Device->SetTexture(0, CoolDown_Texture);
				break;

			case UIType_HPGaugeBox_PlayerA:

				Device->SetTexture(0, PlayerA_GaugeBox_Texture);
				break;

			case UIType_HPGaugeBox_PlayerB:

				Device->SetTexture(0, PlayerB_GaugeBox_Texture);
				break;

			case UIType_HPGaugeBox_Boss:

				Device->SetTexture(0, Boss_GaugeBox_Texture);
				break;

			case UIType_HPGauge_PlayerA:
			case UIType_HPGauge_PlayerB:

				Device->SetTexture(0, Player_Gauge_Texture);
				break;

			case UIType_HPGauge_Boss:

				Device->SetTexture(0, Boss_Gauge_Texture);
				break;

			case UIType_Illust_PlayerA:

				Device->SetTexture(0, PlayerA_Illust_Texture);
				break;

			case UIType_Illust_PlayerB:

				Device->SetTexture(0, PlayerB_Illust_Texture);
				break;

			default:
				break;
			}

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, GameUI[GameUI_No].vertexwk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeGameUIVertex(GAMEUI *GameUIPtr)
{
	// 頂点座標の設定	
	SetGameUIVertex(GameUIPtr);

	// rhwの設定
	GameUIPtr->vertexwk[0].rhw = 1.0f;
	GameUIPtr->vertexwk[1].rhw = 1.0f;
	GameUIPtr->vertexwk[2].rhw = 1.0f;
	GameUIPtr->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	SetGameUIDiffuse(GameUIPtr, GameUIPtr->Alpha);

	// テクスチャ座標の設定
	SetGameUITexture(GameUIPtr);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetGameUITexture(GAMEUI *GameUIPtr)
{
	float Percent = 0.0f;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	switch (GameUIPtr->Type)
	{
	case UIType_ShotIcon_PlayerA:
	case UIType_ShotIcon_PlayerB:
	case UIType_DashIcon_PlayerA:
	case UIType_DashIcon_PlayerB:
	case UIType_HPGaugeBox_PlayerA:
	case UIType_HPGaugeBox_PlayerB:
	case UIType_HPGaugeBox_Boss:
	case UIType_Illust_PlayerA:
	case UIType_Illust_PlayerB:

		GameUIPtr->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		GameUIPtr->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		GameUIPtr->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		GameUIPtr->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		break;

	case UIType_ShotCD_PlayerA:
		if (Percent == 0.0f)
		{
			Player = GetPlayer(Player_A);
			Percent = (float)Player->ShotCount / ShotInterval;
		}
	case UIType_DashCD_PlayerA:
		if (Percent == 0.0f)
		{
			Player = GetPlayer(Player_A);
			Percent = (float)Player->DashCDCount / DashCDInterval;
		}
	case UIType_ShotCD_PlayerB:
		if (Percent == 0.0f)
		{
			Player = GetPlayer(Player_B);
			Percent = (float)Player->ShotCount / ShotInterval;
		}
	case UIType_DashCD_PlayerB:
		if (Percent == 0.0f)
		{
			Player = GetPlayer(Player_B);
			Percent = (float)Player->DashCDCount / DashCDInterval;
		}
		GameUIPtr->vertexwk[0].tex = D3DXVECTOR2(0.0f, Percent);
		GameUIPtr->vertexwk[1].tex = D3DXVECTOR2(1.0f, Percent);
		GameUIPtr->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		GameUIPtr->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		break;

	case UIType_HPGauge_PlayerA:
		if (Percent == 0.0f)
		{
			Player = GetPlayer(Player_A);
			Percent = (float)Player->HP / Player->HP_Max;
		}
	case UIType_HPGauge_PlayerB:
		if (Percent == 0.0f)
		{
			Player = GetPlayer(Player_B);
			Percent = (float)Player->HP / Player->HP_Max;
		}
	case UIType_HPGauge_Boss:
		if (Percent == 0.0f)
		{
			Percent = (float)Boss->HP / Boss->HP_Max;
		}

		GameUIPtr->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		GameUIPtr->vertexwk[1].tex = D3DXVECTOR2(Percent, 0.0f);
		GameUIPtr->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		GameUIPtr->vertexwk[3].tex = D3DXVECTOR2(Percent, 1.0f);
		break;

	default:
		break;
	}


	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetGameUIVertex(GAMEUI *GameUIPtr)
{
	float Percent = 0.0f;
	// パーセント計算フラグ
	bool PercentCalFlag = false;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	switch (GameUIPtr->Type)
	{
	case UIType_ShotIcon_PlayerA:
	case UIType_ShotIcon_PlayerB:
	case UIType_DashIcon_PlayerA:
	case UIType_DashIcon_PlayerB:
	case UIType_HPGaugeBox_PlayerA:
	case UIType_HPGaugeBox_PlayerB:
	case UIType_HPGaugeBox_Boss:
	case UIType_Illust_PlayerA:
	case UIType_Illust_PlayerB:

		GameUIPtr->vertexwk[0].vtx = D3DXVECTOR3(GameUIPtr->pos.x, GameUIPtr->pos.y, 0.0f);
		GameUIPtr->vertexwk[1].vtx = D3DXVECTOR3(GameUIPtr->pos.x + GameUIPtr->Width, GameUIPtr->pos.y, 0.0f);
		GameUIPtr->vertexwk[2].vtx = D3DXVECTOR3(GameUIPtr->pos.x, GameUIPtr->pos.y + GameUIPtr->Height, 0.0f);
		GameUIPtr->vertexwk[3].vtx = D3DXVECTOR3(GameUIPtr->pos.x + GameUIPtr->Width, GameUIPtr->pos.y + GameUIPtr->Height, 0.0f);
		break;

		// クールダウン
	case UIType_ShotCD_PlayerA:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_A);
			Percent = (float)Player->ShotCount / ShotInterval;
			PercentCalFlag = true;
		}
	case UIType_DashCD_PlayerA:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_A);
			Percent = (float)Player->DashCDCount / DashCDInterval;
			PercentCalFlag = true;
		}
	case UIType_ShotCD_PlayerB:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_B);
			Percent = (float)Player->ShotCount / ShotInterval;
			PercentCalFlag = true;
		}
	case UIType_DashCD_PlayerB:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_B);
			Percent = (float)Player->DashCDCount / DashCDInterval;
			PercentCalFlag = true;
		}

		GameUIPtr->vertexwk[0].vtx = D3DXVECTOR3(GameUIPtr->pos.x, GameUIPtr->pos.y + Percent * GameUIPtr->Height, 0.0f);
		GameUIPtr->vertexwk[1].vtx = D3DXVECTOR3(GameUIPtr->pos.x + GameUIPtr->Width, GameUIPtr->pos.y + Percent * GameUIPtr->Height, 0.0f);
		GameUIPtr->vertexwk[2].vtx = D3DXVECTOR3(GameUIPtr->pos.x, GameUIPtr->pos.y + GameUIPtr->Height, 0.0f);
		GameUIPtr->vertexwk[3].vtx = D3DXVECTOR3(GameUIPtr->pos.x + GameUIPtr->Width, GameUIPtr->pos.y + GameUIPtr->Height, 0.0f);
		break;

		// HPゲージ
	case UIType_HPGauge_PlayerA:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_A);
			Percent = (float)Player->HP / Player->HP_Max;
			PercentCalFlag = true;
		}
	case UIType_HPGauge_PlayerB:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_B);
			Percent = (float)Player->HP / Player->HP_Max;
			PercentCalFlag = true;
		}
	case UIType_HPGauge_Boss:
		if (PercentCalFlag == false)
		{
			Percent = (float)Boss->HP / Boss->HP_Max;
			PercentCalFlag = true;
		}
		GameUIPtr->vertexwk[0].vtx = D3DXVECTOR3(GameUIPtr->pos.x, GameUIPtr->pos.y, 0.0f);
		GameUIPtr->vertexwk[1].vtx = D3DXVECTOR3(GameUIPtr->pos.x + Percent * GameUIPtr->Width, GameUIPtr->pos.y, 0.0f);
		GameUIPtr->vertexwk[2].vtx = D3DXVECTOR3(GameUIPtr->pos.x, GameUIPtr->pos.y + GameUIPtr->Height, 0.0f);
		GameUIPtr->vertexwk[3].vtx = D3DXVECTOR3(GameUIPtr->pos.x + Percent * GameUIPtr->Width, GameUIPtr->pos.y + GameUIPtr->Height, 0.0f);
		break;


	default:
		break;
	}


	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetGameUIDiffuse(GAMEUI *GameUIPtr, int Alpha)
{
	D3DXCOLOR Color = WHITE(Alpha);
	float Percent = 0.0f;
	// パーセント計算フラグ
	bool PercentCalFlag = false;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	switch (GameUIPtr->Type)
	{
	case UIType_ShotIcon_PlayerA:
	case UIType_ShotIcon_PlayerB:
	case UIType_DashIcon_PlayerA:
	case UIType_DashIcon_PlayerB:
	case UIType_HPGaugeBox_Boss:
	case UIType_ShotCD_PlayerA:
	case UIType_DashCD_PlayerA:
	case UIType_ShotCD_PlayerB:
	case UIType_DashCD_PlayerB:
	case UIType_Illust_PlayerA:
	case UIType_Illust_PlayerB:

		Color = WHITE(255);
		break;

	case UIType_HPGauge_PlayerA:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_A);
			Percent = (float)Player->HP / Player->HP_Max;
			PercentCalFlag = true;
		}
	case UIType_HPGauge_PlayerB:
		if (PercentCalFlag == false)
		{
			Player = GetPlayer(Player_B);
			Percent = (float)Player->HP / Player->HP_Max;
			PercentCalFlag = true;
		}

		if (Percent >= 0.6f)
		{
			Color = GREEN(255);
		}
		else if (Percent >= 0.2f && Percent < 0.6f)
		{
			Color = YELLOW(255);
		}
		else if (Percent < 0.2f)
		{
			Color = RED(255);
		}
		break;

	case UIType_HPGauge_Boss:
		if (PercentCalFlag == false)
		{
			Percent = (float)Boss->HP / Boss->HP_Max;
			PercentCalFlag = true;
		}

		if (Percent >= 0.67f)
		{
			Color = GREEN(255);
		}
		else if (Percent >= 0.33f && Percent < 0.67f)
		{
			Color = YELLOW(255);
		}
		else if (Percent < 0.33f)
		{
			Color = RED(255);
		}
		break;

	case UIType_HPGaugeBox_PlayerA:

		Color = AZURE(255);
		break;

	case UIType_HPGaugeBox_PlayerB:

		Color = GRASSGREEN(255);
		break;

	default:
		break;
	}

	GameUIPtr->vertexwk[0].diffuse = Color;
	GameUIPtr->vertexwk[1].diffuse = Color;
	GameUIPtr->vertexwk[2].diffuse = Color;
	GameUIPtr->vertexwk[3].diffuse = Color;

	return;
}

//=============================================================================
// ゲームUIの情報を取得する
//=============================================================================
GAMEUI *GetGameUI(int UIType)
{
	return &GameUI[UIType];
}
