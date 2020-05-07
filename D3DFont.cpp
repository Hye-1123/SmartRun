#include "D3DFont.h"

D3DFont::D3DFont()
{
}

D3DFont::~D3DFont()
{
	Release();
}

VOID D3DFont::Init(LPDIRECT3DDEVICE9 device)
{
	D3DXFONT_DESC lf;
	ZeroMemory(&lf, sizeof(D3DXFONT_DESC));

	lf.Height = 48;
	lf.Width = 0;
	lf.Weight = FW_BOLD;
	lf.Quality = DEFAULT_QUALITY;
	lf.MipLevels = D3DX_DEFAULT;
	lf.Italic = FALSE;
	lf.CharSet = DEFAULT_CHARSET;
	lf.OutputPrecision = OUT_DEFAULT_PRECIS;
	lf.PitchAndFamily = DEFAULT_PITCH;
	strcpy(lf.FaceName, "³ª´®°íµñ");

	if (FAILED(D3DXCreateFontIndirect(device, &lf, &pFont)))
	{
		MessageBox(0, TEXT("ÆùÆ® »ý¼º ½ÇÆÐ"), 0, 0);
	}
}

VOID D3DFont::TextDraw(const char* string, RECT lot, int Format)
{
	pFont->DrawTextA(0, string, -1, &lot, Format, D3DCOLOR_XRGB(0, 0, 0));
}

VOID D3DFont::Release()
{
	if (pFont != NULL)
		pFont->Release();
}

