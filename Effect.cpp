//=============================================================================
//
// エフェクト処理 [Effect.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Effect.h"
#include "Player.h"
#include "Boss.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Regenerate				_T("data/Texture/Effect/Regenerate.png")
#define Texture_Regenerate_Width_Total	(1200)
#define Texture_Regenerate_Height_Total	(120)
#define Texture_Regenerate_Divide_X		(10)
#define Texture_Regenerate_Divide_Y		(1)
#define Texture_Regenerate_Width		(Texture_Regenerate_Width_Total / Texture_Regenerate_Divide_X)
#define Texture_Regenerate_Height		(Texture_Regenerate_Height_Total / Texture_Regenerate_Divide_Y)

#define Texture_BulletDead				_T("data/Texture/Effect/BulletDead.png")
#define Texture_BulletDead_Width_Total	(1200)
#define Texture_BulletDead_Height_Total	(120)
#define Texture_BulletDead_Divide_X		(10)
#define Texture_BulletDead_Divide_Y		(1)
#define Texture_BulletDead_Width		(Texture_BulletDead_Width_Total / Texture_BulletDead_Divide_X)
#define Texture_BulletDead_Height		(Texture_BulletDead_Height_Total / Texture_BulletDead_Divide_Y)

#define Texture_DeadEffect				_T("data/Texture/Effect/DeadEffect.png")
#define Texture_DeadEffect_Width_Total	(700)
#define Texture_DeadEffect_Height_Total	(140)
#define Texture_DeadEffect_Divide_X		(5)
#define Texture_DeadEffect_Divide_Y		(1)
#define Texture_DeadEffect_Width		(Texture_DeadEffect_Width_Total / Texture_DeadEffect_Divide_X)
#define Texture_DeadEffect_Height		(Texture_DeadEffect_Height_Total / Texture_DeadEffect_Divide_Y)

#define Texture_Slash					_T("data/Texture/Effect/Slash.png")
#define Texture_Slash_Width_Total		(1080)
#define Texture_Slash_Height_Total		(120)
#define Texture_Slash_Divide_X			(9)
#define Texture_Slash_Divide_Y			(1)
#define Texture_Slash_Width				(Texture_Slash_Width_Total / Texture_Slash_Divide_X)
#define Texture_Slash_Height			(Texture_Slash_Height_Total / Texture_Slash_Divide_Y)

