#include "3DX.h"

D3DCLASS::D3DCLASS()
{
}

D3DCLASS::~D3DCLASS()
{
	Cleanup();
}


HRESULT D3DCLASS::InitD3D(HWND hWnd, BOOL Win, int Width, int Height)
{
	/////*D3D��ü ����*/
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	/////*Device ��ü ����*/
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = Win; //������ ��� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //ȭ�� ������ ���
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; //����� ����, ���÷��� ���˻�� ���
	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	HRESULT lp = m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,//��� ��ġ ����, HAL�� �ϵ���� ���� ����
		hWnd,//������ �ڵ�
		D3DCREATE_HARDWARE_VERTEXPROCESSING, //Device ���� �ɼ�,�ϵ���� ������ ������ ���
		&d3dpp, //Present �Ķ���� ����
		&m_pd3dDevice);

	if (FAILED(lp))
	{
		return E_FAIL;
	}

	//D3DVIEWPORT9 viewPort = { 0, 0, Width, Height, 0.0f, 1.0f };
	//m_pd3dDevice->SetViewport(&viewPort);

	D3DXCreateSprite(m_pd3dDevice, &m_Sprite);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	return S_OK;
}

VOID D3DCLASS::Cleanup()
{
	if (m_pVB != NULL)
		m_pVB->Release();
	if (m_pIB != NULL)
		m_pIB->Release();
	if (m_Sprite != NULL)
		m_Sprite->Release();
	if (m_pd3dDevice != NULL)
		m_pd3dDevice->Release();
	if (m_pD3D != NULL)
		m_pD3D->Release();
}

LPDIRECT3DDEVICE9 D3DCLASS::getDevice()
{
	return m_pd3dDevice;
}

LPD3DXSPRITE D3DCLASS::getSprite()
{
	return m_Sprite;
}

VOID D3DCLASS::SetMatrix()
{
	D3DXMATRIX tempTM;
	D3DXMatrixIdentity(&tempTM);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &tempTM);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &tempTM);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &tempTM);
}