//=============================================================================
//
// スコア処理 [Score.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "Score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Number				_T("data/Texture/UI/Number.png")
#define Texture_Number_Width_Total	(700)
#define Texture_Number_Height_Total	(70)
#define Texture_Number_Divide_X		(10)
#define Texture_Number_Divide_Y		(1)
#define Texture_Number_Width		(Texture_Number_Width_Total / Texture_Number_Divide_X)
#define Texture_Number_Height		(Texture_Number_Height_Total / Texture_Number_Divide_Y)

// スコア表示の初期位置
#define Score_Pos_X					(960)
#define Score_Pos_Y					(40)
// スコアの最大値
#define Score_MAX					(99)		
// スコアの桁数
#define DigitMax					(2)

#define Texture_ScoreBox			_T("data/Texture/UI/ScoreBox.png")
#define Texture_ScoreBox_Width		(250)
#define Texture_ScoreBox_Height		(120)

// タイマーの表示座標
#define ScoreBox_Pos_X				(835)
#define ScoreBox_Pos_Y				(0)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum);
HRESULT MakeScoreBoxVertex(void);
// 頂点座標の設定
void SetScoreVertex(int ScoreDigit);
void SetScoreBoxVertex(void);
// テクスチャ座標の設定
void SetScoreTexture(int ScoreDigit, int ScoreNum);
void SetScoreBoxTexture(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// テクスチャのポインタ
LPDIRECT3DTEXTURE9 ScoreTexture = NULL;
LPDIRECT3DTEXTURE9 ScoreBoxTexture = NULL;
// スコア構造体
SCORE Score[DigitMax];
// スコア枠構造体
SCORE ScoreBox;
// プレイヤースコア
int PlayerScore = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(bool FirstInit)
{
	int ScoreDigit = 0;
	PlayerScore = 0;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].pos = D3DXVECTOR3((float)Score_Pos_X - ScoreDigit * Texture_Number_Width, Score_Pos_Y, 0.0f);
		Score[ScoreDigit].Number = 0;
		// 頂点情報の作成
		MakeScoreVertex(ScoreDigit, Score[ScoreDigit].Number);
	}

	if (FirstInit == true)
	{
		// 頂点情報の作成
		MakeScoreBoxVertex();

		// テクスチャの読み込み
		if (SafeLoad(Texture_Number, &ScoreTexture, "Score") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ScoreBox, &ScoreBoxTexture, "Score") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	// テクスチャの開放
	SafeRelease(ScoreTexture);
	SafeRelease(ScoreBoxTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	int ScoreDigit = 0;
	int Scoretemp = PlayerScore;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].Number = Scoretemp % 10;
		Scoretemp /= 10;

		// テクスチャ座標を設定
		SetScoreTexture(ScoreDigit, Score[ScoreDigit].Number);
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	int ScoreDigit = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, ScoreBoxTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, ScoreBox.vertexwk, sizeof(VERTEX_2D));

	// テクスチャの設定
	Device->SetTexture(0, ScoreTexture);

	// ポリゴンの描画
	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Score[ScoreDigit].vertexwk, sizeof(VERTEX_2D));
	}


	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum)
{
	// 頂点座標の設定	
	SetScoreVertex(ScoreDigit);

	// rhwの設定
	Score[ScoreDigit].vertexwk[0].rhw = 1.0f;
	Score[ScoreDigit].vertexwk[1].rhw = 1.0f;
	Score[ScoreDigit].vertexwk[2].rhw = 1.0f;
	Score[ScoreDigit].vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Score[ScoreDigit].vertexwk[0].diffuse = WHITE(255);
	Score[ScoreDigit].vertexwk[1].diffuse = WHITE(255);
	Score[ScoreDigit].vertexwk[2].diffuse = WHITE(255);
	Score[ScoreDigit].vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetScoreTexture(ScoreDigit, ScoreNum);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetScoreTexture(int ScoreDigit, int ScoreNum)
{
	Score[ScoreDigit].vertexwk[0].tex = D3DXVECTOR2(ScoreNum * 0.1f, 0.0f);
	Score[ScoreDigit].vertexwk[1].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 0.0f);
	Score[ScoreDigit].vertexwk[2].tex = D3DXVECTOR2(ScoreNum * 0.1f, 1.0f);
	Score[ScoreDigit].vertexwk[3].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetScoreVertex(int ScoreDigit)
{
	Score[ScoreDigit].vertexwk[0].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x, Score[ScoreDigit].pos.y, 0.0f);
	Score[ScoreDigit].vertexwk[1].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x + Texture_Number_Width, Score[ScoreDigit].pos.y, 0.0f);
	Score[ScoreDigit].vertexwk[2].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x, Score[ScoreDigit].pos.y + Texture_Number_Height, 0.0f);
	Score[ScoreDigit].vertexwk[3].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x + Texture_Number_Width, Score[ScoreDigit].pos.y + Texture_Number_Height, 0.0f);

	return;
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeScoreBoxVertex(void)
{
	// 頂点座標の設定	
	SetScoreBoxVertex();

	// rhwの設定
	ScoreBox.vertexwk[0].rhw = 1.0f;
	ScoreBox.vertexwk[1].rhw = 1.0f;
	ScoreBox.vertexwk[2].rhw = 1.0f;
	ScoreBox.vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	ScoreBox.vertexwk[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	ScoreBox.vertexwk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	ScoreBox.vertexwk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	ScoreBox.vertexwk[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

	// テクスチャ座標の設定
	SetScoreBoxTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetScoreBoxTexture(void)
{
	ScoreBox.vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ScoreBox.vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ScoreBox.vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ScoreBox.vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetScoreBoxVertex(void)
{
	ScoreBox.vertexwk[0].vtx = D3DXVECTOR3(ScoreBox_Pos_X, ScoreBox_Pos_Y, 0.0f);
	ScoreBox.vertexwk[1].vtx = D3DXVECTOR3(ScoreBox_Pos_X + Texture_ScoreBox_Width, ScoreBox_Pos_Y, 0.0f);
	ScoreBox.vertexwk[2].vtx = D3DXVECTOR3(ScoreBox_Pos_X, ScoreBox_Pos_Y + Texture_ScoreBox_Height, 0.0f);
	ScoreBox.vertexwk[3].vtx = D3DXVECTOR3(ScoreBox_Pos_X + Texture_ScoreBox_Width, ScoreBox_Pos_Y + Texture_ScoreBox_Height, 0.0f);

	return;
}

//=============================================================================
// スコア加算
//=============================================================================
void AddScore(int Score)
{
	PlayerScore += Score;
	PlayerScore = PlayerScore > Score_MAX ? (int)Score_MAX : PlayerScore;

	return;
}

//=============================================================================
// スコアを取得
//=============================================================================
int GetScore(void)
{
	return PlayerScore;
}

//=============================================================================
// スコア数字のテクスチャを取得
//=============================================================================
LPDIRECT3DTEXTURE9 GetNumberTex(void)
{
	return ScoreTexture;
}
