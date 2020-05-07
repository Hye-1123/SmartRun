#include "main.h"
#include "BG.h"
#include "Hurdle.h"
#include "Character.h"
#include "resource.h"

#define Hres 1024		
#define Vres 512

#define dTIMEGETTIME timeGetTime() * 0.001

D3DCLASS d3d;
CHHSound chSound;
D3DFont d3dFont;
LPDIRECTSOUNDBUFFER g_pDS[3] = {NULL};

BackGround bg;
Hurdles hurdle;
CharacterClass Hero;

LPDIRECT3DTEXTURE9 g_pTitleT = NULL;
LPDIRECT3DTEXTURE9 g_pRanking = NULL;
LPDIRECT3DTEXTURE9 g_pGOver = NULL;
LPDIRECT3DTEXTURE9 g_pTrophy[3] = { NULL };
LPDIRECT3DTEXTURE9 g_pStart[2] = { NULL };
LPDIRECT3DTEXTURE9 g_pRank[2] = { NULL };
LPDIRECT3DTEXTURE9 g_pBack = NULL;
LPDIRECT3DTEXTURE9 g_pLife = NULL;

Manager manager;
Ranking ranking[10];
HINSTANCE g_ins;
BOOL bStartOver = FALSE, bRankOver = FALSE;
BOOL bCrash = FALSE;
FLOAT f_LastGene = 0;
int nScore = 0;
int nCount;
char cBuf[21];

