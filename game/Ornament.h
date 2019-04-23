#include"ObjectClass.h"
#include"BmpFile.h"

class Cornament : public CObject 
{
public:
	Cornament(int x, int y, DWORD width, DWORD Height, CSheetFile *ref, LPCSTR str, bool Collide);
	Cornament(RECT rect, CSheetFile *ref, LPCSTR str, bool Collide);
	virtual~Cornament();


private:
	CSheetFile* m_prefBmpSheet;

	RECT m_SrcRect;

	string m_strName;

	virtual bool UpdateCollideRect();

	virtual void UpdateAll();

	//Draw
	virtual HRESULT DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting);

};

