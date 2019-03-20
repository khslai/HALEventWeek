//=============================================================================
//
// Main�w�b�_�[ [main.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_


//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~

#include <time.h>
#include <Windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <tchar.h>


#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include <dsound.h>
#include <dinput.h>
#include <mmsystem.h>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// UI
#define Screen_Width		(1920)					// �E�C���h�E�̕�
#define Screen_Height		(1080)					// �E�C���h�E�̍���
#define ScreenCenter_X		(Screen_Width / 2)		// �E�C���h�E�̒��S���W
#define ScreenCenter_Y		(Screen_Height / 2)		
// �v�Z�p
#define UpVec				(D3DXVECTOR3(0.0f, -1.0f, 0.0f))
#define DownVec				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define LeftVec				(D3DXVECTOR3(-1.0f, 0.0f, 0.0f))
#define RightVec			(D3DXVECTOR3(1.0f, 0.0f, 0.0f))
#define Root2				(1.414214f)
// ���_��
#define	Num_Vertex			(4)					
// �|���S����
#define Num_Polygon			(2)
// �ǂ̌���
#define Wall_Width			(50.0f)
// �I�������[�v�J�E���g
#define RepeatCount			(60)
// �I�������[�v���x
#define RepeatSpeed			(5)

// �F
#define WHITE(Alpha)		D3DCOLOR_RGBA(255, 255, 255, Alpha)
#define BLACK(Alpha)		D3DCOLOR_RGBA(0, 0, 0, Alpha)
#define GRAY(Alpha)			D3DCOLOR_RGBA(64, 64, 64, Alpha)
#define GREEN(Alpha)		D3DCOLOR_RGBA(0, 255, 0, Alpha)
#define BLUE(Alpha)			D3DCOLOR_RGBA(0, 0, 255, Alpha)
#define YELLOW(Alpha)		D3DCOLOR_RGBA(255, 255, 0, Alpha)
#define RED(Alpha)			D3DCOLOR_RGBA(255, 0, 0, Alpha)
#define SKYBLUE(Alpha)		D3DCOLOR_RGBA(135, 206, 235, Alpha)
#define AZURE(Alpha)		D3DCOLOR_RGBA(0, 127, 255, Alpha)
#define GRASSGREEN(Alpha)	D3DCOLOR_RGBA(153, 230, 77, Alpha)
#define ORANGE(Alpha)		D3DCOLOR_RGBA(255, 165, 0, Alpha)
#define PURPLE(Alpha)		D3DCOLOR_RGBA(128, 0, 128, Alpha)

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �e�N�X�`���̃������������[�X
// �����������[�X
// new
#define SafeDelete(Object)			{ if (Object) { delete(Object);			(Object) = NULL; } }
#define SafeDeleteArray(Object)		{ if (Object) { delete[](Object);		(Object) = NULL; } }
// malloc,calloc
#define SafeFree(Object)			{ if (Object) { free(Object);			(Object) = NULL; } }
#define SafeRelease(Object)			{ if (Object) { (Object)->Release();	(Object) = NULL; } }

// �L�����N�^�[�̕���
enum CharacterDirection
{
	Down,
	Left,
	Right,
	Up,
};

// ��ʑJ�ڂ̏��
enum TransitionState
{
	TransitionOver = 0,	// �����Ȃ����
	Fadein,				// �t�F�[�h�C������
	Fadeout,			// �t�F�[�h�A�E�g����
};

// �Q�[���X�e�[�W
enum StagePhase
{
	Stage_Title,
	Stage_MainMenu,
	Stage_Ite,			// �ˎ��
	Stage_Oushi,		// ������
	Stage_Futago,		// �o�q��
	Stage_Shishi,		// ���q��
	Stage_GameEnd,		// �Q�[���N���A
};

// �e�N�X�`���F�̈���
enum Texturecolor
{
	White,
	Red,
	Red_AlphaHalf,
	Green,
	SkyBlue,
	Yellow,
};

// �G�t�F�N�g���L��
enum EffectOwner
{
	Player_A = 0,		// �v���C���[A
	Player_B = 1,		// �v���C���[B
	BossEffect,			// �{�X
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3		vtx;					// ���_���W
	float			rhw;					// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR		diffuse;				// ���ˌ�
	D3DXVECTOR2		tex;					// �e�N�X�`�����W
} VERTEX_2D;

// �摜�\����
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR3			pos;					// ���W
	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`��
	int					Width;					// ����
	int					Height;					// �c��
	int					Type;					// �摜�̎��
	bool				Use;					// �g�p�t���O
}IMAGE;

// �I�����\����
typedef struct
{
	VERTEX_2D			vertexwk[Num_Vertex];	// ���_���i�[���[�N
	D3DXVECTOR2			pos;					// ���W
	int					PreState;				// �O�̑I�����
	int					State;					// �I�����
	bool				InYes;					// ������"YES"��"No"��
}SELECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �f�o�C�X�擾�֐�
LPDIRECT3DDEVICE9 GetDevice(void);
// �Q�[���X�e�[�W�擾�֐�
int GetGameStage(void);
// �Q�[���X�e�[�W�ݒ�֐��i��ʑJ�ځj
void SetGameStage(int Stage);
// �Q�[���̍ď�����
void ReInitialize(void);
// �e�̓X�N���[���͈͓̔����ǂ������`�F�b�N
bool CheckPosition(D3DXVECTOR3 Pos, int Width, int Height);
// �e�N�X�`���ǂݍ��ފ֐�
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc);
// ��̍��W�̈ړ��p�x���v�Z����
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos);

#endif