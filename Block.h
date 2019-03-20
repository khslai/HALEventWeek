//=============================================================================
//
// 障害物処理 [block.h]
// Author： 
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 障害物の最大数
#define Block_Max	(25)								

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
// 障害物構造体
typedef struct	
{
	VERTEX_2D		vertexwk[Num_Vertex];		// 頂点情報格納ワーク
	D3DXVECTOR3		pos;						// 座標
}BLOCK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBlock(bool FirstInit);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
// 障害物取得関数
BLOCK *GetBlock(int Block_No);
// 障害物のテクスチャのサイズを取得する
D3DXVECTOR2 GetBlockSize(void);


#endif