enum GameState
{
	en_Main,
	en_Game,
	en_GameOver,
	en_Ranking
};

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	MSG msg;
	g_ins = hInst;

	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WinProc, 0L, 0L,
		hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_SR)), LoadCursor(hInst, MAKEINTRESOURCE(IDC_ARROW)), 
		NULL, NULL, "SmartRun", NULL 
	};

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, "SmartRun", "Smart Run",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 0, 0, Hres, Vres,
		NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(d3d.InitD3D(hWnd, TRUE, Hres, Vres)))
	{
		// 윈도우 출력
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		SoundInit();
		d3dFont.Init(d3d.getDevice());

		LoadTexture();
		manager.nState = en_Main;
		chSound.Play(g_pDS[0], TRUE);

		srand(time(NULL)); //랜덤 시드 설정

		// 메시지 루프
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (GetMessage(&msg, NULL, 0, 0) == NULL)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (manager.bIsActive)
			{
				MainLoop();
			}
			else
				WaitMessage();
		}
	}

	return 0;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT Pt_m; //마우스 위치
	HMENU hMenu, popupMenu;
	static HIMC ime = NULL;
	int selectMenu;
	int len = 0;

	switch (msg)
	{
	case WM_ACTIVATEAPP:
		manager.bIsActive = wParam;
		f_LastGene = dTIMEGETTIME;
		break;

	case WM_CHAR:
		if (manager.nState == en_GameOver  && nScore > 0)
		{
			if (wParam != VK_BACK)
			{
				len = strlen(cBuf);
				if (len >= 20)
					break;
				cBuf[len] = (char)wParam;
				cBuf[len + 1] = '\0';
			}
		}
		break;
	case WM_MOUSEMOVE:
		::GetCursorPos(&Pt_m);
		::ScreenToClient(hWnd, &Pt_m);
		___Trace("%d %d\n", Pt_m.x, Pt_m.y);

		if (manager.nState == en_Main)
		{
			___Trace("%d\n", manager.nState);
			if (Pt_m.x >= 600 && Pt_m.x <= 856)
			{
				if (Pt_m.y >= 100 && Pt_m.y <= 228)
				{
					bStartOver = TRUE;
					bRankOver = FALSE;
				}

				if (Pt_m.y >= 238 && Pt_m.y <= 366)
				{
					bRankOver = TRUE;
					bStartOver = FALSE;
				}
			}
			else
			{
				bStartOver = FALSE;
				bRankOver = FALSE;
			}
		}
		
		break;
	case WM_LBUTTONDOWN:
		//타이틀 화면 버튼 누를 시 변경 값(화면이 바뀜, 게임화면, 랭킹화면)
		ButtonProcess(LOWORD(lParam), HIWORD(lParam)); //이미지 버튼들
		break;
	case WM_KEYDOWN:
		switch (manager.nState)
		{
		case en_Main:
			if (wParam == VK_ESCAPE)
			{
				if (MessageBox(hWnd, "종료하겠습니까?", "종료", MB_OKCANCEL) == IDOK)
				{
					DestroyWindow(hWnd);
				}
			}
			break;
		case en_Game:
			switch (wParam)
			{
			case VK_ESCAPE: //팝업메뉴
				hMenu = LoadMenu(g_ins, MAKEINTRESOURCE(IDR_MENU2));
				popupMenu = GetSubMenu(hMenu, 0);
				selectMenu = TrackPopupMenu(popupMenu, TPM_CENTERALIGN | TPM_RETURNCMD, 512, 256, 0, hWnd, NULL);

				f_LastGene = dTIMEGETTIME;

				switch (selectMenu)
				{
				case ID_MENU_RESUME:
					break;
				case ID_MENU_TITLE:
					manager.nState = en_Main;
					break;
				}

				DestroyMenu(hMenu);
				return 0;
			case VK_SPACE: //점프버튼
				Hero.SetbJump(TRUE);
				chSound.Play(g_pDS[2], FALSE);
				break;
			}
			break;
		case en_GameOver:
			if (wParam == VK_BACK)
			{
				len = strlen(cBuf);
				if (len == 0)
				{
					cBuf[len] = '\0';
				}
				else
				{
					cBuf[len - 1] = '\0';
				}
			}
			break;
		}
		
		break;
	case WM_DESTROY: //프로그램이 닫힐 때 WM_CLOSE 메시지가 DestoryWindow함수를 호출하는데 그 함수가 WM_DESTROY 메시지를 보냄
		Release();
		chSound.Release();
		PostQuitMessage(0); //큐에 WM_QUIT메시지를 붙이고 리턴 getMessage를 만날경우 False로 루프가 끝나게 됨
		return 0;
	case WM_PAINT:
		Render(); //화면에 그림
		ValidateRect(hWnd, NULL); //화면을 유효화함
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

struct mainLoop {};
/*메인 루프*/
VOID MainLoop()
{
	static BOOL b_onceG = TRUE, b_onceR = TRUE, b_onceO = TRUE;
	int nRand;
	int i;
	
	switch (manager.nState)
	{
		////*메인 화면*////
	case en_Main:
		break;
		////*게임화면*////
	case en_Game:
		if (b_onceG)
		{
			b_onceG = FALSE;
			b_onceR = TRUE;
			nScore = 0;
			chSound.Play(g_pDS[1], TRUE);
			bg.Init(d3d.getDevice()); //배경 초기화
			Hero.Init(d3d.getDevice()); //플레이어 캐릭터 초기화
			hurdle.Init(d3d.getDevice()); //허들 초기화
			hurdle.Generate(0, 512); //허들 만들기
			hurdle.Generate(1, Hres);
			f_LastGene = dTIMEGETTIME; //허들 마지막 생성 시간
		}

		if (dTIMEGETTIME - f_LastGene > 4.000)
		{
			nRand = rand() % 3;
			hurdle.Generate(nRand, Hres);
			f_LastGene = dTIMEGETTIME;
		}

		//충돌 체크
		Attack();

		//게임오버 상황(라이프 0)
		if (Hero.GetLife() < 0)
		{
			manager.nState = en_GameOver;
			b_onceG = TRUE;
			b_onceO = TRUE;
			chSound.Stop(g_pDS[1]); //bgm stop
			nScore = hurdle.GetScore();
			chSound.Play(g_pDS[0], TRUE);
		}
		break;
	case en_GameOver:
		break;
	case en_Ranking:
		if (b_onceR)
		{
			nCount = 0;
			Query("SELECT * FROM rank ORDER BY score DESC LIMIT 10;"); //쿼리 처리
			b_onceR = FALSE;
		}
		break;
	}

	Render();
}

////*화면에 그림*////
VOID Render()
{
	int i;
	char buffer[256]; //출력을 위한 문자열 버퍼
	RECT rNext = {0, 0, 64, 64}, lot; //버튼 범위, 글자출력 위치
	D3DXMATRIX mat1;
	D3DXPLANE pl(1.0f, 0.0f, 0.0f, 0.0f);

	d3d.getDevice()->Clear(0, NULL,
	D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
	D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	if (SUCCEEDED(d3d.getDevice()->BeginScene())) //렌더링 시작을 알림
	{
		d3d.getSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		switch (manager.nState)
		{
			////*메인 화면*////
		case en_Main:
			//메인 화면 배경 그리기
			d3d.getSprite()->Draw(g_pTitleT, NULL, 0, &D3DXVECTOR3(0,0,0.0f), D3DCOLOR_XRGB(255, 255, 255));
			//마우스 위치에 따른 버튼 변경
			if (!bStartOver) //마우스 오버가 아닐때
			{
				d3d.getSprite()->Draw(g_pStart[0], NULL, 0, &D3DXVECTOR3(600, 100, 0), D3DCOLOR_XRGB(255, 255, 255));
			}
			else //마우스 오버일때
			{
				d3d.getSprite()->Draw(g_pStart[1], NULL, 0, &D3DXVECTOR3(600, 100, 0), D3DCOLOR_XRGB(255, 255, 255));
			}
			if (!bRankOver) //마우스 오버가 아닐때
			{
				d3d.getSprite()->Draw(g_pRank[0], NULL, 0, &D3DXVECTOR3(600, 238, 0), D3DCOLOR_XRGB(255, 255, 255));
			}
			else //마우스 오버일때
			{
				d3d.getSprite()->Draw(g_pRank[1], NULL, 0, &D3DXVECTOR3(600, 238, 0), D3DCOLOR_XRGB(255, 255, 255));
			}

			break;
			////*게임화면*////
		case en_Game:
			////*draw*////
			bg.Update(d3d.getSprite()); //배경 업데이트
			hurdle.Update(d3d.getSprite()); //허들 업데이트
			Hero.Update(d3d.getSprite()); //캐릭터 업데이트

			//라이프 그림
			for (i = 0; i < Hero.GetLife(); i++)
			{
				d3d.getSprite()->Draw(g_pLife, NULL, NULL, &D3DXVECTOR3(910 + (i * 20), 10, 0), D3DCOLOR_XRGB(255, 255, 255));
			}

			break;
		case en_GameOver:
			d3d.getSprite()->Draw(g_pGOver, NULL, 0, &D3DXVECTOR3(0, 0, 0.0f), D3DCOLOR_XRGB(255, 255, 255));
			
			D3DXMatrixReflect(&mat1, &pl); //주어진 좌표계를 반전함
			d3d.getSprite()->SetTransform(&mat1);
			d3d.getSprite()->Draw(g_pBack, &rNext, 0, &D3DXVECTOR3(-1000, 408, 0.0f), D3DCOLOR_XRGB(255, 255, 255)); //백버튼 이미지를 좌우반전해서 씀
			D3DXMatrixIdentity(&mat1); //원상복구
			d3d.getSprite()->SetTransform(&mat1);
			
			//문자 출력
			wsprintf(buffer, "SCORE	 :  %d", nScore);
			SetRect(&lot, 0, 200, Hres, Vres);
			d3dFont.TextDraw(buffer, lot, DT_CENTER);
			//스코어가 있을 때만 기록을 받음
			if (nScore > 0)
			{
				wsprintf(buffer, "NickName : %s", cBuf);
				SetRect(&lot, 0, 240, Hres, Vres);
				d3dFont.TextDraw(buffer, lot, DT_CENTER);
			}
			
			break;
		case en_Ranking:
			d3d.getSprite()->Draw(g_pRanking, NULL, 0, &D3DXVECTOR3(0, 0, 0.0f), D3DCOLOR_XRGB(255, 255, 255));
			d3d.getSprite()->Draw(g_pBack, NULL, 0, &D3DXVECTOR3(10, 10, 0.0f), D3DCOLOR_XRGB(255, 255, 255));

			//db쿼리 결과 랭킹 출력
			for (i = 0; i < nCount; i++)
			{
				SetRect(&lot, 350, 10 + i * 50, Hres, Vres);

				switch (ranking[i].nRank)
				{
				case 1: //트로피 순위따라 3위까지 이미지출력
					d3d.getSprite()->Draw(g_pTrophy[0], NULL, 0, &D3DXVECTOR3(300, 10 + i * 50, 0.0f), D3DCOLOR_XRGB(255, 255, 255));
					wsprintf(buffer, "%d. %t %t %s %t %s", ranking[i].nRank, ranking[i].cScore, ranking[i].cName);
					d3dFont.TextDraw(buffer, lot, DT_LEFT);
					break;
				case 2:
					d3d.getSprite()->Draw(g_pTrophy[1], NULL, 0, &D3DXVECTOR3(300, 10 + i * 50, 0.0f), D3DCOLOR_XRGB(255, 255, 255));
					wsprintf(buffer, "%d. %t %t %s %t %s", ranking[i].nRank, ranking[i].cScore, ranking[i].cName);
					d3dFont.TextDraw(buffer, lot, DT_LEFT);
					break;
				case 3:
					d3d.getSprite()->Draw(g_pTrophy[2], NULL, 0, &D3DXVECTOR3(300, 10 + i * 50, 0.0f), D3DCOLOR_XRGB(255, 255, 255));
					wsprintf(buffer, "%d. %t %t %s %t %s", ranking[i].nRank, ranking[i].cScore, ranking[i].cName);
					d3dFont.TextDraw(buffer, lot, DT_LEFT);
					break;
				default: //기타 순위
					wsprintf(buffer, "%d. %t %t %s %t %s", ranking[i].nRank, ranking[i].cScore, ranking[i].cName);
					d3dFont.TextDraw(buffer, lot, DT_LEFT);
					break;
				}
			}
			
			break;
		}
		d3d.getSprite()->End();

		d3d.getDevice()->EndScene(); //렌더링 끝을 알림
	}

	//화면을 전환함
	d3d.getDevice()->Present(NULL, NULL, NULL, NULL);
}

////게임관련 코드////
//충돌 코드
VOID Attack()
{
	int i, length;
	RECT hero, enemy;

	length = hurdle.GetCount();
	hero = Hero.GetLotation();

	for (i = 0; i < length; i++)
	{
		//한번 충돌하면 두번 생명을 깎지 않게 함.
		if (!hurdle.GetCrash(i))
		{
			enemy = hurdle.GetBound(i);
		
			//충돌체크
			if (hero.left <= enemy.right && hero.right >= enemy.left)
			{
				if (hero.top <= enemy.bottom && hero.bottom >= enemy.top)
				{
					Hero.MinusLife(1); //생명을 하나 차감함
					hurdle.SetCrash(i, TRUE); //i번째 허들이 충돌되었음을 등록함
				}
			}
		}
		
	}
	
}

//이미지 버튼 처리
VOID ButtonProcess(int x, int y)
{
	char buf[256] = "\0";

	switch (manager.nState)
	{
	case en_Main:
		if (x >= 600 && x <= 856)
		{
			if (y >= 100 && y <= 228) //start버튼
			{
				chSound.Stop(g_pDS[0]); //메인화면 브금 stop
				manager.nState = en_Game;
			}
			if (y >= 238 && y <= 366) //Rank 버튼
			{
				manager.nState = en_Ranking;
			}
		}
		break;
	case en_Game:
		
		break;
	case en_GameOver:
		if (x >= 936 && x <= 1000)
		{
			if (y >= 408 && y <= 472) //next 버튼
			{
				manager.nState = en_Main;
				chSound.Stop(g_pDS[0]);
				chSound.Play(g_pDS[0], TRUE);

				if (nScore > 0)
				{
					//기록 추가(db)
					wsprintf(buf, "INSERT INTO rank (nickname, score) VALUES(\"%s\", %d);", cBuf, nScore);
					Query(buf);
				}

				memset(cBuf, 0, sizeof(cBuf));
			}
		}
		break;
	case en_Ranking:
		if (x >= 10 && x <= 74)
		{
			if (y >= 10 && y <= 74) //back 버튼
				manager.nState = en_Main;
		}
		break;
	}

}

/////기타/////
//사운드 초기화, 불러오기
VOID SoundInit()
{
	chSound.Init(hWnd);
	chSound.LoadWave((char*)"Sound/Feel Better - Roa Music.wav", &g_pDS[0]);
	chSound.LoadWave((char*)"Sound/happy - MusicbyAden.wav", &g_pDS[1]);
	chSound.LoadWave((char*)"Sound/sound112.wav", &g_pDS[2]);

	chSound.SetVolume(g_pDS[0], 100);
	chSound.SetVolume(g_pDS[1], 100);
	chSound.SetVolume(g_pDS[2], 80);
}

//텍스처 객체 적재
VOID LoadTexture()
{
	D3DXIMAGE_INFO info;
	
	//배경화면
	D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\Title.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pTitleT);
	D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\Ranking.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pRanking);
	D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\GameOver.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pGOver);
	
	//버튼 텍스처들
	{
		D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\start.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pStart[0]);
		D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\start2.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pStart[1]);
		D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\rank.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pRank[0]);
		D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\rank2.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pRank[1]);
		D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\Back.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pBack);
	}
	
	D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\gold.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pTrophy[0]);
	D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\silver.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pTrophy[1]);
	D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\bronze.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pTrophy[2]);
	D3DXCreateTextureFromFileEx(d3d.getDevice(), "Image\\Life.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, &info, NULL, &g_pLife);
}

