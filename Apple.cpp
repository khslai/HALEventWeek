//=============================================================================
//
// アップル処理 [Apple.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "Apple.h"
#include "AttackRange.h"
#include "Transition.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Apple			_T("data/Texture/Enemy/Apple.png")	// 画像
#define Texture_Apple_Width		(70)
#define Texture_Apple_Height	(70)

// アップルの落下速度
#define DropSpeed				(2.0f)
// アップルの落下範囲（左）
#define DropRange_Left			(100.0f)
// アップルの落下範囲（右）
#define DropRange_Right			(1700.0f)
// アップルの落下間隔
#define DropInterval			(90)
// 地面のY座標
#define GroundPosition_Y		(1040.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeAppleVertex(APPLE *Apple);
// 頂点座標の設定
void SetAppleVertex(APPLE *Apple);
// テクスチャ座標の設定
void SetAppleTexture(APPLE *Apple);
// アップル設置
void SetApple();


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャ情報
LPDIRECT3DTEXTURE9		Apple_Texture;
// アップル構造体
APPLE					Apple[Apple_Max];
// 画像中心から頂点までの角度
static float BaseAngle = 0.0f;
// 画像中心から頂点までの距離
static float Radius = 0.0f;
// アップル落下した数
int AppleNum = 0;
// 全部のアップル落下終了
bool AppleDropOver = false;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitApple(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Apple_No = 0;
	APPLE *Apple = GetApple(Apple_No);

	// アップルの初期化処理
	for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
	{
		Apple->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Apple->Use = false;

		// 頂点情報の作成
		MakeAppleVertex(Apple);
	}
	AppleNum = 0;
	AppleDropOver = false;

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Apple_Width / 2, Texture_Apple_Height / 2));
		BaseAngle = atan2f(Texture_Apple_Height, Texture_Apple_Width);

		// テクスチャの読み込み
		if (SafeLoad(Texture_Apple, &Apple_Texture, "Apple") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitApple(void)
{
	SafeRelease(Apple_Texture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateApple(void)
{
	int Apple_No = 0;
	int DropedApple = 0;
	static int Count = 0;
	APPLE *Apple = GetApple(Apple_No);

	// カウント加算
	Count++;

	if (Count % DropInterval == 0 && AppleNum != Apple_Max)
	{
		SetApple();
	}

	for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
	{
		// 使用している状態なら更新する
		if (Apple->Use == true)
		{
			// アップル落下
			Apple->pos.y += DropSpeed;

			//画面外判定処理
			if (Apple->pos.y >= GroundPosition_Y)
			{
				Apple->Use = false;
			}

			// 頂点座標更新
			SetAppleVertex(Apple);
			// テクスチャ座標更新
			SetAppleTexture(Apple);
		}
		else
		{
			DropedApple++;
		}
	}

	// ゲームオーバー
	if (DropedApple == Apple_Max && AppleDropOver == true)
	{
		SetTransition(Fadein);
	}


	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawApple(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Apple_No = 0;
	APPLE *Apple = GetApple(Apple_No);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
	{
		// 使用している状態なら描画する
		if (Apple->Use == true)
		{
			// テクスチャの設定
			Device->SetTexture(0, Apple_Texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Apple->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// アップル設置
//=============================================================================
void SetApple(void)
{
	int Apple_No = 0;

	for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++)
	{
		if (Apple[Apple_No].Use == false)
		{
			Apple[Apple_No].Use = true;
			Apple[Apple_No].pos.x = DropRange_Left + DropRange_Right * (float)rand() / RAND_MAX;
			Apple[Apple_No].pos.y = 0.0f;
			Apple[Apple_No].HitRadius = Radius;

			// 設置したアップルの数を加算
			AppleNum++;
			if (AppleNum == Apple_Max)
			{
				AppleDropOver = true;
			}

			break;
		}
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeAppleVertex(APPLE *Apple)
{
	// 頂点座標の設定
	SetAppleVertex(Apple);

	// rhwの設定
	Apple->vertexwk[0].rhw = 1.0f;
	Apple->vertexwk[1].rhw = 1.0f;
	Apple->vertexwk[2].rhw = 1.0f;
	Apple->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Apple->vertexwk[0].diffuse = WHITE(255);
	Apple->vertexwk[1].diffuse = WHITE(255);
	Apple->vertexwk[2].diffuse = WHITE(255);
	Apple->vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetAppleTexture(Apple);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetAppleVertex(APPLE *Apple)
{
	Apple->vertexwk[0].vtx.x = Apple->pos.x - cosf(BaseAngle) * Radius;
	Apple->vertexwk[0].vtx.y = Apple->pos.y - sinf(BaseAngle) * Radius;
	Apple->vertexwk[1].vtx.x = Apple->pos.x + cosf(BaseAngle) * Radius;
	Apple->vertexwk[1].vtx.y = Apple->pos.y - sinf(BaseAngle) * Radius;
	Apple->vertexwk[2].vtx.x = Apple->pos.x - cosf(BaseAngle) * Radius;
	Apple->vertexwk[2].vtx.y = Apple->pos.y + sinf(BaseAngle) * Radius;
	Apple->vertexwk[3].vtx.x = Apple->pos.x + cosf(BaseAngle) * Radius;
	Apple->vertexwk[3].vtx.y = Apple->pos.y + sinf(BaseAngle) * Radius;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetAppleTexture(APPLE *Apple)
{
	Apple->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Apple->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Apple->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Apple->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// アップル取得関数
//=============================================================================
APPLE *GetApple(int Apple_No)
{
	return(&Apple[Apple_No]);
}
