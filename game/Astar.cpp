#include"Astar.h"
#include"Global_data.h"

void DeleteScore(STSlot<STPathScore*> deldata)
{
	delete deldata.m_Value;
}

bool STPathUpdate::UpdateCurScene(CTileMap* Curmap)
{
	if (m_CurScene != g_enCurScene)
	{
		m_CurScene = g_enCurScene;
		m_enFindResult = eDefault;
		m_pCurMap = Curmap;

		return TRUE;
	}
	else return FALSE;
}