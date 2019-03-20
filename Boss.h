//=============================================================================
//
// �{�X�w�b�_�[ [Boss.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "PlayerBullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �O���[�o���}�N����`
// �{�X�̒e�̍ő�l
#define BossBullet_Max		(500)

// �{�X�̏��
enum BossState
{
	Idle,		// �ҋ@
	Shot,		// �V���b�g
	BossDead,	// ���S
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �{�X�\����
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3			pos;					// ���W
	LPDIRECT3DTEXTURE9	texture;				// �e�N�X�`��
	BULLET				Bullet[BossBullet_Max];	// �{�X�̒e
	float				Radius;					// �摜���S���璸�_�܂ł̋���
	float				HitRadius;				// �����蔻��̉~�̔��a
	float				BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	float				HP;						// �����l
	float				HP_Max;					// �ő吶���l
	int					AnimCount;				// �A�j�����Z�J�E���g
	int					StateCount;				// ��ԉ��Z�J�E���g
	int					AnimPattern;			// �A�j���p�^�[��
	int					State;					// �{�X�̏��
	int					BarrageType;			// �{�X�̒e���̎��
	bool				Exist;					// ���݂��ǂ����̃t���O
}BOSS;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitBoss(bool FirstInit);
// �I������
void UninitBoss(void);
// �X�V����
void UpdateBoss(void);
// �`�揈��
void DrawBoss(void);
// �{�X�̏����擾����
BOSS *GetBoss(void);
// �{�X�̐F�̐ݒ�
void SetBossColor(int Color);

#endif
