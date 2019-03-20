//=============================================================================
//
// �T�E���h�w�b�_�[ [Sound.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �T�E���h�ԍ�
enum Sound_No
{
	BGM_Futago,
	BGM_GameEnd,
	BGM_Ite,
	BGM_MainMenu,
	BGM_Oushi,
	BGM_Shishi,
	BGM_Title,
	SE_BossShot_01,
	SE_BossShot_02,
	SE_BossShot_03,
	SE_BulletFreeze,
	SE_Cancel,
	SE_Determine,
	SE_HitApple,
	SE_HitBlock,
	SE_HitBull,
	SE_HPRestore,
	SE_PlayerBeShot,
	SE_Scream_PlayerA,
	SE_Scream_PlayerB,
	SE_SelectMove,
	SE_ShotArrow,
	SE_Slash,
	SE_SpeedDown,
	SE_StageClear,
	Sound_Max,
};

// �Đ��p�t���O
enum
{
	E_DS8_FLAG_NONE,	// ��񂵂��v���C���Ȃ�
	E_DS8_FLAG_LOOP,	// ���[�v
	E_DS8_FLAG_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT	InitSound(HWND hWnd);
// �ď���������
void ReInitSound(void);
// �I������
void UninitSound(void);
// �X�V����
void UpdateSound(void);
// BGM�ASE��ݒu����
void SetSound(int Sound_No, int Flag, bool FromStart, bool IsBGM);
// �T�E���h���~����
void StopSound(int Sound_No);
// ���炵�Ă���BGM�̔ԍ����擾����
int GetPlayingBGM_No(void);
// �T�E���h�̃t�F�C�h�A�E�g
void SoundFadeOut(int Sound_No);

#endif