//=============================================================================
//
// ボス弾幕処理 [BossBarrage.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "BossBarrage.h"
#include "Player.h"
#include "Boss.h"
#include "AttackRange.h"
#include "Sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// バレット1
#define Texture_BossBullet_1				_T("data/Texture/Bullet/BossBullet_1.png")
#define Texture_BossBullet_1_Width_Total	(192)
#define Texture_BossBullet_1_Height_Total	(32)
#define Texture_BossBullet_1_Divide_X		(6)		// テクスチャ内分割数（横)
#define Texture_BossBullet_1_Divide_Y		(1)		// テクスチャ内分割数（縦)
#define Texture_BossBullet_1_Width			(Texture_BossBullet_1_Width_Total / Texture_BossBullet_1_Divide_X)
#define Texture_BossBullet_1_Height			(Texture_BossBullet_1_Height_Total / Texture_BossBullet_1_Divide_Y)
// バレット2
#define Texture_BossBullet_2				_T("data/Texture/Bullet/BossBullet_2.png")
#define Texture_BossBullet_2_Width			(32)
#define Texture_BossBullet_2_Height			(32)
// バレット3
#define Texture_BossBullet_3				_T("data/Texture/Bullet/BossBullet_3.png")
#define Texture_BossBullet_3_Width_Total	(256)
#define Texture_BossBullet_3_Height_Total	(32)
#define Texture_BossBullet_3_Divide_X		(8)		// テクスチャ内分割数（横)
#define Texture_BossBullet_3_Divide_Y		(1)		// テクスチャ内分割数（縦)
#define Texture_BossBullet_3_Width			(Texture_BossBullet_3_Width_Total / Texture_BossBullet_3_Divide_X)
#define Texture_BossBullet_3_Height			(Texture_BossBullet_3_Height_Total / Texture_BossBullet_3_Divide_Y)

// 弾幕モード1（円形弾幕）一回発射する弾の最大数
#define Mode1_Bullet_Max					(30)
// 弾幕モード2（ミシャグジさま）一回発射する弾の最大数
#define Mode2_Bullet_Max					(30)

