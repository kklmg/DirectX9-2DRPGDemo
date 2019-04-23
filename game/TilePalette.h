#pragma once
#include<string>
#include<ddraw.h>
#include<list>

#include"TileFile.h"
#include"TileSheet.h"
#include"TileSheetFile.h"
#include"Tile.h"

#define PltRow 10
#define PltColumn 10

struct STPaSlot
{
	STPaSlot(): m_TileData(NULL), pTile(NULL) {}
	
	RECT m_SlotPos;
	FSTTileData *m_TileData;
	CTile* pTile;
};


class CTilePalette
{
	//------------------------------------------
	//Construcotr  &  Destroyer
	//------------------------------------------
public:
	CTilePalette(WORD SlotSize);
	~CTilePalette();


	void SetTile(std::list<FSTTileData*>& list);


	//------------------------------------------
	//Update & Draw
	//------------------------------------------
	void Update(CTile** GetSelect);
	HRESULT Draw(LPDIRECTDRAWSURFACE7 DestlpDDS);
	HRESULT DrawTileInfo(LPDIRECTDRAWSURFACE7 DestlpDDS);

	HRESULT DrawSlectedTile(LPDIRECTDRAWSURFACE7 DestlpDDS,RECT rect);

private:
	void InitDDSurFace();
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


	LPDIRECTDRAWSURFACE7 m_lpDDSPalette;	//Palette Surface 

	//------------------------------------------
	//Init Slot
	//------------------------------------------
private:
	void InitSlot();
	POINT GetMapPos(POINT &MousePos);
private:
	STPaSlot m_Slot[PltRow][PltColumn];//Tile Slot

	WORD m_wSlotLength;	//slot length
	WORD m_wWidth;		//palette width
	WORD m_wHeight;		//palette Height

	WORD m_wSlotVolume;
	WORD m_wTileCount;
	STPaSlot*m_pLastSlot;

private:
	//Get real Coord
	WORD CalculateRow(int y);
	WORD CalculateColumn(int x);

private:
	//Now Selected Tile
	WORD  CursleRow;
	WORD  CursleColumn;

	//tile Clicked
	RECT  ClickFrame;

	//------------------------------------------
	//Other
	//------------------------------------------
	//string iterator
	std::list<std::string>::iterator m_Iter;

};

