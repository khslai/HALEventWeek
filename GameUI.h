//=============================================================================
//
// �Q�[��UI�w�b�_�[ [GameUI.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �O���[�o���}�N����`
// UI�̎��
enum UIType
{
	UIType_ShotIcon_PlayerA,
	UIType_ShotCD_PlayerA,
	UIType_DashIcon_PlayerA,
	UIType_DashCD_PlayerA,
	UIType_ShotIcon_PlayerB,
	UIType_ShotCD_PlayerB,
	UIType_DashIcon_PlayerB,
	UIType_DashCD_PlayerB,
	UIType_HPGaugeBox_PlayerA,
	UIType_HPGauge_PlayerA,
	UIType_HPGaugeBox_PlayerB,
	UIType_HPGauge_PlayerB,
	UIType_HPGaugeBox_Boss,
	UIType_HPGauge_Boss,
	UIType_Illust_PlayerA,
	UIType_Illust_PlayerB,
	UIType_Max,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �Q�[��UI�\����
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3			pos;					// ���W
	int					Width;					// ����
	int					Height;					// �c��
	int					Alpha;					// �摜�̓����x
	int					Type;					// UI�̎��
	bool				Use;					// �g�p�t���O
}GAMEUI;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitGameUI(bool FirstInit);
// �I������
void UninitGameUI(void);
// �X�V����
void UpdateGameUI(void);
// �`�揈��
void DrawGameUI(void);
// �Q�[��UI�̏����擾����
GAMEUI *GetGameUI(int UIType);

#endif
