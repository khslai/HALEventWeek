//=============================================================================
//
// プレイヤー処理 [Player->cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Effect.h"
#include "GameUI.h"
#include "AttackRange.h"
#include "Block.h"
#include "CheckHit.h"
#include "Door.h"
#include "GameClear.h"
#include "Score.h"
#include "MainMenu.h"
#include "Timer.h"
#include "Transition.h"
#include "Pause.h"
#include "Sound.h"
#include "Input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Player_A				_T("data/Texture/Player/Player_A.png")
#define Texture_Player_B				_T("data/Texture/Player/Player_B.png")
#define Texture_Player_Width_Total		(300)	// テクスチャの横幅
#define Texture_Player_Height_Total		(400)	// テクスチャの縦幅
#define Texture_Player_Divide_X			(3)		// プレイヤーのテクスチャ内分割数（横)
#define Texture_Player_Divide_Y			(4)		// プレイヤーのテクスチャ内分割数（縦)
#define Texture_Player_Width			(Texture_Player_Width_Total / Texture_Player_Divide_X)
#define Texture_Player_Height			(Texture_Player_Height_Total / Texture_Player_Divide_Y)

#define Texture_HitPoint				_T("data/Texture/Player/HitPoint.png")
#define Texture_HitPoint_Width			(20)
#define Texture_HitPoint_Height			(20)

// アニメーションを切り替えるカウント
#define AnimChangeCount					(15)
// プレイヤーの移動速度
#define MoveSpeed						(7.0f)
// プレイヤーのダッシュ速度
#define DashSpeed						(15.0f)
// プレイヤーの移動速度(氷の上)
#define MoveSpeed_OnIce					(10.0f)					
// プレイヤーの移動速度(OP、ED中)
#define MoveSpeed_OPED					(1.0f)					
// HP再生の時間
#define RegenerateInterval				(60)
// 二人のプレイヤーはこの距離内なら、HP再生する
#define RegenerateRange					(200)
// プレイヤーの当たり半径の係数
#define PlayerHitRadiusRate				(0.15f)
// プレイヤー無敵時間
#define InvincibleTime					(120)					
// 毎回HP再生の回復数値
#define RegenerateHP					(3.0f)
// 双子座のパワーアップ効果
#define RegenePowerUP					(1.5f)
// 低速移動の倍率
#define SlowModeSpeed					(0.6f)

