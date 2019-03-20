//=============================================================================
//
// 当たり判定処理 [CheckHit.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "CheckHit.h"
#include "Player.h"
#include "Effect.h"
#include "Boss.h"
#include "Apple.h"
#include "Wolf.h"
#include "Score.h"
#include "Bull.h"
#include "Block.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 牡牛のダメージ
#define BullDamage				(1.5f)
// プレイヤーの弾のダメージ
#define PlayerBulletDamage		(3.0f)
// ボスの弾のダメージ
#define BossBulletDamage		(17.0f)
// 斬撃の範囲
#define SlashRange				(200.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// BC = バウンディングサークル
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrepos, float BulletSpeed);
// 弾消失をチェックする
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius);
// プレイヤーが撃たれた処理
void PlayerEvent(int Player_No);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();
	APPLE *Apple = GetApple(0);
	WOLF *Wolf = GetWolf(0);
	BULL *Bull = GetBull(0);
	BLOCK *Block = GetBlock(0);
	D3DXVECTOR2 Obj1Size = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 Obj2Size = D3DXVECTOR2(0.0f, 0.0f);
	int GameStage = GetGameStage();
	int Apple_No = 0;
	int Wolf_No = 0;
	int Bull_No = 0;
	int Block_No = 0;
	int Effect_No = 0;
	int Bullet_No = 0;
	int Player_No = 0;
	float Distance = 0.0f;

	switch (GameStage)
	{
	case Stage_Futago:


		break;

	case Stage_Oushi:

		// プレイヤーと牡牛
		// ポインターを初期化
		Player = GetPlayer(Player_A);
		Obj1Size = GetPlayerSize();
		Obj2Size = GetBullSize();
		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			if (Player->Use == true)
			{
				BULL *Bull = GetBull(0);
				for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++, Bull++)
				{
					if (CheckHitBB(Bull->pos, Player->pos, Obj1Size, Obj2Size) == true)
					{
						PlayerEvent(Player_No);
					}
				}
			}
		}
		break;

	case Stage_Ite:

		// プレイヤーの矢とアップル
		// ポインターを初期化
		Player = GetPlayer(Player_A);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet->Use == true)
			{
				// アップルのポインターを初期化
				Apple = GetApple(Apple_No);

				for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
				{
					if (Apple->Use == true)
					{
						if (CheckHitBC((Apple->HitRadius + Player->Bullet[Bullet_No].HitRadius),
							Apple->pos,
							Player->Bullet[Bullet_No].pos,
							Player->Bullet[Bullet_No].PreviousPos,
							Player->Bullet[Bullet_No].Speed) == true)
						{
							Player->Bullet[Bullet_No].Use = false;
							Apple->Use = false;
							SetSound(SE_HitApple, E_DS8_FLAG_NONE, true, false);
							// りんごを射抜けた数を増やす
							AddScore(1);
						}
					}
				}
			}
		}

		// プレイヤーと狼
		// ポインターを初期化
		Obj1Size = GetPlayerSize();
		Obj2Size = GetWolfSize();
		Wolf = GetWolf(0);
		for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
		{
			if (Wolf->Use == true)
			{
				Player = GetPlayer(Player_A);
				if (CheckHitBB(Player->pos, Wolf->pos, Obj1Size, Obj2Size) == true)
				{
					// 敵が消える
					Wolf->Use = false;
					SetSound(SE_SpeedDown, E_DS8_FLAG_NONE, true, false);
					if (Player->Speed >= 2.0f)
					{
						Player->Speed -= 0.25f;
					}
				}
			}
		}

		// 斬撃と狼
		// ポインターを初期化
		Player = GetPlayer(Player_B);
		if (Player->InSlash == true)
		{
			Wolf = GetWolf(0);
			for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
			{
				if (Wolf->Use == true)
				{
					if ((Player->Direction == Left && Wolf->pos.x < Player->pos.x) ||
						(Player->Direction == Right && Wolf->pos.x > Player->pos.x))
					{
						Distance = D3DXVec3LengthSq(&D3DXVECTOR3(Player->pos - Wolf->pos));
						if (Distance <= pow(SlashRange, 2))
						{
							// 狼が消える
							Wolf->Use = false;
						}
					}
				}
			}
			Player->InSlash = false;
		}

		break;

	case Stage_Shishi:

		// プレイヤーの矢とエネミー、ボス
		// ポインターを初期化
		Player = GetPlayer(Player_A);
		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
			{
				if (Player->Bullet[Bullet_No].Use == true)
				{
					// ボス
					if (Boss->Exist == true)
					{
						if (CheckHitBC((Boss->HitRadius + Player->Bullet[Bullet_No].HitRadius),
							Boss->pos,
							Player->Bullet[Bullet_No].pos,
							Player->Bullet[Bullet_No].PreviousPos,
							Player->Bullet[Bullet_No].Speed) == true)
						{
							if (CheckBulletDisappear(Boss->pos,
								Player->Bullet[Bullet_No].pos,
								Player->Bullet[Bullet_No].HitRadius) == true)
							{
								// 1フレイム後弾が消える
								Player->Bullet[Bullet_No].DelayDisappear = true;
							}
							else
							{
								// 今のフレイムに弾が消える
								Player->Bullet[Bullet_No].Use = false;
							}

							SetSound(SE_HitApple, E_DS8_FLAG_NONE, true, false);
							Boss->HP -= PlayerBulletDamage + Player->RegeneBonus;
							if (Boss->HP <= 0)
							{
								Boss->HP = 0;
							}

							SetBossColor(Red);
						}
					}
				}
			}
		}

		// ボスの弾とプレイヤー
		if (Boss->Exist == true)
		{
			Player = GetPlayer(Player_A);
			for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
			{
				if (Player->Use == true)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == true)
						{
							if (CheckHitBC((Player->HitRadius + Boss->Bullet[Bullet_No].HitRadius),
								Player->pos,
								Boss->Bullet[Bullet_No].pos,
								Boss->Bullet[Bullet_No].PreviousPos,
								Boss->Bullet[Bullet_No].Speed) == true)
							{
								Boss->Bullet[Bullet_No].Use = false;
								if (Player->InvincibleCount == 0)
								{
									PlayerEvent(Player_No);
								}
							}
						}
					}
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
// 当たり判定（円と円）
//=============================================================================
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrepos, float BulletSpeed)
{
	int i = 0;
	// 当たり判定の結果
	bool Result = false;
	// 中心点間の長さを計算
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Bulletpostemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// Objectの中心と弾の中心の距離
	float Distance = 0.0f;

	// 弾のスピードは当たり範囲より大きいなら
	if (BulletSpeed > CheckRange)
	{
		// 今の弾と1フレイム前の弾の距離を記録する
		Bulletpostemp = Bullet - BulletPrepos;
		// 弾の飛行距離 / 当たり範囲 = ループ回数
		for (i = 0; i < BulletSpeed / CheckRange; i++)
		{
			// 目標と弾の距離を計算する
			temp = Object - BulletPrepos;
			Distance = D3DXVec3LengthSq(&temp);
			if (Distance <= pow(CheckRange, 2))
			{
				Result = true;
				break;
			}
			else
			{
				// 弾を進んでる、進んでる距離 = 全部の飛行距離 / ループ回数
				BulletPrepos.x += Bulletpostemp.x / (BulletSpeed / CheckRange);
				BulletPrepos.y += Bulletpostemp.y / (BulletSpeed / CheckRange);
				Result = false;
			}
		}
	}
	else
	{
		temp = Object - Bullet;
		Distance = D3DXVec3LengthSq(&temp);
		Result = Distance <= pow(CheckRange, 2) ? true : false;
	}

	return Result;
}

