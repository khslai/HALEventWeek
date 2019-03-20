//=============================================================================
//
// �{�X�e������ [BossBarrage.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "BossBarrage.h"
#include "Player.h"
#include "Boss.h"
#include "AttackRange.h"
#include "Sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �o���b�g1
#define Texture_BossBullet_1				_T("data/Texture/Bullet/BossBullet_1.png")
#define Texture_BossBullet_1_Width_Total	(192)
#define Texture_BossBullet_1_Height_Total	(32)
#define Texture_BossBullet_1_Divide_X		(6)		// �e�N�X�`�����������i��)
#define Texture_BossBullet_1_Divide_Y		(1)		// �e�N�X�`�����������i�c)
#define Texture_BossBullet_1_Width			(Texture_BossBullet_1_Width_Total / Texture_BossBullet_1_Divide_X)
#define Texture_BossBullet_1_Height			(Texture_BossBullet_1_Height_Total / Texture_BossBullet_1_Divide_Y)
// �o���b�g2
#define Texture_BossBullet_2				_T("data/Texture/Bullet/BossBullet_2.png")
#define Texture_BossBullet_2_Width			(32)
#define Texture_BossBullet_2_Height			(32)
// �o���b�g3
#define Texture_BossBullet_3				_T("data/Texture/Bullet/BossBullet_3.png")
#define Texture_BossBullet_3_Width_Total	(256)
#define Texture_BossBullet_3_Height_Total	(32)
#define Texture_BossBullet_3_Divide_X		(8)		// �e�N�X�`�����������i��)
#define Texture_BossBullet_3_Divide_Y		(1)		// �e�N�X�`�����������i�c)
#define Texture_BossBullet_3_Width			(Texture_BossBullet_3_Width_Total / Texture_BossBullet_3_Divide_X)
#define Texture_BossBullet_3_Height			(Texture_BossBullet_3_Height_Total / Texture_BossBullet_3_Divide_Y)

// �e�����[�h1�i�~�`�e���j��񔭎˂���e�̍ő吔
#define Mode1_Bullet_Max					(30)
// �e�����[�h2�i�~�V���O�W���܁j��񔭎˂���e�̍ő吔
#define Mode2_Bullet_Max					(30)

// �e�̏��
enum
{
	StopRotate,			// ��]��~
	InRotate,			// ��]��
	ClockWise,			// ��]�����͉E���
	Counterclockwise,	// ��]�����͍����
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
static HRESULT MakeBulletVertex(BULLET *Bullet);
// ���_���W�̐ݒ�
static void SetBulletVertex(BULLET *Bullet);
// �e�N�X�`�����W�̐ݒ�
static void SetBulletTexture(BULLET *Bullet, int BulletType);
// �{�X�e���ݒu
void BossShot(int BarrageType);
// �{�X�̒e�̈ړ�
static void BulletMove(BULLET *Bullet, int BulletType);
// �񎟌��x�N�g����](x,y���W����)
D3DXVECTOR3 VectorRot(D3DXVECTOR3 Vec, float RotAngle);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
static LPDIRECT3DTEXTURE9 BossBullet_Texture[BarrageMax] = { NULL };
// �摜���S���璸�_�܂ł̊p�x
float BaseAngle[BarrageMax] = { 0.0f };
// �摜���S���璸�_�܂ł̋���
float Radius[BarrageMax] = { 0.0f };


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBossBarrage(bool FirstInit)
{
	int Bullet_No = 0;
	BOSS *Boss = GetBoss();

	for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
	{
		Boss->Bullet[Bullet_No].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].PreviousPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Boss->Bullet[Bullet_No].Radius = 0.0f;
		Boss->Bullet[Bullet_No].HitRadius = 0.0f;
		Boss->Bullet[Bullet_No].BaseAngle = 0.0f;
		Boss->Bullet[Bullet_No].Speed = 0.0f;
		Boss->Bullet[Bullet_No].Count = 0;
		Boss->Bullet[Bullet_No].Color = 0;
		Boss->Bullet[Bullet_No].State = 0;
		Boss->Bullet[Bullet_No].Use = false;

		// ���_���̍쐬
		MakeBulletVertex(&Boss->Bullet[Bullet_No]);
	}

