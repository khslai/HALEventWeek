//=============================================================================
//
// �^�C�}�[���� [Timer.cpp]
// Author�F
//
//=============================================================================
#include "main.h"
#include "Timer.h"
#include "Score.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_TimerBox		_T("data/Texture/UI/TimerBox.png")
#define Texture_TimerBox_Width	(250)
#define Texture_TimerBox_Height	(120)
// �^�C�}�[�̘g�̕\�����W
#define TimerBox_Pos_X			(850)
#define TimerBox_Pos_Y			(0)

// �^�C�}�[�̐����̕�
#define Texture_Number_Width	(70)
// �^�C�}�[�̐����̍���
#define Texture_Number_Height	(70)
// �^�C�}�[�̐����̕\���Ԋu
#define	Timer_Interval			(30)							
// �^�C�}�[�̌���
#define	TimerDigit				(2)			
// �^�C�}�[�̕\�����W
#define Timer_Pos_X				(990)
#define Timer_Pos_Y				(40)

// �J�E���g�_�E���̕\�����W
#define CountDown_Pos_X			(900)
#define CountDown_Pos_Y			(80)
// �����g��
#define	NumZoom					(50)

// ���v���C�̎���
#define OnePlayTime_Oushi		(30)
#define OnePlayTime_Futago		(60)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeTimerVertex(int Digit, int Num);
HRESULT MakeTimerBoxVertex(void);
// ���_���W�̐ݒ�
void SetTimerVertex(int Digit);
void SetTimerBoxVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetTimerTexture(int Digit, int Num);
void SetTimerBoxTexture(void);
// �^�C�}�[�̃J�E���g�_�E����ݒu����
void SetTimerCountDown(void);


