//=============================================================================
//
// �X�e�[�W�J�ڃw�b�_�[ [Transition.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M��
//
//=============================================================================
#ifndef _TRANSITION_H_
#define _TRANSITION_H_


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitTransition(bool FirstInit);
// �I������
void UninitTransition(void);
// �X�V����
void UpdateTransition(void);
// �`�揈��
void DrawTransition(void);
// �t�F�[�h�̏�Ԑݒ�
void SetTransition(int State);
// �t�F�[�h�̏�Ԏ擾
int GetTransition();

#endif
