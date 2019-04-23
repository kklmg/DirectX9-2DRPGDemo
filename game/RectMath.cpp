#include"RectMath.h"
#include<vector>

int GetRectWidth(RECT &_rect)
{
	return _rect.right - _rect.left;
}

int GetRectHeight(RECT &_rect)
{
	return _rect.bottom - _rect.top;
}

int GetRectOblique(RECT &_rect) 
{
	int Height = GetRectHeight(_rect);
	int Width = GetRectWidth(_rect);
		
	return sqrt(Width*Width + Height*Height);
}

POINT GetRectCenter(RECT &_rect)
{
	POINT rectCT;
	rectCT.x = _rect.left + ((_rect.right - _rect.left) >> 1);
	rectCT.y = _rect.top + ((_rect.bottom - _rect.top) >> 1);
	return rectCT;
}


_CIRCLE GetCircleInvRect(RECT &rect)
{
	POINT center = GetRectCenter(rect);
	float Radius = (GetRectOblique(rect) >> 1);

	return { center ,Radius };
}







CRECTNode::CRECTNode()
	:m_pParent(NULL), m_RECT({0}),
	m_dwWidth(0), m_dwHeight(0), m_nOffset_x(0), m_nOffset_y(0), m_RECTORG({0})
{
	m_vector_child.resize(0);
}

CRECTNode::CRECTNode(RECT rect, LPCSTR str) 
	: m_pParent(NULL), m_RECT(rect), m_nOffset_x(0), m_nOffset_y(0), m_RECTORG(rect),m_strName(str)
{
	Init();
}

CRECTNode::~CRECTNode()
{
	m_pParent = NULL;
	DestroyChildren();
}

void CRECTNode::DestroyChildren() 
{
	if (m_vector_child.size() != 0)
	{
		//travelsal
		std::vector<CRECTNode*>::iterator iter;
		for (iter = m_vector_child.begin(); iter != m_vector_child.end(); iter++)
		{
			delete (*iter);
			(*iter) = NULL;
		}
		m_vector_child.clear();
	}
}

void CRECTNode::AddRect(CRECTNode *rectnode)
{
	rectnode->m_pParent = this;
	rectnode->Init();
	m_vector_child.push_back(rectnode);
}

void CRECTNode::AddRect(RECT rect, LPCSTR str)
{
	CRECTNode *node = new CRECTNode(rect,str);

	AddRect(node);
}

void CRECTNode::TraversalNode(void(*ptrf)(RECT rect))
{
	if (ptrf == NULL) return;
	std::vector<CRECTNode*>::iterator iter;

	for (iter = m_vector_child.begin(); iter != m_vector_child.end(); iter++)
	{
		if ((*iter)->m_vector_child.size() != 0) 
		{
			(*iter)->TraversalNode(ptrf);
		}	
		ptrf((*iter)->m_RECT);
	}
}

void CRECTNode::TransformRect(float Hscale, float Vscale, int offset_x, int offset_y)
{
	if (m_pParent == NULL)
	{
		m_RECT = m_RECTORG;

		m_RECT.left += offset_x;
		m_RECT.top += offset_y;
		m_RECT.right = m_RECT.left + m_dwWidth*Hscale;
		m_RECT.bottom = m_RECT.top + m_dwHeight*Vscale;
	}
	else
	{
		m_RECT = m_RECTORG;

		m_RECT.left = m_nOffset_x*Hscale + m_pParent->m_RECT.left;
		m_RECT.top = m_nOffset_y*Vscale + m_pParent->m_RECT.top;
		m_RECT.right = m_RECT.left + Hscale*m_dwWidth;
		m_RECT.bottom = m_RECT.top + Vscale*m_dwHeight;
	}
	//Traversal
	std::vector<CRECTNode*>::iterator iter;
	for (iter = m_vector_child.begin(); iter != m_vector_child.end(); iter++)
	{
		if (m_vector_child.size() != 0)
		{
			(*iter)->TransformRect(Hscale, Vscale, offset_x, offset_y);
		}
	}
}

void CRECTNode::TransformRect(RECT rect)
{
	int offset_x = rect.left - m_RECTORG.left;
	int offset_y = rect.top - m_RECTORG.top;
	float Hscale = (float)GetRectWidth(rect) / (float)m_dwWidth;
	float Vscale= (float)GetRectHeight(rect) / (float)m_dwHeight;

	TransformRect(Hscale, Vscale, offset_x, offset_y);
}