VOID Release()
{
	int i = 0;

	if (g_pTitleT != NULL)
		g_pTitleT->Release();
	if (g_pRanking != NULL)
		g_pRanking->Release();
	if (g_pGOver != NULL)
		g_pGOver->Release();

	for (i = 0; i < 2; i++)
	{
		if (g_pStart[i] != NULL)
			g_pStart[i]->Release();
		if (g_pRank[i] != NULL)
			g_pRank[i]->Release();
	}
	
	for (i = 0; i < 3; i++)
	{
		g_pTrophy[i]->Release();
		g_pDS[i]->Release();
	}

	if (g_pBack != NULL)
		g_pBack->Release();

	if (g_pLife != NULL)
		g_pLife->Release();
}

VOID Query(const char* query)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	int i = 0;
	int nRet;

	if (sqlite3_open("smartrun.db", &db) == SQLITE_OK)
	{
		if (sqlite3_prepare(db, query, -1, &stmt, 0) == SQLITE_OK)
		{
			while (1)
			{
				nRet = sqlite3_step(stmt);
				i = 0;

				if (nRet == SQLITE_ROW)
				{
					strcpy(ranking[nCount].cName, (char*)sqlite3_column_text(stmt, i++));
					strcpy(ranking[nCount].cScore, (char*)sqlite3_column_text(stmt, i));
					ranking[nCount].nRank = nCount + 1;
					nCount++;
				}
				else //SQLITE_DONE
				{
					break;
				}
			}
		}
	}
	else
	{
		MessageBox(hWnd, "DB connection Failed", NULL, MB_OK);
		manager.nState = en_Main;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

}