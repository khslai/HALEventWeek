//=============================================================================
//
// 当たり範囲表示処理 [AttackRange.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "AttackRange.h"
#include "Player.h"
#include "Boss.h"
#include "Effect.h"
#include "Input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_AttackRange			_T("data/Texture/AttackRange.png")
#define Texture_AttackRange_Width	(50)
#define Texture_AttackRange_Height	(50)

// 当たり範囲表示の最大数
#define AttackRange_Max				(999)
// 当たり範囲のアルファ値
#define AttackRange_Alpha			(128)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// AR = AttackRange
// 頂点の作成
HRESULT MakeARVertex(ATTACKRANGE *AttackRangePtr);
// 頂点座標の設定
void SetARVertex(ATTACKRANGE *AttackRangePtr);
// テクスチャ座標の設定
void SetARTexture(ATTACKRANGE *AttackRangePtr);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 AttackRange_Texture = NULL;
// 当たり範囲構造体
ATTACKRANGE AttackRange[AttackRange_Max];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAttackRange(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int AttackRange_No = 0;

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		AttackRange[AttackRange_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		AttackRange[AttackRange_No].Radius = 0.0f;
		AttackRange[AttackRange_No].Owner = 0;
		AttackRange[AttackRange_No].BulletNo = 0;
		AttackRange[AttackRange_No].BarrageNo = 0;
		AttackRange[AttackRange_No].Use = false;

		// 頂点情報の作成
		MakeARVertex(&AttackRange[AttackRange_No]);
	}

	if (FirstInit == true)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_AttackRange, &AttackRange_Texture, "AttackRange") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAttackRange(void)
{
	SafeRelease(AttackRange_Texture);

	return;
}

//=============================================================================
// AttackRange更新処理
//=============================================================================
void UpdateAttackRange(void)
{
	int AttackRange_No = 0;
	int Barrage_No = 0;
	int Bullet_No = 0;
	int Enemy_No = 0;
	PLAYER *Player = NULL;
	BOSS *Boss = GetBoss();

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == true)
		{
			// 当たり範囲の所有者によって、違う処理を行う
			switch (AttackRange[AttackRange_No].Owner)
			{
				// プレイヤー
			case PlayerAR_A:
				Player = GetPlayer(Player_A);
			case PlayerAR_B:

				if (Player == NULL)
				{
					Player = GetPlayer(Player_B);
				}
				AttackRange[AttackRange_No].pos = Player->pos;
				Player = NULL;
				break;

				// プレイヤーの弾
			case PlayerBulletAR_A:
				Player = GetPlayer(Player_A);
			case PlayerBulletAR_B:

				if (Player == NULL)
				{
					Player = GetPlayer(Player_B);
				}
				for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
				{
					if (AttackRange[AttackRange_No].BulletNo == Bullet_No)
					{
						if (Player->Bullet[Bullet_No].Use == true)
						{
							AttackRange[AttackRange_No].pos = Player->Bullet[Bullet_No].pos;
						}
						else
						{
							AttackRange[AttackRange_No].Use = false;
						}
						Player = NULL;
						break;
					}
				}
				break;

				// ボス
			case BossAR:

				if (Boss->Exist == true)
				{
					AttackRange[AttackRange_No].pos = Boss->pos;
				}
				else
				{
					AttackRange[AttackRange_No].Use = false;
				}
				break;

				// ボスの弾
			case BossBulletAR:

				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (AttackRange[AttackRange_No].BulletNo == Bullet_No)
					{
						if (Boss->Bullet[Bullet_No].Use == true)
						{
							AttackRange[AttackRange_No].pos = Boss->Bullet[Bullet_No].pos;
						}
						else
						{
							AttackRange[AttackRange_No].Use = false;
						}
					}
				}
				break;

			default:
				break;
			}

			// 頂点座標の更新
			SetARVertex(&AttackRange[AttackRange_No]);
		}
	}

	return;
}

//=============================================================================
// AttackRange描画処理
//=============================================================================
void DrawAttackRange(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int AttackRange_No = 0;

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == true)
		{
			// テクスチャの設定
			Device->SetTexture(0, AttackRange_Texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, AttackRange[AttackRange_No].vertexwk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeARVertex(ATTACKRANGE *AttackRangePtr)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点座標の設定	
	SetARVertex(AttackRangePtr);

	// rhwの設定
	AttackRangePtr->vertexwk[0].rhw = 1.0f;
	AttackRangePtr->vertexwk[1].rhw = 1.0f;
	AttackRangePtr->vertexwk[2].rhw = 1.0f;
	AttackRangePtr->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	AttackRangePtr->vertexwk[0].diffuse = WHITE(AttackRange_Alpha);
	AttackRangePtr->vertexwk[1].diffuse = WHITE(AttackRange_Alpha);
	AttackRangePtr->vertexwk[2].diffuse = WHITE(AttackRange_Alpha);
	AttackRangePtr->vertexwk[3].diffuse = WHITE(AttackRange_Alpha);

	// テクスチャ座標の設定
	SetARTexture(AttackRangePtr);

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetARVertex(ATTACKRANGE *AttackRangePtr)
{
	AttackRangePtr->vertexwk[0].vtx.x = AttackRangePtr->pos.x - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[0].vtx.y = AttackRangePtr->pos.y - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[1].vtx.x = AttackRangePtr->pos.x + AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[1].vtx.y = AttackRangePtr->pos.y - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[2].vtx.x = AttackRangePtr->pos.x - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[2].vtx.y = AttackRangePtr->pos.y + AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[3].vtx.x = AttackRangePtr->pos.x + AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[3].vtx.y = AttackRangePtr->pos.y + AttackRangePtr->Radius;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetARTexture(ATTACKRANGE *AttackRangePtr)
{
	AttackRangePtr->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	AttackRangePtr->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	AttackRangePtr->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	AttackRangePtr->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// キャラクターの当たり範囲の設置
//=============================================================================
void SetCharacterAR(D3DXVECTOR3 pos, float HitRadius, int Owner)
{
	int AttackRange_No = 0;

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == false)
		{
			AttackRange[AttackRange_No].pos.x = pos.x;
			AttackRange[AttackRange_No].pos.y = pos.y;
			AttackRange[AttackRange_No].Radius = HitRadius;
			AttackRange[AttackRange_No].Owner = Owner;
			AttackRange[AttackRange_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// 弾の当たり範囲の設置
//=============================================================================
void SetBulletAR(D3DXVECTOR3 pos, float HitRadius, int Bullet_No, int Owner)
{
	int AttackRange_No = 0;

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == false)
		{
			AttackRange[AttackRange_No].pos.x = pos.x;
			AttackRange[AttackRange_No].pos.y = pos.y;
			AttackRange[AttackRange_No].Radius = HitRadius;
			AttackRange[AttackRange_No].Owner = Owner;
			AttackRange[AttackRange_No].BulletNo = Bullet_No;
			AttackRange[AttackRange_No].Use = true;
			break;
		}
	}

	return;
}
