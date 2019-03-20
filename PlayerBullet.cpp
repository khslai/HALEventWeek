//=============================================================================
//
// �v���C���[�e������ [PlayerBullet.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "PlayerBullet.h"
#include "Player.h"
#include "Boss.h"
#include "GameUI.h"
#include "AttackRange.h"
#include "Effect.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_PlayerBullet			_T("data/Texture/Bullet/Arrow.png")
#define Texture_PlayerBullet_Width		(60)
#define Texture_PlayerBullet_Height		(100)
// �v���C���[�̒e�̈ړ����x
#define PlayerBulletSpeed				(20.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeBulletVertex(BULLET *Bullet);
// ���_���W�̐ݒ�
static void SetBulletVertex(BULLET *Bullet);
// �e�N�X�`�����W�̐ݒ�
static void SetBulletTexture(BULLET *Bullet);
// �v���C���[�̒e�̈ړ�
static void BulletMove(BULLET *Bullet);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 Bullet_Texture = NULL;
// �摜���S���璸�_�܂ł̊p�x
static float BaseAngle = 0.0f;
// �摜���S���璸�_�܂ł̋���
static float Radius = 0.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayerBullet(bool FirstInit)
{
	int Bullet_No = 0;
	int Player_No = 0;
	PLAYER *Player = GetPlayer(Player_No);

	for (Player_No = 0; Player_No < Player_Max; Player_No++)
	{
		Player = GetPlayer(Player_No);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			Player->Bullet[Bullet_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].MoveDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Player->Bullet[Bullet_No].Radius = 0.0f;
			Player->Bullet[Bullet_No].HitRadius = 0.0f;
			Player->Bullet[Bullet_No].BaseAngle = 0.0f;
			Player->Bullet[Bullet_No].Speed = 0.0f;
			Player->Bullet[Bullet_No].Count = 0;
			Player->Bullet[Bullet_No].Color = 0;
			Player->Bullet[Bullet_No].State = 0;
			Player->Bullet[Bullet_No].Use = false;
			Player->Bullet[Bullet_No].DelayDisappear = false;

			// ���_���̍쐬
			MakeBulletVertex(&Player->Bullet[Bullet_No]);
		}
	}

	if (FirstInit == true)
	{
		Radius = D3DXVec2Length(&D3DXVECTOR2(Texture_PlayerBullet_Width / 2, Texture_PlayerBullet_Height / 2));
		BaseAngle = atan2f(Texture_PlayerBullet_Height, Texture_PlayerBullet_Width);

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_PlayerBullet, &Bullet_Texture, "PlayerBullet") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayerBullet(void)
{
	SafeRelease(Bullet_Texture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayerBullet(void)
{
	int Bullet_No = 0;
	int Player_No = 0;
	PLAYER *Player = GetPlayer(Player_No);

	for (Player_No = 0; Player_No < Player_Max; Player_No++)
	{
		Player = GetPlayer(Player_No);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet[Bullet_No].Use == true)
			{
				BulletMove(&Player->Bullet[Bullet_No]);

				// ���_���W�̍X�V
				SetBulletVertex(&Player->Bullet[Bullet_No]);
			}
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Player_No = 0;
	int Bullet_No = 0;
	PLAYER *Player = GetPlayer(Player_No);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, Bullet_Texture);

	for (Player_No = 0; Player_No < Player_Max; Player_No++)
	{
		Player = GetPlayer(Player_No);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet[Bullet_No].Use == true)
			{
				// �|���S���̕`��
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Player->Bullet[Bullet_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}

//=============================================================================
// �v���C���[�̒e�̈ړ�
//=============================================================================
void BulletMove(BULLET *Bullet)
{
	if (Bullet->DelayDisappear == true)
	{
		Bullet->Use = false;
	}
	else
	{
		// 1�t���C���O�̒e���W���L�^
		Bullet->PreviousPos = Bullet->pos;
		// ���W�X�V
		Bullet->pos += Bullet->MoveDirection * Bullet->Speed;
		// �J�E���g���Z
		Bullet->Count++;

		// �e�̈ړ��������
		if (CheckPosition(Bullet->pos, Texture_PlayerBullet_Width, Texture_PlayerBullet_Height) == true)
		{
			Bullet->Use = false;
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeBulletVertex(BULLET *Bullet)
{

	// ���_���W�̐ݒ�	
	SetBulletVertex(Bullet);

	// rhw�̐ݒ�
	Bullet->vertexwk[0].rhw = 1.0f;
	Bullet->vertexwk[1].rhw = 1.0f;
	Bullet->vertexwk[2].rhw = 1.0f;
	Bullet->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Bullet->vertexwk[0].diffuse = WHITE(255);
	Bullet->vertexwk[1].diffuse = WHITE(255);
	Bullet->vertexwk[2].diffuse = WHITE(255);
	Bullet->vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetBulletTexture(Bullet);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBulletTexture(BULLET *Bullet)
{
	Bullet->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	Bullet->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	Bullet->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	Bullet->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBulletVertex(BULLET *Bullet)
{
	Bullet->vertexwk[0].vtx.x = Bullet->pos.x - cosf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[0].vtx.y = Bullet->pos.y - sinf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[1].vtx.x = Bullet->pos.x + cosf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[1].vtx.y = Bullet->pos.y - sinf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[2].vtx.x = Bullet->pos.x - cosf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[2].vtx.y = Bullet->pos.y + sinf(Bullet->BaseAngle - Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[3].vtx.x = Bullet->pos.x + cosf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;
	Bullet->vertexwk[3].vtx.y = Bullet->pos.y + sinf(Bullet->BaseAngle + Bullet->rot.z) * Bullet->Radius;

	return;
}

//=============================================================================
// �v���C���[�e�̐ݒu
//=============================================================================
void PlayerShot(int Player_No)
{
	int Bullet_No = 0;
	PLAYER *Player = GetPlayer(Player_No);
	GAMEUI *GameUI = GetGameUI(0);

	// �v���C���[�V���b�g
	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == false)
		{
			Player->Bullet[Bullet_No].Use = true;
			Player->Bullet[Bullet_No].DelayDisappear = false;
			Player->Bullet[Bullet_No].pos.x = Player->pos.x;
			Player->Bullet[Bullet_No].pos.y = Player->pos.y;
			Player->Bullet[Bullet_No].Count = 0;
			Player->Bullet[Bullet_No].BaseAngle = BaseAngle;
			switch (Player->Direction)
			{
			case Up:
				Player->Bullet[Bullet_No].MoveDirection = UpVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(0.0f);
				break;
			case Down:
				Player->Bullet[Bullet_No].MoveDirection = DownVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(180.0f);
				break;
			case Left:
				Player->Bullet[Bullet_No].MoveDirection = LeftVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(-90.0f);
				break;
			case Right:
				Player->Bullet[Bullet_No].MoveDirection = RightVec;
				Player->Bullet[Bullet_No].rot.z = D3DXToRadian(90.0f);
				break;
			default:
				break;
			}
			Player->Bullet[Bullet_No].Radius = Radius;
			Player->Bullet[Bullet_No].HitRadius = Radius * 0.1f;
			Player->Bullet[Bullet_No].Speed = PlayerBulletSpeed;

			SetSound(SE_ShotArrow, E_DS8_FLAG_NONE, true, false);

			if (Player_No == Player_A)
			{
				// �N�[���_�E��UI��ݒu����
				GameUI = GetGameUI(UIType_ShotCD_PlayerA);
				GameUI->Use = true;

#if _DEBUG
				// ������͈͕\��
				SetBulletAR(Player->Bullet[Bullet_No].pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletAR_A);
#endif
			}
			else if (Player_No == Player_B)
			{
				// �N�[���_�E��UI��ݒu����
				GameUI = GetGameUI(UIType_ShotCD_PlayerB);
				GameUI->Use = true;

#if _DEBUG
				// ������͈͕\��
				SetBulletAR(Player->Bullet[Bullet_No].pos, Player->Bullet[Bullet_No].HitRadius, Bullet_No, PlayerBulletAR_B);
#endif
			}

			break;
		}
	}

	return;
}
