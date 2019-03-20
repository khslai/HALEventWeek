//=============================================================================
//
// ���������w�b�_�[ [Bull.h]
// Author�F 
//
//=============================================================================
#ifndef _BULL_H_
#define _BULL_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �����̍ő吔		
#define Bull_Max	(2) 


//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct
{
	VERTEX_2D				vertexwk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// ���W
	D3DXVECTOR3				f_pos;					// �G�ƃv���B���[�̍��W�v�Z
	int						AnimCount;
	int						AnimPattern;
	int						part = 0;				// �G�̏��
	float					move_x = 0.0f;			// �w�����̈ړ����x
	float					move_y = 0.0f;			// �x�����̈ړ����x
	float					dealttime = 0.0f;		// ���Ԍv�Z
	float					move_v0 = 10.0f;		// �n�߂̑��x
	float					move_dash_x = 0.0f;
	float					move_dash_y = 0.0f;
	bool					Use;
	bool					posX;					// �v���B���[�̌������@true�͉E�@false�͍�
}BULL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBull(bool FirstInit);
void UninitBull(void);
void UpdateBull(void);
void DrawBull(void);
// �����擾�֐�
BULL *GetBull(int Bull_No);
// �����̃e�N�X�`���̃T�C�Y���擾����
D3DXVECTOR2 GetBullSize(void);


#endif
