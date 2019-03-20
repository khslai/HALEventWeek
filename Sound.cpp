//=============================================================================
//
// �T�E���h���� [Sound.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M��
//
//=============================================================================
#include "main.h"
#include "Sound.h"
#include "Title.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �T�E���h�t�F�C�h�A�E�g�̑��x
#define VolumeDecreaseSpeed (20)

// �T�E���h�t�@�C���̃p�X
const TCHAR* SoundFileName[] =
{
	_T("data/Music/BGM/Futago.wav"),
	_T("data/Music/BGM/GameEnd.wav"),
	_T("data/Music/BGM/Ite.wav"),
	_T("data/Music/BGM/MainMenu.wav"),
	_T("data/Music/BGM/Oushi.wav"),
	_T("data/Music/BGM/Shishi.wav"),
	_T("data/Music/BGM/Title.wav"),
	_T("data/Music/SE/BossShot_01.wav"),
	_T("data/Music/SE/BossShot_02.wav"),
	_T("data/Music/SE/BossShot_03.wav"),
	_T("data/Music/SE/BulletFreeze.wav"),
	_T("data/Music/SE/Cancel.wav"),
	_T("data/Music/SE/Determine.wav"),
	_T("data/Music/SE/HitApple.wav"),
	_T("data/Music/SE/HitBlock.wav"),
	_T("data/Music/SE/HitBull.wav"),
	_T("data/Music/SE/HPRestore.wav"),
	_T("data/Music/SE/PlayerBeShot.wav"),
	_T("data/Music/SE/Scream_PlayerA.wav"),
	_T("data/Music/SE/Scream_PlayerB.wav"),
	_T("data/Music/SE/SelectMove.wav"),
	_T("data/Music/SE/ShotArrow.wav"),
	_T("data/Music/SE/Slash.wav"),
	_T("data/Music/SE/SpeedDown.wav"),
	_T("data/Music/SE/StageClear.wav"),
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �T�E���h�̃��[�h
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No);
// BGM�𗬂��Ă��邩�𒲍�
bool BGMPlaying(int Sound_No);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IDirectSound8			*DirectSound = NULL;				// �T�E���h�C���^�[�t�F�[�X
LPDIRECTSOUNDBUFFER8	SoundBuffer[Sound_Max] = { NULL };	// �T�E���h�p�o�b�t�@
int						PlayingBGM_No = 0;					// �����Ă���BGM�̔ԍ�
bool					SetIntroOver = false;				// �O�t��ݒu�����t���O

//=============================================================================
// ����������
//=============================================================================
// hWnd:�E�B���h�E�n���h��
HRESULT	InitSound(HWND hWnd)
{
	int Sound_No = 0;
	char Message[64];

	// DirectSound�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &DirectSound, NULL)))
	{
		return E_FAIL;
	}

	// �������x���ݒ� 
	if (FAILED(DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return E_FAIL;
	}

	// �T�E���h�̃��[�h
	for (Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No] = LoadSound(Sound_No);
		if (SoundBuffer[Sound_No] == NULL)
		{
			sprintf(Message, "Load Sound No.%02d Failed�I", Sound_No);
			MessageBox(0, Message, "Error", 0);
			return E_FAIL;
		}
	}

	// �J�n�ʒu�ɖ߂�
	for (Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound()
{
	// �T�E���h�o�b�t�@�̊J��
	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SafeRelease(SoundBuffer[Sound_No]);
	}

	// �T�E���h�̊J��
	SafeRelease(DirectSound);

	return;
}


//=============================================================================
// �T�E���h���X�V����
//=============================================================================
void UpdateSound(void)
{
	int GameStage = GetGameStage();
	static bool SetBGM = false;

	switch (GameStage)
	{
	case Stage_Title:
		if (GetShowLogo() == false)
		{
			SetSound(BGM_Title, E_DS8_FLAG_LOOP, false, true);
		}
		break;
	case Stage_MainMenu:
		SetSound(BGM_MainMenu, E_DS8_FLAG_LOOP, false, true);
		break;
	case Stage_Ite:
		SetSound(BGM_Ite, E_DS8_FLAG_LOOP, false, true);
		break;
	case Stage_Oushi:
		SetSound(BGM_Oushi, E_DS8_FLAG_LOOP, false, true);
		break;
	case Stage_Futago:
		SetSound(BGM_Futago, E_DS8_FLAG_LOOP, false, true);
		break;
	case Stage_Shishi:
		SetSound(BGM_Shishi, E_DS8_FLAG_LOOP, false, true);
		break;
	case Stage_GameEnd:
		SetSound(BGM_GameEnd, E_DS8_FLAG_LOOP, false, true);
		break;
	default:
		break;
	}


	return;
}

//=============================================================================
// �T�E���h�̃��[�h
//=============================================================================
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No)
{
	// Sound_No:�T�E���h�i���o�[�i�w�b�_�ɒ�`���ꂽ�񋓌^�萔�j
	// MMIO = �}���`���f�B�A���o�́A�̗��B
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;	// �ȃf�[�^�̑����o�b�t�@
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;		// �ȃf�[�^�̃o�b�t�@
	DSBUFFERDESC buff;							// �o�b�t�@�ݒ�\����

	HMMIO hMmio = NULL;							// MMIO�n���h��
	MMIOINFO mmioInfo;							// �}���`���f�B�A�f�[�^�\����

	MMRESULT mmRes;								// MM�������ʊi�[�ϐ�
	MMCKINFO riffChunk, formatChunk, dataChunk;	// �`�����N�f�[�^�\����
	DWORD size;									// �f�[�^�T�C�Y�i�[�p
	WAVEFORMATEX pcm;							// �ȃf�[�^�t�H�[�}�b�g�\����

	LPVOID pBlock;								// �Z�J���_���o�b�t�@�̃f�[�^�������ݐ�A�h���X
	DWORD  dwSize;								// �Z�J���_���o�b�t�@�̃T�C�Y

	// 1.�n���h�������ƂɃt�@�C�����J��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)SoundFileName[Sound_No], &mmioInfo, MMIO_READ);
	if (!hMmio)
	{
		return NULL;
	}

	// 2.�t�@�C����͇@ RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// �L�[���[�h�����ƂɌ���
	// ������Ȃ�������ُ�I��
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.�t�@�C����͇A �t�H�[�}�b�g�`�����N����
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	// ������Ȃ�������ُ�I��
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// �����������Ƃɓǂݍ���
	// �������ǂݍ��߂Ȃ�������ُ�I��
	if (size != formatChunk.cksize)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// 4.�t�@�C����� �f�[�^�`�����N����
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	// ������Ȃ�������ُ�I��
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.�f�[�^�ǂݍ���
	char *pData = new char[dataChunk.cksize];					// �K�v�ȑ傫���̗̈���m�ۂ���
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// �f�[�^��ǂݍ���
	// �������ǂݍ��߂Ȃ�������ُ�I��
	if (size != dataChunk.cksize)
	{
		SafeDeleteArray(pData);
		return NULL;
	}

	// 6.�Ȃ�ǂݍ��ށu�Z�J���_���o�b�t�@�v��p��
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// �܂�������
	buff.dwSize = sizeof(DSBUFFERDESC);				// ��������e��ݒ�
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// �����o�b�t�@�����
	if (FAILED(DirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
	{
		return NULL;
	}

	// �T�E���h�o�b�t�@�����o��
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
	{
		return NULL;
	}

	// ���o�����瑍���o�b�t�@���̂Ă�
	SafeRelease(pBaseBuffer);

	// 7.���ꂽ�Z�J���_���o�b�t�@�Ƀf�[�^��]��
	// �܂��͏������݂ł���悤�o�b�t�@�����b�N
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
	{
		return NULL;
	}

	memcpy(pBlock, pData, dwSize);	// �f�[�^�]��
	SafeDeleteArray(pData);			// ���̓ǂݍ��ݗ̈������

	// �o�b�t�@���b�N����
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// �Z�J���_���o�b�t�@��Ԃ��Ă悤�₭����...
	return pBuffer;
}

//=============================================================================
// BGM�A2D�T�E���h��ݒu����
//=============================================================================
void SetSound(int Sound_No, int Flag, bool FromStart, bool IsBGM)
{
	if (IsBGM == true)
	{
		PlayingBGM_No = Sound_No;
	}

	if (FromStart == true)
	{
		// ��������炷�̂ŁA�ŏ�����炵�����ꍇ��SetCurrentPosition(0)�����邱��
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}
	// Flag   :1(E_DS8_FLAG_LOOP)�Ȃ烋�[�v�Đ�
	SoundBuffer[Sound_No]->Play(0, 0, Flag);

	return;
}

//=============================================================================
// �T�E���h���~����
//=============================================================================
void StopSound(int Sound_No)
{
	DWORD status;

	SoundBuffer[Sound_No]->GetStatus(&status);
	// ���Ă�����
	if (status & DSBSTATUS_PLAYING)
	{
		// �Ӗ��I�ɂ�Pause�ɂȂ�B
		SoundBuffer[Sound_No]->Stop();
	}

	return;
}

//=============================================================================
// �Đ������ǂ������ׂ�
//=============================================================================
bool BGMPlaying(int Sound_No)
{
	DWORD status;

	SoundBuffer[Sound_No]->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING)
	{
		return true;
	}

	return false;
}

//=============================================================================
// �ď���������
//=============================================================================
void ReInitSound(void)
{
	// �{�����[��������
	LONG Volume = DSBVOLUME_MAX;

	// �t���O������
	SetIntroOver = false;

	// BGM�ԍ�������
	PlayingBGM_No = 0;

	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		// �����Ă�T�E���h���~
		StopSound(Sound_No);

		// �{���̃{�����[���ɖ߂�
		SoundBuffer[Sound_No]->SetVolume(Volume);

		// �v���C�ʒu���擪�ɖ߂�
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return;
}

//=============================================================================
// �T�E���h�t�F�C�h�A�E�g
//=============================================================================
void SoundFadeOut(int Sound_No)
{
	LONG Volume = 0;

	SoundBuffer[Sound_No]->GetVolume(&Volume);
	// �{�����[������������
	Volume -= VolumeDecreaseSpeed;

	// �{�����[���ݒ�
	SoundBuffer[Sound_No]->SetVolume(Volume);

	return;
}

//=============================================================================
// ���炵�Ă���BGM�̔ԍ����擾����
//=============================================================================
int GetPlayingBGM_No(void)
{
	return PlayingBGM_No;
}
