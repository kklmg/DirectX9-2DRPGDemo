#pragma once
#include<string>
#include<ddraw.h>
#include<list>

#include"Tile.h"
#include"TileFile.h"
#include"TileSheet.h"
#include"TileSheetFile.h"


#define PltRow 10
#define PltColumn 10
#define PltSlotCount  (PltRow*PltColumn)

struct STTileSlot 
{
	STTileSlot();
	~STTileSlot();

	bool DrawTile(LPDIRECTDRAWSURFACE7 Destlpdds);
	bool DrawTile(LPDIRECTDRAWSURFACE7 Destlpdds, RECT rect);

	CTile* CreateNewTile(STTileSrc* __TileInfo);
	RECT m_SlotPos;
	CTile*m_pTile;
	STTileSrc* TileInfo;
	//FSTTileData m_TileData;
};


class CTileEditor
{
	enum enSettingMode 
	{
		eSetNone,
		eSetTileName,
		eSetTileSheetName,
		eSetTileType,
		eSetTileObastacle,
	};


	//------------------------------------------
	//Construcotr  &  Destroyer
	//------------------------------------------
public:
	CTileEditor(WORD SlotSize, WORD bmprow,WORD bmpcolumn,LPCSTR SheetName, WORD pathCount...);
	~CTileEditor();	

	//------------------------------------------
	//Update & Draw
	//------------------------------------------
	void Update();
	HRESULT Draw(LPDIRECTDRAWSURFACE7 DestlpDDS);
	HRESULT DrawTileInfo(LPDIRECTDRAWSURFACE7 DestlpDDS);

private:
	void InitUIDestRect();

private:
	//Draw Destination
	RECT m_DrawRect;		//put palette
	RECT m_DrawTileName;
	RECT m_DrawSheetName;
	RECT m_DrawTileType;
	RECT m_DrawUserInput;
	RECT m_DrawTileObstacle;
	RECT m_DrawSelectTile;
	RECT m_DrawGuide;

	LPDIRECTDRAWSURFACE7 m_lpDDSPalette;	//Palette Surface 

	//------------------------------------------
	//Init Slot
	//------------------------------------------
private:
	void InitSlot();
	
private:
	STTileSlot(*m_pSlot)[PltRow][PltColumn];//Tile Slot


	WORD m_wSlotLength;	//slot length
	WORD m_wWidth;		//palette width
	WORD m_wHeight;		//palette Height

	WORD m_wCurPage;		//Palette Page
	WORD m_wMaxPage;		//Palette Page
	//------------------------------------------
	//Tile Loaded
	//------------------------------------------
private:
	FSTTileSheet m_TSInfo;		//Loaded TileSheet Information

	CTileSheet* m_pCurSheet;	//Created TileSheet

	//------------------------------------------
	//User Input
	//------------------------------------------
private:
	//Get real Coord
	WORD CalculateRow(int y);
	WORD CalculateColumn(int x);

private:
	//Now Selected Tile
	WORD  m_wCursleRow;
	WORD  m_wCursleColumn;
	WORD  m_wCurslePage;

	//tile Clicked
	RECT  ClickFrame;

	bool m_bGetInput;
	//Buff Get User input
	std::string m_Inputbuff;

	//Setting
	enSettingMode m_enSetMode;
	std::string m_GuideStr;

	void ReturnToDefaultMode();

	//------------------------------------------
	//Other
	//------------------------------------------
	//string iterator
	std::list<std::string>::iterator m_Iter;

	//---------------------------------------------
	//Save Setting to file
	//---------------------------------------------
	bool SaveToFile(LPCSTR filePath);
	bool SaveTileSheet(LPCSTR filePath);
};

