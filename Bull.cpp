//=============================================================================
//
// �������� [Bull.cpp]
// Author�F 
//
//=============================================================================
#include "main.h"
#include "Bull.h"
#include "Player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Bull				_T("data/Texture/Enemy/Bull.png")
#define Texture_Bull_Width_Total	(1144)
#define Texture_Bull_Height_Total	(216)
#define Texture_Bull_Divide_X		(4)
#define Texture_Bull_Divide_Y		(1)
#define Texture_Bull_Width			(Texture_Bull_Width_Total / Texture_Bull_Divide_X)
#define Texture_Bull_Height			(Texture_Bull_Height_Total / Texture_Bull_Divide_Y)

// �A�j���[�V������؂�ւ���J�E���g
#define AnimChangeCount					(4)
// �����̉����x
#define Bull_Acceleration				(20.0f)		


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBullVertex(BULL *Bull);
// ���_���W�̐ݒ�
void SetBullVertex(BULL *Bull);
// �e�N�X�`�����W�̐ݒ�
void SetBullTexture(BULL *Bull);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`�����
LPDIRECT3DTEXTURE9		Bull_Texture;
// �����\����
BULL					Bull[Bull_Max];
// �摜���S���璸�_�܂ł̊p�x
static float BaseAngle = 0.0f;
// �摜���S���璸�_�܂ł̋���
static float Radius = 0.0f;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBull(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Bull_No = 0;
	BULL *Bull = GetBull(Bull_No);
	PLAYER *Player = GetPlayer(0);

	Bull[0].pos = D3DXVECTOR3(600.0f, 100.0f, 0.0f);
	Bull[0].f_pos = Player[0].pos;
	Bull[1].pos = D3DXVECTOR3(600.0f, 400.0f, 0.0f);
	Bull[1].f_pos = Player[1].pos;

	Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Bull_Width / 2, Texture_Bull_Height / 2));
	BaseAngle = atan2f(Texture_Bull_Height, Texture_Bull_Width);

	// �����̏���������
	for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++)
	{
		Bull[Bull_No].AnimCount = 0;
		Bull[Bull_No].AnimPattern = 0;
		Bull[Bull_No].part = 0;
		Bull[Bull_No].move_x = 0.0f;
		Bull[Bull_No].move_y = 0.0f;
		Bull[Bull_No].dealttime = 0.0f;
		Bull[Bull_No].move_v0 = 10.0f;
		Bull[Bull_No].move_dash_x = Bull[Bull_No].f_pos.x - Bull[Bull_No].pos.x;
		Bull[Bull_No].move_dash_y = Bull[Bull_No].f_pos.y - Bull[Bull_No].pos.y;
		Bull[Bull_No].Use = true;
		Bull[Bull_No].posX = true;

		// ���_���̍쐬
		MakeBullVertex(&Bull[Bull_No]);
	}

	if (FirstInit == true)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Bull, &Bull_Texture, "Bull") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBull(void)
{
	SafeRelease(Bull_Texture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBull(void)
{
	int Bull_No = 0;
	PLAYER *Player = GetPlayer(0);
	BULL *Bull = GetBull(0);

	for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++, Bull++)
	{
		/*�G�͎g�p���Ă���Ȃ�*/
		if (Bull->Use == true)
		{
			// �A�j���[�V�����J�E���g���Z
			Bull->AnimCount++;

			// �A�j���[�V�����̐؂�ւ�
			if ((Bull->AnimCount % AnimChangeCount) == 0)
			{
				Bull->AnimPattern = (Bull->AnimPattern + 1) % Texture_Bull_Divide_X;
			}

			//�����̎��Ԍv�Z
			Bull->dealttime += 0.01f;

			//��ނ���
			if (Bull->part == 0)
			{
				if (Bull->pos.x < Bull->f_pos.x)//�w�����̓G�ƃv���B���[�̋����v�Z
				{
					Bull->posX = true;
					Bull->move_x = -Bull->move_v0 + Bull->dealttime * Bull_Acceleration;//�w�����̑��x��0�܂ł̈ړ��ʌv�Z
					if (Bull->move_x > 0.0f)//�������x��0�ȏ�ɂȂ�����A�_�b�V���Ɉڍs����
					{
						Bull->move_dash_x = Bull->f_pos.x - Bull->pos.x;//�_�b�V���̑��x�̓v���B���[�ƓG�̋���/20
						Bull->part = 1;//�_�b�V�����
					}
				}
				else if (Bull->pos.x > Bull->f_pos.x)
				{
					Bull->posX = false;
					Bull->move_x = Bull->move_v0 + Bull->dealttime * -Bull_Acceleration;
					if (Bull->move_x < 0.0f)
					{
						Bull->move_dash_x = Bull->f_pos.x - Bull->pos.x;
						Bull->part = 1;
					}
				}

				if (Bull->pos.y < Bull->f_pos.y)
				{
					Bull->move_y = -Bull->move_v0 + Bull->dealttime * Bull_Acceleration;
					if (Bull->move_y > 0.0f)
					{
						Bull->move_dash_y = Bull->f_pos.y - Bull->pos.y;
						Bull->part = 1;
					}
				}
				else if (Bull->pos.y > Bull->f_pos.y)
				{
					Bull->move_y = Bull->move_v0 + Bull->dealttime * -Bull_Acceleration;
					if (Bull->move_y < 0.0f)
					{
						Bull->move_dash_y = Bull->f_pos.y - Bull->pos.y;
						Bull->part = 1;
					}
				}

				Bull->pos.x += Bull->move_x;//�ړ��v�Z
				Bull->pos.y += Bull->move_y;
			}

			//�O�Ƀ_�b�V��
			if (Bull->part == 1)
			{
				Bull->move_x = Bull->move_dash_x / 20;
				Bull->move_y = Bull->move_dash_y / 20;

				Bull->pos.x += Bull->move_x;
				Bull->pos.y += Bull->move_y;
			}

			//sinx �ړ��O�� �_�b�V���� ����������
			if (Bull->part == 2)
			{
				if (Bull->dealttime < 6.28f * 3.0f)
				{
					if (Bull->pos.x < Bull->f_pos.x)
					{
						Bull->pos.x -= 1.0f;
					}
					else if (Bull->pos.x > Bull->f_pos.x)
					{
						Bull->pos.x += 1.0f;
					}
					Bull->pos.y -= sinf(Bull->dealttime) * 3.0f;
				}
				if (Bull->dealttime > 6.28f * 3.0f)
				{
					Bull->dealttime = -1.0f;
					Bull->f_pos = Player[Bull_No].pos;
					Bull->part = 3;
				}
				Bull->dealttime += 0.3f;
			}

			// �G���ǂ𓖂�������
			// �E�̕�
			if (Bull->pos.x > Screen_Width - (Texture_Bull_Width / 2 + Wall_Width))
			{
				if (Bull->part == 0)//��Ԃֈڍs
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)//��Ԃֈڍs
				{
					Bull->part = 2;
				}
				Bull->pos.x = Screen_Width - (Texture_Bull_Width / 2 + Wall_Width);
			}
			// ���̕�
			else if (Bull->pos.x < Texture_Bull_Width / 2 + Wall_Width)
			{
				if (Bull->part == 0)
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)
				{
					Bull->part = 2;
				}

				Bull->pos.x = Texture_Bull_Width / 2 + Wall_Width;
			}

			// ���̕�
			if (Bull->pos.y > Screen_Height - (Texture_Bull_Height / 2 + Wall_Width))
			{
				if (Bull->part == 0)
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)
				{
					Bull->part = 2;
				}

				Bull->pos.y = Screen_Height - (Texture_Bull_Height / 2 + Wall_Width);
			}
			// ��̕�
			else if (Bull->pos.y < Texture_Bull_Height / 2 + Wall_Width)
			{
				if (Bull->part == 0)
				{
					Bull->f_pos = Player[Bull_No].pos;
				}
				if (Bull->part == 1)
				{
					Bull->part = 2;
				}

				Bull->pos.y = Texture_Bull_Height / 2 + Wall_Width;
			}

			//���[�v������
			if (Bull->part == 3)
			{
				Bull->move_x = Bull->move_y = 0.0f;
				Bull->f_pos.x = Player[Bull_No].pos.x;
				Bull->f_pos.y = Player[Bull_No].pos.y;
				Bull->move_dash_x = Bull->f_pos.x - Bull->pos.x;
				Bull->move_dash_y = Bull->f_pos.y - Bull->pos.y;

				//�x�e���ԁA0.0f�͋x�e���Ȃ�
				if (Bull->dealttime > 1.0f)
				{
					Bull->part = 0;
					Bull->dealttime = 0.0f;
				}
			}
		}

		// ���_���W�X�V
		SetBullVertex(Bull);
		// �e�N�X�`�����W�X�V
		SetBullTexture(Bull);
	}
	
	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBull(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Bull_No = 0;
	BULL *Bull = GetBull(Bull_No);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++, Bull++)
	{
		// �g�p���Ă����ԂȂ�`�悷��
		if (Bull->Use == true)
		{
			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, Bull_Texture);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Bull->vertexwk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBullVertex(BULL *Bull)
{
	// ���_���W�̐ݒ�	
	SetBullVertex(Bull);	// ���_�̌v�Z����

	// rhw�̐ݒ�
	Bull->vertexwk[0].rhw = 1.0f;
	Bull->vertexwk[1].rhw = 1.0f;
	Bull->vertexwk[2].rhw = 1.0f;
	Bull->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Bull->vertexwk[0].diffuse = WHITE(255);
	Bull->vertexwk[1].diffuse = WHITE(255);
	Bull->vertexwk[2].diffuse = WHITE(255);
	Bull->vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetBullTexture(Bull);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBullTexture(BULL *Bull)
{
	int x = Bull->AnimPattern;
	float sizeX = 1.0f / Texture_Bull_Divide_X;
	float sizeY = 1.0f / Texture_Bull_Divide_Y;

	Bull->vertexwk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	Bull->vertexwk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	Bull->vertexwk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	Bull->vertexwk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBullVertex(BULL *Bull)
{
	//�E�����@�摜�ɂ���Ă�����`�F���W���܂��@���ʂ͉摜���E�Ɍ������Ă���
	if (Bull->posX == true)
	{
		Bull->vertexwk[0].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[0].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
	}
	//������
	else if (Bull->posX == false)
	{
		Bull->vertexwk[0].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[0].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[1].vtx.y = Bull->pos.y - sinf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.x = Bull->pos.x + cosf(BaseAngle) * Radius;
		Bull->vertexwk[2].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.x = Bull->pos.x - cosf(BaseAngle) * Radius;
		Bull->vertexwk[3].vtx.y = Bull->pos.y + sinf(BaseAngle) * Radius;
	}
}

//=============================================================================
// �����擾�֐�
//=============================================================================
BULL *GetBull(int Bull_No)
{
	return &Bull[Bull_No];
}

//=============================================================================
// �����̃e�N�X�`���̃T�C�Y���擾����
//=============================================================================
D3DXVECTOR2 GetBullSize(void)
{
	return D3DXVECTOR2(Texture_Bull_Width - 40, Texture_Bull_Height - 40);
}
