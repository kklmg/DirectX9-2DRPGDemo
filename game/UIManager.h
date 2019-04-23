#pragma once
#include"TextSurface.h"
#include"SequenceAni.h"
#include"Dialog.h"
#include<vector>
#include"EnumGroup.h"
#include"StateBar.h"
#include"Attribute.h"
#include"TextFrame.h"
#include"DialogData.h"


class CUIManager
{
public:
	CUIManager();
	~CUIManager();

	void Init();

	CAni_Sequence* CreateEmoticon(enEmoticon index);

	void LoadTextSheet(LPCSTR FileName, DWORD row, DWORD column, COLORREF key);	

	void DrawTest(LPDIRECTDRAWSURFACE7 dessuf, RECT desRect);

	CTextSheet *GetTextSheet();

	void DrawStateBar();

	void DrawHPBarLittle(CLivingObj *Obj, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect);

	void DrawHPBarLittle(STHP &hp, LPDIRECTDRAWSURFACE7 lpDDSDest, RECT DesRect);

	WORD GetTextWidth(char cr);

	bool IsTalking() const;
	void TriggerTalk(CDialogChain* dialog);	

	WORD __DrawText(LPDIRECTDRAWSURFACE7 lpDDSDest,char cr,int left,int top,int bottom);
	void __DrawText(LPDIRECTDRAWSURFACE7 lpDDSDest, LPCSTR str, RECT rect);
	bool __DrawDialog(LPDIRECTDRAWSURFACE7 lpDDSDestt, bool AB = FALSE);

private:

	void InitTextSheet();

	void InitEmoticon();

	bool m_bTalking;

	CStateBar *m_pStateBar;

	CHPBarLittle *m_pHPBarLittle;

	CTextSheet *m_pTextSheet;

	CDialogMng *m_pDigMng;

	CTFrameMng *m_pTFrameMng;

	//CAni_Sequence **m_ppEmoticon;

	std::vector<STAnimationDesc>m_vector_pEmoticon;
};