RECT CRECTNode::FindRect(LPCSTR str)
{
	//Traversal
	std::vector<CRECTNode*>::iterator iter;
	for (iter = m_vector_child.begin(); iter != m_vector_child.end(); iter++)
	{
		if (m_vector_child.size() != 0)
		{
			if ((*iter)->m_strName == str)return (*iter)->m_RECT;
		}
	}
	
	return{ 0, 0, 0, 0 };
}

void CRECTNode::Init() 
{
	m_dwWidth = GetRectWidth(m_RECT);
	m_dwHeight = GetRectHeight(m_RECT);

	if (m_pParent != NULL) 
	{
		m_nOffset_x = m_RECT.left - m_pParent->m_RECT.left;
		m_nOffset_y = m_RECT.top - m_pParent->m_RECT.top;
	}
}

STRectData* CRECTNode::GetChildrenRect()
{
	STRectData* pRectData = NULL;
	if (m_vector_child.size() != 0) 
	{	
		pRectData = new STRectData;
		pRectData->pRect = new RECT[m_vector_child.size()];
		pRectData->nRectCount = m_vector_child.size();
		vector<CRECTNode*>::iterator iter;
		int i = 0;
		for (iter = m_vector_child.begin(); iter != m_vector_child.end(); iter++,i++) 
		{
			pRectData->pRect[i] = (*iter)->m_RECT;
		}
	}
	return pRectData;
}




//RECT GetScaleRect(RECT _rect, short refPoint, float _scale)
//{
//	RECT rt;
//	int Width = GetRectWidth(_rect)*_scale;
//	int Height = GetRectHeight(_rect)*_scale;
//	switch (refPoint) {
//
//	case reLT: 
//	{ 
//		rt = {	_rect.left,
//				_rect.top,
//				_rect.left + Width,
//				_rect.top + Height };
//	}break;
//	case reLB:
//	{
//		rt = {  _rect.left,
//				_rect.bottom-Height,
//				_rect.left + Width,
//				_rect.bottom };
//	}break;
//	case reRT: 
//	{ 
//		rt = {  _rect.right - Width,
//				_rect.top,
//				_rect.right,
//				_rect.top + Height };
//	}break;
//	case reRB:
//	{
//		rt = {  _rect.right - Width,
//				_rect.bottom - Height,
//				_rect.right,
//				_rect.bottom };
//	}break;
//	case reCT: {
//		POINT rectCT=GetRectCenter(_rect);
//		rt = {
//				rectCT.x - Width / 2,
//				rectCT.y - Height / 2,
//				rectCT.x + Width / 2,
//				rectCT.y + Height / 2 };
//	}break;
//	}
//	return rt;
//
//
//}

RECT CreateRect(POINT rectCT, int width, int height)
{
	RECT rect =
	{
		rectCT.x - (width >> 1) ,
		rectCT.y - (height >> 1),
		rectCT.x + (width >> 1),
		rectCT.y + (height >> 1),
	};
	return rect;

}

RECT GetScaledRect(RECT rect, float Scale_x, float Scale_y)
{
	DWORD rectWidth = GetRectWidth(rect)* Scale_x;
	DWORD rectHeight = GetRectHeight(rect)* Scale_y;
	POINT rectCenter = GetRectCenter(rect);


	RECT ScaledRect = CreateRect(rectCenter, rectWidth, rectHeight);

	return ScaledRect;
}




float CalculateDistance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float CalculateDistance(D3DXVECTOR2& v2_a, D3DXVECTOR2& v2_b)
{
	return CalculateDistance(v2_a.x, v2_a.y, v2_b.x, v2_b.y);
}

bool AreTheSameRect(RECT rectA, RECT rectB)
{
	if (rectA.left = rectB.left
		&&rectA.right == rectB.right
		&&rectA.top == rectB.top
		&&rectA.bottom == rectB.bottom)return TRUE;
	else return FALSE;

}

RECT GetHalfRect(RECT &rect) 
{
	RECT TempRrect = rect;
	
	TempRrect.bottom =(long) (GetRectHeight(rect)>>1)+ rect.top;

	return TempRrect;
}