// 弾の状態
enum
{
	StopRotate,			// 回転停止
	InRotate,			// 回転中
	ClockWise,			// 回転方向は右回り
	Counterclockwise,	// 回転方向は左回り
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
static HRESULT MakeBulletVertex(BULLET *Bullet);
// 頂点座標の設定
static void SetBulletVertex(BULLET *Bullet);
// テクスチャ座標の設定
static void SetBulletTexture(BULLET *Bullet, int BulletType);
// ボス弾幕設置
void BossShot(int BarrageType);
// ボスの弾の移動
static void BulletMove(BULLET *Bullet, int BulletType);
// 二次元ベクトル回転(x,y座標だけ)
D3DXVECTOR3 VectorRot(D3DXVECTOR3 Vec, float RotAngle);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
static LPDIRECT3DTEXTURE9 BossBullet_Texture[BarrageMax] = { NULL };
// 画像中心から頂点までの角度
float BaseAngle[BarrageMax] = { 0.0f };
// 画像中心から頂点までの距離
float Radius[BarrageMax] = { 0.0f };


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBossBarrage(bool FirstInit)
{
	int Bullet_No = 0;
	BOSS *Boss = GetBoss();

	for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
	{
		Boss->Bullet[Bullet_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].Radius = 0.0f;
		Boss->Bullet[Bullet_No].HitRadius = 0.0f;
		Boss->Bullet[Bullet_No].BaseAngle = 0.0f;
		Boss->Bullet[Bullet_No].Speed = 0.0f;
		Boss->Bullet[Bullet_No].Count = 0;
		Boss->Bullet[Bullet_No].Color = 0;
		Boss->Bullet[Bullet_No].State = 0;
		Boss->Bullet[Bullet_No].Use = false;

		// 頂点情報の作成
		MakeBulletVertex(&Boss->Bullet[Bullet_No]);
	}

	if (FirstInit == true)
	{
		Radius[BarrageMode1] = D3DXVec2Length(&D3DXVECTOR2(Texture_BossBullet_1_Width / 2, Texture_BossBullet_1_Height / 2));
		BaseAngle[BarrageMode1] = atan2f(Texture_BossBullet_1_Height, Texture_BossBullet_1_Width);

		Radius[BarrageMode2] = D3DXVec2Length(&D3DXVECTOR2(Texture_BossBullet_2_Width / 2, Texture_BossBullet_2_Height / 2));
		BaseAngle[BarrageMode2] = atan2f(Texture_BossBullet_2_Height, Texture_BossBullet_2_Width);

		Radius[BarrageMode3] = D3DXVec2Length(&D3DXVECTOR2(Texture_BossBullet_3_Width / 2, Texture_BossBullet_3_Height / 2));
		BaseAngle[BarrageMode3] = atan2f(Texture_BossBullet_3_Height, Texture_BossBullet_3_Width);

		// テクスチャの読み込み
		if (SafeLoad(Texture_BossBullet_1, &BossBullet_Texture[BarrageMode1], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossBullet_2, &BossBullet_Texture[BarrageMode2], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossBullet_3, &BossBullet_Texture[BarrageMode3], "Bullet") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBossBarrage(void)
{
	for (int i = 0; i < BarrageMax; i++)
	{
		SafeRelease(BossBullet_Texture[i]);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBossBarrage(void)
{
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();

	if (Boss->Exist == true)
	{
		if (Boss->State == Shot)
		{
			switch (Boss->BarrageType)
			{
				// 円形弾幕
			case BarrageMode1:
				BossShot(BarrageMode1);
				break;
				// ミシャグジさま
			case BarrageMode2:
				BossShot(BarrageMode2);
				break;
				// パーフェクトフリーズ
			case BarrageMode3:
				BossShot(BarrageMode3);
				break;
			default:
				break;
			}

			// 状態カウント加算
			Boss->StateCount++;
		}
		else if (Boss->State == Idle)
		{
			Boss->StateCount++;
			if (Boss->StateCount % 180 == 0)
			{
				Boss->StateCount = 0;
				Boss->State = Shot;
			}
		}

		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// 弾の移動制御
				BulletMove(&Boss->Bullet[Bullet_No], Boss->BarrageType);

				// 頂点座標更新
				SetBulletVertex(&Boss->Bullet[Bullet_No]);
				// テクスチャ更新
				SetBulletTexture(&Boss->Bullet[Bullet_No], Boss->BarrageType);
			}
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBossBarrage(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	BOSS *Boss = GetBoss();
	int Bullet_No = 0;

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	if (Boss->Exist == true)
	{
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// テクスチャの設定
				switch (Boss->BarrageType)
				{
				case BarrageMode1: Device->SetTexture(0, BossBullet_Texture[BarrageMode1]); break;
				case BarrageMode2: Device->SetTexture(0, BossBullet_Texture[BarrageMode2]); break;
				case BarrageMode3: Device->SetTexture(0, BossBullet_Texture[BarrageMode3]); break;
				default:
					break;
				}

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss->Bullet[Bullet_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}


//=============================================================================
// ボスが弾を発射
//=============================================================================
void BossShot(int BarrageType)
{
	int i = 0;
	int Bullet_No = 0;
	// ミシャグジさまの弾の回転角度
	float RotateAngle = 0.0f;
	// ループ何回目
	static int LoopNum = 0;
	BOSS *Boss = GetBoss();
	int Count = Boss->StateCount;

	switch (BarrageType)
	{
	case BarrageMode1:
		// 60カウントずつ弾発射
		if (Count % 60 == 0)
		{
			// 360カウント後、発射停止
			if (Count >= 360)
			{
				LoopNum = 0;
				Boss->State = Idle;
				Boss->StateCount = 0;
				break;
			}

			// ループ回数加算
			LoopNum++;

			// 効果音
			SetSound(SE_BossShot_01, E_DS8_FLAG_NONE, true, false);

			// 弾発射
			for (i = 0; i < Mode1_Bullet_Max; i++)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Boss->Bullet[Bullet_No].Use = true;
						// 座標設置
						Boss->Bullet[Bullet_No].pos = Boss->pos;
						Boss->Bullet[Bullet_No].BaseAngle = BaseAngle[BarrageMode1];
						Boss->Bullet[Bullet_No].Radius = Radius[BarrageMode1];
						// 当たり半径設置
						Boss->Bullet[Bullet_No].HitRadius = Radius[BarrageMode1] * 0.5f;
						// 弾の進行方向設置
						// 毎回のループは12度の角度差がある
						Boss->Bullet[Bullet_No].MoveDirection =
							VectorRot(UpVec, D3DXToRadian(360.0f / Mode1_Bullet_Max * i + LoopNum * 15.0f));
						Boss->Bullet[Bullet_No].Count = 0;
						Boss->Bullet[Bullet_No].Color = 0;
						Boss->Bullet[Bullet_No].State = StopRotate;
						Boss->Bullet[Bullet_No].Speed = 5.0f;

#if _DEBUG
						// 当たり範囲表示
						//SetBulletAR(Boss->Bullet[Bullet_No].pos, Boss->Bullet[Bullet_No].HitRadius, Bullet_No, BossBulletAR);
#endif

						break;
					}
				}
			}
		}
		break;
	case BarrageMode2:
		// ミシャグジさま
		// 90カウントずつ弾発射
		if (Count % 90 == 0)
		{
			// ループ回数加算
			LoopNum++;

			// 効果音
			SetSound(SE_BossShot_02, E_DS8_FLAG_NONE, true, false);

			// 弾発射
			for (i = 0; i < Mode2_Bullet_Max; i++)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					// 使用されていない弾があれば
					if (Boss->Bullet[Bullet_No].Use == false)
					{
						// 弾の再初期化処理
						Boss->Bullet[Bullet_No].Use = true;
						// 座標設置
						Boss->Bullet[Bullet_No].pos = Boss->pos;
						Boss->Bullet[Bullet_No].BaseAngle = BaseAngle[BarrageMode2];
						Boss->Bullet[Bullet_No].Radius = Radius[BarrageMode2] * 2.5f;
						// 当たり半径設置
						Boss->Bullet[Bullet_No].HitRadius = Radius[BarrageMode2] * 0.5f;
						// 弾の進行方向設置
						Boss->Bullet[Bullet_No].MoveDirection =
							VectorRot(UpVec, D3DXToRadian(360.0f / Mode2_Bullet_Max * i));
						Boss->Bullet[Bullet_No].rot.z = D3DXToRadian(0.0f);
						Boss->Bullet[Bullet_No].Color = 0;
						Boss->Bullet[Bullet_No].Count = 0;
						Boss->Bullet[Bullet_No].Speed = 6.0f;
						// ステータス。0か1かで回転方向が変わる
						if (LoopNum % 2 == 0)
						{
							Boss->Bullet[Bullet_No].State = ClockWise;
						}
						else
						{
							Boss->Bullet[Bullet_No].State = Counterclockwise;
						}

#if _DEBUG
						// 当たり範囲表示
						//SetBulletAR(Boss->Bullet[Bullet_No].pos, Boss->Bullet[Bullet_No].HitRadius, Bullet_No, BossBulletAR);
#endif

						break;
					}
				}
			}
		}

		// 弾の回転計算
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// 30～120カウントなら
				if (Boss->Bullet[Bullet_No].Count > 30 && Boss->Bullet[Bullet_No].Count < 120)
				{
					// 90カウントかけて1.2減らす
					Boss->Bullet[Bullet_No].Speed -= 1.2f / 90.0f;
					// 90カウントかけて90°傾ける
					RotateAngle = 1.0f * (Boss->Bullet[Bullet_No].State == ClockWise ? 1 : -1);
					Boss->Bullet[Bullet_No].MoveDirection =
						VectorRot(Boss->Bullet[Bullet_No].MoveDirection, D3DXToRadian(RotateAngle));
					Boss->Bullet[Bullet_No].rot.z += D3DXToRadian(RotateAngle);
				}
			}
		}
		break;
	case BarrageMode3:
		// パーフェクトフリーズ
		// 最初のランダム発射
		if (Count < 240)
		{
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == false)
				{
					// 効果音
					SetSound(SE_BossShot_03, E_DS8_FLAG_NONE, true, false);

					Boss->Bullet[Bullet_No].Use = true;
					// 座標設置
					Boss->Bullet[Bullet_No].pos = Boss->pos;
					Boss->Bullet[Bullet_No].BaseAngle = BaseAngle[BarrageMode3];
					Boss->Bullet[Bullet_No].Radius = Radius[BarrageMode3] * 4.0f;
					// 当たり半径設置
					Boss->Bullet[Bullet_No].HitRadius = Radius[BarrageMode3] * 0.8f;
					// 弾の進行方向設置
					Boss->Bullet[Bullet_No].MoveDirection =
						VectorRot(UpVec, D3DXToRadian(360.0f * (float)rand() / RAND_MAX));
					Boss->Bullet[Bullet_No].rot.z = D3DXToRadian(0.0f);
					Boss->Bullet[Bullet_No].Color = rand() % (6 + 1);
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].State = InRotate;
					Boss->Bullet[Bullet_No].Speed = 5.0f + (3.0f * (float)rand() / RAND_MAX);

#if _DEBUG
					// 当たり範囲表示
					//SetBulletAR(Boss->Bullet[Bullet_No].pos, Boss->Bullet[Bullet_No].HitRadius, Bullet_No, BossBulletAR);
#endif

					break;
				}
			}
		}
		// 240カウントの時に全てストップさせ、白くし、カウントリセット
		else if (Count == 240)
		{
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					// 効果音
					SetSound(SE_BulletFreeze, E_DS8_FLAG_NONE, true, false);
					// 白色
					Boss->Bullet[Bullet_No].Color = 7;
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].State = StopRotate;
					Boss->Bullet[Bullet_No].Speed = 0.0f;
				}
			}
		}
		// フリーズ終了、移動開始
		else if (Count >= 330 && Count < 600)
		{
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					// ランダムな方向に移動し始める
					if (Boss->Bullet[Bullet_No].Count == 150)
					{
						// 全方向ランダム
						Boss->Bullet[Bullet_No].MoveDirection.x = (float)(rand() % (100 + 1) + (-50));
						Boss->Bullet[Bullet_No].MoveDirection.y = (float)(rand() % (100 + 1) + (-50));
						D3DXVec3Normalize(&Boss->Bullet[Bullet_No].MoveDirection, &Boss->Bullet[Bullet_No].MoveDirection);
						Boss->Bullet[Bullet_No].State = InRotate;
					}
					if (Boss->Bullet[Bullet_No].Count > 150)
					{
						// 段々加速
						Boss->Bullet[Bullet_No].Speed += 0.03f;
					}
				}
			}
		}
		else if (Count >= 600)
		{
			Boss->StateCount = 0;
		}
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// ボスの弾の移動
//=============================================================================
void BulletMove(BULLET *Bullet, int BarrageType)
{
	BOSS *Boss = GetBoss();

	// 1フレイム前の弾座標を記録
	Bullet->PreviousPos = Bullet->pos;

	// 座標更新
	Bullet->pos += Bullet->MoveDirection * Bullet->Speed;

	// 弾の移動先を検査
	switch (BarrageType)
	{
	case BarrageMode1:
		if (CheckPosition(Bullet->pos, Texture_BossBullet_1_Width, Texture_BossBullet_1_Height) == true)
		{
			Bullet->Use = false;
		}
		break;
	case BarrageMode2:
		if (CheckPosition(Bullet->pos, Texture_BossBullet_2_Width, Texture_BossBullet_2_Height) == true)
		{
			Bullet->Use = false;
		}
		break;
	case BarrageMode3:
		if (CheckPosition(Bullet->pos, Texture_BossBullet_3_Width, Texture_BossBullet_3_Height) == true)
		{
			Bullet->Use = false;
		}
		break;
	default:
		break;
	}

	// 特定タイプの弾だったら、回転する
	if (Bullet->State == InRotate)
	{
		Bullet->rot.z += 0.1f;
	}

	// カウント加算
	Bullet->Count++;

	return;
}

