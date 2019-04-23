#pragma once

enum enDiagToken
{
	eDigTokend_Scene = 0,
	eDigTokend_Dialog = 1,
	eDigTokend_Character = 2,
	eDigToken_MAX
};


const char* const DigToken[256] =
{
	"*SCENE"			/*  0*/
	,  "*DIALOG"			/*  1*/
	,  "*CHARACTER"			/*  2*/
};