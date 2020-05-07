#ifndef H_Graphic_H
#define H_Graphic_H

#include "3DX.h"

class GraphicClass
{
public:
	GraphicClass();
	virtual ~GraphicClass();
	virtual void Init(LPDIRECT3DDEVICE9 device) = 0;
	virtual void Update(LPD3DXSPRITE sprite) = 0;
	virtual void Release() = 0;

protected :
	FLOAT mn_X, mn_Y;
};

#endif // !H_Graphic_H

