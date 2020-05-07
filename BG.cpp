#include "BG.h"

BackGround::BackGround()
{
}

BackGround::~BackGround()
{
	Release();
}

void BackGround::Init(LPDIRECT3DDEVICE9 device)
{
	D3DXIMAGE_INFO info;

	mn_X = 0, mn_X2 = 1024;
	mn_CX = 0.0f, mn_C2X = 1024;

	if (m_pBG == NULL)
	{
		D3DXCreateTextureFromFileEx(device, "Image\\BG.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &m_pBG);
	}
	if (m_pCloud == NULL)
	{
		D3DXCreateTextureFromFileEx(device, "Image\\Cloud.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &m_pCloud);
	}
	
}

void BackGround::Update(LPD3DXSPRITE sprite)
{
	if (mn_X <= -1024)
	{
		mn_X = 1024;
	}
	if (mn_X2 <= -1024)
	{
		mn_X2 = 1024;
	}
	sprite->Draw(m_pBG, NULL, 0, &D3DXVECTOR3(mn_X, 0, 1.0f), D3DCOLOR_XRGB(255, 255, 255));
	sprite->Draw(m_pBG, NULL, 0, &D3DXVECTOR3(mn_X2, 0, 1.0f), D3DCOLOR_XRGB(255, 255, 255));

	if (mn_CX <= -1024)
	{
		mn_CX = 1024;
	}
	if (mn_C2X <= -1024)
	{
		mn_C2X = 1024;
	}
	sprite->Draw(m_pCloud, NULL, 0, &D3DXVECTOR3(mn_CX, 2, 0.0f), D3DCOLOR_ARGB(255, 255, 255, 255));
	sprite->Draw(m_pCloud, NULL, 0, &D3DXVECTOR3(mn_C2X, 2, 0.0f), D3DCOLOR_ARGB(255, 255, 255, 255));

	mn_X -= 2;
	mn_X2 -= 2;
	mn_CX -= 0.5f;
	mn_C2X -= 0.5f;

}

void BackGround::Release()
{
	if (m_pBG != NULL)
		m_pBG->Release();
	if (m_pCloud != NULL)
		m_pCloud->Release();
}