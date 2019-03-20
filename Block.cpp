//=============================================================================
//
// 障害物処理 [Block.cpp]
// Author：
//
//=============================================================================
#include "main.h"
#include "Block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Block			_T("data/Texture/UI/Block.png")
#define Texture_Block_Width		(80)
#define Texture_Block_Height	(80)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 頂点の作成
HRESULT MakeBlockVertex(BLOCK *Block);
// 頂点座標の設定
void SetBlockVertex(BLOCK *Block);
// テクスチャ座標の設定
void SetBlockTexture(BLOCK *Block);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		Block_Texture = NULL;
// 画像中心から頂点までの角度
static float BaseAngle = 0.0f;
// 画像中心から頂点までの距離
static float Radius = 0.0f;
// 障害物構造体
BLOCK					Block[Block_Max];
// 障害物の座標
D3DXVECTOR3				BlockPos[Block_Max] =
{
	D3DXVECTOR3(390.0f, 150.0f, 0.0f),
	D3DXVECTOR3(193.0f, 210.0f, 0.0f),
	D3DXVECTOR3(450.0f, 210.0f, 0.0f),
	D3DXVECTOR3(525.0f, 210.0f, 0.0f),
	D3DXVECTOR3(860.0f, 210.0f, 0.0f),
	D3DXVECTOR3(110.0f, 360.0f, 0.0f),
	D3DXVECTOR3(690.0f, 360.0f, 0.0f),
	D3DXVECTOR3(190.0f, 440.0f, 0.0f),
	D3DXVECTOR3(530.0f, 440.0f, 0.0f),
	D3DXVECTOR3(530.0f, 520.0f, 0.0f),
	D3DXVECTOR3(325.0f, 590.0f, 0.0f),
	D3DXVECTOR3(255.0f, 670.0f, 0.0f),
	D3DXVECTOR3(780.0f, 670.0f, 0.0f),
	D3DXVECTOR3(595.0f, 780.0f, 0.0f),
	D3DXVECTOR3(1135.0f, 145.0f, 0.0f),
	D3DXVECTOR3(1455.0f, 145.0f, 0.0f),
	D3DXVECTOR3(1055.0f, 200.0f, 0.0f),
	D3DXVECTOR3(1355.0f, 360.0f, 0.0f),
	D3DXVECTOR3(1555.0f, 360.0f, 0.0f),
	D3DXVECTOR3(1135.0f, 435.0f, 0.0f),
	D3DXVECTOR3(1425.0f, 605.0f, 0.0f),
	D3DXVECTOR3(1265.0f, 675.0f, 0.0f),
	D3DXVECTOR3(1705.0f, 785.0f, 0.0f),
	D3DXVECTOR3(1225.0f, 860.0f, 0.0f),
	D3DXVECTOR3(1055.0f, 940.0f, 0.0f),
};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlock(bool FirstInit)
{
	int Block_No = 0;
	BLOCK *Block = GetBlock(Block_No);

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Block_Width / 2, Texture_Block_Height / 2));
		BaseAngle = atan2f(Texture_Block_Height, Texture_Block_Width);

		// 障害物の初期化処理
		for (Block_No = 0; Block_No < Block_Max; Block_No++, Block++)
		{
			// 障害物の位置座標を初期化
			Block->pos = BlockPos[Block_No];	

			// 頂点情報の作成
			MakeBlockVertex(Block);
		}

		// テクスチャの読み込み
		if (SafeLoad(Texture_Block, &Block_Texture, "Block") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	// テクスチャの開放
	SafeRelease(Block_Texture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{
	// 障害物は更新しない

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Block_No = 0;
	BLOCK *Block = GetBlock(Block_No);

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	Device->SetTexture(0, Block_Texture);

	for (Block_No = 0; Block_No < Block_Max; Block_No++, Block++)
	{
		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Block->vertexwk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeBlockVertex(BLOCK *Block)
{
	// 頂点座標の設定
	SetBlockVertex(Block);

	// rhwの設定
	Block->vertexwk[0].rhw = 1.0f;
	Block->vertexwk[1].rhw = 1.0f;
	Block->vertexwk[2].rhw = 1.0f;
	Block->vertexwk[3].rhw = 1.0f;

	// 反射光の設定
	Block->vertexwk[0].diffuse = WHITE(255);
	Block->vertexwk[1].diffuse = WHITE(255);
	Block->vertexwk[2].diffuse = WHITE(255);
	Block->vertexwk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetBlockTexture(Block);

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetBlockVertex(BLOCK *Block)
{
	Block->vertexwk[0].vtx.x = Block->pos.x - Texture_Block_Width / 2;
	Block->vertexwk[0].vtx.y = Block->pos.y - Texture_Block_Height / 2;
	Block->vertexwk[0].vtx.z = 0.0f;

	Block->vertexwk[1].vtx.x = Block->pos.x + Texture_Block_Width / 2;
	Block->vertexwk[1].vtx.y = Block->pos.y - Texture_Block_Height / 2;
	Block->vertexwk[1].vtx.z = 0.0f;

	Block->vertexwk[2].vtx.x = Block->pos.x - Texture_Block_Width / 2;
	Block->vertexwk[2].vtx.y = Block->pos.y + Texture_Block_Height / 2;
	Block->vertexwk[2].vtx.z = 0.0f;

	Block->vertexwk[3].vtx.x = Block->pos.x + Texture_Block_Width / 2;
	Block->vertexwk[3].vtx.y = Block->pos.y + Texture_Block_Height / 2;
	Block->vertexwk[3].vtx.z = 0.0f;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetBlockTexture(BLOCK *Block)
{
	Block->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Block->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Block->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Block->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// 障害物取得関数
//=============================================================================
BLOCK *GetBlock(int Block_No)
{
	return &Block[Block_No];
}

//=============================================================================
// 障害物のテクスチャのサイズを取得する
//=============================================================================
D3DXVECTOR2 GetBlockSize(void)
{
	return D3DXVECTOR2(Texture_Block_Width - 20, Texture_Block_Height - 20);
}