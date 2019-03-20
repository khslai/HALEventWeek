//=============================================================================
//
// �^�C�g����ʏ��� [Title.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Title.h"
#include "BackGround.h"
#include "Transition.h"
#include "Sound.h"
#include "Input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_TeamLogo			_T("data/Texture/BackGround/TeamLogo.png")
#define Texture_TeamLogo_Width		(800)
#define Texture_TeamLogo_Height		(800)
#define TeamLogo_Pos				(D3DXVECTOR3(560.0f,140.0f,0.0f))

#define Texture_TitleBG				_T("data/Texture/BackGround/BackGround_Title.png")
#define Texture_TitleBG_Width		(1920)
#define Texture_TitleBG_Height		(1080)

#define Texture_GameTitle			_T("data/Texture/BackGround/GameTitle.png")
#define Texture_GameTitle_Width		(1200)
#define Texture_GameTitle_Height	(200)

#define Texture_GameStart			_T("data/Texture/UI/GameStart.png")
#define Texture_GameStart_Width		(480)
#define Texture_GameStart_Height	(80)

#define Texture_GameExit			_T("data/Texture/UI/GameExit.png")
#define Texture_GameExit_Width		(480)
#define Texture_GameExit_Height		(80)

// �w�i�X�N���[�����x
#define TitleBGSpeed				(0.0003f)
// �e�L�X�g��`�̍���
#define RectHeight					(100)
// �e�L�X�g�\���̍��W
#define GameTitle_Pos				(D3DXVECTOR3(360.0f,100.0f,0.0f))
#define GameStart_Pos				(D3DXVECTOR3(740.0f,600.0f,0.0f))
#define GameEnd_Pos					(D3DXVECTOR3(740.0f,750.0f,0.0f))

// �`�[�����S�̕\�����x
#define LogoFadeSpeed				(2)

enum TitleImageType
{
	Img_TeamLogo,
	Img_TitleBG,
	Img_Title,
	Img_GameStart,
	Img_GameExit,
	TitleImg_Max,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeTitleVertex(IMAGE *TitleImg);
// ���_���W�̐ݒ�
void SetTitleVertex(IMAGE *TitleImg);
// �e�N�X�`�����W�̐ݒ�
void SetTitleTexture(IMAGE *TitleImg);
// �F�A�����x�̐ݒ�
void SetTitleColor(IMAGE *TitleImg);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		TitleTexture[TitleImg_Max] = { NULL };
// �^�C�g���摜�\����
IMAGE					TitleImg[TitleImg_Max];
// �^�C�g���I����
SELECT					TitleSelect;
// �Q�[���I���t���O
bool					GameExitFlag = false;
// �`�[�����S�̕\���t���O
// �Q�[�������オ�鎞�A�`�[�����S�\��
bool					ShowTeamLogo = true;
// �`�[�����S�̓����x
int						LogoAlpha = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(bool FirstInit)
{
	int Image_No = 0;

	// �I����������
	TitleSelect.State = Img_GameStart;
	TitleSelect.PreState = Img_GameStart;
	TitleSelect.InYes = false;

	// �I���t���O������
	GameExitFlag = false;

	for (Image_No = 0; Image_No < TitleImg_Max; Image_No++)
	{
		TitleImg[Image_No].Use = true;
	}
	if (ShowTeamLogo == false)
	{
		TitleImg[Img_TeamLogo].Use = false;
	}

	if (FirstInit == true)
	{
		TitleImg[Img_TeamLogo].pos = TeamLogo_Pos;
		TitleImg[Img_TeamLogo].Width = Texture_TeamLogo_Width;
		TitleImg[Img_TeamLogo].Height = Texture_TeamLogo_Height;
		TitleImg[Img_TeamLogo].Type = Img_TeamLogo;

		TitleImg[Img_Title].pos = GameTitle_Pos;
		TitleImg[Img_Title].Width = Texture_GameTitle_Width;
		TitleImg[Img_Title].Height = Texture_GameTitle_Height;
		TitleImg[Img_Title].Type = Img_Title;

		TitleImg[Img_TitleBG].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		TitleImg[Img_TitleBG].Width = Texture_TitleBG_Width;
		TitleImg[Img_TitleBG].Height = Texture_TitleBG_Height;
		TitleImg[Img_TitleBG].Type = Img_TitleBG;

		TitleImg[Img_GameStart].pos = GameStart_Pos;
		TitleImg[Img_GameStart].Width = Texture_GameStart_Width;
		TitleImg[Img_GameStart].Height = Texture_GameStart_Height;
		TitleImg[Img_GameStart].Type = Img_GameStart;

		TitleImg[Img_GameExit].pos = GameEnd_Pos;
		TitleImg[Img_GameExit].Width = Texture_GameExit_Width;
		TitleImg[Img_GameExit].Height = Texture_GameExit_Height;
		TitleImg[Img_GameExit].Type = Img_GameExit;

		// ���_���̍쐬
		for (Image_No = 0; Image_No < TitleImg_Max; Image_No++)
		{
			MakeTitleVertex(&TitleImg[Image_No]);
		}

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_TeamLogo, &TitleImg[Img_TeamLogo].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_TitleBG, &TitleImg[Img_TitleBG].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameTitle, &TitleImg[Img_Title].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameStart, &TitleImg[Img_GameStart].Texture, "Title") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GameExit, &TitleImg[Img_GameExit].Texture, "Title") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
{
	for (int Image_No = 0; Image_No < TitleImg_Max; Image_No++)
	{
		SafeRelease(TitleImg[Image_No].Texture);
	}

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{
	static int PressCount = 0;
	static int StayCount = 0;
	static bool FadeOut = false;

	// �^�C�g���\������
	if (ShowTeamLogo == false)
	{
		// Enter
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(Player_A, BUTTON_B))
		{
			SetSound(SE_Determine, E_DS8_FLAG_NONE, true, false);
			if (TitleSelect.State == Img_GameStart)
			{
				SetTransition(Fadein);
			}
			else
			{
				GameExitFlag = true;
			}
		}

		// �I�����ړ�
		// �㉺
		if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP) ||
			GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
			if (TitleSelect.State == Img_GameStart)
			{
				TitleSelect.State = Img_GameExit;
			}
			else
			{
				TitleSelect.State = Img_GameStart;
			}
		}

