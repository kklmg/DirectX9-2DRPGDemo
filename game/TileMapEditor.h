#pragma once
#include"TileMap.h"
#include"TilePalette.h"
#include"CameraClass.h"
#include"TimerClass.h"


class CTileMapEditor
{
	enum emEditMode
	{
		emCreateNew,
		emLoadExist
	};

	enum enView
	{
		eTileMap,
		ePalette
	};

public:
	CTileMapEditor();
	~CTileMapEditor();

	void CreateTileMap(WORD CellWidth, WORD CellHeight, WORD Row, WORD Column, LPCSTR SavePath="NewTileMap");
	bool LoadTileMap(LPCSTR FilePath);

	//set tile
	void Update();
	void Draw(LPDIRECTDRAWSURFACE7 lpDDS);


private:
	emEditMode m_emEditMode;

	POINT GetMapCoord(POINT &ScreenCoord);

	void InitPalette();

	void InitGuide();

	void DrawPalette(LPDIRECTDRAWSURFACE7 lpDDS);

	void DrawSlectedTile(LPDIRECTDRAWSURFACE7 lpDDS);

	void DrawGuide(LPDIRECTDRAWSURFACE7 lpDDS);

	bool SaveMapInfoToFile();

private:
	//Draw Destination
	RECT m_DrawActGuide;	
	RECT m_DrawKeyGuide;
	RECT m_DrawTileMap;

	enView m_enCurView;

	int m_pltIndex;//palette index
	WORD m_pltSleID;

	LPCSTR m_szSavePath;

	CTile** m_pCurSleTile;

	WORD m_wSleRow;
	WORD m_wSleColumn;

	RECT m_rtDrawSlect;

	std::vector<CTilePalette*>m_vectorplt;

	std::string m_strKeyguide;
	std::string m_strActguide;

	CTimer m_Timer;

	
	CTileMap* m_pTileMap;
	CCamera* m_pCamera;
	LPDIRECTDRAWSURFACE7 m_pDDScene;
};

