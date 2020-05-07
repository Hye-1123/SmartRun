#ifndef H_BG_H_
#define H_BG_H_

#include "Graphic.h"

class BackGround : public GraphicClass
{
public:
	BackGround();
	~BackGround();
	
	//»ó¼Ó
	void Init(LPDIRECT3DDEVICE9 device);
	void Update(LPD3DXSPRITE sprite);
	void Release();

private:
	LPDIRECT3DTEXTURE9 m_pBG = NULL;
	LPDIRECT3DTEXTURE9 m_pCloud = NULL;
	
	int mn_X2;
	float mn_CX, mn_C2X;
};



#endif // !H_BG_H_