// エフェクトの最大数
#define Effect_Max						(200)
// アニメーションを切り替えるカウント
#define AnimChangeCount					(5)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeEffectVertex(EFFECT *Effect);
// 頂点座標の設定
void SetEffectVertex(EFFECT *Effect);
// テクスチャ座標の設定
void SetEffectTexture(EFFECT *Effect);
// 色、透明度の設定
void SetEffectDiffuse(EFFECT *Effect, int Alpha);
// エフェクトの計算
void CalculateEffect(EFFECT *Effect);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 EffectTexture[EffectType_Max] = { NULL };
// エフェクト構造体
EFFECT Effect[Effect_Max];
// テクスチャ半径
static float BaseAngle[EffectType_Max] = { 0.0f };
// 中心点と四頂点の成す角
static float Radius[EffectType_Max] = { 0.0f };

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(bool FirstInit)
{
	int Effect_No = 0;

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		Effect[Effect_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Effect[Effect_No].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Effect[Effect_No].Radius = 0.0f;
		Effect[Effect_No].BaseAngle = 0.0f;
		Effect[Effect_No].Count = 0;
		Effect[Effect_No].AnimPattern = 0;
		Effect[Effect_No].Alpha = 0;
		Effect[Effect_No].Type = 0;
		Effect[Effect_No].Use = false;

		// 頂点情報の作成
		MakeEffectVertex(&Effect[Effect_No]);
	}

	if (FirstInit == true)
	{
		Radius[Regenerate] = D3DXVec2Length(&D3DXVECTOR2(Texture_Regenerate_Width / 2, Texture_Regenerate_Height / 2));
		BaseAngle[Regenerate] = atan2f(Texture_Regenerate_Height, Texture_Regenerate_Width);

		Radius[BulletDead] = D3DXVec2Length(&D3DXVECTOR2(Texture_BulletDead_Width / 2, Texture_BulletDead_Height / 2));
		BaseAngle[BulletDead] = atan2f(Texture_BulletDead_Height, Texture_BulletDead_Width);

		Radius[DeadEffect] = D3DXVec2Length(&D3DXVECTOR2(Texture_DeadEffect_Width / 2, Texture_DeadEffect_Height / 2));
		BaseAngle[DeadEffect] = atan2f(Texture_DeadEffect_Height, Texture_DeadEffect_Width);

		Radius[Slash] = D3DXVec2Length(&D3DXVECTOR2(Texture_Slash_Width / 2, Texture_Slash_Height / 2));
		BaseAngle[Slash] = atan2f(Texture_Slash_Height, Texture_Slash_Width);

		// テクスチャの読み込み
		if (SafeLoad(Texture_Regenerate, &EffectTexture[Regenerate], "Effect") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BulletDead, &EffectTexture[BulletDead], "Effect") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_DeadEffect, &EffectTexture[DeadEffect], "Effect") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Slash, &EffectTexture[Slash], "Effect") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	for (int i = 0; i < EffectType_Max; i++)
	{
		SafeRelease(EffectTexture[i]);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	int Effect_No = 0;

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == true)
		{
			Effect[Effect_No].Count++;

			// エフェクト計算
			CalculateEffect(&Effect[Effect_No]);

			// テクスチャ座標更新
			SetEffectTexture(&Effect[Effect_No]);
			// 色、透明度の設定
			SetEffectDiffuse(&Effect[Effect_No], Effect[Effect_No].Alpha);
			// 頂点座標更新
			SetEffectVertex(&Effect[Effect_No]);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Effect_No = 0;

	// 加算合成モードに設置する
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == true)
		{
			// テクスチャの設定
			switch (Effect[Effect_No].Type)
			{
			case Regenerate:

				Device->SetTexture(0, EffectTexture[Regenerate]);
				break;

			case BulletDead:

				Device->SetTexture(0, EffectTexture[BulletDead]);
				break;

			case DeadEffect:

				Device->SetTexture(0, EffectTexture[DeadEffect]);
				break;

			case Slash:

				Device->SetTexture(0, EffectTexture[Slash]);
				break;

			default:
				break;
			}

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Effect[Effect_No].vertexwk, sizeof(VERTEX_2D));
		}
	}

	// 本来の設置を戻す
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeEffectVertex(EFFECT *Effect)
{
	// 頂点座標の設定	
	SetEffectVertex(Effect);

	// rhwの設定
	Effect->vertexwk[0].rhw = 1.0f;
	Effect->vertexwk[1].rhw = 1.0f;
	Effect->vertexwk[2].rhw = 1.0f;
	Effect->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	SetEffectDiffuse(Effect, Effect->Alpha);

	// テクスチャ座標の設定
	SetEffectTexture(Effect);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetEffectTexture(EFFECT *Effect)
{
	float sizeX = 0.0f;

	switch (Effect->Type)
	{
	case Regenerate:
		sizeX = 1.0f / Texture_Regenerate_Divide_X;
		break;
	case BulletDead:
		sizeX = 1.0f / Texture_BulletDead_Divide_X;
		break;
	case DeadEffect:
		sizeX = 1.0f / Texture_DeadEffect_Divide_X;
		break;
	case Slash:
		sizeX = 1.0f / Texture_Slash_Divide_X;
		break;
	default:
		break;
	}

	Effect->vertexwk[0].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX, 0.0f);
	Effect->vertexwk[1].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX + sizeX, 0.0f);
	Effect->vertexwk[2].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX, 1.0f);
	Effect->vertexwk[3].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX + sizeX, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetEffectVertex(EFFECT *Effect)
{
	Effect->vertexwk[0].vtx.x = Effect->pos.x - cosf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[0].vtx.y = Effect->pos.y - sinf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[1].vtx.x = Effect->pos.x + cosf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[1].vtx.y = Effect->pos.y - sinf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[2].vtx.x = Effect->pos.x - cosf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[2].vtx.y = Effect->pos.y + sinf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[3].vtx.x = Effect->pos.x + cosf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[3].vtx.y = Effect->pos.y + sinf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;

	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetEffectDiffuse(EFFECT *Effect, int Alpha)
{
	// 反射光の設定
	Effect->vertexwk[0].diffuse = WHITE(Alpha);
	Effect->vertexwk[1].diffuse = WHITE(Alpha);
	Effect->vertexwk[2].diffuse = WHITE(Alpha);
	Effect->vertexwk[3].diffuse = WHITE(Alpha);

	return;
}

//=============================================================================
// エフェクトの設置
//=============================================================================
void SetEffect(int Owner, int EffectType, D3DXVECTOR3 Pos)
{
	int Effect_No = 0;
	BOSS *Boss = GetBoss();

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == false)
		{
			Effect[Effect_No].Use = true;
			// 座標
			Effect[Effect_No].pos = Pos;
			// 透明度
			Effect[Effect_No].Alpha = 255;
			// カウント
			Effect[Effect_No].Count = 0;
			// 所有者
			Effect[Effect_No].Owner = Owner;

			switch (EffectType)
			{
			case Regenerate:
				
				Effect[Effect_No].BaseAngle = BaseAngle[Regenerate];
				Effect[Effect_No].Radius = Radius[Regenerate];
				Effect[Effect_No].Type = Regenerate;
				// 回転角度
				Effect[Effect_No].rot.z = 0.0f;
				// 一番左の画像から表示
				Effect[Effect_No].AnimPattern = 0;
				break;

			case BulletDead:

				Effect[Effect_No].BaseAngle = BaseAngle[BulletDead];
				Effect[Effect_No].Radius = Radius[BulletDead];
				Effect[Effect_No].Type = BulletDead;
				// 回転角度
				Effect[Effect_No].rot.z = 0.0f;
				// 一番左の画像から表示
				Effect[Effect_No].AnimPattern = 0;
				break;

			case DeadEffect:

				Effect[Effect_No].BaseAngle = BaseAngle[DeadEffect];
				Effect[Effect_No].Radius = Radius[DeadEffect] * 2.0f;
				Effect[Effect_No].Type = DeadEffect;
				// 回転角度
				Effect[Effect_No].rot.z = D3DXToRadian(360.0f * (float)rand() / RAND_MAX);
				// ランダムの画像表示
				Effect[Effect_No].AnimPattern = rand() % (4 + 1);
				break;

			case Slash:

				Effect[Effect_No].BaseAngle = BaseAngle[Slash];
				Effect[Effect_No].Radius = Radius[Slash];
				Effect[Effect_No].Type = Slash;
				// 回転角度
				Effect[Effect_No].rot.z = 0.0f;
				// 一番左の画像から表示
				Effect[Effect_No].AnimPattern = 0;
				break;

			default:
				break;
			}
			break;
		}
	}

	return;
}