// 1Pのスタート位置
#define PlayerA_Start_Pos_Futago		(D3DXVECTOR3(440.0f, 1080.0f, 0.0f))	
#define PlayerA_Start_Pos_Oushi			(D3DXVECTOR3(1250.0f, 380.0f, 0.0f))	
#define PlayerA_Start_Pos_Ite			(D3DXVECTOR3(900.0f, 1000.0f, 0.0f))	
#define PlayerA_Start_Pos_Shishi		(D3DXVECTOR3(480.0f, ScreenCenter_Y - 100.0f, 0.0f))	
// 2Pのスタート位置
#define PlayerB_Start_Pos_Futago		(D3DXVECTOR3(1385.0f, 1080.0f, 0.0f))	
#define PlayerB_Start_Pos_Oushi			(D3DXVECTOR3(1100.0f, 580.0f, 0.0f))	
#define PlayerB_Start_Pos_Ite			(D3DXVECTOR3(1100.0f, 1000.0f, 0.0f))	
#define PlayerB_Start_Pos_Shishi		(D3DXVECTOR3(480.0f, ScreenCenter_Y + 100.0f, 0.0f))

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakePlayerVertex(PLAYER *Player);
// 頂点座標の設定
void SetPlayerVertex(PLAYER *Player);
// テクスチャ座標の設定
void SetPlayerTexture(PLAYER *Player);
// プレイヤー移動
void PlayerMove(int Player_No);
// プレイヤー移動(氷の床の上)
void PlayerMove_OnIce(int Player_No);
// 壁、障害物との当たり判定
void MoveCheck(int Player_No);
// 双子座のオープニング
void PlayOpening(void);
// プレイヤーと障害物の当たり判定
bool BlockCheck(int Player_No);
// プレイヤーとドアの当たり判定処理
bool CheckHitDoor(int Player_No);
// EDのアニメーション(プレイヤーが画面を出るアニメ)を再生する関数 
void PlayEnding(int Player_No);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 HitPoint_Texture = NULL;
// プレイヤー構造体
PLAYER Player[Player_Max];
// OP終了フラグ
bool OpeningOver = false;
// ED終了フラグ
bool EndingOver = false;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(bool FirstInit)
{
	int Player_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(Player_No);

	for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
	{
		switch (GameStage)
		{
		case Stage_Futago:
			OpeningOver = false;
			EndingOver = false;

			if (Player_No == Player_A)
			{
				Player->pos = PlayerA_Start_Pos_Futago;
				Player->Direction = Up;
			}
			else
			{
				Player->pos = PlayerB_Start_Pos_Futago;
				Player->Direction = Up;
			}
			break;
		case Stage_Oushi:
			if (Player_No == Player_A)
			{
				Player->pos = PlayerA_Start_Pos_Oushi;
				Player->Direction = Left;
			}
			else
			{
				Player->pos = PlayerB_Start_Pos_Oushi;
				Player->Direction = Left;
			}
			break;
		case Stage_Ite:
			if (Player_No == Player_A)
			{
				Player->pos = PlayerA_Start_Pos_Ite;
				Player->Direction = Up;
			}
			else
			{
				Player->pos = PlayerB_Start_Pos_Ite;
				Player->Direction = Up;
			}
			break;
		case Stage_Shishi:
			if (Player_No == Player_A)
			{
				Player->pos = PlayerA_Start_Pos_Shishi;
				Player->Direction = Right;
			}
			else
			{
				Player->pos = PlayerB_Start_Pos_Shishi;
				Player->Direction = Right;
			}
			break;
		default:
			break;
		}

		Player->Count = 0;
		Player->ShotCount = 0;
		Player->DashCDCount = 0;
		Player->InvincibleCount = 0;
		Player->AnimPattern = 0;
		Player->Speed = MoveSpeed;
		Player->HP_Max = 100.0f;
		Player->HP = Player->HP_Max;
		Player->SlowSpeed = 1.0f;
		Player->RegeneBonus = 0.0f;
		Player->InDash = false;
		Player->InFlash = false;
		Player->InSlash = false;
		Player->InMove = false;
		Player->Use = true;

		// 頂点情報の作成
		MakePlayerVertex(Player);
	}

	if (FirstInit == true)
	{
		// テクスチャの読み込み
		// プレイヤーA
		Player = GetPlayer(Player_A);
		Player->BaseAngle = atan2f(Texture_Player_Height, Texture_Player_Width);
		Player->Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Player_Width / 2, Texture_Player_Height / 2));
		Player->HitRadius = Player->Radius * PlayerHitRadiusRate;
		if (SafeLoad(Texture_Player_A, &Player->texture, "Player_A") == false)
		{
			return E_FAIL;
		}

		// プレイヤーB
		Player = GetPlayer(Player_B);
		Player->BaseAngle = atan2f(Texture_Player_Height, Texture_Player_Width);
		Player->Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Player_Width / 2, Texture_Player_Height / 2));
		Player->HitRadius = Player->Radius * PlayerHitRadiusRate;
		if (SafeLoad(Texture_Player_B, &Player->texture, "Player_B") == false)
		{
			return E_FAIL;
		}

		// 当たりポイント
		Player = GetPlayer(Player_A);
		Player->HitPoint.BaseAngle = atan2f(Texture_HitPoint_Height, Texture_HitPoint_Width);
		Player->HitPoint.Radius = Player->HitRadius * Root2;
		Player = GetPlayer(Player_B);
		Player->HitPoint.BaseAngle = atan2f(Texture_HitPoint_Height, Texture_HitPoint_Width);
		Player->HitPoint.Radius = Player->HitRadius * Root2;
		if (SafeLoad(Texture_HitPoint, &HitPoint_Texture, "HitPoint") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int Player_No = 0; Player_No < Player_Max; Player_No++)
	{
		SafeRelease(Player->texture);
	}
	SafeRelease(HitPoint_Texture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	int Player_No = 0;
	float Distance = 0.0f;
	static int RegenerateCount = 0;
	int GameStage = GetGameStage();
	// プレイヤーの死亡人数
	int PlayerDead = 0;
	PLAYER *Player = GetPlayer(Player_No);
	GAMEUI *GameUI = GetGameUI(0);
	DOOR *Door = GetDoor(0);

	switch (GameStage)
	{
	case Stage_Futago:

		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			if (Player->Use == true)
			{
				Player->PreviousPos = Player->pos;

				if (OpeningOver == false)
				{
					// オープニング
					PlayOpening();
				}
				// プレイヤーがドアを入ろうとしている、EDのアニメーションを再生
				else if (CheckHitDoor(Player_No) == true && Player->Direction == Up)
				{
					Door = GetDoor(Player_No);
					if (Door->State != DoorOpenEnd)
					{
						Door->State = DoorOpening;
					}
					else if (Door->State == DoorOpenEnd)
					{
						PlayEnding(Player_No);
					}
				}
				else
				{
					// プレイヤー移動
					if (Player->flagOnIce == false)
					{
						PlayerMove(Player_No);
					}
					else
					{
						PlayerMove_OnIce(Player_No);
					}
				}

				// 氷面
				if (Player->pos.y > 70.0f && Player->pos.y < 960.0f)
				{
					Player->flagOnIce = true;
				}
				// 地面
				else
				{
					Player->flagOnIce = false;
				}

				// テクスチャアニメパターン計算
				if ((Player->Count % AnimChangeCount) == 0 && Player->InMove == true && Player->flagOnIce == false)
				{
					Player->AnimPattern = (Player->AnimPattern + 1) % Texture_Player_Divide_X;
				}

				// テクスチャ座標更新
				SetPlayerTexture(Player);
				// 頂点座標更新
				SetPlayerVertex(Player);
			}
		}

		// ステージクリア
		if (EndingOver == true)
		{
			// クリア画像表示
			SetStageClear(true);
			// ステージクリアフラグ設置
			SetClearFlag(ClearFlag_Futago);
		}
		// ゲームオーバー
		if (GetEndCDOver() == true)
		{
			SetTransition(Fadein);
		}

		break;

	case Stage_Oushi:

		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			if (Player->Use == true)
			{
				// テクスチャアニメパターン計算
				if ((Player->Count % AnimChangeCount) == 0 && Player->InMove == true)
				{
					Player->AnimPattern = (Player->AnimPattern + 1) % Texture_Player_Divide_X;
				}

				// プレイヤー移動
				PlayerMove(Player_No);

				// 撃たれた、無敵状態になる
				if (Player->InvincibleCount > 0)
				{
					Player->InvincibleCount++;
					// 2秒後
					if (Player->InvincibleCount >= InvincibleTime)
					{
						Player->InvincibleCount = 0;
					}
				}

				// テクスチャ座標更新
				SetPlayerTexture(Player);
				// 頂点座標更新
				SetPlayerVertex(Player);
			}
			// ゲームオーバー
			else if (Player->Use == false && Player->HP == 0.0f)
			{
				PlayerDead++;
				if (PlayerDead == Player_Max)
				{
					SetTransition(Fadein);
				}
			}
		}

		// ステージクリア
		if (GetEndCDOver() == true)
		{
			// クリア画像表示
			SetStageClear(true);
			// ステージクリアフラグ設置
			SetClearFlag(ClearFlag_Oushi);
		}
		break;

	case Stage_Ite:

		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			// テクスチャアニメパターン計算
			if ((Player->Count % AnimChangeCount) == 0 && Player->InMove == true)
			{
				Player->AnimPattern = (Player->AnimPattern + 1) % Texture_Player_Divide_X;
			}

			// プレイヤー移動
			PlayerMove(Player_No);

			if (Player_No == Player_A)
			{
				// 弾発射
				if (Player->ShotCount == 0)
				{
					if ((GetKeyboardPress(DIK_SPACE) || IsButtonPressed(Player_A, BUTTON_B)) && Player_No == Player_A && Player->Direction == Up)
					{
						PlayerShot(Player_No);
						Player->ShotCount++;
					}
				}
				else
				{
					Player->ShotCount++;
					// クールダウン終了、発射可能
					if (Player->ShotCount % ShotInterval == 0)
					{
						Player->ShotCount = 0;
						if (Player_No == Player_A)
						{
							GameUI = GetGameUI(UIType_ShotCD_PlayerA);
							GameUI->Use = false;
						}
					}
				}
			}
			else if (Player_No == Player_B)
			{
				// 斬撃
				if (Player->ShotCount == 0)
				{
					if ((IsMouseLeftPressed() || IsButtonPressed(Player_B, BUTTON_B)) && Player_No == Player_B)
					{
						SetSound(SE_Slash, E_DS8_FLAG_NONE, true, false);
						Player->ShotCount++;
						Player->InSlash = true;
						if (Player->Direction == Left)
						{
							SetEffect(Player_B, Slash, D3DXVECTOR3(Player->pos.x - 150.0f, Player->pos.y, Player->pos.z));
						}
						else if (Player->Direction == Right)
						{
							SetEffect(Player_B, Slash, D3DXVECTOR3(Player->pos.x + 150.0f, Player->pos.y, Player->pos.z));
						}
					}
				}
				else
				{
					Player->ShotCount++;
					// クールダウン終了、攻撃可能
					if (Player->ShotCount % SlashInterval == 0)
					{
						Player->ShotCount = 0;
					}
				}
			}

			// ステージクリア
			if ((Player_No == Player_A && GetScore() == 20))
			{
				// クリア画像表示
				SetStageClear(true);
				// ステージクリアフラグ設置
				SetClearFlag(ClearFlag_Ite);
			}

			// テクスチャ座標更新
			SetPlayerTexture(Player);
			// 頂点座標更新
			SetPlayerVertex(Player);
		}

		break;

	case Stage_Shishi:

		if (Player[Player_A].Use == true && Player[Player_B].Use == true)
		{
			// 二人のプレイヤー間の距離を計算する
			Distance = D3DXVec3LengthSq(&D3DXVECTOR3(Player[Player_A].pos - Player[Player_B].pos));
			// 一定の距離内なら、HP再生する
			if (Distance < pow(RegenerateRange, 2))
			{
				RegenerateCount++;
				Player[Player_A].RegeneBonus = RegenePowerUP;
				Player[Player_B].RegeneBonus = RegenePowerUP;
				if (RegenerateCount % RegenerateInterval == 0)
				{
					SetSound(SE_HPRestore, E_DS8_FLAG_NONE, true, false);
					Player[Player_A].HP += RegenerateHP;
					Player[Player_B].HP += RegenerateHP;
					if (Player[Player_A].HP >= Player[Player_A].HP_Max)
					{
						Player[Player_A].HP = Player[Player_A].HP_Max;
					}
					if (Player[Player_B].HP >= Player[Player_B].HP_Max)
					{
						Player[Player_B].HP = Player[Player_B].HP_Max;
					}
					SetEffect(Player_A, Regenerate, Player[Player_A].pos);
					SetEffect(Player_B, Regenerate, Player[Player_B].pos);
				}
			}
			else
			{
				RegenerateCount = 0;
				Player[Player_A].RegeneBonus = 0.0f;
				Player[Player_B].RegeneBonus = 0.0f;
			}
		}

		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			if (Player->Use == true)
			{
				// テクスチャアニメパターン計算
				if ((Player->Count % AnimChangeCount) == 0 && Player->InMove == true)
				{
					Player->AnimPattern = (Player->AnimPattern + 1) % Texture_Player_Divide_X;
				}

				// プレイヤー移動
				PlayerMove(Player_No);

				// 撃たれた、無敵状態になる
				if (Player->InvincibleCount > 0)
				{
					Player->InvincibleCount++;
					// 2秒後
					if (Player->InvincibleCount >= InvincibleTime)
					{
						Player->InvincibleCount = 0;
					}
				}

				// 弾発射
				if (Player->ShotCount == 0)
				{
					if ((GetKeyboardPress(DIK_SPACE) || IsButtonPressed(Player_A, BUTTON_B)) && Player_No == Player_A)
					{
						PlayerShot(Player_No);
						Player->ShotCount++;
					}
					if ((IsMouseLeftPressed() || IsButtonPressed(Player_B, BUTTON_B)) && Player_No == Player_B)
					{
						PlayerShot(Player_No);
						Player->ShotCount++;
					}
				}
				else
				{
					Player->ShotCount++;
					// クールダウン終了、発射可能
					if (Player->ShotCount % ShotInterval == 0)
					{
						Player->ShotCount = 0;
						if (Player_No == Player_A)
						{
							GameUI = GetGameUI(UIType_ShotCD_PlayerA);
							GameUI->Use = false;
						}
						else if (Player_No == Player_B)
						{
							GameUI = GetGameUI(UIType_ShotCD_PlayerB);
							GameUI->Use = false;
						}
					}
				}

				// ダッシュ
				if (Player->DashCDCount == 0)
				{
					if ((GetKeyboardPress(DIK_E) || IsButtonPressed(Player_A, BUTTON_R1)) && Player_No == Player_A)
					{
						Player->InDash = true;
						Player->DashCDCount++;
						GameUI = GetGameUI(UIType_DashCD_PlayerA);
						GameUI->Use = true;
					}
					if ((IsMouseRightPressed() || IsButtonPressed(Player_B, BUTTON_R1)) && Player_No == Player_B)
					{
						Player->InDash = true;
						Player->DashCDCount++;
						GameUI = GetGameUI(UIType_DashCD_PlayerB);
						GameUI->Use = true;
					}
				}
				else
				{
					Player->DashCDCount++;
					// ダッシュ終了
					if (Player->InDash == true && Player->DashCDCount % DashTime == 0)
					{
						Player->InDash = false;
					}
					// クールダウン終了
					if (Player->DashCDCount % DashCDInterval == 0)
					{
						Player->DashCDCount = 0;
						if (Player_No == Player_A)
						{
							GameUI = GetGameUI(UIType_DashCD_PlayerA);
							GameUI->Use = false;
						}
						else if (Player_No == Player_B)
						{
							GameUI = GetGameUI(UIType_DashCD_PlayerB);
							GameUI->Use = false;
						}
					}
				}

				// 低速移動
				if (IsButtonPressed(Player_No, BUTTON_L1))
				{
					Player->SlowSpeed = SlowModeSpeed;
				}
				else
				{
					Player->SlowSpeed = 1.0f;
				}

				// テクスチャ座標更新
				SetPlayerTexture(Player);
				// 頂点座標更新
				SetPlayerVertex(Player);
			}
			// ゲームオーバー
			else if (Player->Use == false && Player->HP == 0.0f)
			{
				PlayerDead++;
				if (PlayerDead == Player_Max)
				{
					SetTransition(Fadein);
				}
			}
		}
		break;

	default:
		break;
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Player_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(Player_No);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
	{
		if (Player->Use == true)
		{
			// 撃たれた、無敵カウント始まる
			if (Player->InvincibleCount > 0)
			{
				// 5カウントずつ、描画する←→描画しないの状態を切り替える
				if (Player->InvincibleCount % 5 == 0)
				{
					Player->InFlash = Player->InFlash == false ? true : false;
				}
			}
			// 普通状態
			else
			{
				Player->InFlash = false;
			}

			if (Player->InFlash == false)
			{
				// テクスチャの設定
				Device->SetTexture(0, Player->texture);

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->vertexwk, sizeof(VERTEX_2D));

				if (Player->InDash != true && GameStage == Stage_Shishi)
				{
					// テクスチャの設定
					Device->SetTexture(0, HitPoint_Texture);

					// ポリゴンの描画
					Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->HitPoint.vertexwk, sizeof(VERTEX_2D));
				}
			}
		}
	}

	return;
}

