#include"StateBar.h"
#include"ddutil.h"
#include"Global_data.h"
#include"LivingObjectClass.h"

CStateBar::CStateBar() 
{
	m_lpDDSFrameWork = NULL;

	m_lpDDSHPBar = NULL;

	m_lpDDSMPBar = NULL;

	m_dwHPBarWidth = 0;

	m_dwMPBarWidth = 0;
}

CStateBar::~CStateBar() 
{
	SAFE_RELEASE(m_pRectGroup);

	SAFE_RELEASE_DDSURFACE(m_lpDDSFrameWork);
	SAFE_RELEASE_DDSURFACE(m_lpDDSHPBar);
	SAFE_RELEASE_DDSURFACE(m_lpDDSMPBar);

}

void CStateBar::Init() 
{
	InitFrameWork();
	InitHPBar();
	InitMPBar();

	long rL = HRES*0.01;
	long rT = VRES*0.01;
	long rR = HRES*0.3;
	long rB = VRES*0.25;

	SetDrawRect({ rL,rT,rR,rB });
}

void CStateBar::InitFrameWork() 
{
	m_lpDDSFrameWork = DDLoadBitmap(g_pDD, "UI/layout.bmp", 0, 0);

	DDSetColorKey(m_lpDDSFrameWork, RGB(255, 255, 255));

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_lpDDSFrameWork->GetSurfaceDesc(&ddsd);
	
	RECT FWRect;

	FWRect.left = 0;
	FWRect.top = 0;
	FWRect.right = ddsd.dwWidth; 
	FWRect.bottom= ddsd.dwHeight;

	m_pRectGroup = new CRECTNode(FWRect,"FrameWork");
}

void CStateBar::InitHPBar() 
{
	m_lpDDSHPBar = DDLoadBitmap(g_pDD, "UI/HPbar.bmp", 0, 0);

	DDSetColorKey(m_lpDDSHPBar, RGB(255, 255, 255));

	CRECTNode *node = new CRECTNode({ 58,16,195,37 },"HPBar");

	m_pRectGroup->AddRect(node);

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_lpDDSHPBar->GetSurfaceDesc(&ddsd);

	
	m_dwHPBarWidth = ddsd.dwWidth;
	//get size
	m_rectHPBar.left = 0;
	m_rectHPBar.top = 0;
	m_rectHPBar.right = ddsd.dwWidth;
	m_rectHPBar.bottom = ddsd.dwHeight;
}

void CStateBar::InitMPBar() 
{
	m_lpDDSMPBar = DDLoadBitmap(g_pDD, "UI/MPbar.bmp", 0, 0);

	DDSetColorKey(m_lpDDSMPBar, RGB(255, 255, 255));
	
	CRECTNode *node = new CRECTNode({ 58,32,190,48 },"MPBar");

	m_pRectGroup->AddRect(node);

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_lpDDSMPBar->GetSurfaceDesc(&ddsd);


	m_dwMPBarWidth = ddsd.dwWidth;
	//get size
	m_rectMPBar.left = 0;
	m_rectMPBar.top = 0;
	m_rectMPBar.right = ddsd.dwWidth;
	m_rectMPBar.bottom = ddsd.dwHeight;

}

void CStateBar::SetScale(float Hscale, float Vscale) 
{


}

void CStateBar::SetDrawRect(RECT rect)
{
	m_DrawRect = rect;

	m_pRectGroup->TransformRect(rect);
}

