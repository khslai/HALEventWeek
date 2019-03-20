//=============================================================================
//
// �G�t�F�N�g�w�b�_�[ [Effect.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �O���[�o���}�N����`
// �G�t�F�N�g�̎��
enum EffectType
{
	Regenerate,			// ���W�F�l�iHP�Đ��j
	BulletDead,			// �{�X�̒e�̏���
	DeadEffect,			// �{�X�̎��S�G�t�F�N�g
	Slash,				// �a��
	EffectType_Max,
};


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �G�t�F�N�g�\����
typedef struct
{
	VERTEX_2D		vertexwk[Num_Vertex];		// ���_���i�[���[�N
	D3DXVECTOR3		pos;						// ���W
	D3DXVECTOR3		rot;						// �e�N�X�`���̉�]�p�x
	float			Radius;						// �摜���S���璸�_�܂ł̋���
	float			BaseAngle;					// �摜���S���璸�_�܂ł̊p�x
	int				Count;						// �J�E���g
	int				AnimPattern;				// �e�N�X�`���̉��ԕ���
	int				Alpha;						// ���Z�����A���������p�A���t�@�l
	int				Type;						// �G�t�F�N�g�̎��
	int				Owner;						// �G�t�F�N�g�̏��L��
	bool			Use;						// �g�p�t���O
}EFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitEffect(bool FirstInit);
// �I������
void UninitEffect(void);
// �X�V����
void UpdateEffect(void);
// �`�揈��
void DrawEffect(void);
// �G�t�F�N�g�̐ݒu
void SetEffect(int Owner, int EffectType, D3DXVECTOR3 Pos);

#endif
