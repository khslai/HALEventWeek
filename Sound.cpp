//=============================================================================
//
// サウンド処理 [Sound.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興
//
//=============================================================================
#include "main.h"
#include "Sound.h"
#include "Title.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// サウンドフェイドアウトの速度
#define VolumeDecreaseSpeed (20)

// サウンドファイルのパス
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
// プロトタイプ宣言
//*****************************************************************************
// サウンドのロード
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No);
// BGMを流しているかを調査
bool BGMPlaying(int Sound_No);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IDirectSound8			*DirectSound = NULL;				// サウンドインターフェース
LPDIRECTSOUNDBUFFER8	SoundBuffer[Sound_Max] = { NULL };	// サウンド用バッファ
int						PlayingBGM_No = 0;					// 流しているBGMの番号
bool					SetIntroOver = false;				// 前奏を設置したフラグ

//=============================================================================
// 初期化処理
//=============================================================================
// hWnd:ウィンドウハンドル
HRESULT	InitSound(HWND hWnd)
{
	int Sound_No = 0;
	char Message[64];

	// DirectSoundオブジェクトの作成
	if (FAILED(DirectSoundCreate8(NULL, &DirectSound, NULL)))
	{
		return E_FAIL;
	}

	// 協調レベル設定 
	if (FAILED(DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return E_FAIL;
	}

	// サウンドのロード
	for (Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No] = LoadSound(Sound_No);
		if (SoundBuffer[Sound_No] == NULL)
		{
			sprintf(Message, "Load Sound No.%02d Failed！", Sound_No);
			MessageBox(0, Message, "Error", 0);
			return E_FAIL;
		}
	}

	// 開始位置に戻る
	for (Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound()
{
	// サウンドバッファの開放
	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		SafeRelease(SoundBuffer[Sound_No]);
	}

	// サウンドの開放
	SafeRelease(DirectSound);

	return;
}


//=============================================================================
// サウンドを更新する
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
// サウンドのロード
//=============================================================================
LPDIRECTSOUNDBUFFER8 LoadSound(int Sound_No)
{
	// Sound_No:サウンドナンバー（ヘッダに定義された列挙型定数）
	// MMIO = マルチメディア入出力、の略。
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;	// 曲データの総合バッファ
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;		// 曲データのバッファ
	DSBUFFERDESC buff;							// バッファ設定構造体

	HMMIO hMmio = NULL;							// MMIOハンドル
	MMIOINFO mmioInfo;							// マルチメディアデータ構造体

	MMRESULT mmRes;								// MM処理結果格納変数
	MMCKINFO riffChunk, formatChunk, dataChunk;	// チャンクデータ構造体
	DWORD size;									// データサイズ格納用
	WAVEFORMATEX pcm;							// 曲データフォーマット構造体

	LPVOID pBlock;								// セカンダリバッファのデータ書き込み先アドレス
	DWORD  dwSize;								// セカンダリバッファのサイズ

	// 1.ハンドルをもとにファイルを開く
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)SoundFileName[Sound_No], &mmioInfo, MMIO_READ);
	if (!hMmio)
	{
		return NULL;
	}

	// 2.ファイル解析① RIFFチャンク検索
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// 検索キーワード
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// キーワードをもとに検索
	// 見つからなかったら異常終了
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.ファイル解析② フォーマットチャンク検索
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	// 見つからなかったら異常終了
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// 検索情報をもとに読み込み
	// 正しく読み込めなかったら異常終了
	if (size != formatChunk.cksize)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// 4.ファイル解析 データチャンク検索
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// 検索キーワード
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	// 見つからなかったら異常終了
	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.データ読み込み
	char *pData = new char[dataChunk.cksize];					// 必要な大きさの領域を確保して
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// データを読み込む
	// 正しく読み込めなかったら異常終了
	if (size != dataChunk.cksize)
	{
		SafeDeleteArray(pData);
		return NULL;
	}

	// 6.曲を読み込む「セカンダリバッファ」を用意
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// まず初期化
	buff.dwSize = sizeof(DSBUFFERDESC);				// そこから各種設定
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// 総合バッファを作る
	if (FAILED(DirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
	{
		return NULL;
	}

	// サウンドバッファを取り出す
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
	{
		return NULL;
	}

	// 取り出したら総合バッファを捨てる
	SafeRelease(pBaseBuffer);

	// 7.作られたセカンダリバッファにデータを転送
	// まずは書き込みできるようバッファをロック
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
	{
		return NULL;
	}

	memcpy(pBlock, pData, dwSize);	// データ転送
	SafeDeleteArray(pData);			// 元の読み込み領域を消す

	// バッファロック解除
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// セカンダリバッファを返してようやく完了...
	return pBuffer;
}

//=============================================================================
// BGM、2Dサウンドを設置する
//=============================================================================
void SetSound(int Sound_No, int Flag, bool FromStart, bool IsBGM)
{
	if (IsBGM == true)
	{
		PlayingBGM_No = Sound_No;
	}

	if (FromStart == true)
	{
		// 続きから鳴らすので、最初から鳴らしたい場合はSetCurrentPosition(0)をすること
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}
	// Flag   :1(E_DS8_FLAG_LOOP)ならループ再生
	SoundBuffer[Sound_No]->Play(0, 0, Flag);

	return;
}

//=============================================================================
// サウンドを停止する
//=============================================================================
void StopSound(int Sound_No)
{
	DWORD status;

	SoundBuffer[Sound_No]->GetStatus(&status);
	// 鳴っていたら
	if (status & DSBSTATUS_PLAYING)
	{
		// 意味的にはPauseになる。
		SoundBuffer[Sound_No]->Stop();
	}

	return;
}

//=============================================================================
// 再生中かどうか調べる
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
// 再初期化処理
//=============================================================================
void ReInitSound(void)
{
	// ボリューム初期化
	LONG Volume = DSBVOLUME_MAX;

	// フラグ初期化
	SetIntroOver = false;

	// BGM番号初期化
	PlayingBGM_No = 0;

	for (int Sound_No = 0; Sound_No < Sound_Max; Sound_No++)
	{
		// 流してるサウンドを停止
		StopSound(Sound_No);

		// 本来のボリュームに戻す
		SoundBuffer[Sound_No]->SetVolume(Volume);

		// プレイ位置が先頭に戻る
		SoundBuffer[Sound_No]->SetCurrentPosition(0);
	}

	return;
}

//=============================================================================
// サウンドフェイドアウト
//=============================================================================
void SoundFadeOut(int Sound_No)
{
	LONG Volume = 0;

	SoundBuffer[Sound_No]->GetVolume(&Volume);
	// ボリューム少しずつ減る
	Volume -= VolumeDecreaseSpeed;

	// ボリューム設定
	SoundBuffer[Sound_No]->SetVolume(Volume);

	return;
}

//=============================================================================
// 今鳴らしているBGMの番号を取得する
//=============================================================================
int GetPlayingBGM_No(void)
{
	return PlayingBGM_No;
}
