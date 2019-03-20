//=============================================================================
//
// �����蔻�菈�� [CheckHit.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "CheckHit.h"
#include "Player.h"
#include "Effect.h"
#include "Boss.h"
#include "Apple.h"
#include "Wolf.h"
#include "Score.h"
#include "Bull.h"
#include "Block.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �����̃_���[�W
#define BullDamage				(1.5f)
// �v���C���[�̒e�̃_���[�W
#define PlayerBulletDamage		(3.0f)
// �{�X�̒e�̃_���[�W
#define BossBulletDamage		(17.0f)
// �a���͈̔�
#define SlashRange				(200.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// BC = �o�E���f�B���O�T�[�N��
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrepos, float BulletSpeed);
// �e�������`�F�b�N����
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius);
// �v���C���[�������ꂽ����
void PlayerEvent(int Player_No);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();
	APPLE *Apple = GetApple(0);
	WOLF *Wolf = GetWolf(0);
	BULL *Bull = GetBull(0);
	BLOCK *Block = GetBlock(0);
	D3DXVECTOR2 Obj1Size = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 Obj2Size = D3DXVECTOR2(0.0f, 0.0f);
	int GameStage = GetGameStage();
	int Apple_No = 0;
	int Wolf_No = 0;
	int Bull_No = 0;
	int Block_No = 0;
	int Effect_No = 0;
	int Bullet_No = 0;
	int Player_No = 0;
	float Distance = 0.0f;

	switch (GameStage)
	{
	case Stage_Futago:


		break;

	case Stage_Oushi:

		// �v���C���[�Ɖ���
		// �|�C���^�[��������
		Player = GetPlayer(Player_A);
		Obj1Size = GetPlayerSize();
		Obj2Size = GetBullSize();
		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			if (Player->Use == true)
			{
				BULL *Bull = GetBull(0);
				for (Bull_No = 0; Bull_No < Bull_Max; Bull_No++, Bull++)
				{
					if (CheckHitBB(Bull->pos, Player->pos, Obj1Size, Obj2Size) == true)
					{
						PlayerEvent(Player_No);
					}
				}
			}
		}
		break;

	case Stage_Ite:

		// �v���C���[�̖�ƃA�b�v��
		// �|�C���^�[��������
		Player = GetPlayer(Player_A);
		for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
		{
			if (Player->Bullet->Use == true)
			{
				// �A�b�v���̃|�C���^�[��������
				Apple = GetApple(Apple_No);

				for (Apple_No = 0; Apple_No < Apple_Max; Apple_No++, Apple++)
				{
					if (Apple->Use == true)
					{
						if (CheckHitBC((Apple->HitRadius + Player->Bullet[Bullet_No].HitRadius),
							Apple->pos,
							Player->Bullet[Bullet_No].pos,
							Player->Bullet[Bullet_No].PreviousPos,
							Player->Bullet[Bullet_No].Speed) == true)
						{
							Player->Bullet[Bullet_No].Use = false;
							Apple->Use = false;
							SetSound(SE_HitApple, E_DS8_FLAG_NONE, true, false);
							// ��񂲂��˔��������𑝂₷
							AddScore(1);
						}
					}
				}
			}
		}

		// �v���C���[�ƘT
		// �|�C���^�[��������
		Obj1Size = GetPlayerSize();
		Obj2Size = GetWolfSize();
		Wolf = GetWolf(0);
		for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
		{
			if (Wolf->Use == true)
			{
				Player = GetPlayer(Player_A);
				if (CheckHitBB(Player->pos, Wolf->pos, Obj1Size, Obj2Size) == true)
				{
					// �G��������
					Wolf->Use = false;
					SetSound(SE_SpeedDown, E_DS8_FLAG_NONE, true, false);
					if (Player->Speed >= 2.0f)
					{
						Player->Speed -= 0.25f;
					}
				}
			}
		}

		// �a���ƘT
		// �|�C���^�[��������
		Player = GetPlayer(Player_B);
		if (Player->InSlash == true)
		{
			Wolf = GetWolf(0);
			for (Wolf_No = 0; Wolf_No < Wolf_Max; Wolf_No++, Wolf++)
			{
				if (Wolf->Use == true)
				{
					if ((Player->Direction == Left && Wolf->pos.x < Player->pos.x) ||
						(Player->Direction == Right && Wolf->pos.x > Player->pos.x))
					{
						Distance = D3DXVec3LengthSq(&D3DXVECTOR3(Player->pos - Wolf->pos));
						if (Distance <= pow(SlashRange, 2))
						{
							// �T��������
							Wolf->Use = false;
						}
					}
				}
			}
			Player->InSlash = false;
		}

		break;

	case Stage_Shishi:

		// �v���C���[�̖�ƃG�l�~�[�A�{�X
		// �|�C���^�[��������
		Player = GetPlayer(Player_A);
		for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
		{
			for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
			{
				if (Player->Bullet[Bullet_No].Use == true)
				{
					// �{�X
					if (Boss->Exist == true)
					{
						if (CheckHitBC((Boss->HitRadius + Player->Bullet[Bullet_No].HitRadius),
							Boss->pos,
							Player->Bullet[Bullet_No].pos,
							Player->Bullet[Bullet_No].PreviousPos,
							Player->Bullet[Bullet_No].Speed) == true)
						{
							if (CheckBulletDisappear(Boss->pos,
								Player->Bullet[Bullet_No].pos,
								Player->Bullet[Bullet_No].HitRadius) == true)
							{
								// 1�t���C����e��������
								Player->Bullet[Bullet_No].DelayDisappear = true;
							}
							else
							{
								// ���̃t���C���ɒe��������
								Player->Bullet[Bullet_No].Use = false;
							}

							SetSound(SE_HitApple, E_DS8_FLAG_NONE, true, false);
							Boss->HP -= PlayerBulletDamage + Player->RegeneBonus;
							if (Boss->HP <= 0)
							{
								Boss->HP = 0;
							}

							SetBossColor(Red);
						}
					}
				}
			}
		}

		// �{�X�̒e�ƃv���C���[
		if (Boss->Exist == true)
		{
			Player = GetPlayer(Player_A);
			for (Player_No = 0; Player_No < Player_Max; Player_No++, Player++)
			{
				if (Player->Use == true)
				{
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == true)
						{
							if (CheckHitBC((Player->HitRadius + Boss->Bullet[Bullet_No].HitRadius),
								Player->pos,
								Boss->Bullet[Bullet_No].pos,
								Boss->Bullet[Bullet_No].PreviousPos,
								Boss->Bullet[Bullet_No].Speed) == true)
							{
								Boss->Bullet[Bullet_No].Use = false;
								if (Player->InvincibleCount == 0)
								{
									PlayerEvent(Player_No);
								}
							}
						}
					}
				}
			}
		}
		break;

	default:
		break;
	}


	return;
}

