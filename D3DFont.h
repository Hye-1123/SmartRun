#ifndef H_D3DFONT_
#define H_D3DFONT_

#include "3DX.h"

class D3DFont
{
public:
	D3DFont();
	~D3DFont();

	VOID Init(LPDIRECT3DDEVICE9 device);
	VOID TextDraw(const char* string, RECT lot, int Format);
	VOID Release();

private:
	ID3DXFont* pFont;
};

#endif // !H_D3DFONT_

