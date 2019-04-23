#include"Global_data.h"


//Windwos Mouse
CMouse g_Mouse;

std::string g_keyBuff;
std::string g_keyBuff_figure;

emGameMode g_emGAMEMODE = emGameMode::eGameMode_Normal;

//Direct Draw
//---------------------------
LPDIRECTDRAW7 g_pDD = NULL;

//Direct Draw Surface
LPDIRECTDRAWSURFACE7 g_pDDSPrimary = NULL;

LPDIRECTDRAWSURFACE7 g_pDDSBack = NULL;

LPDIRECTDRAWSURFACE7 g_pDDSVBuffer = NULL;

LPDIRECTDRAWSURFACE7 g_pDDSSBuffer = NULL;

LPDIRECTDRAWSURFACE7 g_pDDSSBuffer2 = NULL;

LPDIRECTDRAWSURFACE7 g_pDDSRect = NULL;

LPDIRECTDRAWSURFACE7 g_pDDSCircle = NULL;
//clipper
LPDIRECTDRAWCLIPPER  g_pDDClipper = NULL;


//palette
LPDIRECTDRAWPALETTE   g_pDDPal = NULL;     // The primary surface palette

PALETTEENTRY	g_Pal[256]; // 1024

//Sound
LPDIRECTSOUNDBUFFER g_pDS = NULL;

//database
CDataBase *g_pDataBase = NULL;

//game system
CGameSystem *g_pGameSystem = NULL;

//quad tree
CQuadTree *g_pQuadTree = NULL;


POINT g_PosC1;

CFpsMng	gFpsMng;

DDSURFACEDESC2 g_ddsd;

enPixelFormat g_enPixelFormat;

int g_nRGBBitCount = BPP;




CGameManager *g_pGameMng = NULL;

CEventManager *g_pEventMng = NULL;

CUIManager *g_pUIMng = NULL;

//AI Manager
CAIManager *g_pAIMng = NULL;

CATTMng *g_pAttMng = NULL;

//hdc
HDC gHdc;

HFONT gFont;

HWND g_hWnd;

CLivingObj* g_pControledObj = NULL;

std::list<CLivingObj*> g_listNearByObj;

CLivingObj* g_pTestdObj01 = NULL;

CLivingObj* g_pTestdObj02 = NULL;

CTileMap* g_pCurMap = NULL;

enScene g_enCurScene = enScene::eSceNone;

CSceneLink *g_pTouchLink = NULL;

CSceneLink *g_pTouchLink_new = NULL;

//bool disheadarea;
bool bShowMapInfo;


CTimeMng *g_pTimeMng = NULL;

bool g_bGamePause = FALSE;