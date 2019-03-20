//=============================================================================
//
// �G�t�F�N�g���� [Effect.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Effect.h"
#include "Player.h"
#include "Boss.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Regenerate				_T("data/Texture/Effect/Regenerate.png")
#define Texture_Regenerate_Width_Total	(1200)
#define Texture_Regenerate_Height_Total	(120)
#define Texture_Regenerate_Divide_X		(10)
#define Texture_Regenerate_Divide_Y		(1)
#define Texture_Regenerate_Width		(Texture_Regenerate_Width_Total / Texture_Regenerate_Divide_X)
#define Texture_Regenerate_Height		(Texture_Regenerate_Height_Total / Texture_Regenerate_Divide_Y)

#define Texture_BulletDead				_T("data/Texture/Effect/BulletDead.png")
#define Texture_BulletDead_Width_Total	(1200)
#define Texture_BulletDead_Height_Total	(120)
#define Texture_BulletDead_Divide_X		(10)
#define Texture_BulletDead_Divide_Y		(1)
#define Texture_BulletDead_Width		(Texture_BulletDead_Width_Total / Texture_BulletDead_Divide_X)
#define Texture_BulletDead_Height		(Texture_BulletDead_Height_Total / Texture_BulletDead_Divide_Y)

#define Texture_DeadEffect				_T("data/Texture/Effect/DeadEffect.png")
#define Texture_DeadEffect_Width_Total	(700)
#define Texture_DeadEffect_Height_Total	(140)
#define Texture_DeadEffect_Divide_X		(5)
#define Texture_DeadEffect_Divide_Y		(1)
#define Texture_DeadEffect_Width		(Texture_DeadEffect_Width_Total / Texture_DeadEffect_Divide_X)
#define Texture_DeadEffect_Height		(Texture_DeadEffect_Height_Total / Texture_DeadEffect_Divide_Y)

#define Texture_Slash					_T("data/Texture/Effect/Slash.png")
#define Texture_Slash_Width_Total		(1080)
#define Texture_Slash_Height_Total		(120)
#define Texture_Slash_Divide_X			(9)
#define Texture_Slash_Divide_Y			(1)
#define Texture_Slash_Width				(Texture_Slash_Width_Total / Texture_Slash_Divide_X)
#define Texture_Slash_Height			(Texture_Slash_Height_Total / Texture_Slash_Divide_Y)