//=============================================================================
// プレイヤー移動
//=============================================================================
void PlayerMove(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	int GameStage = GetGameStage();
	bool UpDownMove = false;
	bool LeftRightMove = false;

	if (GameStage != Stage_Ite)
	{
		// プレイヤー上下移動
		if ((Player_No == Player_A && GetKeyboardPress(DIK_W)) ||
			(Player_No == Player_B && GetKeyboardPress(DIK_UP)) ||
			IsButtonPressed(Player_No, BUTTON_UP))
		{
			Player->Direction = Up;
			Player->InMove = true;
			UpDownMove = true;

			// 一般移動
			if (Player->InDash == false)
			{
				Player->pos.y -= Player->Speed * Player->SlowSpeed;
			}
			// ダッシュ
			else
			{
				Player->pos.y -= DashSpeed;
			}
		}
		else if ((Player_No == Player_A && GetKeyboardPress(DIK_S)) ||
			(Player_No == Player_B && GetKeyboardPress(DIK_DOWN)) ||
			IsButtonPressed(Player_No, BUTTON_DOWN))
		{
			Player->Direction = Down;
			Player->InMove = true;
			UpDownMove = true;

			// 一般移動
			if (Player->InDash == false)
			{
				Player->pos.y += Player->Speed * Player->SlowSpeed;
			}
			// ダッシュ
			else
			{
				Player->pos.y += DashSpeed;
			}
		}
	}

	// プレイヤー左右移動
	if ((Player_No == Player_A && GetKeyboardPress(DIK_A)) ||
		(Player_No == Player_B && GetKeyboardPress(DIK_LEFT)) ||
		IsButtonPressed(Player_No, BUTTON_LEFT))
	{
		Player->Direction = Left;
		Player->InMove = true;
		LeftRightMove = true;

		// 一般移動
		if (Player->InDash == false)
		{
			Player->pos.x -= Player->Speed * Player->SlowSpeed;
		}
		// ダッシュ
		else
		{
			Player->pos.x -= DashSpeed;
		}
	}
	else if ((Player_No == Player_A && GetKeyboardPress(DIK_D)) ||
		(Player_No == Player_B && GetKeyboardPress(DIK_RIGHT)) ||
		IsButtonPressed(Player_No, BUTTON_RIGHT))
	{
		Player->Direction = Right;
		Player->InMove = true;
		LeftRightMove = true;

		// 一般移動
		if (Player->InDash == false)
		{
			Player->pos.x += Player->Speed * Player->SlowSpeed;
		}
		// ダッシュ
		else
		{
			Player->pos.x += DashSpeed;
		}
	}

	// 移動しない
	if (UpDownMove == false && LeftRightMove == false)
	{
		Player->InMove = false;
		if (GameStage == Stage_Ite && Player_No == Player_A)
		{
			Player->Direction = Up;
		}
	}
	else
	{
		Player->Count++;
	}

	//一時停止
	if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(Player_No, BUTTON_START))
	{
		SetPauseFlag(true);
	}

	// 壁、障害物との当たり判定
	MoveCheck(Player_No);

	return;
}

