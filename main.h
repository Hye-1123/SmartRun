#ifndef MAIN_H
#define MAIN_H

#include "DxMng.h"
#include "D3DFont.h"
#include "HHSound.h"
#pragma comment(lib,"sqlite3d.lib")
#include "sqlite3.h"

typedef struct StateManager
{
	BOOL bIsActive;
	int nState;
}Manager;

typedef struct Ranking
{
	int nRank;
	char cScore[256];
	char cName[256];
}Ranking;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT);
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*���� ���� ����*/
VOID MainLoop();
VOID Render();

/*���� ���� �ڵ�*/
VOID Attack();
VOID ButtonProcess(int x, int y);

/*��Ÿ*/
VOID SoundInit();
VOID LoadTexture();
VOID Release();

/*DB����*/
VOID Query(const char* query);

#endif // !MAIN_H

