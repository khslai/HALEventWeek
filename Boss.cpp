//=============================================================================
//
// ボス処理 [Boss.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "AttackRange.h"
#include "DebugProcess.h"
#include "Effect.h"
#include "Transition.h"
#include "MainMenu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Boss				_T("data/Texture/Boss/Boss.png")
#define Texture_Boss_Width_Total	(320)
#define Texture_Boss_Height_Total	(480)
#define Texture_Boss_Divide_X		(4)		// テクスチャ内分割数（横)
#define Texture_Boss_Divide_Y		(6)		// テクスチャ内分割数（縦)
#define Texture_Boss_Width			(Texture_Boss_Width_Total / Texture_Boss_Divide_X)
#define Texture_Boss_Height			(Texture_Boss_Height_Total / Texture_Boss_Divide_Y)

// ボスアニメ変換カウント
#define AnimChangeCount				(15)		


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBossVertex(void);
// 頂点座標の設定
void SetBossVertex(void);
// テクスチャ座標の設定
void SetBossTexture(void);
// 次のボスの段階に入る
void NextBossPhase(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// ボス構造体
BOSS Boss;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(bool FirstInit)
{
	Boss.pos = D3DXVECTOR3(1440.0f, ScreenCenter_Y, 0.0f);
	Boss.Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Boss_Width / 2, Texture_Boss_Height / 2));
	Boss.HitRadius = Boss.Radius * 0.8f;
	Boss.BaseAngle = atan2f(Texture_Boss_Height, Texture_Boss_Width);
	Boss.HP_Max = 100.0f;
	Boss.HP = Boss.HP_Max;
	Boss.AnimPattern = 0;
	Boss.AnimCount = 0;
	Boss.StateCount = 0;
	Boss.BarrageType = BarrageMode1;
	Boss.State = Idle;
	Boss.Exist = true;

	// 頂点情報の作成
	MakeBossVertex();

#if _DEBUG
	SetCharacterAR(Boss.pos, Boss.HitRadius, BossAR);
#endif

	if (FirstInit == true)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Boss, &Boss.texture, "Boss") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{
	SafeRelease(Boss.texture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
	int GameStage = GetGameStage();
	static int Count = 0;
	float HPPercent = 0.0f;

	if (Boss.Exist == true)
	{
		// ボスのアニメ計算
		Boss.AnimCount++;
		if ((Boss.AnimCount % AnimChangeCount) == 0)
		{
			// テクスチャアニメパターン計算
			Boss.AnimPattern = (Boss.AnimPattern + 1) % Texture_Boss_Divide_X;
		}

		HPPercent = Boss.HP / Boss.HP_Max;
		if (Boss.BarrageType == BarrageMode1 && HPPercent <= 0.67f && HPPercent > 0.33f)
		{
			Boss.State = Idle;
			Boss.StateCount = 0;
			Boss.BarrageType = BarrageMode2;
			NextBossPhase();
		}
		else if (Boss.BarrageType == BarrageMode2 && HPPercent <= 0.33f)
		{
			Boss.State = Idle;
			Boss.StateCount = 0;
			Boss.BarrageType = BarrageMode3;
			NextBossPhase();
		}
		else if (Boss.BarrageType == BarrageMode3 && HPPercent <= 0.0f)
		{
			Boss.State = BossDead;
			Boss.StateCount = 0;
			NextBossPhase();
		}

		// 頂点座標更新
		SetBossVertex();
		// テクスチャ座標更新
		SetBossTexture();
		// 色、透明度更新
		SetBossColor(White);
	}
	else if (Boss.Exist == false && Boss.State == BossDead)
	{
		Count++;
		if (Count >= 120)
		{
			Count = 0;
			SetClearFlag(ClearFlag_Shishi);
			SetTransition(Fadein);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (Boss.Exist == true)
	{
		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		Device->SetTexture(0, Boss.texture);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss.vertexwk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// 次のボスの段階に入る
//=============================================================================
void NextBossPhase(void)
{
	for (int Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
	{
		if (Boss.Bullet[Bullet_No].Use == true)
		{
			// 弾消失エフェクト設置
			SetEffect(Bullet_No, BulletDead, Boss.Bullet[Bullet_No].pos);
			Boss.Bullet[Bullet_No].Use = false;
		}
	}

	if (Boss.State == BossDead)
	{
		Boss.Exist = false;
		for (int i = 0; i < 10; i++)
		{
			SetEffect(BossEffect, DeadEffect, Boss.pos);
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBossVertex(void)
{
	// 頂点座標の設定	
	SetBossVertex();

	// rhwの設定
	Boss.vertexwk[0].rhw = 1.0f;
	Boss.vertexwk[1].rhw = 1.0f;
	Boss.vertexwk[2].rhw = 1.0f;
	Boss.vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	SetBossColor(White);

	// テクスチャ座標の設定
	SetBossTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBossTexture(void)
{
	int x = Boss.AnimPattern;
	int y = 0;
	float sizeX = 1.0f / Texture_Boss_Divide_X;
	float sizeY = 1.0f / Texture_Boss_Divide_Y;

	Boss.vertexwk[0].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY);
	Boss.vertexwk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY);
	Boss.vertexwk[2].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY + sizeY);
	Boss.vertexwk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY + sizeY);

	return;
}

//=============================================================================
// ボスの色の設定
//=============================================================================
void SetBossColor(int Color)
{
	switch (Color)
	{
	case White:
		Boss.vertexwk[0].diffuse = WHITE(255);
		Boss.vertexwk[1].diffuse = WHITE(255);
		Boss.vertexwk[2].diffuse = WHITE(255);
		Boss.vertexwk[3].diffuse = WHITE(255);
		break;
		// 撃たれたら、色が変わる
	case Red:
		Boss.vertexwk[0].diffuse = RED(255);
		Boss.vertexwk[1].diffuse = RED(255);
		Boss.vertexwk[2].diffuse = RED(255);
		Boss.vertexwk[3].diffuse = RED(255);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBossVertex(void)
{
	Boss.vertexwk[0].vtx.x = Boss.pos.x - cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[0].vtx.y = Boss.pos.y - sinf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[1].vtx.x = Boss.pos.x + cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[1].vtx.y = Boss.pos.y - sinf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[2].vtx.x = Boss.pos.x - cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[2].vtx.y = Boss.pos.y + sinf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[3].vtx.x = Boss.pos.x + cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[3].vtx.y = Boss.pos.y + sinf(Boss.BaseAngle) * Boss.Radius;

	return;
}

//=============================================================================
// ボスの情報を取得する
//=============================================================================
BOSS *GetBoss(void)
{
	return &Boss;
}

