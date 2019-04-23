#pragma once
//---------------------------------------------
//Head File
//---------------------------------------------
#include<ddraw.h>


//---------------------------------------------
//My Head File
//---------------------------------------------
#include"TimerClass.h"
#include"TileSheet.h"
#include"RectMath.h"

//---------------------------------------------
//Tile related Struct
//---------------------------------------------
typedef struct STTileData
{
	//short TileIndex;
	WORD wTileType;
	WORD wObstacle_level;

} TileData;

#define Rate_TileUpade 0.75

struct STTileSrc
{
	STTileSrc(GRID grid, CTileSheet *TS, WORD type = 0, WORD obs = 0)
		:SrcTile(grid), _TileSheet(TS), bMulti(TS->HasAnimation()),
		TileType(type), TileObstacle(obs), TileName("unDefine")
	{
		if (_TileSheet != NULL)
			SrcRectTile = _TileSheet->GetSourceRect(SrcTile.wRow, SrcTile.wColumn);
	}
	

	STTileSrc(LPCSTR TileName, GRID grid, CTileSheet *TS, WORD type = 0, WORD obs = 0)
		:SrcTile(grid), _TileSheet(TS), bMulti(TS->HasAnimation()),
		TileType(type), TileObstacle(obs) , TileName(TileName)
	{	
		if (_TileSheet != NULL)		
			SrcRectTile = _TileSheet->GetSourceRect(SrcTile.wRow, SrcTile.wColumn);		
	}
	
	
	std::string TileName;

	WORD TileType;
	WORD TileObstacle;

	const GRID SrcTile;
	CTileSheet *_TileSheet;

	const bool bMulti;

	RECT SrcRectTile;

	void DrawTile(LPDIRECTDRAWSURFACE7 DestlpDDS, RECT DestRect,DWORD id)
	{
		_TileSheet->Draw(DestlpDDS, DestRect, SrcTile.wRow, SrcTile.wColumn, id);
	}
	
};


//---------------------------------------------
//Class Tile
//---------------------------------------------
class CTile
{
public:
	CTile(GRID,RECT,STTileSrc* TileSource);
	virtual ~CTile();

	//---------------------------------------------
	//Tile Basic
	//---------------------------------------------
public:
	//---------------------------------------------
	//public:Get tile data infomation
	//---------------------------------------------
	//Get tile position
	std::string GetTileName() const;
	std::string GetTileSheetName() const;

	GRID GetGrid()const;
	RECT GetPostion() const;
	POINT GetTileCenter()const;
	DWORD GetTileID()const;
	

	STTileSrc* GetTileDesc()const;

	WORD GetObstacleLevel() const;
	bool ifIncludeObstacle() const;

	void MarkIncludeObstacle();


	//---------------------------------------------
	//public:Set tile 
	//---------------------------------------------
	void SetTileObstacleLevel(WORD);
	void SetTileType(WORD);
	//void SetPosition(RECT rect);


	//---------------------------------------------
	//Basic Attribute
	//---------------------------------------------

protected:
	const DWORD m_dwID;

	RECT m_rtPos;			//Draw Rect
	const GRID m_gdPos;		//Draw Grid
	
	WORD m_wTileType;		//TIle type
	bool m_bIncObstacle;	//is include obstacle
	WORD m_wObstacle_level;	//obstacle level
	STTileSrc *m_pDesc;
public:
	//---------------------------------------------
	//Draw Tile
	//---------------------------------------------
	virtual void DrawTile(LPDIRECTDRAWSURFACE7 DestSuf,DWORD DrawSetting) = 0;
	virtual void DrawTile(LPDIRECTDRAWSURFACE7 DestSuf,RECT& DestRect, DWORD DrawSetting) = 0;
};


//---------------------------------------------
//Class Static Tile
//---------------------------------------------
class CTile_Static : public CTile
{
public:
	CTile_Static(GRID grid, RECT rect, STTileSrc *TileSource);
	CTile_Static(GRID grid, RECT rect, STTileSrc *TileSource, WORD TileType, WORD ObsLevel);
	virtual~CTile_Static();

	//Draw Tile
	virtual void DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting);
	virtual void DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, RECT& DestRect, DWORD DrawSetting);
};


//---------------------------------------------
//Class Dynamic Tile
//---------------------------------------------
class CTile_Dynamic : public CTile
{
public:
	CTile_Dynamic(GRID grid, RECT rect, STTileSrc *TileSource, float rate);
	CTile_Dynamic(GRID grid,RECT rect, STTileSrc *TileSource,WORD TileType, WORD ObsLevel, float rate);	
	virtual~CTile_Dynamic();


	//---------------------------------------------
	//Tile Update & Draw
	//---------------------------------------------
private:
	void UpdateTile();

public:
	virtual void DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, DWORD DrawSetting);
	virtual void DrawTile(LPDIRECTDRAWSURFACE7 DestSuf, RECT& DestRect, DWORD DrawSetting);

	//---------------------------------------------
	//Basic Attribute
	//---------------------------------------------
private:
	CTileSheet* m_pTileSheet;

	DWORD m_dwIndex;

	DWORD m_dwMaxIndex;

	CTimer m_Timer;

	float m_fRate;
};


CTile* __CreateNewTile(GRID grid, RECT rect, LPCSTR TileName);

CTile* __CreateNewTile(GRID grid, RECT rect, STTileSrc*TileInfo, LPCSTR TileName="UnKnown");