//=============================================================================
// �����蔻��i�~�Ɖ~�j
//=============================================================================
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrepos, float BulletSpeed)
{
	int i = 0;
	// �����蔻��̌���
	bool Result = false;
	// ���S�_�Ԃ̒������v�Z
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Bulletpostemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// Object�̒��S�ƒe�̒��S�̋���
	float Distance = 0.0f;

	// �e�̃X�s�[�h�͓�����͈͂��傫���Ȃ�
	if (BulletSpeed > CheckRange)
	{
		// ���̒e��1�t���C���O�̒e�̋������L�^����
		Bulletpostemp = Bullet - BulletPrepos;
		// �e�̔�s���� / ������͈� = ���[�v��
		for (i = 0; i < BulletSpeed / CheckRange; i++)
		{
			// �ڕW�ƒe�̋������v�Z����
			temp = Object - BulletPrepos;
			Distance = D3DXVec3LengthSq(&temp);
			if (Distance <= pow(CheckRange, 2))
			{
				Result = true;
				break;
			}
			else
			{
				// �e��i��ł�A�i��ł鋗�� = �S���̔�s���� / ���[�v��
				BulletPrepos.x += Bulletpostemp.x / (BulletSpeed / CheckRange);
				BulletPrepos.y += Bulletpostemp.y / (BulletSpeed / CheckRange);
				Result = false;
			}
		}
	}
	else
	{
		temp = Object - Bullet;
		Distance = D3DXVec3LengthSq(&temp);
		Result = Distance <= pow(CheckRange, 2) ? true : false;
	}

	return Result;
}

//=============================================================================
// �����蔻��i��`�Ƌ�`�j
//=============================================================================
bool CheckHitBB(D3DXVECTOR3 Obj1Pos, D3DXVECTOR3 Obj2Pos, D3DXVECTOR2 Obj1Size, D3DXVECTOR2 Obj2Size)
{
	bool Result = false;

	Obj1Size /= 2.0f;	// �����ɂ���
	Obj2Size /= 2.0f;

	if (Obj1Pos.x - Obj1Size.x < Obj2Pos.x + Obj2Size.x &&
		Obj1Pos.x + Obj1Size.x > Obj2Pos.x - Obj2Size.x &&
		Obj1Pos.y - Obj1Size.y < Obj2Pos.y + Obj2Size.y &&
		Obj1Pos.y + Obj1Size.y > Obj2Pos.y - Obj2Size.y)
	{
		Result = true;
	}

	return Result;
}

//=============================================================================
// �e�������`�F�b�N����
//=============================================================================
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius)
{
	bool Result = false;
	float Distance = 0.0f;

	Distance = D3DXVec3LengthSq(&D3DXVECTOR3(Object - Bullet));
	// �����蔻��
	// ���a * 2 = ���a
	Result = Distance >= pow((BulletRadius * 2), 2) ? true : false;

	return Result;
}

//=============================================================================
// �v���C���[�������ꂽ����
//=============================================================================
void PlayerEvent(int Player_No)
{
	PLAYER *Player = GetPlayer(Player_No);
	int GameStage = GetGameStage();

	// ���G�J�E���g���Z
	Player->InvincibleCount++;

	if (GameStage == Stage_Oushi)
	{
		SetSound(SE_HitBull, E_DS8_FLAG_NONE, true, false);
		if (Player_No == Player_A)
		{
			SetSound(SE_Scream_PlayerA, E_DS8_FLAG_NONE, true, false);
		}
		else if (Player_No == Player_B)
		{
			SetSound(SE_Scream_PlayerB, E_DS8_FLAG_NONE, true, false);
		}
		Player->HP -= BullDamage;
	}
	else if (GameStage == Stage_Shishi)
	{
		SetSound(SE_PlayerBeShot, E_DS8_FLAG_NONE, true, false);
		Player->HP -= BossBulletDamage;
	}

	// HP��0�ɂȂ�
	if (Player->HP <= 0.0f)
	{
		Player->HP = 0.0f;
		Player->Use = false;
	}

	return;
}
