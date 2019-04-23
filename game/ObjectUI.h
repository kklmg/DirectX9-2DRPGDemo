#pragma once
#include<ddraw.h>
#include"EnumGroup.h"
#include"SequenceAni.h"
#include"TimerClass.h"
#include"Attribute.h"

class CEmotion 
{
public:
	CEmotion();
	~CEmotion();

	enEmoticon m_enEmotion;

	CAni_Sequence *m_pEmotion;

	void SetEmotion(enEmoticon emotion);

	void RemoveEmotion();

	bool DisPLayEmotion(LPDIRECTDRAWSURFACE7 suf,RECT DesRect);

	bool ExistEmotion()const;

private:

	bool m_bExistEmotion;
};



struct STObjUI
{
public:
	
	void Init();

	void ActiveEmotion(enEmoticon emotion);

	bool DrawEmotion(LPDIRECTDRAWSURFACE7 suf, float lasttime);

	void ActiveDisPlayHP();

	bool DrawHpBar(STHP &hp, LPDIRECTDRAWSURFACE7 suf, float lasttime);

	//	bool DrawAura(LPDIRECTDRAWSURFACE7 suf);

	//	bool DrawBuffEffect(LPDIRECTDRAWSURFACE7 suf);

	//	bool DrawBeAttackEffect(LPDIRECTDRAWSURFACE7 suf);

	RECT m_rtHPBar;

	RECT m_rtEmoticon;

//	RECT m_rtBuff;

//	RECT m_rtAura;

	CEmotion m_Emotion;

	CTimer m_timerDrawHP;

	CTimer m_timerDrawEmotion;
};


