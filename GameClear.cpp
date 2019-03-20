//=============================================================================
//
// �^�C�g����ʏ��� [GameClear.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "GameClear.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Skill_Futago			_T("data/Texture/UI/Skill_Futago.png")
#define Texture_Skill_Ite				_T("data/Texture/UI/Skill_Ite.png")
#define Texture_Skill_Oushi				_T("data/Texture/UI/Skill_Oushi.png")
#define Texture_Skill_Width				(720)
#define Texture_Skill_Height			(300)
#define Texture_Skill_Pos				(D3DXVECTOR3(600.0f, 420.0f, 0.0f))	

#define Texture_StageClear				_T("data/Texture/UI/StageClear.png")
#define Texture_StageClear_Width		(960)
#define Texture_StageClear_Height		(160)
#define Texture_StageClear_Pos			(D3DXVECTOR3(480.0f, 260.0f, 0.0f))	


#define Texture_Illust_Player_A			_T("data/Texture/UI/Illust_Player_A.png")
#define Texture_Illust_Player_B			_T("data/Texture/UI/Illust_Player_B.png")
#define Texture_Illust_Width			(78)
#define Texture_Illust_Height			(168)
#define Texture_Illust_PlayerA_Pos		(D3DXVECTOR3(440.0f, 260.0f, 0.0f))	
#define Texture_Illust_PlayerB_Pos		(D3DXVECTOR3(1400.0f, 260.0f, 0.0f))	

#define Texture_GameClearBG				_T("data/Texture/UI/GameClearBG.png")
#define Texture_GameClearBG_Width		(1920)
#define Texture_GameClearBG_Height		(1080)
#define Texture_GameClearBG_Pos			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	