	if (FirstInit == true)
	{
		Radius[BarrageMode1] = D3DXVec2Length(&D3DXVECTOR2(Texture_BossBullet_1_Width / 2, Texture_BossBullet_1_Height / 2));
		BaseAngle[BarrageMode1] = atan2f(Texture_BossBullet_1_Height, Texture_BossBullet_1_Width);

		Radius[BarrageMode2] = D3DXVec2Length(&D3DXVECTOR2(Texture_BossBullet_2_Width / 2, Texture_BossBullet_2_Height / 2));
		BaseAngle[BarrageMode2] = atan2f(Texture_BossBullet_2_Height, Texture_BossBullet_2_Width);

		Radius[BarrageMode3] = D3DXVec2Length(&D3DXVECTOR2(Texture_BossBullet_3_Width / 2, Texture_BossBullet_3_Height / 2));
		BaseAngle[BarrageMode3] = atan2f(Texture_BossBullet_3_Height, Texture_BossBullet_3_Width);

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_BossBullet_1, &BossBullet_Texture[BarrageMode1], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossBullet_2, &BossBullet_Texture[BarrageMode2], "Bullet") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BossBullet_3, &BossBullet_Texture[BarrageMode3], "Bullet") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBossBarrage(void)
{
	for (int i = 0; i < BarrageMax; i++)
	{
		SafeRelease(BossBullet_Texture[i]);
	}

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBossBarrage(void)
{
	int Bullet_No = 0;
	int GameStage = GetGameStage();
	BOSS *Boss = GetBoss();

	if (Boss->Exist == true)
	{
		if (Boss->State == Shot)
		{
			switch (Boss->BarrageType)
			{
				// �~�`�e��
			case BarrageMode1:
				BossShot(BarrageMode1);
				break;
				// �~�V���O�W����
			case BarrageMode2:
				BossShot(BarrageMode2);
				break;
				// �p�[�t�F�N�g�t���[�Y
			case BarrageMode3:
				BossShot(BarrageMode3);
				break;
			default:
				break;
			}

			// ��ԃJ�E���g���Z
			Boss->StateCount++;
		}
		else if (Boss->State == Idle)
		{
			Boss->StateCount++;
			if (Boss->StateCount % 180 == 0)
			{
				Boss->StateCount = 0;
				Boss->State = Shot;
			}
		}

		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// �e�̈ړ�����
				BulletMove(&Boss->Bullet[Bullet_No], Boss->BarrageType);

				// ���_���W�X�V
				SetBulletVertex(&Boss->Bullet[Bullet_No]);
				// �e�N�X�`���X�V
				SetBulletTexture(&Boss->Bullet[Bullet_No], Boss->BarrageType);
			}
		}
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBossBarrage(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	BOSS *Boss = GetBoss();
	int Bullet_No = 0;

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (Boss->Exist == true)
	{
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// �e�N�X�`���̐ݒ�
				switch (Boss->BarrageType)
				{
				case BarrageMode1: Device->SetTexture(0, BossBullet_Texture[BarrageMode1]); break;
				case BarrageMode2: Device->SetTexture(0, BossBullet_Texture[BarrageMode2]); break;
				case BarrageMode3: Device->SetTexture(0, BossBullet_Texture[BarrageMode3]); break;
				default:
					break;
				}

				// �|���S���̕`��
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Boss->Bullet[Bullet_No].vertexwk, sizeof(VERTEX_2D));
			}
		}
	}

	return;
}