//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		TimerBox_Texture = NULL;
// �^�C�}�[�̎���
int						TimerNumber = 0;
// �^�C�}�[����ON/OFF
bool					TimerUse = true;
// �J�E���g�_�E���t���O
bool					CountDown = false;
// �^�C�}�[�\����
TIMER					Timer[TimerDigit];
// �^�C�}�[�̘g�\����
TIMER					TimerBox;
// �Q�[���X�^�[�g�̃J�E���g�_�E���I���t���O
bool					StartCDOver = false;
// �Q�[���I���̃J�E���g�_�E���I���t���O
bool					EndCDOver = false;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTimer(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �^�C�}�[�̏�����
	StartCDOver = false;
	EndCDOver = false;
	TimerUse = false;
	// �Q�[���J�n�̃J�E���g�_�E��(3�b)
	TimerNumber = 3;
	CountDown = true;

	for (int Digit = 0; Digit < TimerDigit; Digit++)
	{
		Timer[Digit].pos = D3DXVECTOR3((float)Timer_Pos_X - Digit * (Texture_Number_Width + Timer_Interval), Timer_Pos_Y, 0.0f);
		Timer[Digit].Number = 0;

		// ���_���̍쐬
		MakeTimerVertex(Digit, Timer[Digit].Number);
	}

	if (FirstInit == true)
	{
		TimerBox.pos = D3DXVECTOR3(TimerBox_Pos_X, TimerBox_Pos_Y, 0.0f);

		// ���_���̍쐬
		MakeTimerBoxVertex();

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_TimerBox, &TimerBox_Texture, "Timer") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{
	// �e�N�X�`���̊J��
	SafeRelease(TimerBox_Texture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer(void)
{
	int Digit = 0;
	int Timertemp = 0;
	int GameStage = GetGameStage();
	static int Count = 0;

	if (TimerUse == true)
	{
		// �J�E���g�_�E���J�n
		if (CountDown == true)
		{
			SetTimerCountDown();
		}

		Count++;

		// ��b�o��
		if (Count != 0 && Count % 60 == 0)
		{
			TimerNumber--;
			if (TimerNumber <= 0)
			{
				TimerNumber = 0;
			}
		}

		// �J�E���g�_�E���I��
		if (CountDown == true && TimerNumber <= 0)
		{
			if (StartCDOver == false)
			{
				// �Q�[���X�^�[�g�̃J�E���g�_�E���I��
				StartCDOver = true;
				CountDown = false;
				// �^�C�}�[�̎��Ԑݒu
				if (GameStage == Stage_Oushi)
				{

					TimerNumber = OnePlayTime_Oushi;
				}
				else if (GameStage == Stage_Futago)
				{

					TimerNumber = OnePlayTime_Futago;
				}

				// �����̍��W�����Z�b�g
				for (Digit = 0; Digit < TimerDigit; Digit++)
				{
					Timer[Digit].pos = D3DXVECTOR3((float)Timer_Pos_X - Digit * (Texture_Number_Width + Timer_Interval), Timer_Pos_Y, 0.0f);
					Timer[Digit].Number = 0;

					// ���_���̍쐬
					MakeTimerVertex(Digit, Timer[Digit].Number);
				}
			}
			else if (EndCDOver == false)
			{
				// �Q�[���I���̃J�E���g�_�E���I��
				EndCDOver = true;
			}
		}

		// �Q�[���I���̃J�E���g�_�E���J�n
		if (EndCDOver == false && TimerNumber < 10)
		{
			CountDown = true;
		}

		// �����v�Z
		Timertemp = TimerNumber;
		for (Digit = 0; Digit < TimerDigit; Digit++)
		{
			Timer[Digit].Number = Timertemp % 10;
			Timertemp /= 10;

			// �e�N�X�`�����W��ݒ�
			SetTimerTexture(Digit, Timer[Digit].Number);
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
{
	int Digit = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();
	LPDIRECT3DTEXTURE9 NumberTex = GetNumberTex();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (TimerUse == true)
	{
		// �Ō�10�b�ȊO�͕��ʂŕ\��
		if (CountDown == false)
		{
			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, TimerBox_Texture);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, TimerBox.vertexwk, sizeof(VERTEX_2D));

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, NumberTex);

			// �|���S���̕`��
			for (Digit = 0; Digit < TimerDigit; Digit++)
			{
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Timer[Digit].vertexwk, sizeof(VERTEX_2D));
			}
		}
		// �Ō�10�b�̕\��
		else
		{
			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, NumberTex);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Timer[0].vertexwk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeTimerVertex(int Digit, int TimerNum)
{
	// ���_���W�̐ݒ�	
	SetTimerVertex(Digit);

	// rhw�̐ݒ�
	Timer[Digit].vertexwk[0].rhw = 1.0f;
	Timer[Digit].vertexwk[1].rhw = 1.0f;
	Timer[Digit].vertexwk[2].rhw = 1.0f;
	Timer[Digit].vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Timer[Digit].vertexwk[0].diffuse = D3DCOLOR_RGBA(0, 0, 255, 255);
	Timer[Digit].vertexwk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	Timer[Digit].vertexwk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	Timer[Digit].vertexwk[3].diffuse = D3DCOLOR_RGBA(0, 255, 0, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTimerTexture(Digit, TimerNum);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTimerTexture(int Digit, int TimerNum)
{
	Timer[Digit].vertexwk[0].tex = D3DXVECTOR2(TimerNum * 0.1f, 0.0f);
	Timer[Digit].vertexwk[1].tex = D3DXVECTOR2((TimerNum + 1) * 0.1f, 0.0f);
	Timer[Digit].vertexwk[2].tex = D3DXVECTOR2(TimerNum * 0.1f, 1.0f);
	Timer[Digit].vertexwk[3].tex = D3DXVECTOR2((TimerNum + 1) * 0.1f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTimerVertex(int Digit)
{
	Timer[Digit].vertexwk[0].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x, Timer[Digit].pos.y, 0.0f);
	Timer[Digit].vertexwk[1].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x + Texture_Number_Width, Timer[Digit].pos.y, 0.0f);
	Timer[Digit].vertexwk[2].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x, Timer[Digit].pos.y + Texture_Number_Height, 0.0f);
	Timer[Digit].vertexwk[3].vtx =
		D3DXVECTOR3(Timer[Digit].pos.x + Texture_Number_Width, Timer[Digit].pos.y + Texture_Number_Height, 0.0f);

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeTimerBoxVertex(void)
{
	// ���_���W�̐ݒ�	
	SetTimerBoxVertex();

	// rhw�̐ݒ�
	TimerBox.vertexwk[0].rhw = 1.0f;
	TimerBox.vertexwk[1].rhw = 1.0f;
	TimerBox.vertexwk[2].rhw = 1.0f;
	TimerBox.vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	TimerBox.vertexwk[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	TimerBox.vertexwk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	TimerBox.vertexwk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	TimerBox.vertexwk[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTimerBoxTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTimerBoxTexture(void)
{
	TimerBox.vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	TimerBox.vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	TimerBox.vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	TimerBox.vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetTimerBoxVertex(void)
{
	TimerBox.vertexwk[0].vtx = D3DXVECTOR3(TimerBox.pos.x, TimerBox.pos.y, 0.0f);
	TimerBox.vertexwk[1].vtx = D3DXVECTOR3(TimerBox.pos.x + Texture_TimerBox_Width, TimerBox.pos.y, 0.0f);
	TimerBox.vertexwk[2].vtx = D3DXVECTOR3(TimerBox.pos.x, TimerBox.pos.y + Texture_TimerBox_Height, 0.0f);
	TimerBox.vertexwk[3].vtx = D3DXVECTOR3(TimerBox.pos.x + Texture_TimerBox_Width, TimerBox.pos.y + Texture_TimerBox_Height, 0.0f);

	return;
}

//=============================================================================
// �^�C�}�[�̎g�p��ݒu
//=============================================================================
void SetTimerUse(bool Flag)
{
	TimerUse = Flag;
}

//=============================================================================
// �^�C�}�[�̃J�E���g�_�E����ݒu����
//=============================================================================
void SetTimerCountDown(void)
{
	// ���_���W�̐ݒ�
	Timer[0].vertexwk[0].vtx = D3DXVECTOR3(CountDown_Pos_X, CountDown_Pos_Y, 0.0f);
	Timer[0].vertexwk[1].vtx = D3DXVECTOR3(CountDown_Pos_X + Texture_Number_Width + NumZoom, CountDown_Pos_Y, 0.0f);
	Timer[0].vertexwk[2].vtx = D3DXVECTOR3(CountDown_Pos_X, CountDown_Pos_Y + Texture_Number_Height + NumZoom, 0.0f);
	Timer[0].vertexwk[3].vtx = D3DXVECTOR3(CountDown_Pos_X + Texture_Number_Width + NumZoom, CountDown_Pos_Y + Texture_Number_Height + NumZoom, 0.0f);
}

//=============================================================================
// �Q�[���X�^�[�g�̃J�E���g�_�E���t���O���擾
//=============================================================================
bool GetStartCDOver(void)
{
	return StartCDOver;
}

//=============================================================================
// �Q�[���I���̃J�E���g�_�E���t���O���擾
//=============================================================================
bool GetEndCDOver(void)
{
	return EndCDOver;
}