//=============================================================================
// プレイヤー移動(氷の床の上)
//=============================================================================
void PlayerMove_OnIce(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	int GameStage = GetGameStage();

	// プレイヤーは氷の床の上にいる
	if (Player->flagOnIce == true)
	{
		// 滑っている
		if (Player->flagSlide == true)
		{
			if (Player->Direction == Up)
			{
				Player->pos.y -= MoveSpeed_OnIce;
			}
			else if (Player->Direction == Down)
			{
				Player->pos.y += MoveSpeed_OnIce;
			}

			if (Player->Direction == Left)
			{
				Player->pos.x -= MoveSpeed_OnIce;
			}
			else if (Player->Direction == Right)
			{
				Player->pos.x += MoveSpeed_OnIce;
			}
		}
		// 氷の床の上にいるが、滑っていない
		else if (Player->flagSlide == false)
		{
			// プレイヤー上下移動
			if ((Player_No == Player_A && GetKeyboardPress(DIK_W)) ||
				(Player_No == Player_B && GetKeyboardPress(DIK_UP)) ||
				IsButtonPressed(Player_No, BUTTON_UP))
			{
				Player->Direction = Up;
				Player->flagSlide = true;
			}
			else if ((Player_No == Player_A && GetKeyboardPress(DIK_S)) ||
				(Player_No == Player_B && GetKeyboardPress(DIK_DOWN)) ||
				IsButtonPressed(Player_No, BUTTON_DOWN))
			{
				Player->Direction = Down;
				Player->flagSlide = true;
			}

			// プレイヤー左右移動
			if ((Player_No == Player_A && GetKeyboardPress(DIK_A)) ||
				(Player_No == Player_B && GetKeyboardPress(DIK_LEFT)) ||
				IsButtonPressed(Player_No, BUTTON_LEFT))
			{
				Player->Direction = Left;
				Player->flagSlide = true;
			}
			else if ((Player_No == Player_A && GetKeyboardPress(DIK_D)) ||
				(Player_No == Player_B && GetKeyboardPress(DIK_RIGHT)) ||
				IsButtonPressed(Player_No, BUTTON_RIGHT))
			{
				Player->Direction = Right;
				Player->flagSlide = true;
			}
		}
	}

	// 壁、障害物との当たり判定
	MoveCheck(Player_No);

	return;
}