//=============================================================================
// �{�X���e�𔭎�
//=============================================================================
void BossShot(int BarrageType)
{
	int i = 0;
	int Bullet_No = 0;
	// �~�V���O�W���܂̒e�̉�]�p�x
	float RotateAngle = 0.0f;
	// ���[�v�����
	static int LoopNum = 0;
	BOSS *Boss = GetBoss();
	int Count = Boss->StateCount;

	switch (BarrageType)
	{
	case BarrageMode1:
		// 60�J�E���g���e����
		if (Count % 60 == 0)
		{
			// 360�J�E���g��A���˒�~
			if (Count >= 360)
			{
				LoopNum = 0;
				Boss->State = Idle;
				Boss->StateCount = 0;
				break;
			}

			// ���[�v�񐔉��Z
			LoopNum++;

			// ���ʉ�
			SetSound(SE_BossShot_01, E_DS8_FLAG_NONE, true, false);

			// �e����
			for (i = 0; i < Mode1_Bullet_Max; i++)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					if (Boss->Bullet[Bullet_No].Use == false)
					{
						// �e�̍ď���������
						Boss->Bullet[Bullet_No].Use = true;
						// ���W�ݒu
						Boss->Bullet[Bullet_No].pos = Boss->pos;
						Boss->Bullet[Bullet_No].BaseAngle = BaseAngle[BarrageMode1];
						Boss->Bullet[Bullet_No].Radius = Radius[BarrageMode1];
						// �����蔼�a�ݒu
						Boss->Bullet[Bullet_No].HitRadius = Radius[BarrageMode1] * 0.5f;
						// �e�̐i�s�����ݒu
						// ����̃��[�v��12�x�̊p�x��������
						Boss->Bullet[Bullet_No].MoveDirection =
							VectorRot(UpVec, D3DXToRadian(360.0f / Mode1_Bullet_Max * i + LoopNum * 15.0f));
						Boss->Bullet[Bullet_No].Count = 0;
						Boss->Bullet[Bullet_No].Color = 0;
						Boss->Bullet[Bullet_No].State = StopRotate;
						Boss->Bullet[Bullet_No].Speed = 5.0f;

#if _DEBUG
						// ������͈͕\��
						//SetBulletAR(Boss->Bullet[Bullet_No].pos, Boss->Bullet[Bullet_No].HitRadius, Bullet_No, BossBulletAR);
#endif

						break;
					}
				}
			}
		}
		break;
	case BarrageMode2:
		// �~�V���O�W����
		// 90�J�E���g���e����
		if (Count % 90 == 0)
		{
			// ���[�v�񐔉��Z
			LoopNum++;

			// ���ʉ�
			SetSound(SE_BossShot_02, E_DS8_FLAG_NONE, true, false);

			// �e����
			for (i = 0; i < Mode2_Bullet_Max; i++)
			{
				for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
				{
					// �g�p����Ă��Ȃ��e�������
					if (Boss->Bullet[Bullet_No].Use == false)
					{
						// �e�̍ď���������
						Boss->Bullet[Bullet_No].Use = true;
						// ���W�ݒu
						Boss->Bullet[Bullet_No].pos = Boss->pos;
						Boss->Bullet[Bullet_No].BaseAngle = BaseAngle[BarrageMode2];
						Boss->Bullet[Bullet_No].Radius = Radius[BarrageMode2] * 2.5f;
						// �����蔼�a�ݒu
						Boss->Bullet[Bullet_No].HitRadius = Radius[BarrageMode2] * 0.5f;
						// �e�̐i�s�����ݒu
						Boss->Bullet[Bullet_No].MoveDirection =
							VectorRot(UpVec, D3DXToRadian(360.0f / Mode2_Bullet_Max * i));
						Boss->Bullet[Bullet_No].rot.z = D3DXToRadian(0.0f);
						Boss->Bullet[Bullet_No].Color = 0;
						Boss->Bullet[Bullet_No].Count = 0;
						Boss->Bullet[Bullet_No].Speed = 6.0f;
						// �X�e�[�^�X�B0��1���ŉ�]�������ς��
						if (LoopNum % 2 == 0)
						{
							Boss->Bullet[Bullet_No].State = ClockWise;
						}
						else
						{
							Boss->Bullet[Bullet_No].State = Counterclockwise;
						}

#if _DEBUG
						// ������͈͕\��
						//SetBulletAR(Boss->Bullet[Bullet_No].pos, Boss->Bullet[Bullet_No].HitRadius, Bullet_No, BossBulletAR);
#endif

						break;
					}
				}
			}
		}

		// �e�̉�]�v�Z
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				// 30�`120�J�E���g�Ȃ�
				if (Boss->Bullet[Bullet_No].Count > 30 && Boss->Bullet[Bullet_No].Count < 120)
				{
					// 90�J�E���g������1.2���炷
					Boss->Bullet[Bullet_No].Speed -= 1.2f / 90.0f;
					// 90�J�E���g������90���X����
					RotateAngle = 1.0f * (Boss->Bullet[Bullet_No].State == ClockWise ? 1 : -1);
					Boss->Bullet[Bullet_No].MoveDirection =
						VectorRot(Boss->Bullet[Bullet_No].MoveDirection, D3DXToRadian(RotateAngle));
					Boss->Bullet[Bullet_No].rot.z += D3DXToRadian(RotateAngle);
				}
			}
		}
		break;
	case BarrageMode3:
		// �p�[�t�F�N�g�t���[�Y
		// �ŏ��̃����_������
		if (Count < 240)
		{
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == false)
				{
					// ���ʉ�
					SetSound(SE_BossShot_03, E_DS8_FLAG_NONE, true, false);

					Boss->Bullet[Bullet_No].Use = true;
					// ���W�ݒu
					Boss->Bullet[Bullet_No].pos = Boss->pos;
					Boss->Bullet[Bullet_No].BaseAngle = BaseAngle[BarrageMode3];
					Boss->Bullet[Bullet_No].Radius = Radius[BarrageMode3] * 4.0f;
					// �����蔼�a�ݒu
					Boss->Bullet[Bullet_No].HitRadius = Radius[BarrageMode3] * 0.8f;
					// �e�̐i�s�����ݒu
					Boss->Bullet[Bullet_No].MoveDirection =
						VectorRot(UpVec, D3DXToRadian(360.0f * (float)rand() / RAND_MAX));
					Boss->Bullet[Bullet_No].rot.z = D3DXToRadian(0.0f);
					Boss->Bullet[Bullet_No].Color = rand() % (6 + 1);
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].State = InRotate;
					Boss->Bullet[Bullet_No].Speed = 5.0f + (3.0f * (float)rand() / RAND_MAX);

