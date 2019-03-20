//=============================================================================
//
// ������ [Door.cpp]
// Author�F
//
//=============================================================================
#include "main.h"
#include "Door.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Door				_T("data/Texture/UI/Door.png")
#define Texture_Door_Width_Total	(900)	// �e�N�X�`���̉���
#define Texture_Door_Height_Total	(110)	// �e�N�X�`���̏c��
#define Texture_Door_Divide_X		(5)		// �e�N�X�`�����������i��)
#define Texture_Door_Divide_Y		(1)		// �e�N�X�`�����������i�c)
#define Texture_Door_Width			(Texture_Door_Width_Total / Texture_Door_Divide_X)
#define Texture_Door_Height			(Texture_Door_Height_Total / Texture_Door_Divide_Y)

// �A�j���[�V������؂�ւ���J�E���g
#define AnimChangeAnimCount			(15)
#define TEXTURE_Door_SIZE_X			(180/2)	
#define TEXTURE_Door_SIZE_Y			(110/2)	
// ���̕\�����W
#define Door1_Pos					(D3DXVECTOR3(440.0f, 56.0f, 0.0f))	
#define Door2_Pos					(D3DXVECTOR3(1385.0f, 56.0f, 0.0f))	
// ���̍ő吔
#define Door_Max					(2)										
// �A�j���[�V������؂�ւ���J�E���g
#define AnimChangeCount				(15)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeDoorVertex(DOOR *Door);
// ���_���W�̐ݒ�
void SetDoorVertex(DOOR *Door);
// �e�N�X�`�����W�̐ݒ�
void SetDoorTexture(DOOR *Door);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`��
LPDIRECT3DTEXTURE9		Door_Texture = NULL;
// ���\����
DOOR					Door[Door_Max];
// �摜���S���璸�_�܂ł̊p�x
static float BaseAngle = 0.0f;
// �摜���S���璸�_�܂ł̋���
static float Radius = 0.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDoor(bool FirstInit)
{
	int Door_No = 0;
	DOOR *Door = GetDoor(Door_No);

	// ���̏���������
	for (Door_No = 0; Door_No < Door_Max; Door_No++, Door++)
	{
		Door->Use = true;
		Door->AnimPattern = 0;
		// �A�j���J�E���g��������
		Door->AnimCount = 0;
		// �ŏ��́A�h�A���܂��Ă���
		Door->State = DoorClose;
		// ���W�f�[�^��������
		if (Door_No == 0)
		{
			Door->pos = Door1_Pos;
		}
		else if (Door_No == 1)
		{
			Door->pos = Door2_Pos;
		}

		// ���_���̍쐬
		MakeDoorVertex(Door);
	}

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Door_Width / 2, Texture_Door_Height / 2));
		BaseAngle = atan2f(Texture_Door_Height, Texture_Door_Width);

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Door, &Door_Texture, "Door") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDoor(void)
{
	// �e�N�X�`���̊J��
	SafeRelease(Door_Texture);
}

//=============================================================================
// �X�V����
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

				// �e�N�X�`���A�j���p�^�[���v�Z
				if ((Door->AnimCount % AnimChangeCount) == 0)
				{
					Door->AnimPattern = (Door->AnimPattern + 1) % Texture_Door_Divide_X;
				}

				// ���_���W�̐ݒ�
				SetDoorVertex(Door);
				// �e�N�X�`�����W�̐ݒ�
				SetDoorTexture(Door);
			}
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDoor(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Door_No = 0;
	DOOR *Door = GetDoor(Door_No);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, Door_Texture);

	for (Door_No = 0; Door_No < Door_Max; Door_No++, Door++)
	{
		// �g�p���Ă����ԂȂ�`�悷��
		if (Door->Use == true)
		{
			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Door->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeDoorVertex(DOOR *Door)
{
	// ���_���W�̐ݒ�
	SetDoorVertex(Door);

	// rhw�̐ݒ�
	Door->vertexwk[0].rhw = 1.0f;
	Door->vertexwk[1].rhw = 1.0f;
	Door->vertexwk[2].rhw = 1.0f;
	Door->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Door->vertexwk[0].diffuse = WHITE(255);
	Door->vertexwk[1].diffuse = WHITE(255);
	Door->vertexwk[2].diffuse = WHITE(255);
	Door->vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetDoorTexture(Door);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetDoorVertex(DOOR *Door)
{
	// ���_���W�̐ݒ�
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
// �e�N�X�`�����W�̐ݒ�
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
// ���擾�֐�
//=============================================================================
DOOR *GetDoor(int Door_No)
{
	return &Door[Door_No];
}

//=============================================================================
// ���̃e�N�X�`���̃T�C�Y���擾����
//=============================================================================
D3DXVECTOR2 GetDoorSize(void)
{
	return D3DXVECTOR2(Texture_Door_Width - 50, Texture_Door_Height - 50);
}