//=============================================================================
// 壁、障害物との当たり判定
//=============================================================================
void MoveCheck(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	int GameStage = GetGameStage();

	if (GameStage == Stage_Ite || GameStage == Stage_Shishi)
	{
		// 左の壁
		if (Player->pos.x < Texture_Player_Width / 2)
		{
			Player->pos.x = Texture_Player_Width / 2;
		}
		// 右の壁
		else if (Player->pos.x > Screen_Width - (Texture_Player_Width / 2))
		{
			Player->pos.x = Screen_Width - (Texture_Player_Width / 2);
		}
		// 上の壁
		if (Player->pos.y < Texture_Player_Height / 2)
		{
			Player->pos.y = Texture_Player_Height / 2;
		}
		// 下の壁
		else if (Player->pos.y > Screen_Height - (Texture_Player_Height / 2))
		{
			Player->pos.y = Screen_Height - (Texture_Player_Height / 2);
		}
	}
	else if (GameStage == Stage_Oushi)
	{
		// 左の壁
		if (Player->pos.x < Texture_Player_Width / 2 + Wall_Width)
		{
			Player->pos.x = Texture_Player_Width / 2 + Wall_Width;
		}
		// 右の壁
		else if (Player->pos.x > Screen_Width - (Texture_Player_Width / 2 + Wall_Width))
		{
			Player->pos.x = Screen_Width - (Texture_Player_Width / 2 + Wall_Width);
		}
		// 上の壁
		if (Player->pos.y < Texture_Player_Height / 2 + Wall_Width)
		{
			Player->pos.y = Texture_Player_Height / 2 + Wall_Width;
		}
		// 下の壁
		else if (Player->pos.y > Screen_Height - (Texture_Player_Height / 2 + Wall_Width))
		{
			Player->pos.y = Screen_Height - (Texture_Player_Height / 2 + Wall_Width);
		}
	}
	else if (GameStage == Stage_Futago)
	{
		// プレイヤーが画面外に出ようとしたら、元の位置に戻る
		if (Player_No == Player_A)
		{
			if (Player->pos.x > ScreenCenter_X - (Texture_Player_Width + Wall_Width) / 2 ||
				Player->pos.x < Texture_Player_Width / 2 + Wall_Width)
			{
				Player->pos.x = Player->PreviousPos.x;
				Player->flagSlide = false;
			}
			if (Player->pos.y > Screen_Height - Texture_Player_Height / 2 ||
				Player->pos.y < Texture_Player_Height / 2 + Wall_Width)
			{
				Player->pos.y = Player->PreviousPos.y;
				Player->flagSlide = false;
			}
		}
		else if (Player_No == Player_B)
		{
			if (Player->pos.x < ScreenCenter_X + (Texture_Player_Width + Wall_Width) / 2 ||
				Player->pos.x > Screen_Width - Texture_Player_Width / 2 - Wall_Width)
			{
				Player->pos.x = Player->PreviousPos.x;
				Player->flagSlide = false;
			}
			if (Player->pos.y > Screen_Height - Texture_Player_Height / 2 ||
				Player->pos.y < Texture_Player_Height / 2 + Wall_Width)
			{
				Player->pos.y = Player->PreviousPos.y;
				Player->flagSlide = false;
			}
		}

		// プレイヤーが障害物にぶつかったら、位置座標を更新しない
		if (BlockCheck(Player_No) == true && Player->Direction == Player->dirBlock)
		{
			Player->pos = Player->PreviousPos;
			Player->flagSlide = false;
		}
	}

	return;
}