#if _DEBUG
					// ������͈͕\��
					//SetBulletAR(Boss->Bullet[Bullet_No].pos, Boss->Bullet[Bullet_No].HitRadius, Bullet_No, BossBulletAR);
#endif

					break;
				}
			}
		}
		// 240�J�E���g�̎��ɑS�ăX�g�b�v�����A�������A�J�E���g���Z�b�g
		else if (Count == 240)
		{
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					// ���ʉ�
					SetSound(SE_BulletFreeze, E_DS8_FLAG_NONE, true, false);
					// ���F
					Boss->Bullet[Bullet_No].Color = 7;
					Boss->Bullet[Bullet_No].Count = 0;
					Boss->Bullet[Bullet_No].State = StopRotate;
					Boss->Bullet[Bullet_No].Speed = 0.0f;
				}
			}
		}
		// �t���[�Y�I���A�ړ��J�n
		else if (Count >= 330 && Count < 600)
		{
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					// �����_���ȕ����Ɉړ����n�߂�
					if (Boss->Bullet[Bullet_No].Count == 150)
					{
						// �S���������_��
						Boss->Bullet[Bullet_No].MoveDirection.x = (float)(rand() % (100 + 1) + (-50));
						Boss->Bullet[Bullet_No].MoveDirection.y = (float)(rand() % (100 + 1) + (-50));
						D3DXVec3Normalize(&Boss->Bullet[Bullet_No].MoveDirection, &Boss->Bullet[Bullet_No].MoveDirection);
						Boss->Bullet[Bullet_No].State = InRotate;
					}
					if (Boss->Bullet[Bullet_No].Count > 150)
					{
						// �i�X����
						Boss->Bullet[Bullet_No].Speed += 0.03f;
					}
				}
			}
		}
		else if (Count >= 600)
		{
			Boss->StateCount = 0;
		}
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// �{�X�̒e�̈ړ�
//=============================================================================
void BulletMove(BULLET *Bullet, int BarrageType)
{
	BOSS *Boss = GetBoss();

	// 1�t���C���O�̒e���W���L�^
	Bullet->PreviousPos = Bullet->pos;

	// ���W�X�V
	Bullet->pos += Bullet->MoveDirection * Bullet->Speed;

	// �e�̈ړ��������
	switch (BarrageType)
	{
	case BarrageMode1:
		if (CheckPosition(Bullet->pos, Texture_BossBullet_1_Width, Texture_BossBullet_1_Height) == true)
		{
			Bullet->Use = false;
		}
		break;
	case BarrageMode2:
		if (CheckPosition(Bullet->pos, Texture_BossBullet_2_Width, Texture_BossBullet_2_Height) == true)
		{
			Bullet->Use = false;
		}
		break;
	case BarrageMode3:
		if (CheckPosition(Bullet->pos, Texture_BossBullet_3_Width, Texture_BossBullet_3_Height) == true)
		{
			Bullet->Use = false;
		}
		break;
	default:
		break;
	}

	// ����^�C�v�̒e��������A��]����
	if (Bullet->State == InRotate)
	{
		Bullet->rot.z += 0.1f;
	}

	// �J�E���g���Z
	Bullet->Count++;

	return;
}

