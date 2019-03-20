//=============================================================================
//
// �T���� [Wolf.cpp]
// Author�F 
//
//=============================================================================
#include "main.h"
#include "Wolf.h"
#include "AttackRange.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Wolf				_T("data/Texture/Enemy/Wolf.png")
#define Texture_Wolf_Width_Total	(300)
#define Texture_Wolf_Height_Total	(400)
#define Texture_Wolf_Divide_X		(3)
#define Texture_Wolf_Divide_Y		(4)
#define Texture_Wolf_Width			(Texture_Wolf_Width_Total / Texture_Wolf_Divide_X)
#define Texture_Wolf_Height			(Texture_Wolf_Height_Total / Texture_Wolf_Divide_Y)

// �A�j���[�V������؂�ւ���J�E���g
#define AnimChangeAnimCount					(15)
// �T�̈ړ����x
#define WolfMoveSpeed					(1.5f)
// �T�̏o���Ԋu
#define SetWolfInterval					(120)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeWolfVertex(WOLF *Wolf);
// ���_���W�̐ݒ�
void SetWolfVertex(WOLF *Wolf);
// �e�N�X�`�����W�̐ݒ�
void SetWolfTexture(WOLF *Wolf);
// �T�ݒu
void SetWolf();


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`�����
LPDIRECT3DTEXTURE9		Wolf_Texture;
// �T�\����
WOLF					Wolf[Wolf_Max];
// �摜���S���璸�_�܂ł̊p�x
static float BaseAngle = 0.0f;
// �摜���S���璸�_�܂ł̋���
static float Radius = 0.0f;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitWolf(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Wolf_No = 0;
	WOLF *Wolf = GetWolf(Wolf_No);

	// �T�̏���������
	for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
	{
		Wolf->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Wolf->HitRadius = 0.0f;
		Wolf->AnimCount = 0;
		Wolf->AnimPattern = 0;
		Wolf->Direction = 0;
		Wolf->Use = false;

		// ���_���̍쐬
		MakeWolfVertex(Wolf);
	}

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Wolf_Width / 2, Texture_Wolf_Height / 2));
		BaseAngle = atan2f(Texture_Wolf_Height, Texture_Wolf_Width);

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Wolf, &Wolf_Texture, "Wolf") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWolf(void)
{
	SafeRelease(Wolf_Texture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWolf(void)
{
	int Wolf_No = 0;
	static int AnimCount = 0;
	static int SetWolfCD = 0;
	WOLF *Wolf = GetWolf(Wolf_No);

	// �J�E���g���Z
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
		// �g�p���Ă����ԂȂ�X�V����
		if (Wolf->Use == true)
		{
			// �J�E���g���Z
			Wolf->AnimCount++;

			// �e�N�X�`���A�j���p�^�[���v�Z
			if ((Wolf->AnimCount % AnimChangeAnimCount) == 0)
			{
				Wolf->AnimPattern = (Wolf->AnimPattern + 1) % Texture_Wolf_Divide_X;
			}

			// �T�ړ�
			if (Wolf->Direction == Right)
			{
				Wolf->pos.x += WolfMoveSpeed;
			}
			else if (Wolf->Direction == Left)
			{
				Wolf->pos.x -= WolfMoveSpeed;
			}

			// ���_���W�X�V
			SetWolfVertex(Wolf);
			// �e�N�X�`�����W�X�V
			SetWolfTexture(Wolf);
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWolf(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Wolf_No = 0;
	WOLF *Wolf = GetWolf(Wolf_No);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
	{
		// �g�p���Ă����ԂȂ�`�悷��
		if (Wolf->Use == true)
		{
			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, Wolf_Texture);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Wolf->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// �T�ݒu
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
// ���_�̍쐬
//=============================================================================
HRESULT MakeWolfVertex(WOLF *Wolf)
{
	// ���_���W�̐ݒ�
	SetWolfVertex(Wolf);

	// rhw�̐ݒ�
	Wolf->vertexwk[0].rhw = 1.0f;
	Wolf->vertexwk[1].rhw = 1.0f;
	Wolf->vertexwk[2].rhw = 1.0f;
	Wolf->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Wolf->vertexwk[0].diffuse = WHITE(255);
	Wolf->vertexwk[1].diffuse = WHITE(255);
	Wolf->vertexwk[2].diffuse = WHITE(255);
	Wolf->vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetWolfTexture(Wolf);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
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
// �e�N�X�`�����W�̐ݒ�
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
// �T�擾�֐�
//=============================================================================
WOLF *GetWolf(int Wolf_No)
{
	return(&Wolf[Wolf_No]);
}

//=============================================================================
// �T�̃e�N�X�`���̃T�C�Y���擾����
//=============================================================================
D3DXVECTOR2 GetWolfSize(void)
{
	return D3DXVECTOR2(Texture_Wolf_Width - 40, Texture_Wolf_Height - 40);
}
