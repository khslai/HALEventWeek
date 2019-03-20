//=============================================================================
//
// �{�X���� [Boss.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "AttackRange.h"
#include "DebugProcess.h"
#include "Effect.h"
#include "Transition.h"
#include "MainMenu.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Boss				_T("data/Texture/Boss/Boss.png")
#define Texture_Boss_Width_Total	(320)
#define Texture_Boss_Height_Total	(480)
#define Texture_Boss_Divide_X		(4)		// �e�N�X�`�����������i��)
#define Texture_Boss_Divide_Y		(6)		// �e�N�X�`�����������i�c)
#define Texture_Boss_Width			(Texture_Boss_Width_Total / Texture_Boss_Divide_X)
#define Texture_Boss_Height			(Texture_Boss_Height_Total / Texture_Boss_Divide_Y)

// �{�X�A�j���ϊ��J�E���g
#define AnimChangeCount				(15)		


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBossVertex(void);
// ���_���W�̐ݒ�
void SetBossVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetBossTexture(void);
// ���̃{�X�̒i�K�ɓ���
void NextBossPhase(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �{�X�\����
BOSS Boss;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBoss(bool FirstInit)
{
	Boss.pos = D3DXVECTOR3(1440.0f, ScreenCenter_Y, 0.0f);
	Boss.Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_Boss_Width / 2, Texture_Boss_Height / 2));
	Boss.HitRadius = Boss.Radius * 0.8f;
	Boss.BaseAngle = atan2f(Texture_Boss_Height, Texture_Boss_Width);
	Boss.HP_Max = 100.0f;
	Boss.HP = Boss.HP_Max;
	Boss.AnimPattern = 0;
	Boss.AnimCount = 0;
	Boss.StateCount = 0;
	Boss.BarrageType = BarrageMode1;
	Boss.State = Idle;
	Boss.Exist = true;

	// ���_���̍쐬
	MakeBossVertex();

#if _DEBUG
	SetCharacterAR(Boss.pos, Boss.HitRadius, BossAR);
#endif

	if (FirstInit == true)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Boss, &Boss.texture, "Boss") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{
	SafeRelease(Boss.texture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{
	int GameStage = GetGameStage();
	static int Count = 0;
	float HPPercent = 0.0f;

	if (Boss.Exist == true)
	{
		// �{�X�̃A�j���v�Z
		Boss.AnimCount++;
		if ((Boss.AnimCount % AnimChangeCount) == 0)
		{
			// �e�N�X�`���A�j���p�^�[���v�Z
			Boss.AnimPattern = (Boss.AnimPattern + 1) % Texture_Boss_Divide_X;
		}

		HPPercent = Boss.HP / Boss.HP_Max;
		if (Boss.BarrageType == BarrageMode1 && HPPercent <= 0.67f && HPPercent > 0.33f)
		{
			Boss.State = Idle;
			Boss.StateCount = 0;
			Boss.BarrageType = BarrageMode2;
			NextBossPhase();
		}
		else if (Boss.BarrageType == BarrageMode2 && HPPercent <= 0.33f)
		{
			Boss.State = Idle;
			Boss.StateCount = 0;
			Boss.BarrageType = BarrageMode3;
			NextBossPhase();
		}
		else if (Boss.BarrageType == BarrageMode3 && HPPercent <= 0.0f)
		{
			Boss.State = BossDead;
			Boss.StateCount = 0;
			NextBossPhase();
		}

		// ���_���W�X�V
		SetBossVertex();
		// �e�N�X�`�����W�X�V
		SetBossTexture();
		// �F�A�����x�X�V
		SetBossColor(White);
	}
	else if (Boss.Exist == false && Boss.State == BossDead)
	{
		Count++;
		if (Count >= 120)
		{
			Count = 0;
			SetClearFlag(ClearFlag_Shishi);
			SetTransition(Fadein);
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	if (Boss.Exist == true)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, Boss.texture);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss.vertexwk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// ���̃{�X�̒i�K�ɓ���
//=============================================================================
void NextBossPhase(void)
{
	for (int Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
	{
		if (Boss.Bullet[Bullet_No].Use == true)
		{
			// �e�����G�t�F�N�g�ݒu
			SetEffect(Bullet_No, BulletDead, Boss.Bullet[Bullet_No].pos);
			Boss.Bullet[Bullet_No].Use = false;
		}
	}

	if (Boss.State == BossDead)
	{
		Boss.Exist = false;
		for (int i = 0; i < 10; i++)
		{
			SetEffect(BossEffect, DeadEffect, Boss.pos);
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBossVertex(void)
{
	// ���_���W�̐ݒ�	
	SetBossVertex();

	// rhw�̐ݒ�
	Boss.vertexwk[0].rhw = 1.0f;
	Boss.vertexwk[1].rhw = 1.0f;
	Boss.vertexwk[2].rhw = 1.0f;
	Boss.vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	SetBossColor(White);

	// �e�N�X�`�����W�̐ݒ�
	SetBossTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBossTexture(void)
{
	int x = Boss.AnimPattern;
	int y = 0;
	float sizeX = 1.0f / Texture_Boss_Divide_X;
	float sizeY = 1.0f / Texture_Boss_Divide_Y;

	Boss.vertexwk[0].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY);
	Boss.vertexwk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY);
	Boss.vertexwk[2].tex = D3DXVECTOR2((float)x * sizeX, (float)y * sizeY + sizeY);
	Boss.vertexwk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, (float)y * sizeY + sizeY);

	return;
}

//=============================================================================
// �{�X�̐F�̐ݒ�
//=============================================================================
void SetBossColor(int Color)
{
	switch (Color)
	{
	case White:
		Boss.vertexwk[0].diffuse = WHITE(255);
		Boss.vertexwk[1].diffuse = WHITE(255);
		Boss.vertexwk[2].diffuse = WHITE(255);
		Boss.vertexwk[3].diffuse = WHITE(255);
		break;
		// �����ꂽ��A�F���ς��
	case Red:
		Boss.vertexwk[0].diffuse = RED(255);
		Boss.vertexwk[1].diffuse = RED(255);
		Boss.vertexwk[2].diffuse = RED(255);
		Boss.vertexwk[3].diffuse = RED(255);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBossVertex(void)
{
	Boss.vertexwk[0].vtx.x = Boss.pos.x - cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[0].vtx.y = Boss.pos.y - sinf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[1].vtx.x = Boss.pos.x + cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[1].vtx.y = Boss.pos.y - sinf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[2].vtx.x = Boss.pos.x - cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[2].vtx.y = Boss.pos.y + sinf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[3].vtx.x = Boss.pos.x + cosf(Boss.BaseAngle) * Boss.Radius;
	Boss.vertexwk[3].vtx.y = Boss.pos.y + sinf(Boss.BaseAngle) * Boss.Radius;

	return;
}

//=============================================================================
// �{�X�̏����擾����
//=============================================================================
BOSS *GetBoss(void)
{
	return &Boss;
}