//=============================================================================
// �񎟌��x�N�g����](x,y���W����)
//=============================================================================
D3DXVECTOR3 VectorRot(D3DXVECTOR3 Vec, float RotAngle)
{
	D3DXVECTOR3 AfterRotVec3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// (x0,y0)�Ɗp����]��(x1,y1)
	// x1 = x0cos�� - y0sin��
	// y1 = x0sin�� + y0cos��
	AfterRotVec3.x = Vec.x * cosf(RotAngle) - Vec.y * sinf(RotAngle);
	AfterRotVec3.y = Vec.x * sinf(RotAngle) + Vec.y * cosf(RotAngle);
	D3DXVec3Normalize(&AfterRotVec3, &AfterRotVec3);

	return AfterRotVec3;
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
	SetBulletTexture(Bullet, 0);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetBulletTexture(BULLET *Bullet, int BarrageType)
{
	int x = Bullet->Color;
	float sizeX = 0.0f;

	switch (BarrageType)
	{
	case BarrageMode1:
		if (sizeX == 0.0f)
		{
			sizeX = 1.0f / Texture_BossBullet_1_Divide_X;
		}
	case BarrageMode3:
		if (sizeX == 0.0f)
		{
			sizeX = 1.0f / Texture_BossBullet_3_Divide_X;
		}
		Bullet->vertexwk[0].tex = D3DXVECTOR2((float)x * sizeX, 0.0f);
		Bullet->vertexwk[1].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 0.0f);
		Bullet->vertexwk[2].tex = D3DXVECTOR2((float)x * sizeX, 1.0f);
		Bullet->vertexwk[3].tex = D3DXVECTOR2((float)x * sizeX + sizeX, 1.0f);
		break;
	case BarrageMode2:
		Bullet->vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		Bullet->vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		Bullet->vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		Bullet->vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		break;
	default:
		break;
	}


	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetBulletVertex(BULLET *Bullet)
{
	BOSS *Boss = GetBoss();

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