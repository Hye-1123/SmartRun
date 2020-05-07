#ifndef D3DX_H_
#define D3DX_H_

#include <d3d9.h>
#include <d3dx9.h>

class D3DCLASS
{
public:
	D3DCLASS();
	~D3DCLASS();
	HRESULT InitD3D(HWND hWnd, BOOL Win, int Width, int Height);
	
	VOID Cleanup();

	LPDIRECT3DDEVICE9 getDevice();
	LPD3DXSPRITE getSprite();
	VOID SetMatrix();

private:
	LPDIRECT3D9 m_pD3D = NULL;      //3D��ü
	LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;//�� ��ü�κ��� ���������� ��ü, 2d�� surface
	LPD3DXSPRITE m_Sprite = NULL;
	LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9 m_pIB = NULL;
	
};


#endif // !D3DX_H_

