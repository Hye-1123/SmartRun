#ifndef H_CHARACTER_H
#define H_CHARACTER_H

#include "Graphic.h"

typedef struct SpriteAni
{
	int count;
	RECT *r_frame;
	FLOAT f_LastTime;
}HeroSprite;

class CharacterClass : GraphicClass
{
public:
	CharacterClass();
	~CharacterClass();

	//»ó¼Ó
	void Init(LPDIRECT3DDEVICE9 device);
	void Update(LPD3DXSPRITE sprite);
	void Release();

	////
	int GetLife();
	VOID MinusLife(int n);
	RECT GetLotation();
	VOID SetbJump(BOOL b_Jump);
	
protected:
	VOID ChangeFrame();
	VOID FadeAlpha();

private:
	LPDIRECT3DTEXTURE9 m_pCh, m_pChJump;
	HeroSprite mhero;
	int mnFrame;
	int mnLife;
	int mnAlpha;
	BOOL bCrash;
	BOOL bJump;
	BOOL bLanding;
};



#endif // !H_CHARACTER_H

