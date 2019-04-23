#pragma once

enum enDiagueToken
{
	eDigTokend_Scene = 0,
	eDigTokend_Dialogue = 1,
	eDigTokend_Character = 2,
	eDigToken_MAX
};


const char* const DigToken[256] =
{
	"*SCENE"			/*  0*/
	,  "*DIALOGUE"			/*  1*/
	,  "*CHARACTER"			/*  2*/
};