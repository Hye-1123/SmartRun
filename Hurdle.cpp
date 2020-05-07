#include "Hurdle.h"

enum HurdleKind
{
	en_Wood,
	en_Stone,
	en_Stump
};

Hurdles::Hurdles()
{
}

Hurdles::~Hurdles()
{
	Release();
}

void Hurdles::Init(LPDIRECT3DDEVICE9 device)
{
	D3DXIMAGE_INFO info;

	if (m_pWood == NULL)
		D3DXCreateTextureFromFileEx(device, "Image\\Wood.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &m_pWood);
	if (m_pStone == NULL)
		D3DXCreateTextureFromFileEx(device, "Image\\Stone.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &m_pStone);
	if (m_pStump == NULL)
		D3DXCreateTextureFromFileEx(device, "Image\\Stump.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &m_pStump);

	if (!hurdle.empty())
	{
		hurdle.clear();
	}

	mn_Y = 260;
	count = 0;
}

void Hurdles::Update(LPD3DXSPRITE sprite)
{
	int i;

	for (i = 0; i < count; i++)
	{
		switch (hurdle[i].nKind)
		{
		case en_Wood:
			sprite->Draw(m_pWood, NULL, 0, &D3DXVECTOR3(hurdle[i].nX, mn_Y, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case en_Stone:
			sprite->Draw(m_pStone, NULL, 0, &D3DXVECTOR3(hurdle[i].nX, mn_Y + 30, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case en_Stump:
			sprite->Draw(m_pStump, NULL, 0, &D3DXVECTOR3(hurdle[i].nX, mn_Y + 50, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		
		hurdle[i].nX-=2;
	}

	if (count)
	{
		if (hurdle[0].nX < -150)
		{
			ScoreUp();
			hurdle.pop_front(); //제거
			count--;
		}
	}
}

void Hurdles::Generate(int nKind, int nX)
{
	HurdleInfo temp;

	temp.bCrash = FALSE;
	temp.nKind = nKind;
	temp.nX = nX;

	hurdle.push_back(temp);

	count++;
}

//점수 코드
void Hurdles::ScoreUp()
{
	if (!hurdle[0].bCrash)
	{
		switch (hurdle[0].nKind)
		{
		case en_Wood:
			mnScore += 100;
			break;
		case en_Stone:
			mnScore += 50;
			break;
		case en_Stump:
			mnScore += 20;
			break;
		}
	}
}

int Hurdles::GetCount()
{
	return count;
}

int Hurdles::GetScore()
{
	return mnScore;
}

int Hurdles::GetKind(int nIndex)
{
	return hurdle[nIndex].nKind;
}

RECT Hurdles::GetBound(int nIndex)
{
	RECT rBound;

	switch (hurdle[nIndex].nKind)
	{
	case en_Wood:
		SetRect(&rBound, hurdle[nIndex].nX, mn_Y + 20, hurdle[nIndex].nX + 100, mn_Y + 80);
		break;
	case en_Stone:
		SetRect(&rBound, hurdle[nIndex].nX + 20, mn_Y + 30, hurdle[nIndex].nX + 90, mn_Y + 110); 
		break;
	case en_Stump:
		SetRect(&rBound, hurdle[nIndex].nX, mn_Y + 50, hurdle[nIndex].nX + 56, mn_Y + 106); 
		break;
	}

	return rBound;
}

BOOL Hurdles::GetCrash(int nIndex)
{
	return hurdle[nIndex].bCrash;
}

VOID Hurdles::SetCrash(int nIndex, BOOL bcrash)
{
	hurdle[nIndex].bCrash = bcrash;
}

void Hurdles::Release()
{
	if (m_pWood != NULL)
		m_pWood->Release();
	if (m_pStone != NULL)
		m_pStone->Release();
	if (m_pStump != NULL)
		m_pStump->Release();
}
