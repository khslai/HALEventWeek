//=============================================================================
//
// ��Q������ [Block.cpp]
// Author�F
//
//=============================================================================
#include "main.h"
#include "Block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Block			_T("data/Texture/UI/Block.png")
#define Texture_Block_Width		(80)
#define Texture_Block_Height	(80)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBlockVertex(BLOCK *Block);
// ���_���W�̐ݒ�
void SetBlockVertex(BLOCK *Block);
// �e�N�X�`�����W�̐ݒ�
void SetBlockTexture(BLOCK *Block);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		Block_Texture = NULL;
// �摜���S���璸�_�܂ł̊p�x
static float BaseAngle = 0.0f;
// �摜���S���璸�_�܂ł̋���
static float Radius = 0.0f;
// ��Q���\����
BLOCK					Block[Block_Max];
// ��Q���̍��W
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
// ����������
//=============================================================================
HRESULT InitBlock(bool FirstInit)
{
	int Block_No = 0;
	BLOCK *Block = GetBlock(Block_No);

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Block_Width / 2, Texture_Block_Height / 2));
		BaseAngle = atan2f(Texture_Block_Height, Texture_Block_Width);

		// ��Q���̏���������
		for (Block_No = 0; Block_No < Block_Max; Block_No++, Block++)
		{
			// ��Q���̈ʒu���W��������
			Block->pos = BlockPos[Block_No];	

			// ���_���̍쐬
			MakeBlockVertex(Block);
		}

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Block, &Block_Texture, "Block") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlock(void)
{
	// �e�N�X�`���̊J��
	SafeRelease(Block_Texture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlock(void)
{
	// ��Q���͍X�V���Ȃ�

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Block_No = 0;
	BLOCK *Block = GetBlock(Block_No);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, Block_Texture);

	for (Block_No = 0; Block_No < Block_Max; Block_No++, Block++)
	{
		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Block->vertexwk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBlockVertex(BLOCK *Block)
{
	// ���_���W�̐ݒ�
	SetBlockVertex(Block);

	// rhw�̐ݒ�
	Block->vertexwk[0].rhw = 1.0f;
	Block->vertexwk[1].rhw = 1.0f;
	Block->vertexwk[2].rhw = 1.0f;
	Block->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Block->vertexwk[0].diffuse = WHITE(255);
	Block->vertexwk[1].diffuse = WHITE(255);
	Block->vertexwk[2].diffuse = WHITE(255);
	Block->vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetBlockTexture(Block);

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
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
// �e�N�X�`�����W�̐ݒ�
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
// ��Q���擾�֐�
//=============================================================================
BLOCK *GetBlock(int Block_No)
{
	return &Block[Block_No];
}

//=============================================================================
// ��Q���̃e�N�X�`���̃T�C�Y���擾����
//=============================================================================
D3DXVECTOR2 GetBlockSize(void)
{
	return D3DXVECTOR2(Texture_Block_Width - 20, Texture_Block_Height - 20);
}