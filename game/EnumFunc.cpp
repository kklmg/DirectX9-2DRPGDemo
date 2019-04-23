#include"EnumGroup.h"

enDirection __GetReverseDir(enDirection dir)
{
	switch (dir)
	{
	case eDown: return eUp;
		break;
	case eLeft: return eRight;
		break;
	case eRight: return eLeft;
		break;
	case eUp: return eDown;
		break;
	case eLeft_Up: return eRight_Down;
		break;
	case eRight_Up: return eLeft_Down;
		break;
	case eLeft_Down: return eRight_Up;
		break;
	case eRight_Down: return eLeft_Up;
		break;
	}

}