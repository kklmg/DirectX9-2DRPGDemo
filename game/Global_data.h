#pragma once
#include<ddraw.h>
#include<list>
#include<string>

#include"Mouse.h"
#include"GameSystem.h"
#include"DataBase.h"

#include"FpsMng.h"
#include"UIManager.h"
#include"SceneMng.h"
#include"EventMng.h"
#include"ATTMng.h"

#include"LivingObjectClass.h"
#include"TimeManager.h"

#include"HHSound.h"

using namespace std;

#define SystemMeomory FALSE
#define VideoMeomory TRUE
#define nBackbuffcount 2



enum emGameMode
{
	eGameMode_Normal,
	eGameMode_TileEditror,
	eGameMode_TileMapEditor
};

extern emGameMode g_emGAMEMODE;

//Windows Input
extern CMouse g_Mouse;
extern std::string g_keyBuff;
extern std::string g_keyBuff_figure;

//Game System
extern CGameSystem *g_pGameSystem;




//Direct Draw 
extern LPDIRECTDRAW7 g_pDD ;


//Direct Draw Surface
extern LPDIRECTDRAWSURFACE7 g_pDDSPrimary ;

extern LPDIRECTDRAWSURFACE7 g_pDDSBack ;

extern LPDIRECTDRAWSURFACE7 g_pDDSVBuffer ;

extern LPDIRECTDRAWSURFACE7 g_pDDSSBuffer ;

extern LPDIRECTDRAWSURFACE7 g_pDDSSBuffer2 ;


//used in test
extern LPDIRECTDRAWSURFACE7 g_pDDSRect;
extern LPDIRECTDRAWSURFACE7 g_pDDSCircle;

//Direct Draw Clipper
extern LPDIRECTDRAWCLIPPER  g_pDDClipper ;


//Direct Draw Palette
extern LPDIRECTDRAWPALETTE   g_pDDPal;     // The primary surface palette

extern PALETTEENTRY	g_Pal[256]; // 1024


//Direct Sound
extern LPDIRECTSOUNDBUFFER g_pDS;


//Direct Draw Surface Pixelformat
extern enPixelFormat g_enPixelFormat;

extern int g_nRGBBitCount;



//Game manager
extern CGameManager *g_pGameMng;

extern CEventManager *g_pEventMng;


//Data base
extern CDataBase *g_pDataBase;


//quad tree for collision detection
extern CQuadTree *g_pQuadTree;


//UI Manager
extern CUIManager *g_pUIMng;

//AI Manager
extern CAIManager *g_pAIMng;

//Attribute Manager
extern CATTMng *g_pAttMng;

//DC handle
extern HDC gHdc;


//Window Handle
extern HWND g_hWnd;


//Global declare
extern DDSURFACEDESC2 g_ddsd;

extern POINT g_PosC1;

extern HFONT gFont;

//global game object
extern CLivingObj* g_pControledObj;

extern std::list<CLivingObj*> g_listNearByObj;

extern CLivingObj* g_pTestdObj01;

extern CLivingObj* g_pTestdObj02;



extern CTileMap* g_pCurMap;

extern enScene g_enCurScene;

extern CSceneLink *g_pTouchLink;

extern CSceneLink *g_pTouchLink_new;

extern CTimeMng *g_pTimeMng;



#define __GetCurTime (g_pTimeMng->GetCurTime())

#define __GetDeltaTime (g_pTimeMng->GetDeltaTime())


extern bool g_bGamePause;

//Tick
//GetTickCount()
//extern Dword g_TickCount;

//Programe Name
#define NAME                "game"
#define TITLE               "game"


//Frame per second
#define DisplayFrame 0
#define DisplayRate 1000/30
#define FPS 0



#define dTIMEGETTIME timeGetTime()* 0.001


//display setting

#define HRES  1024
#define VRES  768
#define BPP    32


//define color
#define WHITE    RGB(255,255,255)
#define BLACK    RGB(  0,  0,  0)
#define RED      RGB(255,  0,  0)
#define GREEN    RGB(  0,255,  0)
#define BLUE     RGB(  0,  0,255)
#define YELLOW	 RGB(255,255,  0)
#define ORANGE	 RGB(255,165,  0)
#define GOLD	 RGB(255,215,  0)
#define GRAY	 RGB(192,192,192)
#define PURPLE	 RGB(128,  0,128)
#define Pink	 RGB(255,192,203)

#define WHITE_dcp   255,255,255
#define BLACK_dcp	  0,  0,  0
#define RED_dcp     255,  0,  0
#define GREEN_dcp     0,255,  0
#define BLUE_dcp      0,  0,255
#define YELLOW_dcp	255,255,  0
#define ORANGE_dcp	255,165,  0
#define GOLD_dcp	255,215,  0
#define GRAY_dcp	192,192,192
#define PURPLE_dcp	128,  0,128
#define Pink_dcp	255,192,203


//bool show map info
extern bool bShowMapInfo;

//Define Safe Release
#define SAFE_RELEASE(p) {if(p!=NULL){delete p; p=NULL;}}

#define SAFE_RELEASE_DDSURFACE(p){if(p!=NULL){p->Release(); p=NULL;}}

#define SAFE_RELEASE_Multi(p){if(p!=NULL){delete[] p; p=NULL;}}

