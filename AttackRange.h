//=============================================================================
//
// ������͈͕\���w�b�_�[ [AttackRange.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _ATTACKRANGE_H_
#define _ATTACKRANGE_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// ������͈͂̎��
enum AttackRangeType
{
	PlayerAR_A,
	PlayerBulletAR_A,
	PlayerAR_B,
	PlayerBulletAR_B,
	BossAR,
	BossBulletAR,
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ������͈͍\����
typedef struct 
{
	VERTEX_2D		vertexwk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3		pos;					// ���W
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	int				Owner;					// ���L��
	int				BulletNo;				// �Ή�����o���b�g�ԍ�
	int				BarrageNo;				// �Ή�����e���ԍ�
	bool			Use;					// �g�p�t���O
}ATTACKRANGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitAttackRange(bool FirstInit);
// �I������
void UninitAttackRange(void);
// �X�V����
void UpdateAttackRange(void);
// �`�揈��
void DrawAttackRange(void);
// �L�����N�^�[�̓�����͈͂̐ݒu
void SetCharacterAR(D3DXVECTOR3 Pos, float HitRadius, int Owner);
// �e�̓�����͈͂̐ݒu
void SetBulletAR(D3DXVECTOR3 Pos, float HitRadius, int Bullet_No, int Owner);

#endif
