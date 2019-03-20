//=============================================================================
//
// 牡牛処理 [Bull.cpp]
// Author： 
//
//=============================================================================
#include "main.h"
#include "Bull.h"
#include "Player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Bull				_T("data/Texture/Enemy/Bull.png")
#define Texture_Bull_Width_Total	(1144)
#define Texture_Bull_Height_Total	(216)
#define Texture_Bull_Divide_X		(4)
#define Texture_Bull_Divide_Y		(1)
#define Texture_Bull_Width			(Texture_Bull_Width_Total / Texture_Bull_Divide_X)
#define Texture_Bull_Height			(Texture_Bull_Height_Total / Texture_Bull_Divide_Y)

// アニメーションを切り替えるカウント
#define AnimChangeCount					(4)
// 牡牛の加速度
#define Bull_Acceleration				(20.0f)		


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBullVertex(BULL *Bull);
// 頂点座標の設定
void SetBullVertex(BULL *Bull);
// テクスチャ座標の設定
void SetBullTexture(BULL *Bull);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャ情報
LPDIRECT3DTEXTURE9		Bull_Texture;
// 牡牛構造体
BULL					Bull[Bull_Max];
// 画像中心から頂点までの角度
static float BaseAngle = 0.0f;
// 画像中心から頂点までの距離
static float Radius = 0.0f;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBull(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Bull_No = 0;
	BULL *Bull = GetBull(Bull_No);
	PLAYER *Player = GetPlayer(0);

	Bull[0].pos = D3DXVECTOR3(600.0f, 100.0f, 0.0f);
	Bull[0].f_pos = Player[0].pos;
	Bull[1].pos = D3DXVECTOR3(600.0f, 400.0f, 0.0f);
	Bull[1].f_pos = Player[1].pos;

	Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Bull_Width / 2, Texture_Bull_Height / 2));
	BaseAngle = atan2f(Texture_Bull_Height, Texture_Bull_Width);

	// 牡牛の初期化処理
	for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++)
	{
		Bull[Bull_No].AnimCount = 0;
		Bull[Bull_No].AnimPattern = 0;
		Bull[Bull_No].part = 0;
		Bull[Bull_No].move_x = 0.0f;
		Bull[Bull_No].move_y = 0.0f;
		Bull[Bull_No].dealttime = 0.0f;
		Bull[Bull_No].move_v0 = 10.0f;
		Bull[Bull_No].move_dash_x = Bull[Bull_No].f_pos.x - Bull[Bull_No].pos.x;
		Bull[Bull_No].move_dash_y = Bull[Bull_No].f_pos.y - Bull[Bull_No].pos.y;
		Bull[Bull_No].Use = true;
		Bull[Bull_No].posX = true;

		// 頂点情報の作成
		MakeBullVertex(&Bull[Bull_No]);
	}

	if (FirstInit == true)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Bull, &Bull_Texture, "Bull") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBull(void)
{
	SafeRelease(Bull_Texture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBull(void)
{
	int Bull_No = 0;
	PLAYER *Player = GetPlayer(0);
	BULL *Bull = GetBull(0);

	for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++, Bull++)
	{
		/*敵は使用しているなら*/
		if (Bull->Use == true)
		{
			// アニメーションカウント加算
			Bull->AnimCount++;

			// アニメーションの切り替え
			if ((Bull->AnimCount % AnimChangeCount) == 0)
			{
				Bull->AnimPattern = (Bull->AnimPattern + 1) % Texture_Bull_Divide_X;
			}

			//動くの時間計算
			Bull->dealttime += 0.01f;

			//後退して
			if (Bull->part == 0)
			{
				if (Bull->pos.x < Bull->f_pos.x)//Ｘ方向の敵とプレィヤーの距離計算
				{
					Bull->posX = true;
					Bull->move_x = -Bull->move_v0 + Bull->dealttime * Bull_Acceleration;//Ｘ方向の速度が0までの移動量計算
					if (Bull->move_x > 0.0f)//もし速度が0以上になったら、ダッシュに移行する
					{
						Bull->move_dash_x = Bull->f_pos.x - Bull->pos.x;//ダッシュの速度はプレィヤーと敵の距離/20
						Bull->part = 1;//ダッシュ状態
					}
				}
				else if (Bull->pos.x > Bull->f_pos.x)
				{
					Bull->posX = false;
					Bull->move_x = Bull->move_v0 + Bull->dealttime * -Bull_Acceleration;
					if (Bull->move_x < 0.0f)
					{
						Bull->move_dash_x = Bull->f_pos.x - Bull->pos.x;
						Bull->part = 1;
					}
				}

				if (Bull->pos.y < Bull->f_pos.y)
				{
					Bull->move_y = -Bull->move_v0 + Bull->dealttime * Bull_Acceleration;
					if (Bull->move_y > 0.0f)
					{
						Bull->move_dash_y = Bull->f_pos.y - Bull->pos.y;
						Bull->part = 1;
					}
				}
				else if (Bull->pos.y > Bull->f_pos.y)
				{
					Bull->move_y = Bull->move_v0 + Bull->dealttime * -Bull_Acceleration;
					if (Bull->move_y < 0.0f)
					{
						Bull->move_dash_y = Bull->f_pos.y - Bull->pos.y;
						Bull->part = 1;
					}
				}

				Bull->pos.x += Bull->move_x;//移動計算
				Bull->pos.y += Bull->move_y;
			}

			//前にダッシュ
			if (Bull->part == 1)
			{
				Bull->move_x = Bull->move_dash_x / 20;
				Bull->move_y = Bull->move_dash_y / 20;

				Bull->pos.x += Bull->move_x;
				Bull->pos.y += Bull->move_y;
			}

			//sinx 移動軌跡 ダッシュ後 慣性を入れる
			if (Bull->part == 2)
			{
				if (Bull->dealttime < 6.28f * 3.0f)
				{
					if (Bull->pos.x < Bull->f_pos.x)
					{
						Bull->pos.x -= 1.0f;
					}
					else if (Bull->pos.x > Bull->f_pos.x)
					{
						Bull->pos.x += 1.0f;
					}
					Bull->pos.y -= sinf(Bull->dealttime) * 3.0f;
				}
				if (Bull->dealttime > 6.28f * 3.0f)
				{
					Bull->dealttime = -1.0f;
					Bull->f_pos = Player[Bull_No].pos;
					Bull->part = 3;
				}
				Bull->dealttime += 0.3f;
			}

			// 敵が壁を当たったら
			// 右の壁
			if (Bull->pos.x > Screen_Width - (Texture_Bull_Width / 2 + Wall_Width))
			{
				if (Bull->part == 0)//状態へ移行
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)//状態へ移行
				{
					Bull->part = 2;
				}
				Bull->pos.x = Screen_Width - (Texture_Bull_Width / 2 + Wall_Width);
			}
			// 左の壁
			else if (Bull->pos.x < Texture_Bull_Width / 2 + Wall_Width)
			{
				if (Bull->part == 0)
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)
				{
					Bull->part = 2;
				}

				Bull->pos.x = Texture_Bull_Width / 2 + Wall_Width;
			}

			// 下の壁
			if (Bull->pos.y > Screen_Height - (Texture_Bull_Height / 2 + Wall_Width))
			{
				if (Bull->part == 0)
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)
				{
					Bull->part = 2;
				}

				Bull->pos.y = Screen_Height - (Texture_Bull_Height / 2 + Wall_Width);
			}
			// 上の壁
			else if (Bull->pos.y < Texture_Bull_Height / 2 + Wall_Width)
			{
				if (Bull->part == 0)
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)
				{
					Bull->part = 2;
				}

				Bull->pos.y = Texture_Bull_Height / 2 + Wall_Width;
			}

			//ループさせる
			if (Bull->part == 3)
			{
				Bull->move_x = Bull->move_y = 0.0f;
				Bull->f_pos.x = Player[Bull_No].pos.x;
				Bull->f_pos.y = Player[Bull_No].pos.y;
				Bull->move_dash_x = Bull->f_pos.x - Bull->pos.x;
				Bull->move_dash_y = Bull->f_pos.y - Bull->pos.y;

				//休憩時間、0.0fは休憩しない
				if (Bull->dealttime > 1.0f)
				{
					Bull->part = 0;
					Bull->dealttime = 0.0f;
				}
			}
		}

		// 頂点座標更新
		SetBullVertex(Bull);
		// テクスチャ座標更新
		SetBullTexture(Bull);
	}
	
	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBull(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Bull_No = 0;
	BULL *Bull = GetBull(Bull_No);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++, Bull++)
	{
		// 使用している状態なら描画する
		if (Bull->Use == true)
		{
			// テクスチャの設定
			Device->SetTexture(0, Bull_Texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bull->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBullVertex(BULL *Bull)
{
	// 頂点座標の設定	
	SetBullVertex(Bull);	// 頂点の計算処理

	// rhwの設定
	Bull->vertexwk[0].rhw = 1.0f;
	Bull->vertexwk[1].rhw = 1.0f;
	Bull->vertexwk[2].rhw = 1.0f;
	Bull->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Bull->vertexwk[0].diffuse = WHITE(255);
	Bull->vertexwk[1].diffuse = WHITE(255);
	Bull->vertexwk[2].diffuse = WHITE(255);
	Bull->vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetBullTexture(Bull);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBullTexture(BULL *Bull)
{
	int x = Bull->AnimPattern;
	float sizeX = 1.0f / Texture_Bull_Divide_X;
	float sizeY = 1.0f / Texture_Bull_Divide_Y;

	Bull->vertexwk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	Bull->vertexwk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	Bull->vertexwk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	Bull->vertexwk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBullVertex(BULL *Bull)
{
	//右方向　画像にょってこれもチェンジします　普通は画像が右に向かっている
	if (Bull->posX == true)
	{
		Bull->vertexwk[0].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[0].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
	}
	//左方向
	else if (Bull->posX == false)
	{
		Bull->vertexwk[0].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[0].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
	}
}

//=============================================================================
// 牡牛取得関数
//=============================================================================
BULL *GetBull(int Bull_No)
{
	return &Bull[Bull_No];
}

//=============================================================================
// 牡牛のテクスチャのサイズを取得する
//=============================================================================
D3DXVECTOR2 GetBullSize(void)
{
	return D3DXVECTOR2(Texture_Bull_Width - 40, Texture_Bull_Height - 40);
}
