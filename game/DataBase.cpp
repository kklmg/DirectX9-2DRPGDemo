#include"DataBase.h"
#include"ddutil.h"
#include"Global_data.h"
//#include"TilePalette.h"

CDataMng<LPDIRECTDRAWSURFACE7> *g_pDDS_Emoticon = NULL;

CDataMng<STAnimationDesc> *g_AniDesc_Emoticon = NULL;


LPDIRECTDRAWSURFACE7 __CreateDDS(LPCSTR bmpfile, COLORREF key)
{
	LPDIRECTDRAWSURFACE7 lpDDS = NULL;
	//create surface
	lpDDS = DDLoadBitmap(g_pDD, bmpfile, 0, 0);

	//set color key
	DDSetColorKey(lpDDS, key);

	return lpDDS;
}

/////////////////
//Data Mng temp//
/////////////////
template<typename T>
CDataMng<T>::CDataMng(DWORD count, T InitValue, void(*fptr)(DWORD index)) 
	:m_fptrCreate(fptr), m_vector(count,InitValue){}

template<typename T>
CDataMng<T>::~CDataMng()
{
	m_fptrCreate = NULL;
	m_vector.clear();
}

template<typename T>
T CDataMng<T>::GetData(DWORD index)
{
	m_fptrCreate(index);
	return m_vector[index];
}


CDataMng<LPDIRECTDRAWSURFACE7>::~CDataMng()
{
	for (int i = 0; i < m_vector.size(); i++)
	{
		if (m_vector[i] != NULL)
		{
			m_vector[i]->Release();
			m_vector[i] = NULL;
		}			
	}
	m_vector.clear();
}

////////////
//DataBase//
////////////

CDataBase::CDataBase() 
	: m_vector_TileSheet(10,NULL),m_vector_ImgBuilding(50,NULL), m_vector_SheetFile(10,NULL),
	m_vector_SpriteSheet(50,NULL),m_vector_SpriteRow(50,NULL),m_vector_Tile(80,NULL),
	m_pKnightImg(NULL), m_pArcherImg(NULL)
{
}

CDataBase::~CDataBase()
{
	//SAFE_RELEASE(m_pDDSMng);

	SAFE_RELEASE(g_pDDS_Emoticon);
	SAFE_RELEASE(g_AniDesc_Emoticon);

	m_TileSheetSrc.ClearData();
	m_TileSrc.ClearData();


	ClearAll();
}

bool CDataBase::Init()
{
	InitEmoticonData();
	ParsingTileData("Data/Tile.txt");
	ParsingTileSheetData("Data/TileSheet.txt");


	return TRUE;
}

void CDataBase::ParsingTileData(LPCSTR FilePath)
{
	m_TileDataFile.Parsing(FilePath);
	int testsize = m_TileDataFile.m_DataCon.GetSize();
}
void CDataBase::ParsingTileSheetData(LPCSTR FilePath) 
{
	m_TSDataFile.Parsing(FilePath);
}

//void CDataBase::CreateTileAll(std::vector<CTilePalette*> &GetVector)
//{
//	m_TileDataFile.
//
//
//	CTilePalette* palette = new CTilePalette(50);
//
//	
//	int count = m_TileDataFile.m_DataCon.GetSize();
//	
//	
//
//	PltRow 
// PltColumn 
//
//
//	STTileSrc* src = GetTile("");
//	CTile* newTile = NULL;
//	if (src->bMulti) 
//	{
//		newTile = CTile_Dynamic(,);
//	
//	}
//	=(,)
//
//
//}


CSheetFile * CDataBase::GetSheetFile(DWORD index) 
{
	if (index > m_vector_SheetFile.size())return NULL;

	if (m_vector_SheetFile[index] == NULL) 
	{	
		InitSheetFile(index);	
	}
	return m_vector_SheetFile[index];
}


void CDataBase::ClearSheetFile() 
{
	vector<CSheetFile*>::iterator Iter;
	for (Iter = m_vector_SheetFile.begin(); Iter != m_vector_SheetFile.end(); Iter++)
	{
		if ((*Iter) != NULL)
		{
			delete (*Iter);
			(*Iter) = NULL;
		}
	}
}


