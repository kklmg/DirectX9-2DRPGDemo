#include"Ornament.h"

Cornament::Cornament(int x, int y, DWORD width, DWORD Height, CSheetFile *ref, LPCSTR str, bool Collide)
	:m_prefBmpSheet(ref), m_strName(str)
{
	CRECTNode *node= m_prefBmpSheet->GetRectNode(str);
	m_SrcRect = node->m_RECTORG;

	CObject::Init(x, y, width, Height);

	//CObject::m_bDrawFirst = TRUE;

	CObject::m_enObjType = enObjectType::eOTOrnament;

	//get scaled rect
	node->TransformRect(m_rectDraw);
	if (Collide)
	{
		CObject::CreateCollideArea(FALSE);

		if (node->m_vector_child.size() == 0) 
		{		
			RECT collision = m_rectDraw;
			
			int tempHeight = GetRectHeight(collision);
			collision.top += tempHeight / 3;

			CObject::m_pCollideArea->AddCollideRect(collision);
		}
		else
		{
			for (int i = 0; i < node->m_vector_child.size(); ++i) 
			{
				CObject::m_pCollideArea->AddCollideRect(node->m_vector_child[i]->m_RECT);
			}
		}	
	}
}
Cornament::Cornament(RECT rect, CSheetFile *ref, LPCSTR str, bool Collide) :m_prefBmpSheet(ref), m_strName(str)
{
	CRECTNode *node = m_prefBmpSheet->GetRectNode(str);
	m_SrcRect = node->m_RECTORG;

	POINT pt = GetRectCenter(rect);

	CObject::Init(pt.x, pt.y, GetRectWidth(rect), GetRectHeight(rect));

	CObject::m_bDrawFirst = TRUE;

	CObject::m_enObjType = enObjectType::eOTOrnament;

	//get scaled rect
	node->TransformRect(m_rectDraw);
	if (Collide)
	{
		CObject::CreateCollideArea(FALSE);

		if (node->m_vector_child.size() == 0)
		{
			RECT collision = m_rectDraw;

			int tempHeight = GetRectHeight(collision);
			collision.top += tempHeight / 3;

			CObject::m_pCollideArea->AddCollideRect(collision);
		}
		else
		{
			for (int i = 0; i < node->m_vector_child.size(); ++i)
			{
				CObject::m_pCollideArea->AddCollideRect(node->m_vector_child[i]->m_RECT);
			}
		}
	}

}
Cornament::~Cornament()
{



}

CSheetFile* m_prefBmp;

bool Cornament::UpdateCollideRect()
{
	return TRUE;
}

void Cornament::UpdateAll()
{
	//CObject::UpdateObject();
}


HRESULT Cornament::DrawObject(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting)
{
	return m_prefBmpSheet->Draw(DestSuf, m_rectDraw, m_SrcRect, DrawSetting);
}