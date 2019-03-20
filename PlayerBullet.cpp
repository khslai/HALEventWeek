//=============================================================================
//
// プレイヤー弾幕処理 [PlayerBullet.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "PlayerBullet.h"
#include "Player.h"
#include "Boss.h"
#include "GameUI.h"
#include "AttackRange.h"
#include "Effect.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_PlayerBullet			_T("data/Texture/Bullet/Arrow.png")
#define Texture_PlayerBullet_Width		(60)
#define Texture_PlayerBullet_Height		(100)
// プレイヤーの弾の移動速度
#define PlayerBulletSpeed				(20.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBulletVertex(BULLET *Bullet);
// 頂点座標の設定
static void SetBulletVertex(BULLET *Bullet);
// テクスチャ座標の設定
static void SetBulletTexture(BULLET *Bullet);
// プレイヤーの弾の移動
static void BulletMove(BULLET *Bullet);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 Bullet_Texture = NULL;
// 画像中心から頂点までの角度
static float BaseAngle = 0.0f;
// 画像中心から頂点までの距離
static float Radius = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayerBullet(bool FirstInit)
{
	int Bullet_No = 0;
	int Player_No = 0;
	PLAYER *Player = GetPlayer(Player_No);

	for (Player_No = 0; Player_No < Player_Max; Player_No++)
	{
		Player = GetPlayer(Player_No);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			Player->Bullet[Bullet_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].MoveDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].Radius = 0.0f;
			Player->Bullet[Bullet_No].HitRadius = 0.0f;
			Player->Bullet[Bullet_No].BaseAngle = 0.0f;
			Player->Bullet[Bullet_No].Speed = 0.0f;
			Player->Bullet[Bullet_No].Count = 0;
			Player->Bullet[Bullet_No].Color = 0;
			Player->Bullet[Bullet_No].State = 0;
			Player->Bullet[Bullet_No].Use = false;
			Player->Bullet[Bullet_No].DelayDisappear = false;

			// 頂点情報の作成
			MakeBulletVertex(&Player->Bullet[Bullet_No]);
		}
	}

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_PlayerBullet_Width / 2, Texture_PlayerBullet_Height / 2));
		BaseAngle = atan2f(Texture_PlayerBullet_Height, Texture_PlayerBullet_Width);

		// テクスチャの読み込み
		if (SafeLoad(Texture_PlayerBullet, &Bullet_Texture, "PlayerBullet") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayerBullet(void)
{
	SafeRelease(Bullet_Texture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayerBullet(void)
{
	int Bullet_No = 0;
	int Player_No = 0;
	PLAYER *Player = GetPlayer(Player_No);

	for (Player_No = 0; Player_No < Player_Max; Player_No++)
	{
		Player = GetPlayer(Player_No);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet[Bullet_No].Use == true)
			{
				BulletMove(&Player->Bullet[Bullet_No]);

				// 頂点座標の更新
				SetBulletVertex(&Player->Bullet[Bullet_No]);
			}
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Player_No = 0;
	int Bullet_No = 0;
	PLAYER *Player = GetPlayer(Player_No);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, Bullet_Texture);

	for (Player_No = 0; Player_No < Player_Max; Player_No++)
	{
		Player = GetPlayer(Player_No);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet[Bullet_No].Use == true)
			{
				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->Bullet[Bullet_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}

//=============================================================================
// プレイヤーの弾の移動
//=============================================================================
void BulletMove(BULLET *Bullet)
{
	if (Bullet->DelayDisappear == true)
	{
		Bullet->Use = false;
	}
	else
	{
		// 1フレイム前の弾座標を記録
		Bullet->PreviousPos = Bullet->pos;
		// 座標更新
		Bullet->pos += Bullet->MoveDirection * Bullet->Speed;
		// カウント加算
		Bullet->Count++;

		// 弾の移動先を検査
		if (CheckPosition(Bullet->pos, Texture_PlayerBullet_Width, Texture_PlayerBullet_Height) == true)
		{
			Bullet->Use = false;
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBulletVertex(BULLET *Bullet)
{

	// 頂点座標の設定	
	SetBulletVertex(Bullet);

	// rhwの設定
	Bullet->vertexwk[0].rhw = 1.0f;
	Bullet->vertexwk[1].rhw = 1.0f;
	Bullet->vertexwk[2].rhw = 1.0f;
	Bullet->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Bullet->vertexwk[0].diffuse = WHITE(255);
	Bullet->vertexwk[1].diffuse = WHITE(255);
	Bullet->vertexwk[2].diffuse = WHITE(255);
	Bullet->vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetBulletTexture(Bullet);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBulletTexture(BULLET *Bullet)
{
	Bullet->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Bullet->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Bullet->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Bullet->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBulletVertex(BULLET *Bullet)
{
	Bullet->vertexwk[0].vtx.x = Bullet->pos.x - cosf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[0].vtx.y = Bullet->pos.y - sinf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[1].vtx.x = Bullet->pos.x + cosf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[1].vtx.y = Bullet->pos.y - sinf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[2].vtx.x = Bullet->pos.x - cosf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[2].vtx.y = Bullet->pos.y + sinf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[3].vtx.x = Bullet->pos.x + cosf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[3].vtx.y = Bullet->pos.y + sinf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;

	return;
}

//=============================================================================
// プレイヤー弾の設置
//=============================================================================
void PlayerShot(int Player_No)
{
	int Bullet_No = 0;
	PLAYER *Player = GetPlayer(Player_No);
	GAMEUI *GameUI = GetGameUI(0);

	// プレイヤーショット
	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == false)
		{
			Player->Bullet[Bullet_No].Use = true;
			Player->Bullet[Bullet_No].DelayDisappear = false;
			Player->Bullet[Bullet_No].pos.x = Player->pos.x;
			Player->Bullet[Bullet_No].pos.y = Player->pos.y;
			Player->Bullet[Bullet_No].Count = 0;
			Player->Bullet[Bullet_No].BaseAngle = BaseAngle;
			switch (Player->Direction)
			{
			case Up:
				Player->Bullet[Bullet_No].MoveDirection = UpVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(0.0f);
				break;
			case Down:
				Player->Bullet[Bullet_No].MoveDirection = DownVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(180.0f);
				break;
			case Left:
				Player->Bullet[Bullet_No].MoveDirection = LeftVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(-90.0f);
				break;
			case Right:
				Player->Bullet[Bullet_No].MoveDirection = RightVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(90.0f);
				break;
			default:
				break;
			}
			Player->Bullet[Bullet_No].Radius = Radius;
			Player->Bullet[Bullet_No].HitRadius = Radius * 0.1f;
			Player->Bullet[Bullet_No].Speed = PlayerBulletSpeed;

			SetSound(SE_ShotArrow, E_DS8_FLAG_NONE, true, false);

			if (Player_No == Player_A)
			{
				// クールダウンUIを設置する
				GameUI = GetGameUI(UIType_ShotCD_PlayerA);
				GameUI->Use = true;

#if _DEBUG
				// 当たり範囲表示
				SetBulletAR(Player->Bullet[Bullet_No].pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletAR_A);
#endif
			}
			else if (Player_No == Player_B)
			{
				// クールダウンUIを設置する
				GameUI = GetGameUI(UIType_ShotCD_PlayerB);
				GameUI->Use = true;

#if _DEBUG
				// 当たり範囲表示
				SetBulletAR(Player->Bullet[Bullet_No].pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletAR_B);
#endif
			}

			break;
		}
	}

	return;
}