void CDataBase::InitSheetFile(DWORD index) 
{
	switch (index)
	{
	case eOrnament_COM: 
	{	
		CRECTNode *NewNode;
		m_vector_SheetFile[index] = new CSheetFile();
		m_vector_SheetFile[index]->LoadSheetFile("UI/ORACOM.bmp", "ORACOM", TRUE, RGB(255, 0, 255));

		NewNode = new CRECTNode({ 772,1440,894,1533 });
		NewNode->AddRect({772,1440,801,1533});
		NewNode->AddRect({ 772,1496,894,1533 });
		NewNode->AddRect({ 865,1440,894,1533 });
		m_vector_SheetFile[index]->AddSubRect(NewNode, "COUNTER00");

		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 900,1603,924,1630 }), "WALL_SHIELD00");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 932,1605,956,1629 }), "WALL_SHIELD01");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 933,1674,955,1693 }), "WALL_SWORD");




		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 896,1673,928,1693 }), "SWORD00");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 928,1829,960,1852 }), "SWORDTABLE");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 928,1797,960,1820 }), "FORGINGTABLE");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 961,1473,991,1504 }), "ARMOUR00");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 994,1473,1022,1504 }), "ARMOUR01");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 992,1803,1024,1852 }), "ARMOURHUGE");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 964,1419,988,1467 }), "CHEST00");	
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 368,1664,432,1728 }), "FURNACE00");
		m_vector_SheetFile[index]->AddSubRect(new CRECTNode({ 448,1668,480,1728 }), "FURNACE01");






	/*	m_vector_SheetFile[index]->AddSubRect({5,41,22,54}, "Grass01");
		m_vector_SheetFile[index]->AddSubRect({ 34,39,56,57 }, "Grass02");
		m_vector_SheetFile[index]->AddSubRect({ 63,34,91,62 }, "Grass03");
		m_vector_SheetFile[index]->AddSubRect({ 95,37,119,59 }, "Grass04");*/
	}
		break;
	case eOrnament_COM2:
	{
		m_vector_SheetFile[index] = new CSheetFile();
		m_vector_SheetFile[index]->LoadSheetFile("UI/ORACOM.bmp", "Sheet03", TRUE, RGB(255, 255, 255));
		//m_vector_SheetFile[index]->AddSubRect({ 5,41,22,54 }, "Grass01");
		//m_vector_SheetFile[index]->AddSubRect({ 34,39,56,57 }, "Grass02");
		//m_vector_SheetFile[index]->AddSubRect({ 63,34,91,62 }, "Grass03");
		//m_vector_SheetFile[index]->AddSubRect({ 95,37,119,59 }, "Grass04");
	}
		break;
	case eOrnament_GRASS:
	{
		m_vector_SheetFile[index] = new CSheetFile();
		m_vector_SheetFile[index]->LoadSheetFile("UI/ORACOM.bmp", "Sheet03", TRUE, RGB(255, 255, 255));
		/*m_vector_SheetFile[index]->AddSubRect({ 5,41,22,54 }, "Grass01");
		m_vector_SheetFile[index]->AddSubRect({ 34,39,56,57 }, "Grass02");
		m_vector_SheetFile[index]->AddSubRect({ 63,34,91,62 }, "Grass03");
		m_vector_SheetFile[index]->AddSubRect({ 95,37,119,59 }, "Grass04");*/
	}
	break;
	default:
		break;
	}
}

	









//Emoticon
void CDataBase::InitEmoticonData() 
{
	//Init Emoticon DDS
	g_pDDS_Emoticon = new CDataMng<LPDIRECTDRAWSURFACE7>(20, NULL, CreateEmoticonDDS);
	//g_pDDS_Emoticon->Init(20, NULL);
	//g_pDDS_Emoticon->SetCreateFptr(&CreateEmoticonDDS);
	

	//Init Emoticon Desc
	STAnimationDesc desc;
	g_AniDesc_Emoticon = new CDataMng<STAnimationDesc>(20, desc, CreateEmoticonDesc);
	//g_AniDesc_Emoticon->Init(20, desc);
	//g_AniDesc_Emoticon->SetCreateFptr(&CreateEmoticonDesc);

}

void CDataBase::CreateEmoticonDDS(DWORD EmoIndex)
{
	COLORREF colorkey = RGB(0, 128, 128);

	switch (EmoIndex)
	{
	case eAmaze:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo00.bmp", colorkey);
		break;
	case eQuestion:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo01.bmp", colorkey);
		break;
	case eThink:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo02.bmp", colorkey);
		break;
	case eBother:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo03.bmp", colorkey);
		break;
	case eHappy:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo04.bmp", colorkey);
		break;
	case eSpeak:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo05.bmp", colorkey);
		break;
	case eSleep:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo06.bmp", colorkey);
		break;
	case eIdea:		g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo07.bmp", colorkey);
		break;
	case eFright:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo08.bmp", colorkey);
		break;
	case eAwkward:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo09.bmp", colorkey);
		break;
	case eGrief:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo10.bmp", colorkey);
		break;
	case eDelight:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo11.bmp", colorkey);
		break;
	case eAngry:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo12.bmp", colorkey);
		break;
	case eSkull:	g_pDDS_Emoticon->m_vector[EmoIndex] = __CreateDDS("UI/Emo13.bmp", colorkey);
		break;
	default:
		break;
	}
}