enum ImageType
{
	Img_StageClear,
	Img_PlayerA_Illust,
	Img_PlayerB_Illust,
	Img_GameClearBG,
	Img_Skill_Oushi,
	Img_Skill_Futago,
	Img_Skill_Ite,
	ImageType_Max,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeImageVertex(IMAGE *Image);
// ���_���W�̐ݒ�
void SetImageVertex(IMAGE *Image);
// �e�N�X�`�����W�̐ݒ�
void SetImageTexture(IMAGE *Image);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �摜�\����
IMAGE Image[ImageType_Max];
// �X�e�[�W�N���A�t���O
bool StageClear = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGameClear(bool FirstInit)
{
	int Image_No = 0;
	int GameStage = GetGameStage();

	for (Image_No = 0; Image_No < ImageType_Max; Image_No++)
	{
		Image[Image_No].Use = false;
	}

	// �X�e�[�W�N���A�t���O������
	StageClear = false;

	switch (GameStage)
	{
	case Stage_Oushi:

		Image[Img_StageClear].Use = true;
		Image[Img_PlayerA_Illust].Use = true;
		Image[Img_PlayerB_Illust].Use = true;
		Image[Img_Skill_Oushi].Use = true;
		break;

	case Stage_Futago:

		Image[Img_StageClear].Use = true;
		Image[Img_PlayerA_Illust].Use = true;
		Image[Img_PlayerB_Illust].Use = true;
		Image[Img_Skill_Futago].Use = true;
		break;

	case Stage_Ite:

		Image[Img_StageClear].Use = true;
		Image[Img_PlayerA_Illust].Use = true;
		Image[Img_PlayerB_Illust].Use = true;
		Image[Img_Skill_Ite].Use = true;
		break;

	case Stage_Shishi:

		Image[Img_GameClearBG].Use = true;
		break;

	default:
		break;
	}

	if (FirstInit == true)
	{
		Image[Img_StageClear].pos = Texture_StageClear_Pos;
		Image[Img_StageClear].Width = Texture_StageClear_Width;
		Image[Img_StageClear].Height = Texture_StageClear_Height;
		Image[Img_StageClear].Type = Img_StageClear;

		Image[Img_PlayerA_Illust].pos = Texture_Illust_PlayerA_Pos;
		Image[Img_PlayerA_Illust].Width = Texture_Illust_Width;
		Image[Img_PlayerA_Illust].Height = Texture_Illust_Height;
		Image[Img_PlayerA_Illust].Type = Img_PlayerA_Illust;

		Image[Img_PlayerB_Illust].pos = Texture_Illust_PlayerB_Pos;
		Image[Img_PlayerB_Illust].Width = Texture_Illust_Width;
		Image[Img_PlayerB_Illust].Height = Texture_Illust_Height;
		Image[Img_PlayerB_Illust].Type = Img_PlayerB_Illust;

		Image[Img_GameClearBG].pos = Texture_GameClearBG_Pos;
		Image[Img_GameClearBG].Width = Texture_GameClearBG_Width;
		Image[Img_GameClearBG].Height = Texture_GameClearBG_Height;
		Image[Img_GameClearBG].Type = Img_GameClearBG;

		Image[Img_Skill_Oushi].pos = Texture_Skill_Pos;
		Image[Img_Skill_Oushi].Width = Texture_Skill_Width;
		Image[Img_Skill_Oushi].Height = Texture_Skill_Height;
		Image[Img_Skill_Oushi].Type = Img_Skill_Oushi;

		Image[Img_Skill_Futago].pos = Texture_Skill_Pos;
		Image[Img_Skill_Futago].Width = Texture_Skill_Width;
		Image[Img_Skill_Futago].Height = Texture_Skill_Height;
		Image[Img_Skill_Futago].Type = Img_Skill_Futago;

		Image[Img_Skill_Ite].pos = Texture_Skill_Pos;
		Image[Img_Skill_Ite].Width = Texture_Skill_Width;
		Image[Img_Skill_Ite].Height = Texture_Skill_Height;
		Image[Img_Skill_Ite].Type = Img_Skill_Ite;

		// ���_���̍쐬
		for (Image_No = 0; Image_No < ImageType_Max; Image_No++)
		{
			MakeImageVertex(&Image[Image_No]);
		}

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Skill_Futago, &Image[Img_Skill_Futago].Texture, "GameClear") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Skill_Ite, &Image[Img_Skill_Ite].Texture, "GameClear") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Skill_Oushi, &Image[Img_Skill_Oushi].Texture, "GameClear") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_StageClear, &Image[Img_StageClear].Texture, "GameClear") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Illust_Player_A, &Image[Img_PlayerA_Illust].Texture, "GameClear") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Illust_Player_B, &Image[Img_PlayerB_Illust].Texture, "GameClear") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameClearBG, &Image[Img_GameClearBG].Texture, "GameClear") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGameClear(void)
{
	// �e�N�X�`���̊J��
	SafeRelease(Image[Img_Skill_Futago].Texture);
	SafeRelease(Image[Img_Skill_Ite].Texture);
	SafeRelease(Image[Img_Skill_Oushi].Texture);
	SafeRelease(Image[Img_StageClear].Texture);
	SafeRelease(Image[Img_PlayerA_Illust].Texture);
	SafeRelease(Image[Img_PlayerB_Illust].Texture);
	SafeRelease(Image[Img_GameClearBG].Texture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGameClear(void)
{
	int GameStage = GetGameStage();

	if (StageClear == true)
	{
		switch (GameStage)
		{
		case Stage_Futago:
		case Stage_Oushi:
		case Stage_Ite:
		case Stage_GameEnd:

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
			{
				SetTransition(Fadein);
			}
			break;

		default:
			break;
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGameClear(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Image_No = 0;

	if (StageClear == true)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		for (Image_No = 0; Image_No < ImageType_Max; Image_No++)
		{
			if (Image[Image_No].Use == true)
			{
				// �e�N�X�`���̐ݒ�
				switch (Image[Image_No].Type)
				{
				case Img_StageClear:

					Device->SetTexture(0, Image[Img_StageClear].Texture);
					break;

				case Img_PlayerA_Illust:

					Device->SetTexture(0, Image[Img_PlayerA_Illust].Texture);
					break;

				case Img_PlayerB_Illust:

					Device->SetTexture(0, Image[Img_PlayerB_Illust].Texture);
					break;

				case Img_GameClearBG:

					Device->SetTexture(0, Image[Img_GameClearBG].Texture);
					break;

				case Img_Skill_Oushi:

					Device->SetTexture(0, Image[Img_Skill_Oushi].Texture);
					break;

				case Img_Skill_Futago:

					Device->SetTexture(0, Image[Img_Skill_Futago].Texture);
					break;

				case Img_Skill_Ite:

					Device->SetTexture(0, Image[Img_Skill_Ite].Texture);
					break;

				default:
					break;
				}

				// �|���S���̕`��
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Image[Image_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeImageVertex(IMAGE *Image)
{
	// ���_���W�̐ݒ�
	SetImageVertex(Image);

	// rhw�̐ݒ�
	Image->vertexwk[0].rhw = 1.0f;
	Image->vertexwk[1].rhw = 1.0f;
	Image->vertexwk[2].rhw = 1.0f;
	Image->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Image->vertexwk[0].diffuse = WHITE(255);
	Image->vertexwk[1].diffuse = WHITE(255);
	Image->vertexwk[2].diffuse = WHITE(255);
	Image->vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetImageTexture(Image);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetImageVertex(IMAGE *Image)
{
	Image->vertexwk[0].vtx = D3DXVECTOR3(Image->pos.x, Image->pos.y, 0.0f);
	Image->vertexwk[1].vtx = D3DXVECTOR3(Image->pos.x + Image->Width, Image->pos.y, 0.0f);
	Image->vertexwk[2].vtx = D3DXVECTOR3(Image->pos.x, Image->pos.y + Image->Height, 0.0f);
	Image->vertexwk[3].vtx = D3DXVECTOR3(Image->pos.x + Image->Width, Image->pos.y + Image->Height, 0.0f);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetImageTexture(IMAGE *Image)
{
	Image->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Image->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Image->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Image->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// �X�e�[�W�N���A�̐ݒ�
//=============================================================================
void SetStageClear(bool Flag)
{
	SetSound(SE_StageClear, E_DS8_FLAG_NONE, true, false);
	StageClear = Flag;
}

//=============================================================================
// �X�e�[�W�N���A���擾
//=============================================================================
bool GetStageClear(void)
{
	return StageClear;
}
