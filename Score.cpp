//=============================================================================
//
// �X�R�A���� [Score.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Texture_Number				_T("data/Texture/UI/Number.png")
#define Texture_Number_Width_Total	(700)
#define Texture_Number_Height_Total	(70)
#define Texture_Number_Divide_X		(10)
#define Texture_Number_Divide_Y		(1)
#define Texture_Number_Width		(Texture_Number_Width_Total / Texture_Number_Divide_X)
#define Texture_Number_Height		(Texture_Number_Height_Total / Texture_Number_Divide_Y)

// �X�R�A�\���̏����ʒu
#define Score_Pos_X					(960)
#define Score_Pos_Y					(40)
// �X�R�A�̍ő�l
#define Score_MAX					(99)		
// �X�R�A�̌���
#define DigitMax					(2)

#define Texture_ScoreBox			_T("data/Texture/UI/ScoreBox.png")
#define Texture_ScoreBox_Width		(250)
#define Texture_ScoreBox_Height		(120)

// �^�C�}�[�̕\�����W
#define ScoreBox_Pos_X				(835)
#define ScoreBox_Pos_Y				(0)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum);
HRESULT MakeScoreBoxVertex(void);
// ���_���W�̐ݒ�
void SetScoreVertex(int ScoreDigit);
void SetScoreBoxVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetScoreTexture(int ScoreDigit, int ScoreNum);
void SetScoreBoxTexture(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 ScoreTexture = NULL;
LPDIRECT3DTEXTURE9 ScoreBoxTexture = NULL;
// �X�R�A�\����
SCORE Score[DigitMax];
// �X�R�A�g�\����
SCORE ScoreBox;
// �v���C���[�X�R�A
int PlayerScore = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitScore(bool FirstInit)
{
	int ScoreDigit = 0;
	PlayerScore = 0;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].pos = D3DXVECTOR3((float)Score_Pos_X - ScoreDigit * Texture_Number_Width, Score_Pos_Y, 0.0f);
		Score[ScoreDigit].Number = 0;
		// ���_���̍쐬
		MakeScoreVertex(ScoreDigit, Score[ScoreDigit].Number);
	}

	if (FirstInit == true)
	{
		// ���_���̍쐬
		MakeScoreBoxVertex();

		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Number, &ScoreTexture, "Score") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_ScoreBox, &ScoreBoxTexture, "Score") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	// �e�N�X�`���̊J��
	SafeRelease(ScoreTexture);
	SafeRelease(ScoreBoxTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	int ScoreDigit = 0;
	int Scoretemp = PlayerScore;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].Number = Scoretemp % 10;
		Scoretemp /= 10;

		// �e�N�X�`�����W��ݒ�
		SetScoreTexture(ScoreDigit, Score[ScoreDigit].Number);
	}

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	int ScoreDigit = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, ScoreBoxTexture);

	// �|���S���̕`��
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, ScoreBox.vertexwk, sizeof(VERTEX_2D));

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, ScoreTexture);

	// �|���S���̕`��
	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, Score[ScoreDigit].vertexwk, sizeof(VERTEX_2D));
	}


	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum)
{
	// ���_���W�̐ݒ�	
	SetScoreVertex(ScoreDigit);

	// rhw�̐ݒ�
	Score[ScoreDigit].vertexwk[0].rhw = 1.0f;
	Score[ScoreDigit].vertexwk[1].rhw = 1.0f;
	Score[ScoreDigit].vertexwk[2].rhw = 1.0f;
	Score[ScoreDigit].vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	Score[ScoreDigit].vertexwk[0].diffuse = WHITE(255);
	Score[ScoreDigit].vertexwk[1].diffuse = WHITE(255);
	Score[ScoreDigit].vertexwk[2].diffuse = WHITE(255);
	Score[ScoreDigit].vertexwk[3].diffuse = WHITE(255);

	// �e�N�X�`�����W�̐ݒ�
	SetScoreTexture(ScoreDigit, ScoreNum);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetScoreTexture(int ScoreDigit, int ScoreNum)
{
	Score[ScoreDigit].vertexwk[0].tex = D3DXVECTOR2(ScoreNum * 0.1f, 0.0f);
	Score[ScoreDigit].vertexwk[1].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 0.0f);
	Score[ScoreDigit].vertexwk[2].tex = D3DXVECTOR2(ScoreNum * 0.1f, 1.0f);
	Score[ScoreDigit].vertexwk[3].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetScoreVertex(int ScoreDigit)
{
	Score[ScoreDigit].vertexwk[0].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x, Score[ScoreDigit].pos.y, 0.0f);
	Score[ScoreDigit].vertexwk[1].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x + Texture_Number_Width, Score[ScoreDigit].pos.y, 0.0f);
	Score[ScoreDigit].vertexwk[2].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x, Score[ScoreDigit].pos.y + Texture_Number_Height, 0.0f);
	Score[ScoreDigit].vertexwk[3].vtx =
		D3DXVECTOR3(Score[ScoreDigit].pos.x + Texture_Number_Width, Score[ScoreDigit].pos.y + Texture_Number_Height, 0.0f);

	return;
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeScoreBoxVertex(void)
{
	// ���_���W�̐ݒ�	
	SetScoreBoxVertex();

	// rhw�̐ݒ�
	ScoreBox.vertexwk[0].rhw = 1.0f;
	ScoreBox.vertexwk[1].rhw = 1.0f;
	ScoreBox.vertexwk[2].rhw = 1.0f;
	ScoreBox.vertexwk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	ScoreBox.vertexwk[0].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	ScoreBox.vertexwk[1].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	ScoreBox.vertexwk[2].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);
	ScoreBox.vertexwk[3].diffuse = D3DCOLOR_RGBA(0, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetScoreBoxTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetScoreBoxTexture(void)
{
	ScoreBox.vertexwk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ScoreBox.vertexwk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ScoreBox.vertexwk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ScoreBox.vertexwk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetScoreBoxVertex(void)
{
	ScoreBox.vertexwk[0].vtx = D3DXVECTOR3(ScoreBox_Pos_X, ScoreBox_Pos_Y, 0.0f);
	ScoreBox.vertexwk[1].vtx = D3DXVECTOR3(ScoreBox_Pos_X + Texture_ScoreBox_Width, ScoreBox_Pos_Y, 0.0f);
	ScoreBox.vertexwk[2].vtx = D3DXVECTOR3(ScoreBox_Pos_X, ScoreBox_Pos_Y + Texture_ScoreBox_Height, 0.0f);
	ScoreBox.vertexwk[3].vtx = D3DXVECTOR3(ScoreBox_Pos_X + Texture_ScoreBox_Width, ScoreBox_Pos_Y + Texture_ScoreBox_Height, 0.0f);

	return;
}

//=============================================================================
// �X�R�A���Z
//=============================================================================
void AddScore(int Score)
{
	PlayerScore += Score;
	PlayerScore = PlayerScore > Score_MAX ? (int)Score_MAX : PlayerScore;

	return;
}

//=============================================================================
// �X�R�A���擾
//=============================================================================
int GetScore(void)
{
	return PlayerScore;
}

//=============================================================================
// �X�R�A�����̃e�N�X�`�����擾
//=============================================================================
LPDIRECT3DTEXTURE9 GetNumberTex(void)
{
	return ScoreTexture;
}
