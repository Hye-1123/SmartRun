#include "Character.h"

#define dTIMEGETTIME timeGetTime() * 0.001

CharacterClass::CharacterClass()
{
}

CharacterClass::~CharacterClass()
{
	Release();
}

//�ʱ⼼��
void CharacterClass::Init(LPDIRECT3DDEVICE9 device)
{
	D3DXIMAGE_INFO info;
	static RECT rtemp[6] = { {0, 0, 160, 256}, {160, 0, 320, 256}, {320, 0, 480, 256}, {480, 0, 640, 256}, {640, 0, 800, 256}, {800, 0, 960, 256} };

	mn_X = 20;
	mn_Y = 220;
	mnFrame = 0;
	mnAlpha = 255;
	mnLife = 3;
	bJump = FALSE;
	mhero.count = 6;
	mhero.r_frame = rtemp;

	if (m_pCh == NULL)
	{
		D3DXCreateTextureFromFileEx(device, "Image\\character.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &m_pCh);
	}
	if (m_pChJump == NULL)
	{
		D3DXCreateTextureFromFileEx(device, "Image\\jump.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &m_pChJump);
	}
	
	mhero.f_LastTime = dTIMEGETTIME;
}

//ȭ�鰻���ϸ鼭 �׸� �͵�
void CharacterClass::Update(LPD3DXSPRITE sprite)
{
	if (bCrash) //�ε����� �� ���̵� �� �ƿ�
	{
		FadeAlpha();
	}

	switch (bJump)
	{
	case FALSE:
		sprite->Draw(m_pCh, &mhero.r_frame[mnFrame], NULL, &D3DXVECTOR3(mn_X, mn_Y, 0.0f), D3DCOLOR_ARGB(mnAlpha , 255, 255, 255));
		
		ChangeFrame();
		if (mnFrame >= mhero.count)
			mnFrame = 0;
		break;
	case TRUE:
		sprite->Draw(m_pChJump, &mhero.r_frame[mnFrame], NULL, &D3DXVECTOR3(mn_X, mn_Y, 0.0f), D3DCOLOR_ARGB(mnAlpha, 255, 255, 255));
		
		if (mn_Y > 50 && !bLanding)
		{
			mn_Y -= 1.5;
			mnFrame = 1;
			if (mn_Y <= 50)
			{
				bLanding = TRUE; //�������� ������ ������
				mnFrame = 2;
			}
		}
		else if (mn_Y < 220 && bLanding)
		{
			mn_Y += 2;

			if (mn_Y >= 220)
			{
				bJump = FALSE;
			}
		}

		break;
	}
}

//��ü ����
void CharacterClass::Release()
{
	if (m_pCh != NULL)
		m_pCh->Release();
	if (m_pChJump != NULL)
		m_pChJump->Release();
}

int CharacterClass::GetLife()
{
	return mnLife;
}

VOID CharacterClass::MinusLife(int n)
{
	mnLife -= n;
	bCrash = TRUE;
}

RECT CharacterClass::GetLotation()
{
	RECT temp;

	if (bJump)
	{
		SetRect(&temp, mn_X, mn_Y, mn_X + 60, mn_Y + 125);
	}
	else
	{
		SetRect(&temp, mn_X, mn_Y, mn_X + 54, mn_Y + 115);
	}

	return temp;
}

VOID CharacterClass::SetbJump(BOOL b_Jump)
{
	bJump = b_Jump;

	if (b_Jump)
	{
		bLanding = FALSE;
		mnFrame = 0;
	}
}

//������ �ٲ��ִ� �ڵ�
VOID CharacterClass::ChangeFrame()
{
	if (dTIMEGETTIME - mhero.f_LastTime > 0.2)
	{
		mnFrame++;
		mhero.f_LastTime = dTIMEGETTIME;
	}
}

//���̵��ξƿ� ���ִ� �ڵ�
VOID  CharacterClass::FadeAlpha()
{
	static BOOL bTurning = TRUE;

	if (mnAlpha > 150 && bTurning)
	{
		mnAlpha--;

		if (mnAlpha <= 150)
		{
			bTurning = FALSE;
		}
	}
	else if (mnAlpha < 255 && !bTurning)
	{
		mnAlpha++;

		if (mnAlpha >= 255)
		{
			mnAlpha = 255;
			bTurning = TRUE;
			bCrash = FALSE;
		}
	}
}