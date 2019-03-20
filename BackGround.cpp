//=============================================================================
//
// �w�i���� [BackGround.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "BackGround.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_BackGround_Ite			_T("data/Texture/BackGround/BackGround_Ite.png")
#define Texture_BackGround_Oushi		_T("data/Texture/BackGround/BackGround_Oushi.png")
#define Texture_BackGround_Futago		_T("data/Texture/BackGround/BackGround_Futago.png")
#define Texture_BackGround_Shishi		_T("data/Texture/BackGround/BackGround_Shishi.jpg")

enum BGType
{
	BackGround_Ite,			// �ˎ���w�i
	BackGround_Oushi,		// �������w�i
	BackGround_Futago,		// �o�q���w�i
	BackGround_Shishi,		// ���q���w�i
	BackGround_Max,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBackGroundVertex(void);
// ���_���W�̐ݒ�
void SetBackGroundTexture(void);
// �e�N�X�`�����W�̐ݒ�
void SetBackGroundVertex(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 BackGroundTexture[BackGround_Max] = { NULL };
// �w�i�\����
BACKGROUND BackGround;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBackGround(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	BackGround.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_���̍쐬
	MakeBackGroundVertex();

	if (FirstInit == true)
	{
		// �e�N�X�`���̓ǂݍ���
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
// �I������
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
// �X�V����
//=============================================================================
void UpdateBackGround(void)
{

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBackGround(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int GameStage = GetGameStage();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
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

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BackGround.vertexwk, sizeof(VERTEX_2D));

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBackGroundVertex(void)
{
	// ���_���W�̐ݒ�	
	SetBackGroundVertex();

	// rhw�̐ݒ�
	BackGround.vertexwk[0].rhw = 1.0f;
	BackGround.vertexwk[1].rhw = 1.0f;
	BackGround.vertexwk[2].rhw = 1.0f;
	BackGround.vertexwk[3].rhw = 1.0f;

	// �F�A�����x�̐ݒ�
	BackGround.vertexwk[0].diffuse = WHITE(255);
	BackGround.vertexwk[1].diffuse = WHITE(255);
	BackGround.vertexwk[2].diffuse = WHITE(255);
	BackGround.vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetBackGroundTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
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
// ���_���W�̐ݒ�
//=============================================================================
void SetBackGroundVertex(void)
{
	BackGround.vertexwk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BackGround.vertexwk[1].vtx = D3DXVECTOR3(Screen_Width, 0.0f, 0.0f);
	BackGround.vertexwk[2].vtx = D3DXVECTOR3(0.0f, Screen_Height, 0.0f);
	BackGround.vertexwk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0.0f);

	return;
}
