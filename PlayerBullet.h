//=============================================================================
//
// �v���C���[�e���w�b�_�[ [PlayerBullet.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �O���[�o���}�N����`
// �v���C���[�̒e�̍ő�l
#define PlayerBullet_Max	(30)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �e�\����
typedef struct
{
	VERTEX_2D		vertexwk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3		pos;					// �e�̍��W
	D3DXVECTOR3		PreviousPos;			// 1�t���C���O�̒e�̍��W
	D3DXVECTOR3		rot;					// �e�N�X�`���̉�]�p�x
	D3DXVECTOR3		MoveDirection;			// �ړ�������ύX����p�x
	float			Radius;					// �摜���S���璸�_�܂ł̋���
	float			HitRadius;				// �����蔻��̉~�̔��a
	float			BaseAngle;				// �摜���S���璸�_�܂ł̊p�x
	float			Speed;					// �e�̃X�s�[�h
	int				Count;					// �J�E���g
	int				Color;					// �F
	int				State;					// �e�̏��
	bool			Use;					// ���˂��ꂽ���̃t���O
	bool			DelayDisappear;			// �������x��邩�̃t���O
}BULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitPlayerBullet(bool FirstInit);
// �I������
void UninitPlayerBullet(void);
// �X�V����
void UpdatePlayerBullet(void);
// �`�揈��
void DrawPlayerBullet(void);
// �v���C���[�̒e����
void PlayerShot(int Player_No);

#endif
