//=============================================================================
//
// 背景処理 [BackGround.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "BackGround.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_BackGround_Ite			_T("data/Texture/BackGround/BackGround_Ite.png")
#define Texture_BackGround_Oushi		_T("data/Texture/BackGround/BackGround_Oushi.png")
#define Texture_BackGround_Futago		_T("data/Texture/BackGround/BackGround_Futago.png")
#define Texture_BackGround_Shishi		_T("data/Texture/BackGround/BackGround_Shishi.jpg")

enum BGType
{
	BackGround_Ite,			// 射手座背景
	BackGround_Oushi,		// 牡牛座背景
	BackGround_Futago,		// 双子座背景
	BackGround_Shishi,		// 獅子座背景
	BackGround_Max,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBackGroundVertex(void);
// 頂点座標の設定
void SetBackGroundTexture(void);
// テクスチャ座標の設定
void SetBackGroundVertex(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 BackGroundTexture[BackGround_Max] = { NULL };
// 背景構造体
BACKGROUND BackGround;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBackGround(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	BackGround.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点情報の作成
	MakeBackGroundVertex();

	if (FirstInit == true)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_BackGround_Ite, &BackGroundTexture[BackGround_Ite], "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BackGround_Futago, &BackGroundTexture[BackGround_Futago], "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BackGround_Oushi, &BackGroundTexture[BackGround_Oushi], "BackGround") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BackGround_Shishi, &BackGroundTexture[BackGround_Shishi], "BackGround") == false)
		{
			return E_FAIL;
		}

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBackGround(void)
{
	for (int i = 0; i < BackGround_Max; i++)
	{
		SafeRelease(BackGroundTexture[i]);
	}

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBackGround(void)
{

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBackGround(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int GameStage = GetGameStage();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	switch (GameStage)
	{
	case Stage_Ite:
		Device->SetTexture(0, BackGroundTexture[BackGround_Ite]);
		break;
	case Stage_Futago:
		Device->SetTexture(0, BackGroundTexture[BackGround_Futago]);
		break;
	case Stage_Oushi:
		Device->SetTexture(0, BackGroundTexture[BackGround_Oushi]);
		break;
	case Stage_Shishi:
		Device->SetTexture(0, BackGroundTexture[BackGround_Shishi]);
		break;
	default:
		break;
	}

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BackGround.vertexwk, sizeof(VERTEX_2D));

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBackGroundVertex(void)
{
	// 頂点座標の設定	
	SetBackGroundVertex();

	// rhwの設定
	BackGround.vertexwk[0].rhw = 1.0f;
	BackGround.vertexwk[1].rhw = 1.0f;
	BackGround.vertexwk[2].rhw = 1.0f;
	BackGround.vertexwk[3].rhw = 1.0f;

	// 色、透明度の設定
	BackGround.vertexwk[0].diffuse = WHITE(255);
	BackGround.vertexwk[1].diffuse = WHITE(255);
	BackGround.vertexwk[2].diffuse = WHITE(255);
	BackGround.vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetBackGroundTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBackGroundTexture(void)
{
	BackGround.vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	BackGround.vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	BackGround.vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	BackGround.vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBackGroundVertex(void)
{
	BackGround.vertexwk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BackGround.vertexwk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BackGround.vertexwk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BackGround.vertexwk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	return;
}