void CDataBase::CreateEmoticonDesc(DWORD index)
{
	switch (index)
	{
	case eAmaze:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 3, 1, 1);
		break;
	case eQuestion:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	case eThink:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	case eBother:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	case eHappy:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	case eSpeak:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	case eSleep:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 6, 1, 1);
		break;
	case eIdea:		g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	case eFright:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	case eAwkward:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 5, 1, 1);
		break;
	case eGrief:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 5, 1, 1);
		break;
	case eDelight:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 6, 1, 1);
		break;
	case eAngry:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 2, 1, 1);
		break;
	case eSkull:	g_AniDesc_Emoticon->m_vector[index].SetAniSeqDesc(g_pDDS_Emoticon->GetData(index), 4, 1, 1);
		break;
	default:
		break;
	}
}

STAnimationDesc CDataBase::GetEmoticonDesc(DWORD index)
{
	return	g_AniDesc_Emoticon->GetData(index);
}


STTileSrc* CDataBase::GetTile(enTile etile)
{
	if (m_vector_Tile[etile] == NULL)
	{
		InitTile(etile);
	}
	return m_vector_Tile[etile];
}
STTileSrc* CDataBase::GetTile(LPCSTR TileName)
{
	STTileSrc* TileSrc = NULL;
	FSTTileData* TileDataFile = NULL;

	std::list<std::string>::iterator iter;
	//Find Tile Sheet

	if (TileSrc = m_TileSrc.GetData(TileName))
		return TileSrc;

	//find from File
	else if (TileDataFile = m_TileDataFile.m_DataCon.GetData(TileName))
	{	
		CTileSheet* TS = GetTileSheet(TileDataFile->RefTileSheet.data());
		if (TS == NULL)
			return NULL;
		//Create New Tile
		TileSrc = new STTileSrc(
		{ TileDataFile->RefRow,TileDataFile->RefColumn},
			TS,
			TileDataFile->TileType,
			TileDataFile->TileObsLevel		
		);
	
		//Save to map container
		m_TileSrc.AddData(TileName, TileSrc);

		return TileSrc;
	}

	else return NULL;


}

void CDataBase::GetAllTileData(std::list<FSTTileData*> &GetList) 
{
	m_TileDataFile.m_DataCon.GetAllValue(GetList);
}

void CDataBase::ClearTile() 
{
	vector<STTileSrc*>::iterator TileIt;
	for (TileIt = m_vector_Tile.begin(); TileIt != m_vector_Tile.end(); TileIt++)
	{
		if ((*TileIt) != NULL)
		{
			delete (*TileIt);
			(*TileIt) = NULL;
		}
	}
}