void CStateBar::DrawStateBar(CLivingObj *Obj, LPDIRECTDRAWSURFACE7 lpDDSDest)
{
	if (Obj == NULL)return;

	//calculate scale
	float HPScale= (float)Obj->GetCurHp()/ (float)Obj->GetMaxHp();
	float MPScale = (float)Obj->GetCurMp() / (float)Obj->GetMaxMp();

	//////////
	//HP bar//
	//////////

	//figue out hp bar dest rect
	RECT HPDES = m_pRectGroup->m_vector_child[0]->m_RECT;

	DWORD HPDES_width = GetRectWidth(HPDES)*HPScale;

	HPDES.right = HPDES.left + HPDES_width;

	//figure out hp bar src rect
	RECT HPSRC = m_rectHPBar;

	HPSRC.right = HPSRC.left + m_dwHPBarWidth*HPScale;

	///////////
	//MP Bar//
	//////////

	//figue out mp bar dest rect
	RECT MPDES = m_pRectGroup->m_vector_child[1]->m_RECT;

	DWORD MPDES_width = GetRectWidth(MPDES)*MPScale;

	MPDES.right = MPDES.left + MPDES_width;

	//figure out mp bar src rect
	RECT MPSRC = m_rectMPBar;

	MPSRC.right = MPSRC.left + m_dwMPBarWidth*MPScale;

	lpDDSDest->Blt(&HPDES, m_lpDDSHPBar, &HPSRC, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	lpDDSDest->Blt(&MPDES, m_lpDDSMPBar, &MPSRC, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	lpDDSDest->Blt(&m_DrawRect, m_lpDDSFrameWork, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
}





CHPBarLittle::CHPBarLittle() :m_lpDDSHPBar( NULL), m_lpDDSHPBarFW(NULL)
{
}

CHPBarLittle::~CHPBarLittle() 
{
	SAFE_RELEASE_DDSURFACE(m_lpDDSHPBar);
	SAFE_RELEASE_DDSURFACE(m_lpDDSHPBarFW);

	if (m_pRTnode_FW != NULL) 
	{
		delete m_pRTnode_FW;
		m_pRTnode_FW = NULL;
	}

}

void CHPBarLittle::Init() 
{
	InitBarFrameWork();
	InitHPBar();
}

void CHPBarLittle::InitBarFrameWork() 
{
	//load bitmap to surface
	m_lpDDSHPBarFW = DDLoadBitmap(g_pDD, "UI/HPbarFW_c.bmp", 0, 0);

	//set color key
	DDSetColorKey(m_lpDDSHPBarFW, RGB(255, 255, 255));

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_lpDDSHPBarFW->GetSurfaceDesc(&ddsd);

	RECT FWRect;

	FWRect.left = 0;
	FWRect.top = 0;
	FWRect.right = ddsd.dwWidth;
	FWRect.bottom = ddsd.dwHeight;

	//Init bound rect 
	m_pRTnode_FW = new CRECTNode(FWRect,"FrameWork");
}

void CHPBarLittle::InitHPBar() 
{
	//load bitmap to surface
	m_lpDDSHPBar = DDLoadBitmap(g_pDD, "UI/HPbar_c.bmp", 0, 0);

	//set color key
	DDSetColorKey(m_lpDDSHPBar, RGB(255, 255, 255));

	//bound rect 
	m_pRTnode_FW->AddRect(RECT{ 10,10,390,40 },"HPBar");


	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_lpDDSHPBar->GetSurfaceDesc(&ddsd);

	//save value
	m_dwHPBarWidth = ddsd.dwWidth;
	//get size
	m_rectHPBar.left = 0;
	m_rectHPBar.top = 0;
	m_rectHPBar.right = ddsd.dwWidth;
	m_rectHPBar.bottom = ddsd.dwHeight;
}

void CHPBarLittle::UpdateDrawRect(RECT rect) 
{
	m_pRTnode_FW->TransformRect(rect);
}

void CHPBarLittle::DrawHPBar(CLivingObj *Obj, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect)
{

	if (Obj == NULL)return;

	//calculate scale
	float HPScale = (float)Obj->GetCurHp() / (float)Obj->GetMaxHp();

	//////////
	//HP bar//
	//////////

	UpdateDrawRect(DesRect);

	//figue out hp bar dest rect
	RECT HPDES = m_pRTnode_FW->m_vector_child[0]->m_RECT;

	DWORD HPDES_width = GetRectWidth(HPDES)*HPScale;

	HPDES.right = HPDES.left + HPDES_width;

	//figure out hp bar src rect
	RECT HPSRC = m_rectHPBar;

	HPSRC.right = HPSRC.left + m_dwHPBarWidth*HPScale;




	
	
	lpDDSDest->Blt(&HPDES, m_lpDDSHPBar, &HPSRC, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	lpDDSDest->Blt(&DesRect, m_lpDDSHPBarFW, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
}

void CHPBarLittle::DrawHPBar(STHP &HP, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect)
{
	//calculate scale
	float HPScale = (float)HP.nCurHP / (float)HP.dwMAXHP;
	
	//////////
	//HP bar//
	//////////

	UpdateDrawRect(DesRect);

	//figue out hp bar dest rect
	RECT HPDES = m_pRTnode_FW->m_vector_child[0]->m_RECT;

	DWORD HPDES_width = GetRectWidth(HPDES)*HPScale;

	HPDES.right = HPDES.left + HPDES_width;

	//figure out hp bar src rect
	RECT HPSRC = m_rectHPBar;

	HPSRC.right = HPSRC.left + m_dwHPBarWidth*HPScale;


	lpDDSDest->Blt(&HPDES, m_lpDDSHPBar, &HPSRC, DDBLT_KEYSRC | DDBLT_WAIT, NULL);
	lpDDSDest->Blt(&DesRect, m_lpDDSHPBarFW, NULL, DDBLT_KEYSRC | DDBLT_WAIT, NULL);

}