		// �I�������[�v
		if (GetKeyboardRepeat(DIK_UP) || IsButtonRepeat(0, BUTTON_UP) ||
			GetKeyboardRepeat(DIK_DOWN) || IsButtonRepeat(0, BUTTON_DOWN))
		{
			PressCount++;
			if (PressCount >= RepeatCount && PressCount % RepeatSpeed == 0)
			{
				SetSound(SE_SelectMove, E_DS8_FLAG_NONE, true, false);
				if (TitleSelect.State == Img_GameStart)
				{
					TitleSelect.State = Img_GameExit;
				}
				else
				{
					TitleSelect.State = Img_GameStart;
				}
			}
		}

		// �v���X�J�E���g������
		if (GetKeyboardRelease(DIK_UP) || IsButtonReleased(0, BUTTON_UP) ||
			GetKeyboardRelease(DIK_DOWN) || IsButtonReleased(0, BUTTON_DOWN))
		{
			PressCount = 0;
		}

		for (int Image_No = 0; Image_No < TitleImg_Max; Image_No++)
		{
			// �F�A�����x�̐ݒ�
			SetTitleColor(&TitleImg[Image_No]);
		}
	}
	// �`�[�����S�\������
	else
	{
		if (FadeOut == false)
		{
			LogoAlpha += LogoFadeSpeed;
			if (LogoAlpha >= 255)
			{
				LogoAlpha = 255;
				FadeOut = true;
			}
		}
		else
		{
			if (StayCount >= 60)
			{
				LogoAlpha -= LogoFadeSpeed;
				if (LogoAlpha <= 0)
				{
					LogoAlpha = 0;
					StayCount = 0;
					FadeOut = false;
					ShowTeamLogo = false;
					TitleImg[Img_TeamLogo].Use = false;
				}
			}
			else
			{
				StayCount++;
			}
		}

		// �F�A�����x�̐ݒ�
		SetTitleColor(&TitleImg[Img_TeamLogo]);
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �^�C�g���\��
	if (ShowTeamLogo == false)
	{
		for (int Image_No = 0; Image_No < TitleImg_Max; Image_No++)
		{
			if (TitleImg[Image_No].Use == true)
			{
				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, TitleImg[Image_No].Texture);

				// �|���S���̕`��
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleImg[Image_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}
	// �`�[�����S�\��
	else
	{
		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, TitleImg[Img_TeamLogo].Texture);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TitleImg[Img_TeamLogo].vertexwk, sizeof(VERTEX_2D));
	}

	return;
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeTitleVertex(IMAGE *TitleImg)
{
	// �e�N�X�`�����W�̐ݒ�
	SetTitleTexture(TitleImg);

	// rhw�̐ݒ�
	TitleImg->vertexwk[0].rhw = 1.0f;
	TitleImg->vertexwk[1].rhw = 1.0f;
	TitleImg->vertexwk[2].rhw = 1.0f;
	TitleImg->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	TitleImg->vertexwk[0].diffuse = WHITE(255);
	TitleImg->vertexwk[1].diffuse = WHITE(255);
	TitleImg->vertexwk[2].diffuse = WHITE(255);
	TitleImg->vertexwk[3].diffuse = WHITE(255);

	// ���_���W�̐ݒ�
	SetTitleVertex(TitleImg);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTitleTexture(IMAGE *TitleImg)
{
	TitleImg->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TitleImg->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TitleImg->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TitleImg->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTitleVertex(IMAGE *TitleImg)
{
	TitleImg->vertexwk[0].vtx = D3DXVECTOR3(TitleImg->pos.x, TitleImg->pos.y, 0.0f);
	TitleImg->vertexwk[1].vtx = D3DXVECTOR3(TitleImg->pos.x + TitleImg->Width, TitleImg->pos.y, 0.0f);
	TitleImg->vertexwk[2].vtx = D3DXVECTOR3(TitleImg->pos.x, TitleImg->pos.y + TitleImg->Height, 0.0f);
	TitleImg->vertexwk[3].vtx = D3DXVECTOR3(TitleImg->pos.x + TitleImg->Width, TitleImg->pos.y + TitleImg->Height, 0.0f);

	return;
}

//=============================================================================
// �F�A�����x�̐ݒ�
//=============================================================================
void SetTitleColor(IMAGE *TitleImg)
{
	switch (TitleImg->Type)
	{
	case Img_TeamLogo:

		TitleImg->vertexwk[0].diffuse = WHITE(LogoAlpha);
		TitleImg->vertexwk[1].diffuse = WHITE(LogoAlpha);
		TitleImg->vertexwk[2].diffuse = WHITE(LogoAlpha);
		TitleImg->vertexwk[3].diffuse = WHITE(LogoAlpha);
		break;

	case Img_GameStart:

		if (TitleSelect.State == Img_GameStart)
		{
			TitleImg->vertexwk[0].diffuse = GREEN(255);
			TitleImg->vertexwk[1].diffuse = GREEN(255);
			TitleImg->vertexwk[2].diffuse = GREEN(255);
			TitleImg->vertexwk[3].diffuse = GREEN(255);
		}
		else
		{
			TitleImg->vertexwk[0].diffuse = WHITE(255);
			TitleImg->vertexwk[1].diffuse = WHITE(255);
			TitleImg->vertexwk[2].diffuse = WHITE(255);
			TitleImg->vertexwk[3].diffuse = WHITE(255);
		}
		break;

	case Img_GameExit:

		if (TitleSelect.State == Img_GameStart)
		{
			TitleImg->vertexwk[0].diffuse = WHITE(255);
			TitleImg->vertexwk[1].diffuse = WHITE(255);
			TitleImg->vertexwk[2].diffuse = WHITE(255);
			TitleImg->vertexwk[3].diffuse = WHITE(255);
		}
		else
		{
			TitleImg->vertexwk[0].diffuse = GREEN(255);
			TitleImg->vertexwk[1].diffuse = GREEN(255);
			TitleImg->vertexwk[2].diffuse = GREEN(255);
			TitleImg->vertexwk[3].diffuse = GREEN(255);
		}
		break;

	default:
		break;
	}

	return;
}

//=============================================================================
// �Q�[���I���t���O���擾
//=============================================================================
bool GetGameExit(void)
{
	return GameExitFlag;
}

//=============================================================================
// ���S�\���t���O���擾
//=============================================================================
bool GetShowLogo(void)
{
	return ShowTeamLogo;
}