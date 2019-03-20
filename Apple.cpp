//=============================================================================
//
// �A�b�v������ [Apple.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "Apple.h"
#include "AttackRange.h"
#include "Transition.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Apple			_T("data/Texture/Enemy/Apple.png")	// �摜
#define Texture_Apple_Width		(70)
#define Texture_Apple_Height	(70)

// �A�b�v���̗������x
#define DropSpeed				(2.0f)
// �A�b�v���̗����͈́i���j
#define DropRange_Left			(100.0f)
// �A�b�v���̗����͈́i�E�j
#define DropRange_Right			(1700.0f)
// �A�b�v���̗����Ԋu
#define DropInterval			(90)
// �n�ʂ�Y���W
#define GroundPosition_Y		(1040.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeAppleVertex(APPLE *Apple);
// ���_���W�̐ݒ�
void SetAppleVertex(APPLE *Apple);
// �e�N�X�`�����W�̐ݒ�
void SetAppleTexture(APPLE *Apple);
// �A�b�v���ݒu
void SetApple();


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`�����
LPDIRECT3DTEXTURE9		Apple_Texture;
// �A�b�v���\����
APPLE					Apple[Apple_Max];
// �摜���S���璸�_�܂ł̊p�x
static float BaseAngle = 0.0f;
// �摜���S���璸�_�܂ł̋���
static float Radius = 0.0f;
// �A�b�v������������
int AppleNum = 0;
// �S���̃A�b�v�������I��
bool AppleDropOver = false;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitApple(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Apple_No = 0;
	APPLE *Apple = GetApple(Apple_No);

	// �A�b�v���̏���������
	for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
	{
		Apple->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Apple->Use = false;

		// ���_���̍쐬
		MakeAppleVertex(Apple);
	}
	AppleNum = 0;
	AppleDropOver = false;

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Apple_Width / 2, Texture_Apple_Height / 2));
		BaseAngle = atan2f(Texture_Apple_Height, Texture_Apple_Width);

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Apple, &Apple_Texture, "Apple") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitApple(void)
{
	SafeRelease(Apple_Texture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateApple(void)
{
	int Apple_No = 0;
	int DropedApple = 0;
	static int Count = 0;
	APPLE *Apple = GetApple(Apple_No);

	// �J�E���g���Z
	Count++;

	if (Count % DropInterval == 0 && AppleNum != Apple_Max)
	{
		SetApple();
	}

	for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
	{
		// �g�p���Ă����ԂȂ�X�V����
		if (Apple->Use == true)
		{
			// �A�b�v������
			Apple->pos.y += DropSpeed;

			//��ʊO���菈��
			if (Apple->pos.y >= GroundPosition_Y)
			{
				Apple->Use = false;
			}

			// ���_���W�X�V
			SetAppleVertex(Apple);
			// �e�N�X�`�����W�X�V
			SetAppleTexture(Apple);
		}
		else
		{
			DropedApple++;
		}
	}

	// �Q�[���I�[�o�[
	if (DropedApple == Apple_Max && AppleDropOver == true)
	{
		SetTransition(Fadein);
	}


	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawApple(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Apple_No = 0;
	APPLE *Apple = GetApple(Apple_No);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
	{
		// �g�p���Ă����ԂȂ�`�悷��
		if (Apple->Use == true)
		{
			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, Apple_Texture);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Apple->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// �A�b�v���ݒu
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

			// �ݒu�����A�b�v���̐������Z
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
// ���_�̍쐬
//=============================================================================
HRESULT MakeAppleVertex(APPLE *Apple)
{
	// ���_���W�̐ݒ�
	SetAppleVertex(Apple);

	// rhw�̐ݒ�
	Apple->vertexwk[0].rhw = 1.0f;
	Apple->vertexwk[1].rhw = 1.0f;
	Apple->vertexwk[2].rhw = 1.0f;
	Apple->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Apple->vertexwk[0].diffuse = WHITE(255);
	Apple->vertexwk[1].diffuse = WHITE(255);
	Apple->vertexwk[2].diffuse = WHITE(255);
	Apple->vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetAppleTexture(Apple);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
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
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetAppleTexture(APPLE *Apple)
{
	Apple->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Apple->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Apple->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Apple->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// �A�b�v���擾�֐�
//=============================================================================
APPLE *GetApple(int Apple_No)
{
	return(&Apple[Apple_No]);
}
