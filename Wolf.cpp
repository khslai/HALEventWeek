//=============================================================================
//
// 狼処理 [Wolf.cpp]
// Author： 
//
//=============================================================================
#include "main.h"
#include "Wolf.h"
#include "AttackRange.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Wolf				_T("data/Texture/Enemy/Wolf.png")
#define Texture_Wolf_Width_Total	(300)
#define Texture_Wolf_Height_Total	(400)
#define Texture_Wolf_Divide_X		(3)
#define Texture_Wolf_Divide_Y		(4)
#define Texture_Wolf_Width			(Texture_Wolf_Width_Total / Texture_Wolf_Divide_X)
#define Texture_Wolf_Height			(Texture_Wolf_Height_Total / Texture_Wolf_Divide_Y)

// アニメーションを切り替えるカウント
#define AnimChangeAnimCount					(15)
// 狼の移動速度
#define WolfMoveSpeed					(1.5f)
// 狼の出現間隔
#define SetWolfInterval					(120)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeWolfVertex(WOLF *Wolf);
// 頂点座標の設定
void SetWolfVertex(WOLF *Wolf);
// テクスチャ座標の設定
void SetWolfTexture(WOLF *Wolf);
// 狼設置
void SetWolf();


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャ情報
LPDIRECT3DTEXTURE9		Wolf_Texture;
// 狼構造体
WOLF					Wolf[Wolf_Max];
// 画像中心から頂点までの角度
static float BaseAngle = 0.0f;
// 画像中心から頂点までの距離
static float Radius = 0.0f;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWolf(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Wolf_No = 0;
	WOLF *Wolf = GetWolf(Wolf_No);

	// 狼の初期化処理
	for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
	{
		Wolf->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Wolf->HitRadius = 0.0f;
		Wolf->AnimCount = 0;
		Wolf->AnimPattern = 0;
		Wolf->Direction = 0;
		Wolf->Use = false;

		// 頂点情報の作成
		MakeWolfVertex(Wolf);
	}

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Wolf_Width / 2, Texture_Wolf_Height / 2));
		BaseAngle = atan2f(Texture_Wolf_Height, Texture_Wolf_Width);

		// テクスチャの読み込み
		if (SafeLoad(Texture_Wolf, &Wolf_Texture, "Wolf") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWolf(void)
{
	SafeRelease(Wolf_Texture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWolf(void)
{
	int Wolf_No = 0;
	static int AnimCount = 0;
	static int SetWolfCD = 0;
	WOLF *Wolf = GetWolf(Wolf_No);

	// カウント加算
	AnimCount++;

	if (SetWolfCD == 0)
	{
		SetWolfCD = rand() % (30 + 1) + SetWolfInterval;
	}

	if (AnimCount % SetWolfCD == 0)
	{
		SetWolf();
		SetWolfCD = 0;
	}

	for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
	{
		// 使用している状態なら更新する
		if (Wolf->Use == true)
		{
			// カウント加算
			Wolf->AnimCount++;

			// テクスチャアニメパターン計算
			if ((Wolf->AnimCount % AnimChangeAnimCount) == 0)
			{
				Wolf->AnimPattern = (Wolf->AnimPattern + 1) % Texture_Wolf_Divide_X;
			}

			// 狼移動
			if (Wolf->Direction == Right)
			{
				Wolf->pos.x += WolfMoveSpeed;
			}
			else if (Wolf->Direction == Left)
			{
				Wolf->pos.x -= WolfMoveSpeed;
			}

			// 頂点座標更新
			SetWolfVertex(Wolf);
			// テクスチャ座標更新
			SetWolfTexture(Wolf);
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWolf(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Wolf_No = 0;
	WOLF *Wolf = GetWolf(Wolf_No);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
	{
		// 使用している状態なら描画する
		if (Wolf->Use == true)
		{
			// テクスチャの設定
			Device->SetTexture(0, Wolf_Texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Wolf->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 狼設置
//=============================================================================
void SetWolf(void)
{
	int Wolf_No = 0;
	static bool LeftDir = false;

	for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++)
	{
		if (Wolf[Wolf_No].Use == false)
		{
			Wolf[Wolf_No].Use = true;
			Wolf[Wolf_No].HitRadius = Radius;
			Wolf[Wolf_No].AnimCount = 0;
			Wolf[Wolf_No].AnimPattern = 0;
			if (LeftDir == true)
			{
				Wolf[Wolf_No].pos = D3DXVECTOR3(2000.0f, 980.0f, 0.0f);
				Wolf[Wolf_No].Direction = Left;
				LeftDir = false;
			}
			else
			{
				Wolf[Wolf_No].pos = D3DXVECTOR3(-100.0f, 980.0f, 0.0f);
				Wolf[Wolf_No].Direction = Right;
				LeftDir = true;
			}

			break;
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeWolfVertex(WOLF *Wolf)
{
	// 頂点座標の設定
	SetWolfVertex(Wolf);

	// rhwの設定
	Wolf->vertexwk[0].rhw = 1.0f;
	Wolf->vertexwk[1].rhw = 1.0f;
	Wolf->vertexwk[2].rhw = 1.0f;
	Wolf->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Wolf->vertexwk[0].diffuse = WHITE(255);
	Wolf->vertexwk[1].diffuse = WHITE(255);
	Wolf->vertexwk[2].diffuse = WHITE(255);
	Wolf->vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetWolfTexture(Wolf);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetWolfVertex(WOLF *Wolf)
{
	Wolf->vertexwk[0].vtx.x = Wolf->pos.x - cosf(BaseAngle) * Radius;
	Wolf->vertexwk[0].vtx.y = Wolf->pos.y - sinf(BaseAngle) * Radius;
	Wolf->vertexwk[1].vtx.x = Wolf->pos.x + cosf(BaseAngle) * Radius;
	Wolf->vertexwk[1].vtx.y = Wolf->pos.y - sinf(BaseAngle) * Radius;
	Wolf->vertexwk[2].vtx.x = Wolf->pos.x - cosf(BaseAngle) * Radius;
	Wolf->vertexwk[2].vtx.y = Wolf->pos.y + sinf(BaseAngle) * Radius;
	Wolf->vertexwk[3].vtx.x = Wolf->pos.x + cosf(BaseAngle) * Radius;
	Wolf->vertexwk[3].vtx.y = Wolf->pos.y + sinf(BaseAngle) * Radius;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetWolfTexture(WOLF *Wolf)
{
	int x = Wolf->AnimPattern;
	int y = Wolf->Direction;
	float sizeX = 1.0f / Texture_Wolf_Divide_X;
	float sizeY = 1.0f / Texture_Wolf_Divide_Y;

	Wolf->vertexwk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	Wolf->vertexwk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	Wolf->vertexwk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	Wolf->vertexwk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 狼取得関数
//=============================================================================
WOLF *GetWolf(int Wolf_No)
{
	return(&Wolf[Wolf_No]);
}

//=============================================================================
// 狼のテクスチャのサイズを取得する
//=============================================================================
D3DXVECTOR2 GetWolfSize(void)
{
	return D3DXVECTOR2(Texture_Wolf_Width - 40, Texture_Wolf_Height - 40);
}