//=============================================================================
// 二次元ベクトル回転(x,y座標だけ)
//=============================================================================
D3DXVECTOR3 VectorRot(D3DXVECTOR3 Vec, float RotAngle)
{
	D3DXVECTOR3 AfterRotVec3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// (x0,y0)θ角を回転後(x1,y1)
	// x1 = x0cosθ - y0sinθ
	// y1 = x0sinθ + y0cosθ
	AfterRotVec3.x = Vec.x * cosf(RotAngle) - Vec.y * sinf(RotAngle);
	AfterRotVec3.y = Vec.x * sinf(RotAngle) + Vec.y * cosf(RotAngle);
	D3DXVec3Normalize(&AfterRotVec3, &AfterRotVec3);

	return AfterRotVec3;
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
	SetBulletTexture(Bullet, 0);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBulletTexture(BULLET *Bullet, int BarrageType)
{
	int x = Bullet->Color;
	float sizeX = 0.0f;

	switch (BarrageType)
	{
	case BarrageMode1:
		if (sizeX == 0.0f)
		{
			sizeX = 1.0f / Texture_BossBullet_1_Divide_X;
		}
	case BarrageMode3:
		if (sizeX == 0.0f)
		{
			sizeX = 1.0f / Texture_BossBullet_3_Divide_X;
		}
		Bullet->vertexwk[0].tex = D3DXVECTOR2((float)x * sizeX, 0.0f);
		Bullet->vertexwk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 0.0f);
		Bullet->vertexwk[2].tex = D3DXVECTOR2((float)x * sizeX, 1.0f);
		Bullet->vertexwk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 1.0f);
		break;
	case BarrageMode2:
		Bullet->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Bullet->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Bullet->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Bullet->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		break;
	default:
		break;
	}


	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBulletVertex(BULLET *Bullet)
{
	BOSS *Boss = GetBoss();

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