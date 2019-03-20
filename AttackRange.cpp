//=============================================================================
//
// ������͈͕\������ [AttackRange.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "AttackRange.h"
#include "Player.h"
#include "Boss.h"
#include "Effect.h"
#include "Input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_AttackRange			_T("data/Texture/AttackRange.png")
#define Texture_AttackRange_Width	(50)
#define Texture_AttackRange_Height	(50)

// ������͈͕\���̍ő吔
#define AttackRange_Max				(999)
// ������͈͂̃A���t�@�l
#define AttackRange_Alpha			(128)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// AR = AttackRange
// ���_�̍쐬
HRESULT MakeARVertex(ATTACKRANGE *AttackRangePtr);
// ���_���W�̐ݒ�
void SetARVertex(ATTACKRANGE *AttackRangePtr);
// �e�N�X�`�����W�̐ݒ�
void SetARTexture(ATTACKRANGE *AttackRangePtr);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 AttackRange_Texture = NULL;
// ������͈͍\����
ATTACKRANGE AttackRange[AttackRange_Max];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitAttackRange(bool FirstInit)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int AttackRange_No = 0;

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		AttackRange[AttackRange_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		AttackRange[AttackRange_No].Radius = 0.0f;
		AttackRange[AttackRange_No].Owner = 0;
		AttackRange[AttackRange_No].BulletNo = 0;
		AttackRange[AttackRange_No].BarrageNo = 0;
		AttackRange[AttackRange_No].Use = false;

		// ���_���̍쐬
		MakeARVertex(&AttackRange[AttackRange_No]);
	}

	if (FirstInit == true)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_AttackRange, &AttackRange_Texture, "AttackRange") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAttackRange(void)
{
	SafeRelease(AttackRange_Texture);

	return;
}

//=============================================================================
// AttackRange�X�V����
//=============================================================================
void UpdateAttackRange(void)
{
	int AttackRange_No = 0;
	int Barrage_No = 0;
	int Bullet_No = 0;
	int Enemy_No = 0;
	PLAYER *Player = NULL;
	BOSS *Boss = GetBoss();

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == true)
		{
			// ������͈͂̏��L�҂ɂ���āA�Ⴄ�������s��
			switch (AttackRange[AttackRange_No].Owner)
			{
				// �v���C���[
			case PlayerAR_A:
				Player = GetPlayer(Player_A);
			case PlayerAR_B:

				if (Player == NULL)
				{
					Player = GetPlayer(Player_B);
				}
				AttackRange[AttackRange_No].pos = Player->pos;
				Player = NULL;
				break;

				// �v���C���[�̒e
			case PlayerBulletAR_A:
				Player = GetPlayer(Player_A);
			case PlayerBulletAR_B:

				if (Player == NULL)
				{
					Player = GetPlayer(Player_B);
				}
				for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
				{
					if (AttackRange[AttackRange_No].BulletNo == Bullet_No)
					{
						if (Player->Bullet[Bullet_No].Use == true)
						{
							AttackRange[AttackRange_No].pos = Player->Bullet[Bullet_No].pos;
						}
						else
						{
							AttackRange[AttackRange_No].Use = false;
						}
						Player = NULL;
						break;
					}
				}
				break;

				// �{�X
			case BossAR:

				if (Boss->Exist == true)
				{
					AttackRange[AttackRange_No].pos = Boss->pos;
				}
				else
				{
					AttackRange[AttackRange_No].Use = false;
				}
				break;

				// �{�X�̒e
			case BossBulletAR:

				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (AttackRange[AttackRange_No].BulletNo == Bullet_No)
					{
						if (Boss->Bullet[Bullet_No].Use == true)
						{
							AttackRange[AttackRange_No].pos = Boss->Bullet[Bullet_No].pos;
						}
						else
						{
							AttackRange[AttackRange_No].Use = false;
						}
					}
				}
				break;

			default:
				break;
			}

			// ���_���W�̍X�V
			SetARVertex(&AttackRange[AttackRange_No]);
		}
	}

	return;
}

//=============================================================================
// AttackRange�`�揈��
//=============================================================================
void DrawAttackRange(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int AttackRange_No = 0;

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == true)
		{
			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, AttackRange_Texture);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, AttackRange[AttackRange_No].vertexwk, sizeof(VERTEX_2D));
		}
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeARVertex(ATTACKRANGE *AttackRangePtr)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���W�̐ݒ�	
	SetARVertex(AttackRangePtr);

	// rhw�̐ݒ�
	AttackRangePtr->vertexwk[0].rhw = 1.0f;
	AttackRangePtr->vertexwk[1].rhw = 1.0f;
	AttackRangePtr->vertexwk[2].rhw = 1.0f;
	AttackRangePtr->vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	AttackRangePtr->vertexwk[0].diffuse = WHITE(AttackRange_Alpha);
	AttackRangePtr->vertexwk[1].diffuse = WHITE(AttackRange_Alpha);
	AttackRangePtr->vertexwk[2].diffuse = WHITE(AttackRange_Alpha);
	AttackRangePtr->vertexwk[3].diffuse = WHITE(AttackRange_Alpha);

	// �e�N�X�`�����W�̐ݒ�
	SetARTexture(AttackRangePtr);

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetARVertex(ATTACKRANGE *AttackRangePtr)
{
	AttackRangePtr->vertexwk[0].vtx.x = AttackRangePtr->pos.x - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[0].vtx.y = AttackRangePtr->pos.y - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[1].vtx.x = AttackRangePtr->pos.x + AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[1].vtx.y = AttackRangePtr->pos.y - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[2].vtx.x = AttackRangePtr->pos.x - AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[2].vtx.y = AttackRangePtr->pos.y + AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[3].vtx.x = AttackRangePtr->pos.x + AttackRangePtr->Radius;
	AttackRangePtr->vertexwk[3].vtx.y = AttackRangePtr->pos.y + AttackRangePtr->Radius;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetARTexture(ATTACKRANGE *AttackRangePtr)
{
	AttackRangePtr->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	AttackRangePtr->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	AttackRangePtr->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	AttackRangePtr->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


//=============================================================================
// �L�����N�^�[�̓�����͈͂̐ݒu
//=============================================================================
void SetCharacterAR(D3DXVECTOR3 pos, float HitRadius, int Owner)
{
	int AttackRange_No = 0;

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == false)
		{
			AttackRange[AttackRange_No].pos.x = pos.x;
			AttackRange[AttackRange_No].pos.y = pos.y;
			AttackRange[AttackRange_No].Radius = HitRadius;
			AttackRange[AttackRange_No].Owner = Owner;
			AttackRange[AttackRange_No].Use = true;
			break;
		}
	}

	return;
}

//=============================================================================
// �e�̓�����͈͂̐ݒu
//=============================================================================
void SetBulletAR(D3DXVECTOR3 pos, float HitRadius, int Bullet_No, int Owner)
{
	int AttackRange_No = 0;

	for (AttackRange_No = 0; AttackRange_No < AttackRange_Max; AttackRange_No++)
	{
		if (AttackRange[AttackRange_No].Use == false)
		{
			AttackRange[AttackRange_No].pos.x = pos.x;
			AttackRange[AttackRange_No].pos.y = pos.y;
			AttackRange[AttackRange_No].Radius = HitRadius;
			AttackRange[AttackRange_No].Owner = Owner;
			AttackRange[AttackRange_No].BulletNo = Bullet_No;
			AttackRange[AttackRange_No].Use = true;
			break;
		}
	}

	return;
}
