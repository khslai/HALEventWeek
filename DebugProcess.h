//=============================================================================
//
// �f�o�b�O�\���w�b�_�[ [DebugProcess.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _DEBUGPROCESS_H_
#define _DEBUGPROCESS_H_

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitDebugProcess(bool FirstInit);
// �I������
void UninitDebugProcess(void);
// �X�V����
void UpdateDebugProcess(void);
// �`�揈��
void DrawDebugProcess(void);
// �f�o�b�O���b�Z�[�W�\��
void PrintDebugMsg(const char *fmt,...);

#endif