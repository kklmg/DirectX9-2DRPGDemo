#pragma once

enum enDirection
{
	eDown,
	eLeft,
	eRight,
	eUp,	
	eLeft_Up,
	eRight_Up,
	eLeft_Down,
	eRight_Down,
};

enDirection __GetReverseDir(enDirection dir);

enum enShape
{
	eLine,
	eRect,
	eCircle,
};


enum enPhysicalStrength 
{
	epyNoCollide = 0,
	epyWeak = 1,
	epyMiddle = 2,
	epyStrong = 3,
	epyVeryStrong =4,
	epyFixed = 5,
};

enum enAction
{
	eStand,
	eWalk,
	eRun,
	eAttack_1,
	eAttack_2
};

enum enAttackStage
{
	eAtkEnd,
	eAtkInit,
	eAtkRunning,
	eAtkWaitCoolTime,
};

enum enObjectForce
{
	eForFriend,
	eForHostile,
	eForFNeutral
};

enum enScene
{
	eSceNone,
	eSceTown,
	eSceForest01,
	eSceForest02,
	eSceForest03,
	eSceForest04,
	eSceInHome,
	eSceInGrocery,
	eSceInGuild,
	eSceInSmithy,
	eSceInHotel,
	eSceInChurch,
	eSceInHouse01,
	eSceInHouse02,
	eSceInHouse03,
	eSceInHouse04,
	eSceInHouse05,
	eSceInHouse06,
	esceMountain,
	eSceCoast
};





//enum object
enum enObjectType 
{
	eOTUnKnown,
	eOTOrnament,
	eOTHero,
	eOTMonster,
	eOTNpc,
	eOTBuilding,
};

enum enHero 
{
	eKnight,
	eArcher,
	eAssasin,
	eMagician,
	eWarrior
};

enum enNPC
{
	eNPC_01=10,
	eNPC_02,
	eNPC_03,
	eNPC_04,
	eNPC_05,
	eNPC_06,
	eNPC_07,
	eNPC_08,
	eNPC_09,
	eNPC_OLDM,
	eNPC_OLDF,
	eNPC_AS,
	eNPC_BOAT,
	eNPC_BOY,
	eNPC_GIRL,
	eNPC_DOG,
	eNPC_DOG2,
	eNPC_CAT,
	eNPC_PIG,
	eNPC_DEER,
};


enum enMonster
{
	eMRabbit,

};

enum enSpriteSheet
{
	eSSMagician,
	eSSWarrior,
	eSSAssassin
};

enum enSpriteRow
{
	eSRRabbit
};

enum enEmoticon
{
	eAmaze = 0,
	eQuestion = 1,
	eThink = 2,
	eBother = 3,
	eHappy = 4,
	eSpeak = 5,
	eSleep = 6,
	eIdea = 7,
	eFright = 8,
	eAwkward = 9,
	eGrief = 10,
	eDelight = 11,
	eAngry = 12,
	eSkull = 13,
	eNoEmotion=14,
};

enum enLOBJ 
{
	eCommon,
	eHeroKnight,
	eHeroArcher,
	eHeroAssasin,
	eHeroMagician,
	eHeroWarrior,
	eMonRabbit,
};


enum enColor
{
	eCrWhite,
	eCrBlack,
	eCrRed,
	eCrGreen,
	eCrBlue,
	eCrYellow,
	eCrOrange,
	eCrGold,
	eCrGray,
	eCrPurple,
	eCrPink,
};

enum enOrnamentSheet
{
	eOrnament_COM,
	eOrnament_COM2,
	eOrnament_GRASS,






};


enum enTile 
{
	eTile_Black,
	eTile_Grass_Light01,
	eTile_Grass_Light02,
	eTile_Grass_Light03,
	eTile_Grass_Dusk,
	eTile_Grass_Dark01,
	eTile_Grass_Dark02,
	eTile_SlabStone_WhiteOval,
	eTile_SlabStone_WhiteOval_Chipped,
	eTile_SlabStone_BrownOval,
	eTile_SlabStone_BrownOval_Chipped,
	eTile_SlabStone_WhiteRect,
	eTile_SlabStone_WhiteRect_Chipped,
	eTile_SlabStone_WhiteRect02,
	eTile_SlabStone_WhiteRect03,
	eTile_Star01,
	eTile_Star02,
	eTile_Circle,

	eTile_Water_Shallow,
	eTile_Water_Deep,

	eTile_WaterFall01,
	eTile_WaterFall02,
	eTile_WaterFall03,

	eTile_Beach_LT,
	eTile_Beach_T,
	eTile_Beach_RT,
	eTile_Beach_L,
	eTile_Beach_R,
	eTile_Beach_LB,
	eTile_Beach_B,
	eTile_Beach_RB,

	eTile_Cliff01,
	eTile_Cliff02,
	eTile_Cliff03,	
	eTile_Cliff_LT,
	eTile_Cliff_T,
	eTile_Cliff_RT,
	eTile_Cliff_L,
	eTile_Cliff_R,
	eTile_Cliff_LB,
	eTile_Cliff_B,
	eTile_Cliff_RB,

	eTile_Roof01,
	eTile_Roof02,
	eTile_Roof03,
	eTile_StoneWall01,
	eTile_StoneWall02,
	eTile_StoneWall03,
	eTile_StoneWall04,

	eTile_WoodWall01,
	eTile_WoodWall02,
	eTile_WoodWall03,
	eTile_WoodWall04,

	eTile_Floor01,
	eTile_Floor02,
	eTile_Floor03,
	eTile_Floor04,

	eTile_Pillar01,
	eTile_Pillar02,
	eTile_Pillar03,
	eTile_Pillar04,
};