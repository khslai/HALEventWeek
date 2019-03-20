//=============================================================================
//
// Main���� [main.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "resource.h"
#include "DebugProcess.h"
#include "Input.h"
#include "BackGround.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Boss.h"
#include "BossBarrage.h"
#include "Effect.h"
#include "GameUI.h"
#include "CheckHit.h"
#include "AttackRange.h"
#include "Apple.h"
#include "Wolf.h"
#include "Score.h"
#include "Bull.h"
#include "Timer.h"
#include "Block.h"
#include "Door.h"
#include "GameClear.h"
#include "MainMenu.h"
#include "Transition.h"
#include "Title.h"
#include "Pause.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")			// �E�C���h�E�̃N���X��
#define WINDOW_NAME			_T("�ӂ����STARGAZING")	// �E�C���h�E�̃L���v�V������
#define FPS					(60)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;							// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;					// Device�I�u�W�F�N�g(�`��ɕK�v)
int						GameStage = Stage_Title;				// �Q�[���X�e�[�W
int						GameCount = 0;							// �Q�[���J�E���g
int						FPSCount = 0;							// FPS�J�E���g
#if _DEBUG
bool					DisplayDebugMsg = true;					// �f�o�b�O�\��ON/OFF
#else
bool					DisplayDebugMsg = false;
#endif


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// �Q�[���A�C�R��
	HICON MyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	// ���Ԍv���p
	DWORD dwExecLastTime;	// �O���ʏ����̎���
	DWORD dwFPSLastTime;	// �O��FPG�v�Z�̎���
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwFrameCount;		// ��b�ԉ��񏈗����� = FPS

	// �^�X�N�o�[���������̈�̃T�C�Y���擾
	RECT rt;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		MyIcon,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		MyIcon
	};
	HWND		hWnd;
	MSG			msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rt.right,
		rt.bottom,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (hWnd == NULL)
	{
		return false;
	}

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while (1)
	{
		// �Q�[���I��
		if (GetGameExit() == true)
		{
			break;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// �V�X�e���������擾
			dwCurrentTime = timeGetTime();

			// 1�b���ƂɎ��s
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)
			{

#ifdef _DEBUG
				// FPS�v�Z
				//if (GameStage == Stage_Game)
				//{
				//	FPSCount = dwFrameCount;
				//}
#endif
				// FPS�𑪒肵��������ۑ�
				dwFPSLastTime = dwCurrentTime;
				// �J�E���g���N���A
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))	// 1/60�b���ƂɎ��s
			{
				// ��������������ۑ�
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				// �����񐔂̃J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
			// [ESC]�L�[�������ꂽ
		case VK_ESCAPE:
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = Screen_Width;			// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = Screen_Height;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;			// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.BackBufferFormat = d3ddm.Format;			// �J���[���[�h�̎w��

	if (bWindow)
	{
		// �E�B���h�E���[�h
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;						// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = 0;							// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		// �C���^�[�o��
	}
	else
	{
		// �t���X�N���[�����[�h
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;							// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,										// �f�B�X�v���C�^�C�v
		hWnd,												// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,				// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,												// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&g_pD3DDevice)))									// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// �����_���̏�����
	srand((unsigned)time(NULL));

#ifdef _DEBUG
	// �f�o�b�O�e�X�g�̏���������
	if (FAILED(InitDebugProcess(true)))
	{
		MessageBox(0, "DebugProcess Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// ������͈͕\���̏���������
	if (FAILED(InitAttackRange(true)))
	{
		MessageBox(0, "AttackRange Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}
#endif

	// ���͏����̏�����
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBox(0, "DirectInput Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �T�E���h�̏�����
	if (FAILED(InitSound(hWnd)))
	{
		MessageBox(0, "Sound Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �^�C�g���̏���������
	if (FAILED(InitTitle(true)))
	{
		MessageBox(0, "Title Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// ���C�����j���[�̏���������
	if (FAILED(InitMainMenu(true)))
	{
		MessageBox(0, "MainMenu Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �ꎞ��~��ʂ̏���������
	if (FAILED(InitPause(true)))
	{
		MessageBox(0, "Pause Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �X�e�[�W�J�ډ�ʂ̏�����
	if (FAILED(InitTransition(true)))
	{
		MessageBox(0, "Transition Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �w�i�̏���������
	if (FAILED(InitBackGround(true)))
	{
		MessageBox(0, "BackGround Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �Q�[���N���A�̏���������
	if (FAILED(InitGameClear(true)))
	{
		MessageBox(0, "GameClear Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// ��Q���̏���������
	if (FAILED(InitBlock(true)))
	{
		MessageBox(0, "Block Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// ���̏���������
	if (FAILED(InitDoor(true)))
	{
		MessageBox(0, "Door Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �Q�[��UI�̏���������
	if (FAILED(InitGameUI(true)))
	{
		MessageBox(0, "GameUI Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �X�R�A�̏���������
	if (FAILED(InitScore(true)))
	{
		MessageBox(0, "Score Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �^�C�}�[�̏���������
	if (FAILED(InitTimer(true)))
	{
		MessageBox(0, "Timer Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �v���C���[�̏���������
	if (FAILED(InitPlayer(true)))
	{
		MessageBox(0, "Player Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �v���C���[�̒e�̏���������
	if (FAILED(InitPlayerBullet(true)))
	{
		MessageBox(0, "PlayerBullet Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �A�b�v���̏���������
	if (FAILED(InitApple(true)))
	{
		MessageBox(0, "Apple Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �T�̏���������
	if (FAILED(InitWolf(true)))
	{
		MessageBox(0, "Wolf Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �����̏���������
	if (FAILED(InitBull(true)))
	{
		MessageBox(0, "Bull Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �{�X�̏���������
	if (FAILED(InitBoss(true)))
	{
		MessageBox(0, "Boss Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �{�X�̒e���̏���������
	if (FAILED(InitBossBarrage(true)))
	{
		MessageBox(0, "BossBarrage Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	// �G�t�F�N�g�̏���������
	if (FAILED(InitEffect(true)))
	{
		MessageBox(0, "Effect Initialize Failed�I", "Error", 0);
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	int i = 0;

#ifdef _DEBUG
	// �f�o�b�O�e�X�g�̏I������
	UninitDebugProcess();

	// ������͈͕\���̏I������
	UninitAttackRange();
#endif

	// ���͏����̏I������
	UninitInput();

	// �T�E���h�̏I������
	UninitSound();

	// �^�C�g���̏I������
	UninitTitle();

	// ���C�����j���[�̏I������
	UninitMainMenu();

	// �ꎞ��~��ʂ̏I������
	UninitPause();

	// �X�e�[�W�J�ډ�ʂ̏I������
	UninitTransition();

	// �w�i�̏I������
	UninitBackGround();

	// �Q�[���N���A�̏I������
	UninitGameClear();

	// ��Q���̏I������
	UninitBlock();

	// ���̏I������
	UninitDoor();

	// �Q�[��UI�̏I������
	UninitGameUI();

	// �X�R�A�̏I������
	UninitScore();

	// �^�C�}�[�̏I������
	UninitTimer();

	// �v���C���[�̏I������
	UninitPlayer();

	// �v���C���[�̒e�̏I������
	UninitPlayerBullet();

	// �A�b�v���̏I������
	UninitApple();

	// �T�̏I������
	UninitWolf();

	// �����̏I������
	UninitBull();

	// �{�X�̏I������
	UninitBoss();

	// �{�X�̒e���̏I������
	UninitBossBarrage();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �f�o�C�X�̊J��
	SafeRelease(g_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̊J��
	SafeRelease(g_pD3D);
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	int Transition = GetTransition();

#if _DEBUG
	// �f�o�b�O�\��ON/OFF
	if (GetKeyboardTrigger(DIK_F1))
	{
		DisplayDebugMsg = DisplayDebugMsg ? false : true;
	}
#endif

	// ���͂̍X�V����
	UpdateInput();

	// �T�E���h�̍X�V����
	UpdateSound();

	switch (GameStage)
	{
	case Stage_Title:

		// �^�C�g���̍X�V����
		UpdateTitle();
		break;

	case Stage_MainMenu:

		// ���C�����j���[�̍X�V����
		UpdateMainMenu();
		break;

	case Stage_Futago:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// �w�i�̍X�V����
			UpdateBackGround();

			// �v���C���[�̍X�V����
			UpdatePlayer();

			// ��Q���̍X�V����
			UpdateBlock();

			// ���̍X�V����
			UpdateDoor();

			// �^�C�}�[�̍X�V����
			UpdateTimer();

			// �����蔻�菈��
			CheckHit();
		}

		// �ꎞ��~��ʂ̍X�V����
		UpdatePause();

		// �Q�[���N���A�̍X�V����
		UpdateGameClear();
		break;

	case Stage_Oushi:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// �w�i�̍X�V����
			UpdateBackGround();

			// �v���C���[�̍X�V����
			UpdatePlayer();

			if (GetStartCDOver() == true)
			{
				// �����̍X�V����
				UpdateBull();
			}

			// �Q�[��UI�̍X�V����
			UpdateGameUI();

			// �^�C�}�[�̍X�V����
			UpdateTimer();

			// �����蔻�菈��
			CheckHit();
		}

		// �ꎞ��~��ʂ̍X�V����
		UpdatePause();

		// �Q�[���N���A�̍X�V����
		UpdateGameClear();

		break;

	case Stage_Ite:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// �w�i�̍X�V����
			UpdateBackGround();

			// �v���C���[�̍X�V����
			UpdatePlayer();

			// �v���C���[�̒e�̍X�V����
			UpdatePlayerBullet();

			// �A�b�v���̍X�V����
			UpdateApple();

			// �T�̍X�V����
			UpdateWolf();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();

			// �Q�[��UI�̍X�V����
			UpdateGameUI();

			// �����蔻�菈��
			CheckHit();

			// �X�R�A�̍X�V����
			UpdateScore();
		}

		// �ꎞ��~��ʂ̍X�V����
		UpdatePause();

		// �Q�[���N���A�̍X�V����
		UpdateGameClear();

		break;

	case Stage_Shishi:

		if (GetStageClear() != true && GetPauseFlag() != true && Transition == TransitionOver)
		{
			// �w�i�̍X�V����
			UpdateBackGround();

			// �v���C���[�̍X�V����
			UpdatePlayer();

			// �v���C���[�̒e�̍X�V����
			UpdatePlayerBullet();

			// �{�X�̍X�V����
			UpdateBoss();

			// �{�X�̒e���̍X�V����
			UpdateBossBarrage();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();

			// �Q�[��UI�̍X�V����
			UpdateGameUI();

#if _DEBUG
			// ������͈͕\���̍X�V����
			//UpdateAttackRange();
#endif

				// �����蔻�菈��
			CheckHit();
		}

		// �ꎞ��~��ʂ̍X�V����
		UpdatePause();

		// �Q�[���N���A�̍X�V����
		UpdateGameClear();

		break;

	case Stage_GameEnd:

		// �Q�[���N���A�̍X�V����
		UpdateGameClear();
		break;

	default:
		break;
	}

	// �X�e�[�W�J�ډ�ʂ̍X�V����
	UpdateTransition();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (GameStage)
		{
		case Stage_Title:

			// �^�C�g���̕`�揈��
			DrawTitle();
			break;

		case Stage_MainMenu:

			// ���C�����j���[�̕`�揈��
			DrawMainMenu();
			break;

		case Stage_Futago:

			// �w�i�̕`�揈��
			DrawBackGround();

			// ��Q���̕`�揈��
			DrawBlock();

			// ���̕`�揈��
			DrawDoor();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �^�C�}�[�̕`�揈��
			DrawTimer();

			// �ꎞ��~��ʂ̕`�揈��
			DrawPause();

			// �Q�[���N���A�̕`�揈��
			DrawGameClear();
			break;

		case Stage_Oushi:

			// �w�i�̕`�揈��
			DrawBackGround();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �����̍X�V����
			DrawBull();

			// �Q�[��UI�̕`�揈��
			DrawGameUI();

			// �^�C�}�[�̕`�揈��
			DrawTimer();

			// �ꎞ��~��ʂ̕`�揈��
			DrawPause();

			// �Q�[���N���A�̕`�揈��
			DrawGameClear();
			break;

		case Stage_Ite:

			// �w�i�̕`�揈��
			DrawBackGround();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// �A�b�v���̕`�揈��
			DrawApple();

			// �T�̕`�揈��
			DrawWolf();

			// �G�t�F�N�g�̕`�揈��
			DrawEffect();

			// �Q�[��UI�̕`�揈��
			DrawGameUI();

			// �X�R�A�̕`�揈��
			DrawScore();

			// �ꎞ��~��ʂ̕`�揈��
			DrawPause();

			// �Q�[���N���A�̕`�揈��
			DrawGameClear();
			break;

		case Stage_Shishi:

			// �w�i�̕`�揈��
			DrawBackGround();

			// �v���C���[�̕`�揈��
			DrawPlayer();

			// �v���C���[�̒e�̕`�揈��
			DrawPlayerBullet();

			// �{�X�̕`�揈��
			DrawBoss();

			// �{�X�̒e���̕`�揈��
			DrawBossBarrage();

			// �G�t�F�N�g�̕`�揈��
			DrawEffect();

			// �Q�[��UI�̕`�揈��
			DrawGameUI();

#if _DEBUG
			// ������͈͕\���̕`�揈��
			//DrawAttackRange();
#endif

			// �ꎞ��~��ʂ̕`�揈��
			DrawPause();

			// �Q�[���N���A�̕`�揈��
			DrawGameClear();
			break;

		case Stage_GameEnd:

			// �Q�[���N���A�̕`�揈��
			DrawGameClear();
			break;

		default:
			break;
		}

		// �X�e�[�W�J�ډ�ʂ̕`�揈��
		DrawTransition();

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

#if _DEBUG
	// �f�o�b�O�\���̕`��
	if (DisplayDebugMsg)
	{
		DrawDebugProcess();
	}
#endif

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�擾�֐�
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

//=============================================================================
// �e�N�X�`���ǂݍ��ފ֐�
//=============================================================================
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc)
{
	char Message[64];

	D3DXCreateTextureFromFile(g_pD3DDevice, SrcFile, TexturePtr);
	if (*TexturePtr == NULL)
	{
		sprintf(Message, "Load %s Texture Failed�I", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return false;
	}

	return true;
}

//=============================================================================
// �Q�[���X�e�[�W�擾�֐�
//=============================================================================
int GetGameStage(void)
{
	return GameStage;
}

//=============================================================================
// �Q�[���X�e�[�W�ݒ�֐��i��ʑJ�ځj
//=============================================================================
void SetGameStage(int Stage)
{
	GameStage = Stage;

	return;
}

//=============================================================================
// �e�̓X�N���[���͈͓̔����ǂ������`�F�b�N
//=============================================================================
bool CheckPosition(D3DXVECTOR3 Pos, int Width, int Height)
{
	bool Result = false;

	if (Pos.x - Width / 2 >= Screen_Width ||
		Pos.x + Width / 2 <= 0 ||
		Pos.y - Height / 2 >= Screen_Height ||
		Pos.y + Height / 2 <= 0)
	{
		Result = true;
	}

	return Result;
}

//=============================================================================
// ��̍��W�̈ړ��p�x���v�Z����
//=============================================================================
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos)
{
	return atan2f(ObjectPos.y - ReferencePos.y, ObjectPos.x - ReferencePos.x);
}

//=============================================================================
// �Q�[���̍ď�����
//=============================================================================
void ReInitialize(void)
{
	int GameStage = GetGameStage();

	// �J�E���g�̍ď�����
	GameCount = 0;
	FPSCount = 0;

	// �T�E���h�̍ď���������
	ReInitSound();

	switch (GameStage)
	{
	case Stage_Title:

		// ���C�����j���[�̍ď�����
		InitMainMenu(false);
		break;

	case Stage_Futago:

		// �w�i�̍ď�����
		InitBackGround(false);

		// �ꎞ��~��ʂ̍ď�����
		InitPause(false);

		// �Q�[���N���A�̍ď�����
		InitGameClear(false);

		// ��Q���̍ď�����
		InitBlock(false);

		// ���̍ď�����
		InitDoor(false);

		// �^�C�}�[�̍ď�����
		InitTimer(false);

		// �v���C���[�̍ď�����
		InitPlayer(false);
		break;

	case Stage_Oushi:

		// �w�i�̍ď�����
		InitBackGround(false);

		// �ꎞ��~��ʂ̍ď�����
		InitPause(false);

		// �Q�[���N���A�̍ď�����
		InitGameClear(false);

		// �^�C�}�[�̍ď�����
		InitTimer(false);

		// �v���C���[�̍ď�����
		InitPlayer(false);

		// �����̍ď�����
		InitBull(false);

		// �Q�[��UI�̍ď�����
		InitGameUI(false);
		break;

	case Stage_Ite:

		// �w�i�̍ď�����
		InitBackGround(false);

		// �ꎞ��~��ʂ̍ď�����
		InitPause(false);

		// �Q�[���N���A�̍ď�����
		InitGameClear(false);

		// �v���C���[�̍ď�����
		InitPlayer(false);

		// �Q�[��UI�̍ď�����
		InitGameUI(false);

		// �X�R�A�̍ď�����
		InitScore(false);

		// �v���C���[�̒e�̍ď�����
		InitPlayerBullet(false);

		// �A�b�v���̍ď�����
		InitApple(false);

		// �T�̍ď�����
		InitWolf(false);

		// �G�t�F�N�g�̍ď�����
		InitEffect(false);
		break;

	case Stage_Shishi:

		// �w�i�̍ď�����
		InitBackGround(false);

		// �ꎞ��~��ʂ̍ď�����
		InitPause(false);

		// �Q�[���N���A�̍ď�����
		InitGameClear(false);

		// �v���C���[�̍ď�����
		InitPlayer(false);

		// �v���C���[�̒e�̍ď�����
		InitPlayerBullet(false);

		// �{�X�̍ď�����
		InitBoss(false);

		// �{�X�̒e���̍ď�����
		InitBossBarrage(false);

		// �Q�[��UI�̍ď�����
		InitGameUI(false);

		// �G�t�F�N�g�̍ď�����
		InitEffect(false);
		break;

	default:
		break;
	}

#if _DEBUG
	InitDebugProcess(false);
#endif

	return;
}