//=============================================================================
// エフェクトの計算
//=============================================================================
void CalculateEffect(EFFECT *Effect)
{
	PLAYER *Player = GetPlayer(0);

	switch (Effect->Type)
	{
	case Regenerate:
		// エフェクト終了
		if (Effect->Count >= AnimChangeCount * Texture_Regenerate_Divide_X)
		{
			Effect->Use = false;
		}
		// テクスチャアニメパターン計算
		else
		{
			if (Effect->Owner == Player_A)
			{
				Player = GetPlayer(Player_A);
			}
			else if (Effect->Owner == Player_B)
			{
				Player = GetPlayer(Player_B);
			}
			Effect->pos = Player->pos;

			if ((Effect->Count % AnimChangeCount) == 0)
			{
				Effect->AnimPattern = (Effect->AnimPattern + 1) % Texture_Regenerate_Divide_X;
			}
		}
		break;

	case BulletDead:
		// エフェクト終了
		if (Effect->Count >= AnimChangeCount * Texture_BulletDead_Divide_X)
		{
			Effect->Use = false;
		}

		// テクスチャアニメパターン計算
		if ((Effect->Count % AnimChangeCount) == 0)
		{
			Effect->AnimPattern = (Effect->AnimPattern + 1) % Texture_BulletDead_Divide_X;
		}
		break;

	case DeadEffect:

		// エフェクト終了
		if (Effect->Count >= 60.0f)
		{
			Effect->Use = false;
		}

		// エフェクト拡大
		Effect->Radius += 0.2f;
		break;

	case Slash:

		// エフェクト終了
		if (Effect->Count >= AnimChangeCount * Texture_Slash_Divide_X)
		{
			Effect->Use = false;
		}

		// テクスチャアニメパターン計算
		if ((Effect->Count % AnimChangeCount) == 0)
		{
			Effect->AnimPattern = (Effect->AnimPattern + 1) % Texture_Slash_Divide_X;
		}
		break;

	default:
		break;
	}

	return;
}