//=============================================================================
//
// 扉処理 [Door.cpp]
// Author：
//
//=============================================================================
#include "main.h"
#include "Door.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Door				_T("data/Texture/UI/Door.png")
#define Texture_Door_Width_Total	(900)	// テクスチャの横幅
#define Texture_Door_Height_Total	(110)	// テクスチャの縦幅
#define Texture_Door_Divide_X		(5)		// テクスチャ内分割数（横)
#define Texture_Door_Divide_Y		(1)		// テクスチャ内分割数（縦)
#define Texture_Door_Width			(Texture_Door_Width_Total / Texture_Door_Divide_X)
#define Texture_Door_Height			(Texture_Door_Height_Total / Texture_Door_Divide_Y)

// アニメーションを切り替えるカウント
#define AnimChangeAnimCount			(15)
#define TEXTURE_Door_SIZE_X			(180/2)	
#define TEXTURE_Door_SIZE_Y			(110/2)	
// 扉の表示座標
#define Door1_Pos					(D3DXVECTOR3(440.0f, 56.0f, 0.0f))	
#define Door2_Pos					(D3DXVECTOR3(1385.0f, 56.0f, 0.0f))	
// 扉の最大数
#define Door_Max					(2)										
// アニメーションを切り替えるカウント
#define AnimChangeCount				(15)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeDoorVertex(DOOR *Door);
// 頂点座標の設定
void SetDoorVertex(DOOR *Door);
// テクスチャ座標の設定
void SetDoorTexture(DOOR *Door);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャ
LPDIRECT3DTEXTURE9		Door_Texture = NULL;
// 扉構造体
DOOR					Door[Door_Max];
// 画像中心から頂点までの角度
static float BaseAngle = 0.0f;
// 画像中心から頂点までの距離
static float Radius = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDoor(bool FirstInit)
{
	int Door_No = 0;
	DOOR *Door = GetDoor(Door_No);

	// 扉の初期化処理
	for (Door_No = 0; Door_No < Door_Max; Door_No++, Door++)
	{
		Door->Use = true;
		Door->AnimPattern = 0;
		// アニメカウントを初期化
		Door->AnimCount = 0;
		// 最初は、ドアが閉まっている
		Door->State = DoorClose;
		// 座標データを初期化
		if (Door_No == 0)
		{
			Door->pos = Door1_Pos;
		}
		else if (Door_No == 1)
		{
			Door->pos = Door2_Pos;
		}

		// 頂点情報の作成
		MakeDoorVertex(Door);
	}

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Door_Width / 2, Texture_Door_Height / 2));
		BaseAngle = atan2f(Texture_Door_Height, Texture_Door_Width);

		// テクスチャの読み込み
		if (SafeLoad(Texture_Door, &Door_Texture, "Door") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDoor(void)
{
	// テクスチャの開放
	SafeRelease(Door_Texture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDoor(void)
{
	int Door_No = 0;
	DOOR *Door = GetDoor(Door_No);

	for (Door_No = 0; Door_No < Door_Max; Door_No++, Door++)
	{
		if (Door->Use == true)
		{
			if (Door->State == DoorOpening)
			{
				Door->AnimCount++;

				if (Door->AnimCount >= AnimChangeCount * Texture_Door_Divide_X)
				{
					Door->State = DoorOpenEnd;
					return;
				}

				// テクスチャアニメパターン計算
				if ((Door->AnimCount % AnimChangeCount) == 0)
				{
					Door->AnimPattern = (Door->AnimPattern + 1) % Texture_Door_Divide_X;
				}

				// 頂点座標の設定
				SetDoorVertex(Door);
				// テクスチャ座標の設定
				SetDoorTexture(Door);
			}
		}
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDoor(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Door_No = 0;
	DOOR *Door = GetDoor(Door_No);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, Door_Texture);

	for (Door_No = 0; Door_No < Door_Max; Door_No++, Door++)
	{
		// 使用している状態なら描画する
		if (Door->Use == true)
		{
			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Door->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeDoorVertex(DOOR *Door)
{
	// 頂点座標の設定
	SetDoorVertex(Door);

	// rhwの設定
	Door->vertexwk[0].rhw = 1.0f;
	Door->vertexwk[1].rhw = 1.0f;
	Door->vertexwk[2].rhw = 1.0f;
	Door->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Door->vertexwk[0].diffuse = WHITE(255);
	Door->vertexwk[1].diffuse = WHITE(255);
	Door->vertexwk[2].diffuse = WHITE(255);
	Door->vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetDoorTexture(Door);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetDoorVertex(DOOR *Door)
{
	// 頂点座標の設定
	Door->vertexwk[0].vtx.x = Door->pos.x - cosf(BaseAngle) * Radius;
	Door->vertexwk[0].vtx.y = Door->pos.y - sinf(BaseAngle) * Radius;
	Door->vertexwk[1].vtx.x = Door->pos.x + cosf(BaseAngle) * Radius;
	Door->vertexwk[1].vtx.y = Door->pos.y - sinf(BaseAngle) * Radius;
	Door->vertexwk[2].vtx.x = Door->pos.x - cosf(BaseAngle) * Radius;
	Door->vertexwk[2].vtx.y = Door->pos.y + sinf(BaseAngle) * Radius;
	Door->vertexwk[3].vtx.x = Door->pos.x + cosf(BaseAngle) * Radius;
	Door->vertexwk[3].vtx.y = Door->pos.y + sinf(BaseAngle) * Radius;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetDoorTexture(DOOR *Door)
{
	int x = Door->AnimPattern;
	int y = 0;
	float sizeX = 1.0f / Texture_Door_Divide_X;
	float sizeY = 1.0f / Texture_Door_Divide_Y;

	Door->vertexwk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	Door->vertexwk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	Door->vertexwk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	Door->vertexwk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 扉取得関数
//=============================================================================
DOOR *GetDoor(int Door_No)
{
	return &Door[Door_No];
}

//=============================================================================
// 扉のテクスチャのサイズを取得する
//=============================================================================
D3DXVECTOR2 GetDoorSize(void)
{
	return D3DXVECTOR2(Texture_Door_Width - 50, Texture_Door_Height - 50);
}

