//=============================================================================
//
// �v���C���[�w�b�_�[ [Player.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "PlayerBullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �O���[�o���}�N����`
// �v���C���[�̍ő�l��
#define Player_Max			(2)
// �v���C���[�̒e���ˊԊu
#define ShotInterval		(90)
// �v���C���[�̎a���Ԋu
#define SlashInterval		(30)
// �_�b�V���̎���
#define DashTime			(90)
// �v���C���[�̃_�b�V���Ԋu
#define DashCDInterval		(1800)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ������|�C���g�\����
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];		// ���_���i�[���[�N
	float				Radius;						// �摜���S���璸�_�܂ł̋���
	float				BaseAngle;					// �摜���S���璸�_�܂ł̊p�x
}HITPOINT;

// �v���C���[�\����
typedef	struct
{
	VERTEX_2D			vertexwk[Num_Vertex];		// ���_���i�[���[�N
	LPDIRECT3DTEXTURE9	texture;					// �e�N�X�`��
	D3DXVECTOR3			pos;						// �v���C���[�̍��W
	D3DXVECTOR3			PreviousPos;				// 1�t���C���O�̍��W
	BULLET				Bullet[PlayerBullet_Max];	// �v���C���[�̒e
	HITPOINT			HitPoint;					// ������|�C���g
	int					Direction;					// ����
	int					Count;						// �v���C���[�̃A�j���J�E���g
	int					ShotCount;					// �v���C���[�̃V���b�g�J�E���g
	int					DashCDCount;				// �_�b�V���̃N�[���_�E���J�E���g
	int					InvincibleCount;			// ���G�J�E���g
	int					AnimPattern;				// �v���C���[�̃A�j���p�^�[��
	int					dirBlock;					// ��Q���ɂԂ��������A�v���C���[�̌���
	float				Speed;						// �ړ����x
	float				HP;							// �����l
	float				HP_Max;						// �ő吶���l
	float				Radius;						// �摜���S���璸�_�܂ł̋���
	float				HitRadius;					// �����蔻��̉~�̔��a
	float				BaseAngle;					// �摜���S���璸�_�܂ł̊p�x
	float				RegeneBonus;				// �o�q���̃p���[�A�b�v�{�[�i�X
	float				SlowSpeed;					// �ᑬ���[�h�̈ړ����x
	bool				InDash;						// �_�b�V���t���O
	bool				InFlash;					// ���G��Ԃ̃t���b�V��
	bool				InSlash;					// �a��
	bool				InMove;						// �ړ���
	bool				HitBlock;					// ��Q���ɂԂ���t���O
	bool				Exist;						// �g�p�t���O
	bool				flagOnIce;					// �v���C���[���X�̏��̏�ɂ��邩�ǂ����𔻒f����t���O
	bool				flagSlide;					// �v���C���[���X�̏��̏�Ɋ����Ă��邩�ǂ����𔻒f����t���O
	bool				Use;						// �g�p�t���O
}PLAYER;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitPlayer(bool FirstInit);
// �I������
void UninitPlayer(void);
// �X�V����
void UpdatePlayer(void);
// �`�揈��
void DrawPlayer(void);
// �v���C���[�̏����擾����
PLAYER *GetPlayer(int Player_No);
// �v���C���[�̃e�N�X�`���̃T�C�Y���擾����
D3DXVECTOR2 GetPlayerSize(void);

#endif