//=============================================================================
// プレイヤーと障害物の当たり判定
//=============================================================================
bool BlockCheck(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	D3DXVECTOR2 Obj1Size = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 Obj2Size = D3DXVECTOR2(0.0f, 0.0f);
	// 障害物のポインターを初期化
	BLOCK *Block = GetBlock(0);
	int Block_No = 0;

	Obj1Size = GetPlayerSize();
	Obj2Size = GetBlockSize();
	for (Block_No = 0; Block_No < Block_Max; Block_No++, Block++)
	{
		if (CheckHitBB(Player->pos, Block->pos, Obj1Size, Obj2Size) == true)
		{
			// 障害物にぶつかった時、プレイヤーの向きを記録
			Player->dirBlock = Player->Direction;
			SetSound(SE_HitBlock, E_DS8_FLAG_NONE, true, false);
			return true;
		}
	}

	return false;
}

//=============================================================================
// プレイヤーとドアの当たり判定処理
//=============================================================================
bool CheckHitDoor(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	D3DXVECTOR2 Obj1Size = GetPlayerSize();
	D3DXVECTOR2 Obj2Size = GetDoorSize();
	DOOR *Door = GetDoor(Player_No);

	if (CheckHitBB(Player->pos, Door->pos, Obj1Size, Obj2Size) == true)
	{
		return true;
	}

	return false;
}