// �G�t�F�N�g�̍ő吔
#define Effect_Max						(200)
// �A�j���[�V������؂�ւ���J�E���g
#define AnimChangeCount					(5)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeEffectVertex(EFFECT *Effect);
// ���_���W�̐ݒ�
void SetEffectVertex(EFFECT *Effect);
// �e�N�X�`�����W�̐ݒ�
void SetEffectTexture(EFFECT *Effect);
// �F�A�����x�̐ݒ�
void SetEffectDiffuse(EFFECT *Effect, int Alpha);
// �G�t�F�N�g�̌v�Z
void CalculateEffect(EFFECT *Effect);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 EffectTexture[EffectType_Max] = { NULL };
// �G�t�F�N�g�\����
EFFECT Effect[Effect_Max];
// �e�N�X�`�����a
static float BaseAngle[EffectType_Max] = { 0.0f };
// ���S�_�Ǝl���_�̐����p
static float Radius[EffectType_Max] = { 0.0f };

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(bool FirstInit)
{
	int Effect_No = 0;

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		Effect[Effect_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Effect[Effect_No].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Effect[Effect_No].Radius = 0.0f;
		Effect[Effect_No].BaseAngle = 0.0f;
		Effect[Effect_No].Count = 0;
		Effect[Effect_No].AnimPattern = 0;
		Effect[Effect_No].Alpha = 0;
		Effect[Effect_No].Type = 0;
		Effect[Effect_No].Use = false;

		// ���_���̍쐬
		MakeEffectVertex(&Effect[Effect_No]);
	}

	if (FirstInit == true)
	{
		Radius[Regenerate] = D3DXVec2Length(&D3DXVECTOR2(Texture_Regenerate_Width / 2, Texture_Regenerate_Height / 2));
		BaseAngle[Regenerate] = atan2f(Texture_Regenerate_Height, Texture_Regenerate_Width);

		Radius[BulletDead] = D3DXVec2Length(&D3DXVECTOR2(Texture_BulletDead_Width / 2, Texture_BulletDead_Height / 2));
		BaseAngle[BulletDead] = atan2f(Texture_BulletDead_Height, Texture_BulletDead_Width);

		Radius[DeadEffect] = D3DXVec2Length(&D3DXVECTOR2(Texture_DeadEffect_Width / 2, Texture_DeadEffect_Height / 2));
		BaseAngle[DeadEffect] = atan2f(Texture_DeadEffect_Height, Texture_DeadEffect_Width);

		Radius[Slash] = D3DXVec2Length(&D3DXVECTOR2(Texture_Slash_Width / 2, Texture_Slash_Height / 2));
		BaseAngle[Slash] = atan2f(Texture_Slash_Height, Texture_Slash_Width);

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Regenerate, &EffectTexture[Regenerate], "Effect") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BulletDead, &EffectTexture[BulletDead], "Effect") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_DeadEffect, &EffectTexture[DeadEffect], "Effect") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Slash, &EffectTexture[Slash], "Effect") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	for (int i = 0; i < EffectType_Max; i++)
	{
		SafeRelease(EffectTexture[i]);
	}

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	int Effect_No = 0;

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == true)
		{
			Effect[Effect_No].Count++;

			// �G�t�F�N�g�v�Z
			CalculateEffect(&Effect[Effect_No]);

			// �e�N�X�`�����W�X�V
			SetEffectTexture(&Effect[Effect_No]);
			// �F�A�����x�̐ݒ�
			SetEffectDiffuse(&Effect[Effect_No], Effect[Effect_No].Alpha);
			// ���_���W�X�V
			SetEffectVertex(&Effect[Effect_No]);
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Effect_No = 0;

	// ���Z�������[�h�ɐݒu����
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == true)
		{
			// �e�N�X�`���̐ݒ�
			switch (Effect[Effect_No].Type)
			{
			case Regenerate:

				Device->SetTexture(0, EffectTexture[Regenerate]);
				break;

			case BulletDead:

				Device->SetTexture(0, EffectTexture[BulletDead]);
				break;

			case DeadEffect:

				Device->SetTexture(0, EffectTexture[DeadEffect]);
				break;

			case Slash:

				Device->SetTexture(0, EffectTexture[Slash]);
				break;

			default:
				break;
			}

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Effect[Effect_No].vertexwk, sizeof(VERTEX_2D));
		}
	}

	// �{���̐ݒu��߂�
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeEffectVertex(EFFECT *Effect)
{
	// ���_���W�̐ݒ�	
	SetEffectVertex(Effect);

	// rhw�̐ݒ�
	Effect->vertexwk[0].rhw = 1.0f;
	Effect->vertexwk[1].rhw = 1.0f;
	Effect->vertexwk[2].rhw = 1.0f;
	Effect->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	SetEffectDiffuse(Effect, Effect->Alpha);

	// �e�N�X�`�����W�̐ݒ�
	SetEffectTexture(Effect);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetEffectTexture(EFFECT *Effect)
{
	float sizeX = 0.0f;

	switch (Effect->Type)
	{
	case Regenerate:
		sizeX = 1.0f / Texture_Regenerate_Divide_X;
		break;
	case BulletDead:
		sizeX = 1.0f / Texture_BulletDead_Divide_X;
		break;
	case DeadEffect:
		sizeX = 1.0f / Texture_DeadEffect_Divide_X;
		break;
	case Slash:
		sizeX = 1.0f / Texture_Slash_Divide_X;
		break;
	default:
		break;
	}

	Effect->vertexwk[0].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX, 0.0f);
	Effect->vertexwk[1].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX + sizeX, 0.0f);
	Effect->vertexwk[2].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX, 1.0f);
	Effect->vertexwk[3].tex = D3DXVECTOR2((float)(Effect->AnimPattern)* sizeX + sizeX, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetEffectVertex(EFFECT *Effect)
{
	Effect->vertexwk[0].vtx.x = Effect->pos.x - cosf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[0].vtx.y = Effect->pos.y - sinf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[1].vtx.x = Effect->pos.x + cosf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[1].vtx.y = Effect->pos.y - sinf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[2].vtx.x = Effect->pos.x - cosf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[2].vtx.y = Effect->pos.y + sinf(Effect->BaseAngle - Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[3].vtx.x = Effect->pos.x + cosf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;
	Effect->vertexwk[3].vtx.y = Effect->pos.y + sinf(Effect->BaseAngle + Effect->rot.z) * Effect->Radius;

	return;
}

//=============================================================================
// �F�A�����x�̐ݒ�
//=============================================================================
void SetEffectDiffuse(EFFECT *Effect, int Alpha)
{
	// ���ˌ��̐ݒ�
	Effect->vertexwk[0].diffuse = WHITE(Alpha);
	Effect->vertexwk[1].diffuse = WHITE(Alpha);
	Effect->vertexwk[2].diffuse = WHITE(Alpha);
	Effect->vertexwk[3].diffuse = WHITE(Alpha);

	return;
}

//=============================================================================
// �G�t�F�N�g�̐ݒu
//=============================================================================
void SetEffect(int Owner, int EffectType, D3DXVECTOR3 Pos)
{
	int Effect_No = 0;
	BOSS *Boss = GetBoss();

	for (Effect_No = 0; Effect_No < Effect_Max; Effect_No++)
	{
		if (Effect[Effect_No].Use == false)
		{
			Effect[Effect_No].Use = true;
			// ���W
			Effect[Effect_No].pos = Pos;
			// �����x
			Effect[Effect_No].Alpha = 255;
			// �J�E���g
			Effect[Effect_No].Count = 0;
			// ���L��
			Effect[Effect_No].Owner = Owner;

			switch (EffectType)
			{
			case Regenerate:
				
				Effect[Effect_No].BaseAngle = BaseAngle[Regenerate];
				Effect[Effect_No].Radius = Radius[Regenerate];
				Effect[Effect_No].Type = Regenerate;
				// ��]�p�x
				Effect[Effect_No].rot.z = 0.0f;
				// ��ԍ��̉摜����\��
				Effect[Effect_No].AnimPattern = 0;
				break;

			case BulletDead:

				Effect[Effect_No].BaseAngle = BaseAngle[BulletDead];
				Effect[Effect_No].Radius = Radius[BulletDead];
				Effect[Effect_No].Type = BulletDead;
				// ��]�p�x
				Effect[Effect_No].rot.z = 0.0f;
				// ��ԍ��̉摜����\��
				Effect[Effect_No].AnimPattern = 0;
				break;

			case DeadEffect:

				Effect[Effect_No].BaseAngle = BaseAngle[DeadEffect];
				Effect[Effect_No].Radius = Radius[DeadEffect] * 2.0f;
				Effect[Effect_No].Type = DeadEffect;
				// ��]�p�x
				Effect[Effect_No].rot.z = D3DXToRadian(360.0f * (float)rand() / RAND_MAX);
				// �����_���̉摜�\��
				Effect[Effect_No].AnimPattern = rand() % (4 + 1);
				break;

			case Slash:

				Effect[Effect_No].BaseAngle = BaseAngle[Slash];
				Effect[Effect_No].Radius = Radius[Slash];
				Effect[Effect_No].Type = Slash;
				// ��]�p�x
				Effect[Effect_No].rot.z = 0.0f;
				// ��ԍ��̉摜����\��
				Effect[Effect_No].AnimPattern = 0;
				break;

			default:
				break;
			}
			break;
		}
	}

	return;
}

//=============================================================================
// �G�t�F�N�g�̌v�Z
//=============================================================================
void CalculateEffect(EFFECT *Effect)
{
	PLAYER *Player = GetPlayer(0);

	switch (Effect->Type)
	{
	case Regenerate:
		// �G�t�F�N�g�I��
		if (Effect->Count >= AnimChangeCount * Texture_Regenerate_Divide_X)
		{
			Effect->Use = false;
		}
		// �e�N�X�`���A�j���p�^�[���v�Z
		else
		{
			if (Effect->Owner == Player_A)
			{
				Player = GetPlayer(Player_A);
			}
			else if (Effect->Owner == Player_B)
			{
				Player = GetPlayer(Player_B);
			}
			Effect->pos = Player->pos;

			if ((Effect->Count % AnimChangeCount) == 0)
			{
				Effect->AnimPattern = (Effect->AnimPattern + 1) % Texture_Regenerate_Divide_X;
			}
		}
		break;

	case BulletDead:
		// �G�t�F�N�g�I��
		if (Effect->Count >= AnimChangeCount * Texture_BulletDead_Divide_X)
		{
			Effect->Use = false;
		}

		// �e�N�X�`���A�j���p�^�[���v�Z
		if ((Effect->Count % AnimChangeCount) == 0)
		{
			Effect->AnimPattern = (Effect->AnimPattern + 1) % Texture_BulletDead_Divide_X;
		}
		break;

	case DeadEffect:

		// �G�t�F�N�g�I��
		if (Effect->Count >= 60.0f)
		{
			Effect->Use = false;
		}

		// �G�t�F�N�g�g��
		Effect->Radius += 0.2f;
		break;

	case Slash:

		// �G�t�F�N�g�I��
		if (Effect->Count >= AnimChangeCount * Texture_Slash_Divide_X)
		{
			Effect->Use = false;
		}

		// �e�N�X�`���A�j���p�^�[���v�Z
		if ((Effect->Count % AnimChangeCount) == 0)
		{
			Effect->AnimPattern = (Effect->AnimPattern + 1) % Texture_Slash_Divide_X;
		}
		break;

	default:
		break;
	}

	return;
}