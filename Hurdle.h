#ifndef H_Hurdle_H
#define H_Hurdle_H

#include "Graphic.h"
#include <deque>

using namespace std;

typedef struct HurdleInfo
{
	int nKind;
	int nX;
	BOOL bCrash;
}HurdleInfo;

class Hurdles : public GraphicClass
{
public:
	Hurdles();
	~Hurdles();

	//»ó¼Ó
	void Init(LPDIRECT3DDEVICE9 device);
	void Update(LPD3DXSPRITE sprite);
	void Release();

	void Generate(int nKind, int nX);
	void ScoreUp();
	int GetCount();
	int GetScore();
	int GetKind(int nIndex);
	RECT GetBound(int nIndex);
	BOOL GetCrash(int nIndex);
	VOID SetCrash(int nIndex, BOOL bcrash);

private:
	LPDIRECT3DTEXTURE9 m_pWood = NULL;
	LPDIRECT3DTEXTURE9 m_pStone = NULL;
	LPDIRECT3DTEXTURE9 m_pStump = NULL;

	deque <HurdleInfo> hurdle;
	int count;
	int mnScore;
};



#endif // !H_Huddle_H



