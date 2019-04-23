#include"TextFrame.h"
#include"Global_data.h"
#include<string>

RECT GetDestRect(enAlign align, int left, int top, int bottom, WORD Width, WORD Height)
{
	RECT Dest;

	Dest.left = left;
	Dest.right = left + Width;

	if (align == eAlign_Bottom)
	{
		Dest.top = bottom - Height;
		Dest.bottom = bottom;
	}
	if (align == eAlign_Middle)
	{
		int middle = top + ((bottom - top) >> 1);
		int halfHeight = Height >> 1;

		Dest.top = middle - halfHeight;
		Dest.bottom = middle + halfHeight;
	}
	if (align == eAlign_top)
	{
		Dest.top = top;
		Dest.bottom = top + Height;
	}
	return Dest;
}

STTextFrame::STTextFrame(LPCSTR szBitmap,COLORREF key, enAlign align) 
	:m_pSuf(NULL), m_bInitRect(FALSE), m_fScale_x(1), m_fScale_Y(1)
{
	m_pSuf=DDLoadBitmapFastEX(g_pDD, szBitmap, TRUE, key);

	m_wWidth = m_pSuf->RectEx.width;
	m_wHeight = m_pSuf->RectEx.Height;

	m_enAlign = align;
}

STTextFrame::~STTextFrame() 
{
	SAFE_RELEASE(m_pSuf);
}

void STTextFrame::__DrawText(LPDIRECTDRAWSURFACE7 Dest,int left, int top, int bottom)
{
	if (m_pSuf == NULL)return;

	m_rtDrawRect= GetDestRect(m_enAlign,left, top,  bottom, m_wWidth, m_wHeight);

	Dest->Blt(&m_rtDrawRect, m_pSuf->lpDDS, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}

void STTextFrame::SetScale(float x, float y) 
{
	m_fScale_x = x;
	m_fScale_Y = y;

	m_wWidth = m_pSuf->RectEx.width*m_fScale_x;
	m_wHeight = m_pSuf->RectEx.Height*m_fScale_Y;
}


CTFrameMng::CTFrameMng() 
{
}

CTFrameMng::~CTFrameMng() 
{
	m_mapiter = m_mapTextData.begin();

	for (; m_mapiter != m_mapTextData.end();) 
	{
		delete (*m_mapiter).second;
		

		m_mapTextData.erase(m_mapiter++);
	}

}

void CTFrameMng::Init() 
{
	int i;
	//Insert figure : 0,1,...9
	for (i = 48; i < 58; ++i) 
	{
		InsertText(i,char(i));	
	}

	//Insert AlphaBet A~Z
	for (i = 65; i < 91; ++i) 
	{
		InsertText(i, char(i));
	}

	//Insert AlphaBet a~z
	for (i = 97; i < 123; ++i) 
	{	
		string str = "s";
		str += char(i);

		InsertText(i, str.data());
	}
	//Insert Mark
	InsertText('#', '#');
	InsertText('$', '$');
	InsertText('%', '%');
	InsertText('&', '&');
	InsertText('@', '@');
	

	InsertText(' ', "_Blank");

	InsertText('+', '+', eAlign_Middle);
	InsertText('-', '-', eAlign_Middle);
	InsertText('*', "_Star", eAlign_top);

	InsertText('/', "_Slash", eAlign_Middle);
	
	InsertText('=', "_Equal", eAlign_Middle);

	InsertText('<', "_Less", eAlign_Middle);
	InsertText('>', "_Large", eAlign_Middle);

	

	InsertText('~', '~', eAlign_Middle);


	InsertText('.', "_Period");
	InsertText(',', "_Comma");
	InsertText('!', "_Exc", eAlign_Middle);
	InsertText('?', "_Que");
	InsertText(':', "_Colon", eAlign_Middle);
	InsertText(';', "_Semi", eAlign_Middle);

	InsertText('_', '_');
	InsertText('"', "_Quote",eAlign_top);
	
	InsertText('(', "_ParL", eAlign_Middle);
	InsertText(')', "_ParR", eAlign_Middle);
	InsertText('[', "_SqbL", eAlign_Middle);
	InsertText(']', "_SqbR", eAlign_Middle);

	//InsertText(' ', "_SQuote");
}

WORD CTFrameMng::GetFontWidth(char cr) 
{
	std::map<char, STTextFrame*>::iterator iter= m_mapTextData.find(cr);
	
	if (iter == m_mapTextData.end())return 0;

	else return (*iter).second->m_wWidth;
}

WORD CTFrameMng::Draw(char cr, LPDIRECTDRAWSURFACE7 Dest, int left, int top, int bottom)
{
	m_mapiter = m_mapTextData.find(cr);

	if (m_mapiter == m_mapTextData.end())return 0;
	else
	{
		(*m_mapiter).second->__DrawText(Dest, left, top, bottom);
		return (*m_mapiter).second->m_wWidth;
	}
}

bool CTFrameMng::Draw(LPCSTR str, LPDIRECTDRAWSURFACE7 Dest,RECT rect)
{
	int i = 0;
	int Left = rect.left;

	while (str[i] != '\0') 
	{
		m_mapiter = m_mapTextData.find(str[i]);
	
		if (m_mapiter == m_mapTextData.end()) 
		{
			++i;
			continue;
		}

		(*m_mapiter).second->__DrawText(Dest, Left, rect.top, rect.bottom);

		Left += (*m_mapiter).second->m_wWidth;

		if (Left > rect.right)break;
		++i;
	}
	return TRUE;
}




void CTFrameMng::InsertText(char Cr, LPCSTR szBitmap,enAlign align)
{
	string str = "UI/FontSpilt/";
	str += szBitmap;
	str += ".bmp";
	
	m_mapTextData[Cr] = new STTextFrame(str.data(), RGB(0, 0, 0), align);
	//m_mapTextData[Cr]->SetScale(5, 5);
}

void CTFrameMng::InsertText(char Cr, char Bitmap, enAlign align)
{
	string str = "UI/FontSpilt/";
	str += Bitmap;
	str += ".bmp";

	m_mapTextData[Cr] = new STTextFrame(str.data(), RGB(0, 0, 0), align);
	//m_mapTextData[Cr]->SetScale(5,5);
}

