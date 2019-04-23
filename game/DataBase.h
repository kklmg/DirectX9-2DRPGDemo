#pragma once
#include<vector>
#include"TileSheet.h"
#include"KnightImage.h"
#include"ArcherImage.h"
#include"CommonSpriteSheet.h"
#include"CommonSpriteRow.h"
#include"BuildingImage.h"
#include"EnumGroup.h"
#include"BmpFile.h"

#include"ConCTN.h"
#include"TileFile.h"
#include"TileSheetFile.h"

using namespace std;

LPDIRECTDRAWSURFACE7 __CreateDDS(LPCSTR bmpfile, COLORREF key);

enum enTileSheet
{
	eTSBasic,
	eTSGrass,
	eTSRoof,
	eTSWall,
	eTSFloor,
	eTSWater,
	eTSCliff
};

enum enBuilding
{
	eMyhome,
	eNormalHouse1,
	eNormalHouse2,
	eNormalHouse3,
	eSpecialHouse1,
	eSmith,
	eGroceryStore,
	eFactory,
	eAdvancedHouse1,
	eAdvancedHouse2,
	eArena,
	ePalace,
	eChurch,
	eWareHouse,
	eGuild
};


template<typename T>
class CDataMng
{
public:
	CDataMng(DWORD count, T InitValue, void(*fptr)(DWORD index));
	~CDataMng();

	T GetData(DWORD index);

	void(*m_fptrCreate)(DWORD index);

	vector<T>m_vector;	
};

extern CDataMng<LPDIRECTDRAWSURFACE7> *g_pDDS_Emoticon;

extern CDataMng<STAnimationDesc> *g_AniDesc_Emoticon;




class CDataBase
{
public:
	//friend CDataMng;
public:
	CDataBase();
	~CDataBase();

	bool Init();

	void ClearAll();
	//---------------------------------------------------------------
	//Parsing Data
	//---------------------------------------------------------------
	void ParsingTileData(LPCSTR FilePath);
	void ParsingTileSheetData(LPCSTR FilePath);


	//---------------------------------------------------------------
	//bmp Sheet
	//---------------------------------------------------------------
public:
	CBuildingImg* GetBuildingImg(DWORD index);

	void ClearBuildingImg();

private:
	void InitBuildingImg(DWORD index);

	vector <CBuildingImg*>m_vector_ImgBuilding;



	//------------------------------------------------------------
	//emoticon
	//-------------------------------------------------------------
public:
	static void CDataBase::CreateEmoticonDDS(DWORD EmoIndex);

	static void CDataBase::CreateEmoticonDesc(DWORD index);

	STAnimationDesc GetEmoticonDesc(DWORD index);
private:
	void InitEmoticonData();

	vector<STAnimationDesc>m_vector_pEmoticon;



	//---------------------------------------------------------------
	//Tile Data
	//---------------------------------------------------------------
public:
	//Get Tile
	STTileSrc* GetTile(enTile etile);
	STTileSrc* GetTile(LPCSTR TileName);

	CTileSheet* GetTileSheet(DWORD index);

	void GetAllTileData(std::list<FSTTileData*> &GetVector);

	//Clear
	void ClearTile();
	void ClearTileImg();
private:
	//Init
	void InitTile(enTile etile);

	void InitTileSheet(DWORD index);
	CTileSheet* GetTileSheet(LPCSTR TileName);

	//Tile Image Container
	vector<CTileSheet*>m_vector_TileSheet;
	vector<STTileSrc*>m_vector_Tile;


	//-----------------------------------------------------------
	//Sheet File data
	//-----------------------------------------------------------
public:
	CSheetFile * GetSheetFile(DWORD index);

	void ClearSheetFile();

private:
	void InitSheetFile(DWORD index);

	vector <CSheetFile*>m_vector_SheetFile;

	//----------------------------------------------------------
	//Creature Image
	//----------------------------------------------------------
public:
	CKnightImage* GetKnightImg();

	CArcherImage* GetArcherImg();

	CCommonSpriteSheet* GetSpriteSheet(DWORD index);

	CCommonSpriteRow* GetSpriteRow(DWORD index);

	void ClearSpriteSheet();

	void ClearSpriteRow();

private:
	void InitSpriteSheet(DWORD index);

	void InitSpriteRow(DWORD index);

	CKnightImage* m_pKnightImg;

	CArcherImage* m_pArcherImg;

	vector<CCommonSpriteSheet*>m_vector_SpriteSheet;

	vector<CCommonSpriteRow*>m_vector_SpriteRow;

	//Tile Data
	FSTTileDataGroup m_TileDataFile;
	FSTTileSheetGroup m_TSDataFile;

	CDataContainer<CTileSheet> m_TileSheetSrc;
	CDataContainer<STTileSrc> m_TileSrc;
	//CDataContainer<> m_TileSheetSrc;
};



//
//class CDDSDataMng
//{
//public:
//	CDDSDataMng();
//	~CDDSDataMng();
//
//	void Init();
//
//	LPDIRECTDRAWSURFACE7 GetEmoticonDDS(DWORD index);
//
//
//
//	
//	
//
//private:
//
//
//	CDataMng<LPDIRECTDRAWSURFACE7> *m_pDDSMng_Emoticon;
//
//
//
//
//	bool CreateEmoticonDDS(DWORD index);
//
//
//	LPDIRECTDRAWSURFACE7 CreateDDS(LPCSTR bmpfile, COLORREF key);
//	
//	vector<LPDIRECTDRAWSURFACE7>m_vector_DDSEmoticon;
//
//	
//};
//

