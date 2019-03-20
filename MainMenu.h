//=============================================================================
//
// ���C�����j���[�w�b�_�[ [MainMenu.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _MAINMENU_H_
#define _MAINMENU_H_


//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�[���I�[�o�[�I�������
enum MainMenuSelectState
{
	IteStage,		// ���č�
	FutagoStage,	// �ӂ�����
	OushiStage,		// ��������
	ShishiStage,	// ������
	StageCheck,		// �X�e�[�W�m�F���
};

enum StageClearFlag
{
	ClearFlag_Ite,
	ClearFlag_Futago,
	ClearFlag_Oushi,
	ClearFlag_Shishi,
	ClearFlag_Max,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitMainMenu(bool FirstInit);
// �I������
void UninitMainMenu(void);
// �X�V����
void UpdateMainMenu(void);
// �`�揈��
void DrawMainMenu(void);
// �I�������X�e�[�W���擾����
int GetSelectStage(void);
// �X�e�[�W�N���A�t���O��ݒu
void SetClearFlag(int StageClearFlag);

#endif