void CDataBase::InitTile(enTile etile)
{
	switch (etile)
	{
	case eTile_Black:	m_vector_Tile[etile] = new STTileSrc({ 0,1 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Grass_Light01:	m_vector_Tile[etile] = new STTileSrc({ 3,5 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Grass_Light02:	m_vector_Tile[etile] = new STTileSrc({ 4,5 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Grass_Light03:	m_vector_Tile[etile] = new STTileSrc({ 0,4 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Grass_Dusk:	m_vector_Tile[etile] = new STTileSrc({ 3,4 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Grass_Dark01:	m_vector_Tile[etile] = new STTileSrc({ 4,1 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Grass_Dark02:	m_vector_Tile[etile] = new STTileSrc({ 0,2 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_WhiteOval:	m_vector_Tile[etile] = new STTileSrc({ 9,0 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_WhiteOval_Chipped:	m_vector_Tile[etile] = new STTileSrc({ 9,4 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_BrownOval:	m_vector_Tile[etile] = new STTileSrc({ 9,1 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_BrownOval_Chipped:	m_vector_Tile[etile] = new STTileSrc({ 9,5 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_WhiteRect:	m_vector_Tile[etile] = new STTileSrc({ 8,0 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_WhiteRect_Chipped:	m_vector_Tile[etile] = new STTileSrc({ 8,4 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_WhiteRect02:	m_vector_Tile[etile] = new STTileSrc({ 8,0 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_SlabStone_WhiteRect03:	m_vector_Tile[etile] = new STTileSrc({ 8,3 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Star01:	m_vector_Tile[etile] = new STTileSrc({ 2,5 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Star02:	m_vector_Tile[etile] = new STTileSrc({ 6,1 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Circle:	m_vector_Tile[etile] = new STTileSrc({ 0,5 }, GetTileSheet(enTileSheet::eTSBasic));
		break;
	case eTile_Water_Shallow:	m_vector_Tile[etile] = new STTileSrc({ 5,3 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Water_Deep:	m_vector_Tile[etile] = new STTileSrc({ 5,5 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_WaterFall01:	m_vector_Tile[etile] = new STTileSrc({ 1,0 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_WaterFall02:	m_vector_Tile[etile] = new STTileSrc({ 2,0 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_WaterFall03:	m_vector_Tile[etile] = new STTileSrc({ 3,0 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_LT:	m_vector_Tile[etile] = new STTileSrc({ 6,6 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_T:		m_vector_Tile[etile] = new STTileSrc({ 6,7 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_RT:	m_vector_Tile[etile] = new STTileSrc({ 6,8 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_L:		m_vector_Tile[etile] = new STTileSrc({ 7,6 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_R:		m_vector_Tile[etile] = new STTileSrc({ 7,8 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_LB:	m_vector_Tile[etile] = new STTileSrc({ 8,6 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_B:		m_vector_Tile[etile] = new STTileSrc({ 8,7 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Beach_RB:	m_vector_Tile[etile] = new STTileSrc({ 8,8 }, GetTileSheet(enTileSheet::eTSWater));
		break;
	case eTile_Cliff01:		m_vector_Tile[etile] = new STTileSrc({ 3,0 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff02:		m_vector_Tile[etile] = new STTileSrc({ 3,1 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff03:		m_vector_Tile[etile] = new STTileSrc({ 3,2 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_LT:	m_vector_Tile[etile] = new STTileSrc({ 0,0 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_T:		m_vector_Tile[etile] = new STTileSrc({ 0,1 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_RT:	m_vector_Tile[etile] = new STTileSrc({ 0,2 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_L:		m_vector_Tile[etile] = new STTileSrc({ 1,0 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_R:		m_vector_Tile[etile] = new STTileSrc({ 1,2 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_LB:	m_vector_Tile[etile] = new STTileSrc({ 2,0 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_B:		m_vector_Tile[etile] = new STTileSrc({ 2,1 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Cliff_RB:	m_vector_Tile[etile] = new STTileSrc({ 2,2 }, GetTileSheet(enTileSheet::eTSCliff));
		break;
	case eTile_Roof01:		m_vector_Tile[etile] = new STTileSrc({ 0,0 }, GetTileSheet(enTileSheet::eTSRoof));
		break;
	case eTile_Roof02:		m_vector_Tile[etile] = new STTileSrc({ 0,2 }, GetTileSheet(enTileSheet::eTSRoof));
		break;
	case eTile_Roof03:		m_vector_Tile[etile] = new STTileSrc({ 2,2 }, GetTileSheet(enTileSheet::eTSRoof));
		break;
	case eTile_StoneWall01:	m_vector_Tile[etile] = new STTileSrc({ 3,3 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_StoneWall02:	m_vector_Tile[etile] = new STTileSrc({ 3,4 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_StoneWall03:	m_vector_Tile[etile] = new STTileSrc({ 4,6 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_StoneWall04:	m_vector_Tile[etile] = new STTileSrc({ 4,7 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_WoodWall01:	m_vector_Tile[etile] = new STTileSrc({ 0,1 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_WoodWall02:	m_vector_Tile[etile] = new STTileSrc({ 1,4 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_WoodWall03:	m_vector_Tile[etile] = new STTileSrc({ 1,6 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_WoodWall04:	m_vector_Tile[etile] = new STTileSrc({ 4,2 }, GetTileSheet(enTileSheet::eTSWall));
		break;
	case eTile_Floor01:		m_vector_Tile[etile] = new STTileSrc({ 0,1 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	case eTile_Floor02:		m_vector_Tile[etile] = new STTileSrc({ 3,0 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	case eTile_Floor03:		m_vector_Tile[etile] = new STTileSrc({ 3,1 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	case eTile_Floor04:		m_vector_Tile[etile] = new STTileSrc({ 2,4 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	case eTile_Pillar01:	m_vector_Tile[etile] = new STTileSrc({ 1,0 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	case eTile_Pillar02:	m_vector_Tile[etile] = new STTileSrc({ 1,1 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	case eTile_Pillar03:	m_vector_Tile[etile] = new STTileSrc({ 1,2 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	case eTile_Pillar04:	m_vector_Tile[etile] = new STTileSrc({ 1,3 }, GetTileSheet(enTileSheet::eTSFloor));
		break;
	default:
		break;
	}
}

void CDataBase::InitTileSheet(DWORD index)
{
	switch (index)
	{
		case eTSBasic:
		{
			m_vector_TileSheet[index] = new CTileSheet_Single();
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileBasic.bmp", 10, 6); 
		} break;
		case eTSWall:	
		{
			m_vector_TileSheet[index] = new CTileSheet_Single();
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileWall.bmp", 6, 8);
		}break;
		case eTSGrass:
		{
			m_vector_TileSheet[index] = new CTileSheet_Single();
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileGrass.bmp", 8, 3);
		}break;
		case eTSFloor:
		{
			m_vector_TileSheet[index] = new CTileSheet_Single();
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileFloor.bmp", 8, 8);
		} break;
		case eTSCliff:
		{
			m_vector_TileSheet[index] = new CTileSheet_Single();
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileCliff.bmp", 5, 8);
		} break;
		case eTSRoof:
		{
			m_vector_TileSheet[index] = new CTileSheet_Single();
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileRoof.bmp", 4, 8);
		} break;
		case eTSWater:
		{
			m_vector_TileSheet[index] = new CTileSheet_Multi();
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileWater0.bmp", 13, 9);
			m_vector_TileSheet[index]->LoadBitMapFile("map/tile/TileWater1.bmp", 13, 9);
	
		}break;


	default:
		break;
	}
}




void CDataBase::ClearTileImg()
{
	vector<CTileSheet*>::iterator TileIt;
	for (TileIt = m_vector_TileSheet.begin(); TileIt != m_vector_TileSheet.end(); TileIt++)
	{
		if ((*TileIt) != NULL)
		{
			delete (*TileIt);
			(*TileIt) = NULL;
		}
	}
}

CTileSheet* CDataBase::GetTileSheet(DWORD index)
{
	if (m_vector_TileSheet[index] == NULL)
	{
		InitTileSheet(index);	
	}
	return m_vector_TileSheet[index];
}
CTileSheet* CDataBase::GetTileSheet(LPCSTR TileSheetName)
{
	CTileSheet* TileSheetSrc = NULL;
	FSTTileSheet* TSDataFile = NULL;

	std::list<std::string>::iterator iter;
	//Find Tile Sheet
	if (TileSheetSrc = m_TileSheetSrc.GetData(TileSheetName))
		return TileSheetSrc;

	//find from File
	else if (TSDataFile = m_TSDataFile.m_DataCon.GetData(TileSheetName))
	{
		iter = TSDataFile->FilePath.begin();

		//Create Static File
		if (TSDataFile->FilePath.size() == 1)
		{
			TileSheetSrc = new CTileSheet_Single();
			TileSheetSrc->LoadBitMapFile((*iter).data(), TSDataFile->Row, TSDataFile->Column);
		}
		//Create Dynamic Tile
		else
		{
			TileSheetSrc = new CTileSheet_Multi();
			while (iter != TSDataFile->FilePath.end())
			{
				TileSheetSrc->LoadBitMapFile((*iter).data(), TSDataFile->Row, TSDataFile->Column);
				++iter;
			}
		}
		m_TileSheetSrc.AddData(TileSheetName, TileSheetSrc);
		return TileSheetSrc;
	}

	else return NULL;
}

CBuildingImg* CDataBase::GetBuildingImg(DWORD index)
{
	if (m_vector_ImgBuilding[index] == NULL)
	{
		InitBuildingImg(index);
	}
	return m_vector_ImgBuilding[index];

}


void CDataBase::InitBuildingImg(DWORD index) 
{
	switch (index)
	{
	case eMyhome:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/myhome.bmp", RGB(255, 0, 255));
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 56,174,80,222 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 8,rtTop,54,222 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 82,rtTop,130,222 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 56,rtTop,80,174 });
	} break;
	case eNormalHouse1:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/NormalHouse1.bmp", RGB(255, 0, 255));
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 19,80,35,112 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 3,rtTop,19,112 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 35,rtTop,85,114 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 19,rtTop,35,80 });
	} break;
	case eNormalHouse2:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/NormalHouse2.bmp", RGB(255, 0, 255));
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 19,95,35,127 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 3,rtTop,19,127 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 35,rtTop,83,127 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 19,rtTop,35,95 });
	} break;
	case eNormalHouse3:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/NormalHouse3.bmp", RGB(255, 0, 255)); 
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 16,64,32,96 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 0,rtTop,16,96 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 32,rtTop,80,96 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 16,rtTop,32,64 });
	}break;
	case eAdvancedHouse1:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/AdvancedHouse1.bmp", RGB(255, 0, 255)); 
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 16 ,128, 48 ,158 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 0,rtTop,16,160 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 48,rtTop,144,160 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 16,rtTop,48,128 });
	} break;
	case eAdvancedHouse2:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/AdvancedHouse2.bmp", RGB(255, 0, 255)); 
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 47,139,65,160 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 4,rtTop,47,153 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 65,rtTop,108,153 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 47,rtTop,65,139 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 34,151,47,160 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 65,151,78,160 });
	} break;
	case eSpecialHouse1:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/SpecialHouse1.bmp", RGB(255, 0, 255));
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 41,65,57,79 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 2,rtTop,41,80 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 57,rtTop,98,80 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 41,rtTop,57,65 });
	} break;
	case eSmith:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/Smith.bmp", RGB(255, 0, 255)); 
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 64,214,112,266 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 0,rtTop,65,280 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 111,rtTop,176,283 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 65,rtTop,113,214 });	
	} break;
	case eGroceryStore:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/GroceryStore.bmp", RGB(255, 0, 255)); 
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 46,75,66,94 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 5,rtTop,46,96 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 66,rtTop,96,96 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 16,rtTop,66,75 });
	} break;
	case eFactory:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/Factory.bmp", RGB(255, 0, 255)); 
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();
		m_vector_ImgBuilding[index]->AddLinkRect({ 117,270,145,312 });
		m_vector_ImgBuilding[index]->AddLinkRect({ 121,214,142,247 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 56,268,117,312 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 144,268,179,313 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 12,rtTop,56,292 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 178,rtTop,205,302 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 51,rtTop,121,239 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 143,rtTop,183,243 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 121,rtTop,142,215 });
	
	
	
	} break;
	case eChurch:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/Church.bmp", RGB(255, 0, 255));
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();

		m_vector_ImgBuilding[index]->AddLinkRect({ 47,139,65,159 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 4,rtTop,48,159 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 64,rtTop,108,159 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 48,rtTop,66,140 });
	}break;
	case eWareHouse:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/WareHouse.bmp", RGB(255, 0, 255) );
		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();

		m_vector_ImgBuilding[index]->AddLinkRect({ 68, 138, 98, 175 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 1,rtTop,69,176 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 98,rtTop,157,176 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 68,rtTop,98,138 });	
	}break; 
	case eGuild:
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/Guild.bmp", RGB(255, 0, 255));

		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();

		m_vector_ImgBuilding[index]->AddLinkRect({ 113,188,161,237 });

		m_vector_ImgBuilding[index]->AddCollideRect({ 18,rtTop,114,206 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 158,rtTop,254,205 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 110,rtTop,163,188 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 78,203,114,237 });
		m_vector_ImgBuilding[index]->AddCollideRect({ 159,203,195,237});
	}break;
	case ePalace://removed
	{
		m_vector_ImgBuilding[index] = new CBuildingImg("map/house/Palace.bmp", RGB(255, 0, 255)); 

		WORD rtTop = m_vector_ImgBuilding[index]->GetCommonTop();

		m_vector_ImgBuilding[index]->AddLinkRect({ 52,188,166,223 });
	}break;

	//case eTileWall:	m_vector_ImgBuilding[index] = new CBuildingImg("map/tile/TileWall.bmp", 6, 8, RGB(255, 255, 255)); break;
	//case eTileGrass:m_vector_ImgBuilding[index] = new CBuildingImg("map/tile/TileGrass.bmp", 8, 3, RGB(255, 255, 255)); break;
	//case eTileFloor:m_vector_ImgBuilding[index] = new CBuildingImg("map/tile/TileFlower.bmp", 10, 10, RGB(255, 255, 255)); break;

	default:
		break;
	}

}


void CDataBase::ClearBuildingImg() 
{
	vector <CBuildingImg*>::iterator BuildingIt;
	for (BuildingIt = m_vector_ImgBuilding.begin(); BuildingIt != m_vector_ImgBuilding.end(); BuildingIt++)
	{
		if ((*BuildingIt) != NULL)
		{
			delete (*BuildingIt);
			(*BuildingIt) = NULL;
		}
	}
}

void CDataBase::InitSpriteSheet(DWORD index)
{
	switch (index)
	{
		case eSSMagician:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/Magician/walk.bmp", 4, 4, 1.1, 1.2, RGB(255, 0, 255));
			m_vector_SpriteSheet[index]->LoadAniNormalAttack("unit/Magician/attack.bmp", 4, 4, 1.6, 2.6, RGB(255, 0, 255));
		}break;

		case eSSWarrior:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/Warrior/walk.bmp", 4, 4, 1.1, 1.2, RGB(255, 0, 255));
			m_vector_SpriteSheet[index]->LoadAniNormalAttack("unit/Warrior/attack.bmp", 4, 4, 1.6, 2.6, RGB(255, 0, 255));
		}break;

		case eSSAssassin:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/Assassin/walk.bmp", 4, 4, 1.1, 1.2, RGB(255, 0, 255));
			m_vector_SpriteSheet[index]->LoadAniNormalAttack("unit/Assassin/attack.bmp", 4, 4, 1.6, 2.6, RGB(255, 0, 255));
		}break;

		case eNPC_01:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_01.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_02:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_02.bmp", 4, 3, 1.2, 1.1, RGB(255, 0, 255));
		}break;

		case eNPC_03:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_03.bmp", 4, 4, 1.1, 0.85, RGB(255, 0, 255));
		}break;

		case eNPC_04:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_04.bmp", 4, 4, 1, 1, RGB(255, 0, 255));
		}break;

		case eNPC_05:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_05.bmp", 4, 4, 1.2, 1.2, RGB(255, 0, 255));
		}break;

		case eNPC_06:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_06.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_07:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_07.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		
		}break;

		case eNPC_08:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_08.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_09:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_09.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_OLDM:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_OLDM.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_OLDF:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_OLDF.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_AS:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_AS.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_BOY:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_BOY.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_GIRL:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_GIRL.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_BOAT:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_BOAT.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_DOG:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_DOG.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_DOG2:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_DOG2.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_CAT:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_CAT.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_PIG:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_PIG.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		case eNPC_DEER:
		{
			m_vector_SpriteSheet[index] = new CCommonSpriteSheet();
			m_vector_SpriteSheet[index]->LoadAniWalk("unit/NPC/NPC_DEER.bmp", 4, 4, 1.3, 1.4, RGB(255, 0, 255));
		}break;

		default:
			break;
	}
}

CCommonSpriteSheet* CDataBase::GetSpriteSheet(DWORD index)
{
	if (m_vector_SpriteSheet[index] == NULL)
	{
		InitSpriteSheet(index);
	}
	return m_vector_SpriteSheet[index];

}

void CDataBase::ClearSpriteSheet()
{
	vector <CCommonSpriteSheet*>::iterator Iter;
	for (Iter = m_vector_SpriteSheet.begin(); Iter != m_vector_SpriteSheet.end(); Iter++)
	{
		if ((*Iter) != NULL)
		{
			delete (*Iter);
			(*Iter) = NULL;
		}
	}
}

void CDataBase::InitSpriteRow(DWORD index)
{
	switch (index)
	{
	case eSRRabbit:
	{
		m_vector_SpriteRow[index] = new CCommonSpriteRow();

		m_vector_SpriteRow[index]->LoadAniWalk(eLeft, "unit/monster/Rabbit/moveleft.bmp", 1, 1, 4, RGB(255, 0, 255));
		m_vector_SpriteRow[index]->LoadAniWalk(eRight, "unit/monster/Rabbit/moveright.bmp", 1, 1, 4, RGB(255, 0, 255));
		m_vector_SpriteRow[index]->LoadAniWalk(eUp, "unit/monster/Rabbit/moveup.bmp", 1, 1, 4, RGB(255, 0, 255));
		m_vector_SpriteRow[index]->LoadAniWalk(eDown, "unit/monster/Rabbit/movedown.bmp", 1, 1, 4, RGB(255, 0, 255));

		//attack				
		m_vector_SpriteRow[index]->LoadAniNormalAttack(eLeft, "unit/monster/Rabbit/atkleft.bmp", 1, 0.8, 1, RGB(255, 0, 255));
		m_vector_SpriteRow[index]->LoadAniNormalAttack(eRight, "unit/monster/Rabbit/atkright.bmp", 1, 0.8, 1, RGB(255, 0, 255));
		m_vector_SpriteRow[index]->LoadAniNormalAttack(eUp, "unit/monster/Rabbit/atkup.bmp", 1, 0.8, 1, RGB(255, 0, 255));
		m_vector_SpriteRow[index]->LoadAniNormalAttack(eDown, "unit/monster/Rabbit/atkdown.bmp",1, 0.8, 1, RGB(255, 0, 255));

	}break;




	default:
		break;
	}
	//		IMG_Hero[1]->LoadAniWalk("unit/npc5/walk.bmp", 1.3, 1.1, 4, 4, RGB(255, 0, 255));
	//		IMG_Hero[1]->LoadAniAttack("unit/npc5/attack.bmp", 1.5, 1, 4, 4, RGB(255, 0, 255));
	//		IMG_Hero[1]->LoadAniAttack2("unit/npc5/attack2.bmp", 1.5, 1, 4, 4, RGB(255, 0, 255));
}

CCommonSpriteRow* CDataBase::GetSpriteRow(DWORD index)
{
	if (m_vector_SpriteRow[index] == NULL)
	{
		InitSpriteRow(index);
	}
	return m_vector_SpriteRow[index];
}

void CDataBase::ClearSpriteRow()
{
	vector <CCommonSpriteRow*>::iterator Iter;
	for (Iter = m_vector_SpriteRow.begin(); Iter != m_vector_SpriteRow.end(); Iter++)
	{
		if ((*Iter) != NULL)
		{
			delete (*Iter);
			(*Iter) = NULL;
		}
	}

}


CKnightImage* CDataBase::GetKnightImg() 
{
	if (m_pKnightImg != NULL) return m_pKnightImg;

	m_pKnightImg = new CKnightImage;

	//no weapon
	m_pKnightImg->LoadAniWalk(eLeft, "unit/hero/walkleft.bmp", 1, 1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniWalk(eRight, "unit/hero/walkright.bmp", 1, 1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniWalk(eUp, "unit/hero/walkup.bmp", 1, 1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniWalk(eDown, "unit/hero/walkdown.bmp", 1, 1, 4, RGB(255, 0, 255));


	m_pKnightImg->LoadAniRun(eLeft, "unit/hero/runleft.bmp", 1.1, 1.1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniRun(eRight, "unit/hero/runright.bmp", 1.1, 1.1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniRun(eUp, "unit/hero/runup.bmp", 1.1, 1.1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniRun(eDown, "unit/hero/rundown.bmp", 1.1, 1.1, 4, RGB(255, 0, 255));


	m_pKnightImg->LoadAniStand(eLeft, "unit/hero/left.bmp", 1, 1, 1, RGB(255, 0, 255));
	m_pKnightImg->LoadAniStand(eRight, "unit/hero/right.bmp", 1, 1, 1, RGB(255, 0, 255));
	m_pKnightImg->LoadAniStand(eUp, "unit/hero/up.bmp", 1, 1, 1, RGB(255, 0, 255));
	m_pKnightImg->LoadAniStand(eDown, "unit/hero/down.bmp", 1, 1, 1, RGB(255, 0, 255));
	
	//use sword
	m_pKnightImg->LoadAniWalk_F(eLeft, "unit/hero/walkleft2.bmp", 1.7, 1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniWalk_F(eRight, "unit/hero/walkright2.bmp", 1.7, 1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniWalk_F(eUp, "unit/hero/walkup2.bmp", 1.5, 1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniWalk_F(eDown, "unit/hero/walkdown2.bmp", 1.3, 1, 4, RGB(255, 0, 255));


	m_pKnightImg->LoadAniRun_F(eLeft, "unit/hero/runleft2.bmp", 1.45, 1.1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniRun_F(eRight, "unit/hero/runright2.bmp", 1.45, 1.1, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniRun_F(eUp, "unit/hero/runup2.bmp", 1.9, 1.3, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniRun_F(eDown, "unit/hero/rundown2.bmp", 2, 1.3, 4, RGB(255, 0, 255));


	m_pKnightImg->LoadAniStand_F(eLeft, "unit/hero/standleft2.bmp", 1, 1, 1, RGB(255, 0, 255));
	m_pKnightImg->LoadAniStand_F(eRight, "unit/hero/standright2.bmp", 1, 1, 1, RGB(255, 0, 255));
	m_pKnightImg->LoadAniStand_F(eUp, "unit/hero/standup2.bmp", 1.3, 1, 1, RGB(255, 0, 255));
	m_pKnightImg->LoadAniStand_F(eDown, "unit/hero/standdown2.bmp", 1.45, 1, 1, RGB(255, 0, 255));

	//attack				
	m_pKnightImg->LoadAniNormalAttack(eLeft, "unit/hero/attackleft.bmp", 2.5, 1.3, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniNormalAttack(eRight, "unit/hero/attackright.bmp", 2.5, 1.3, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniNormalAttack(eUp, "unit/hero/attackup.bmp", 2.5, 1.5, 4, RGB(255, 0, 255));
	m_pKnightImg->LoadAniNormalAttack(eDown, "unit/hero/attackdown.bmp", 2.1, 1.9, 4, RGB(255, 0, 255));

	//portrait		
	m_pKnightImg->LoadPortrait("unit/hero/portrait.bmp", RGB(255, 255, 255));
			
	return m_pKnightImg;
}

CArcherImage* CDataBase::GetArcherImg() 
{
	if (m_pArcherImg == NULL) 
	{	
		m_pArcherImg = new CArcherImage();
		m_pArcherImg->LoadAniWalk("unit/Archer/walk.bmp", 4, 4, 1.1, 1.2,  RGB(255, 0, 255));
		m_pArcherImg->LoadAniNormalAttack("unit/Archer/attack.bmp", 4, 4, 1.4, 2.2, RGB(255, 0, 255));
		m_pArcherImg->LoadAniBowAttack("unit/Archer/attack2.bmp", 4, 4, 1.3, 1.7, RGB(255, 0, 255));
	}
	return m_pArcherImg;
}


STAnimationDesc GetEmoticonDesc(DWORD index) 
{
	STAnimationDesc desc;
	return desc;
}

void CDataBase::ClearAll() 
{	
	if (m_pKnightImg != NULL)
	{
		delete m_pKnightImg;
		m_pKnightImg = NULL;
	}

	if (m_pArcherImg != NULL)
	{
		delete m_pArcherImg;
		m_pArcherImg = NULL;
	}

	ClearSheetFile();
	ClearTile();
	ClearTileImg();
	ClearBuildingImg();
	ClearSpriteSheet();
	ClearSpriteRow();
}