//=============================================================================
// 当たり判定（矩形と矩形）
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 Obj1Pos, D3DXVECTOR3 Obj2Pos, D3DXVECTOR2 Obj1Size, D3DXVECTOR2 Obj2Size)
{
	bool Result = false;

	Obj1Size /= 2.0f;	// 半分にする
	Obj2Size /= 2.0f;

	if (Obj1Pos.x - Obj1Size.x < Obj2Pos.x + Obj2Size.x &&
		Obj1Pos.x + Obj1Size.x > Obj2Pos.x - Obj2Size.x &&
		Obj1Pos.y - Obj1Size.y < Obj2Pos.y + Obj2Size.y &&
		Obj1Pos.y + Obj1Size.y > Obj2Pos.y - Obj2Size.y)
	{
		Result = true;
	}

	return Result;
}

//=============================================================================
// 弾消失をチェックする
//=============================================================================
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius)
{
	bool Result = false;
	float Distance = 0.0f;

	Distance = D3DXVec3LengthSq(&D3DXVECTOR3(Object - Bullet));
	// 当たり判定
	// 半径 * 2 = 直径
	Result = Distance >= pow((BulletRadius * 2), 2) ? true : false;

	return Result;
}

//=============================================================================
// プレイヤーが撃たれた処理
//=============================================================================
void PlayerEvent(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	int GameStage = GetGameStage();

	// 無敵カウント加算
	Player->InvincibleCount++;

	if (GameStage == Stage_Oushi)
	{
		SetSound(SE_HitBull, E_DS8_FLAG_NONE, true, false);
		if (Player_No == Player_A)
		{
			SetSound(SE_Scream_PlayerA, E_DS8_FLAG_NONE, true, false);
		}
		else if (Player_No == Player_B)
		{
			SetSound(SE_Scream_PlayerB, E_DS8_FLAG_NONE, true, false);
		}
		Player->HP -= BullDamage;
	}
	else if (GameStage == Stage_Shishi)
	{
		SetSound(SE_PlayerBeShot, E_DS8_FLAG_NONE, true, false);
		Player->HP -= BossBulletDamage;
	}

	// HPが0になる
	if (Player->HP <= 0.0f)
	{
		Player->HP = 0.0f;
		Player->Use = false;
	}

	return;
}