//=============================================================================
// OPのアニメーション(プレイヤーが画面内に入るアニメ)を再生する関数
//=============================================================================
void PlayOpening(void)
{
	int Player_No = 0;
	PLAYER *Player = GetPlayer(Player_No);

	for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
	{
		Player->pos.y -= MoveSpeed_OPED;
		Player->Direction = Up;
		Player->Count++;
		Player->InMove = true;

		// プレイヤーがスタート位置に着いたら、歩くアニメーションの再生を止める、OPのアニメーション再生も止める
		if (Player->pos.y <= 1000.0f)
		{
			OpeningOver = true;
		}
	}
}

//=============================================================================
// EDのアニメーション(プレイヤーが画面を出るアニメ)を再生する関数 
//=============================================================================
void PlayEnding(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	static int ClearNum = 0;

	// プレイヤーが画面を出るアニメーションの処理
	Player->pos.y -= MoveSpeed_OPED;
	Player->Direction = Up;
	Player->Count++;
	Player->InMove = true;

	// プレイヤーが画面外を出たら、歩くアニメーションの再生を止める、ED_1Pのアニメーション再生も止める
	if (Player->pos.y <= 0.0f)
	{
		ClearNum++;
		if (ClearNum == Player_Max)
		{
			ClearNum = 0;
			EndingOver = true;
		}
		Player->Use = false;
	}

	return;
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakePlayerVertex(PLAYER *Player)
{
	// 頂点座標の設定	
	SetPlayerTexture(Player);

	// rhwの設定
	Player->vertexwk[0].rhw = 1.0f;
	Player->vertexwk[1].rhw = 1.0f;
	Player->vertexwk[2].rhw = 1.0f;
	Player->vertexwk[3].rhw = 1.0f;

	Player->HitPoint.vertexwk[0].rhw = 1.0f;
	Player->HitPoint.vertexwk[1].rhw = 1.0f;
	Player->HitPoint.vertexwk[2].rhw = 1.0f;
	Player->HitPoint.vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Player->vertexwk[0].diffuse = WHITE(255);
	Player->vertexwk[1].diffuse = WHITE(255);
	Player->vertexwk[2].diffuse = WHITE(255);
	Player->vertexwk[3].diffuse = WHITE(255);

	Player->HitPoint.vertexwk[0].diffuse = WHITE(255);
	Player->HitPoint.vertexwk[1].diffuse = WHITE(255);
	Player->HitPoint.vertexwk[2].diffuse = WHITE(255);
	Player->HitPoint.vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetPlayerVertex(Player);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetPlayerTexture(PLAYER *Player)
{
	int GameStage = GetGameStage();
	int x = Player->AnimPattern;
	int y = Player->Direction;
	float sizeX = 1.0f / Texture_Player_Divide_X;
	float sizeY = 1.0f / Texture_Player_Divide_Y;

	// プレイヤー
	Player->vertexwk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	Player->vertexwk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	Player->vertexwk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	Player->vertexwk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	// 当たりポイント
	if (GameStage == Stage_Shishi)
	{
		Player->HitPoint.vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Player->HitPoint.vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Player->HitPoint.vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Player->HitPoint.vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetPlayerVertex(PLAYER *Player)
{
	int GameStage = GetGameStage();

	// プレイヤー
	Player->vertexwk[0].vtx.x = Player->pos.x - cosf(Player->BaseAngle) * Player->Radius;
	Player->vertexwk[0].vtx.y = Player->pos.y - sinf(Player->BaseAngle) * Player->Radius;
	Player->vertexwk[1].vtx.x = Player->pos.x + cosf(Player->BaseAngle) * Player->Radius;
	Player->vertexwk[1].vtx.y = Player->pos.y - sinf(Player->BaseAngle) * Player->Radius;
	Player->vertexwk[2].vtx.x = Player->pos.x - cosf(Player->BaseAngle) * Player->Radius;
	Player->vertexwk[2].vtx.y = Player->pos.y + sinf(Player->BaseAngle) * Player->Radius;
	Player->vertexwk[3].vtx.x = Player->pos.x + cosf(Player->BaseAngle) * Player->Radius;
	Player->vertexwk[3].vtx.y = Player->pos.y + sinf(Player->BaseAngle) * Player->Radius;

	// 当たりポイント
	if (GameStage == Stage_Shishi)
	{
		Player->HitPoint.vertexwk[0].vtx.x = Player->pos.x - cosf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
		Player->HitPoint.vertexwk[0].vtx.y = Player->pos.y - sinf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
		Player->HitPoint.vertexwk[1].vtx.x = Player->pos.x + cosf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
		Player->HitPoint.vertexwk[1].vtx.y = Player->pos.y - sinf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
		Player->HitPoint.vertexwk[2].vtx.x = Player->pos.x - cosf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
		Player->HitPoint.vertexwk[2].vtx.y = Player->pos.y + sinf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
		Player->HitPoint.vertexwk[3].vtx.x = Player->pos.x + cosf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
		Player->HitPoint.vertexwk[3].vtx.y = Player->pos.y + sinf(Player->HitPoint.BaseAngle) * Player->HitPoint.Radius;
	}

	return;
}

//=============================================================================
// プレイヤーの情報を取得する
//=============================================================================
PLAYER *GetPlayer(int Player_No)
{
	return &Player[Player_No];
}

//=============================================================================
// プレイヤーのテクスチャのサイズを取得する
//=============================================================================
D3DXVECTOR2 GetPlayerSize(void)
{
	return D3DXVECTOR2(Texture_Player_Width - 40, Texture_Player_Height - 40);
}

