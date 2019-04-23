#pragma once
#include"SceneMng.h"
#include"Global_data.h"


typedef map<enScene, bool(CSceneMng::*)(void)>::iterator SceneIter;

typedef pair<enScene, bool(CSceneMng::*)(void)> ScenePair;

void CSceneMng::InitSceneContainer()
{
	m_stlMap_Scene[enScene::eSceTown] = &CSceneMng::CS_Town;
	m_stlMap_Scene[enScene::eSceInHome] = &CSceneMng::CS_InHome;
	m_stlMap_Scene[enScene::eSceInSmithy] = &CSceneMng::CS_InSmith;
	m_stlMap_Scene[enScene::eSceForest01] = &CSceneMng::CS_Forest_1;
	m_stlMap_Scene[enScene::eSceInGuild] = &CSceneMng::CS_InGuild;
	m_stlMap_Scene[enScene::eSceInHouse01] = &CSceneMng::CS_InRoom01;
	m_stlMap_Scene[enScene::eSceInHouse02] = &CSceneMng::CS_InRoom02;
	m_stlMap_Scene[enScene::eSceInHouse03] = &CSceneMng::CS_InRoom03;
	m_stlMap_Scene[enScene::eSceInHouse04] = &CSceneMng::CS_InRoom04;
	m_stlMap_Scene[enScene::eSceInHouse05] = &CSceneMng::CS_InRoom05;
	m_stlMap_Scene[enScene::eSceInHouse06] = &CSceneMng::CS_InRoom06;
	m_stlMap_Scene[enScene::esceMountain] = &CSceneMng::CS_Mountain;
	m_stlMap_Scene[enScene::eSceCoast] = &CSceneMng::CS_Coast;
}

bool CSceneMng::CS_Town()
{
	CreateTileMap("Data/Map/Town");
	

	//--------------
	//Create Link//
	//---------------
	CreateLink({ 0, 6 }, { 0,7 }, eSceForest01, { 29, 24 }, { 29,25 },eUp);
	CreateLink({ 7, 0 }, { 8,0 }, esceMountain, {17, 19 }, { 18,19 },eLeft);
	CreateLink({ 23, 16 }, { 23,17 }, eSceCoast, { 0, 5 }, { 0,6 },eDown);

	int i = 0;

	

	//----------------//
	//create building 
	//---------------------------------------------------------------------------------------------------------
	CreateBuilding({ 8,8 }, { 12,11 }, enBuilding::eNormalHouse2, enScene::eSceInHouse01, { 11,7 }, { 11,8 });
	CreateBuilding({ 8,12 }, { 12,15 }, enBuilding::eNormalHouse2, enScene::eSceInHouse02, { 11,7 }, { 11,8 });
	CreateBuilding({ 11,8 }, { 15,11 }, enBuilding::eNormalHouse2, enScene::eSceInHouse03, { 11,7 }, { 11,8 });
	CreateBuilding({ 11,12 }, { 15,15 }, enBuilding::eNormalHouse2, enScene::eSceInHouse04, { 11,7 }, { 11,8 });
	CreateBuilding({ 11,19 }, { 15,22 }, enBuilding::eNormalHouse3, enScene::eSceInHouse05, { 13,7 }, { 13,8 });
	CreateBuilding({ 2,9 }, { 6,14 }, enBuilding::eGuild, enScene::eSceInGuild, { 14,14 }, { 14,15 });
	CreateBuilding({ 11,24 }, { 15,27 }, enBuilding::eMyhome, enScene::eSceInHome, { 14,9 }, { 14,10 });
	CreateBuilding({ 17,8 }, { 21,11 }, enBuilding::eNormalHouse1, enScene::eSceInHouse01, { 11,7 }, { 11,8 });
	CreateBuilding({ 17,12 }, { 21,15 }, enBuilding::eNormalHouse1, enScene::eSceInHouse04, { 11,7 }, { 11,8 });
	CreateBuilding({ 17,19 }, { 21,22 }, enBuilding::eNormalHouse3, enScene::eSceInHouse01, { 11,7 }, { 11,8 });
	CreateBuilding({ 17,24 }, { 21,27 }, enBuilding::eNormalHouse3, enScene::eSceInHouse04, { 11,7 }, { 11,8 });
	CreateBuilding({ 2,19 }, { 6,22 }, enBuilding::eSmith, enScene::eSceInSmithy, { 11,7}, { 11,8 });

	CreateBuilding({ 2,24 }, { 6,27 }, enBuilding::eGroceryStore, enScene::eSceInHouse03, { 11,7 }, { 11,8 });
	CreateBuilding({ 8,19 }, { 12,22 }, enBuilding::eNormalHouse3, enScene::eSceInHouse02, { 11,7 }, { 11,8 });
	CreateBuilding({ 8,24 }, { 12,27 }, enBuilding::eNormalHouse3, enScene::eSceInHouse02, { 11,7 }, { 11,8 });
	CreateBuilding({ 11,2 }, { 15,5 }, enBuilding::eNormalHouse1, enScene::eSceInHouse03, { 11,7 }, { 11,8 });
	CreateBuilding({ 8,2 }, { 12,5 }, enBuilding::eNormalHouse1, enScene::eSceInHouse02, { 11,7 }, { 11,8 });
	CreateBuilding({ 2,2 }, { 6,5 }, enBuilding::eNormalHouse1, enScene::eSceInHouse04, { 11,7 }, { 11,8 });
	CreateBuilding({ 17,2 }, { 21,5 }, enBuilding::eNormalHouse1, enScene::eSceInHouse03, { 11,7 }, { 11,8 });

	



	//----------------
	//create NPC /
	//----------------
	CNPC *pNPC = NULL;

	pNPC = m_pObjMng->CreateNpc(eNPC_01, 100, 400);
	pNPC->SetDialog(m_pDigDataMng->GetDialogChain("0"));

	pNPC = m_pObjMng->CreateNpc(eNPC_02, 150, 400);
	pNPC->SetDialog(m_pDigDataMng->GetDialogChain("1"));

	pNPC = m_pObjMng->CreateNpc(eNPC_03, 200, 400);
	pNPC->SetDialog(m_pDigDataMng->GetDialogChain("2"));

	m_pObjMng->CreateNpc(eNPC_03, 800, 400);
	m_pObjMng->CreateNpc(eNPC_04, 850, 400);
	m_pObjMng->CreateNpc(eNPC_05, 900, 400);
	m_pObjMng->CreateNpc(eNPC_06, 350, 800);
	m_pObjMng->CreateNpc(eNPC_07, 400, 800);
	m_pObjMng->CreateNpc(eNPC_08, 800, 800);
	m_pObjMng->CreateNpc(eNPC_09, 850, 800);
	m_pObjMng->CreateNpc(eNPC_AS, 900, 800);



	//----------------
	//set map sound /
	//----------------
	//m_pSound->LoadWave("music/town.wav", &g_pDS);
	//m_pSound->SetVolume(g_pDS, 100);
	//m_pSound->Play(g_pDS, false);


	return TRUE;
}

bool CSceneMng::CS_Town_North()
{










	return TRUE;
}

bool CSceneMng::CS_Forest_1() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	//CreateTileMap(20, 20, 66);
	CreateTileMap("Data/TilemapSample");


	//-----------------
	//Set Tile at Map /
	//-----------------
	//SetTileAll(enTile::eTile_Grass_Light03, 0, 0);



	//SetTileMapEdge(enTile::eTile_Pillar01, 0, 1);
	//SetTile({ 14,9 }, enTile::eTile_SlabStone_BrownOval, 0, 0);
	//SetTile({ 14,10 }, enTile::eTile_SlabStone_BrownOval, 0, 0);
	
	//SetTileH(5,enTile::eTile_Water_Shallow, 0, 1);

	//SetTileH(6, enTile::eTile_Water_Shallow, 0, 1);

	//SetTileH(7, enTile::eTile_Cliff01, 0, 1);
	//SetTileH(8, enTile::eTile_Cliff02, 0, 1);
	//SetTileH(9, enTile::eTile_Cliff03, 0, 1);
	//SetTileH(10, enTile::eTile_Cliff03, 0, 1);
	//SetTileH(16, enTile::eTile_Water_Shallow, 0, 1);


	//g_pTestdObj01=CreateMonster({ 600,200 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 600,200 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 500,200 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 400,200 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 300,200 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 200,200 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 100,200 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 600,400 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 600,600 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 600,800 }, eMRabbit, eForHostile, FALSE);

	CreateMonster({ 1200,1000 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1200,1000 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1200,1000 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1200,1000 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1200,1000 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1200,1000 }, eMRabbit, eForHostile, FALSE);

	/*int i = 0;


	while (i<20)
	{
		CreateMonster({ __Rand_x(),__Rand_y() }, eMRabbit, eForHostile, FALSE);
		++i;
	}*/



	return TRUE;
}

bool CSceneMng::CS_Forest_2()
{
	return TRUE;
}

bool CSceneMng::CS_Forest_3() 
{
	return TRUE;
}

bool CSceneMng::CS_InHome()
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap(15, 20, 50);

	//-----------------
	//Set Tile at Map /
	//-----------------
	SetTileAll(enTile::eTile_Floor01, 0, 0);
	SetTileMapEdge(enTile::eTile_Pillar01, 0, 1);
	SetTile({ 14,9 }, enTile::eTile_Floor03, 0, 0);
	SetTile({ 14,10 },enTile::eTile_Floor03, 0, 0);

	//--------------
	//Create Link//
	//---------------
	//CreateLink({ 14, 9 }, { 14,10 }, eSceTown, { 0, 5 }, { 0,6 });
	

	//-----------------
	//create building /
	//-----------------


	//----------------
	//create monster /
	//----------------
	//m_pObjMng->CreateMonster(eMRabbit, rand() % GetTileMap()->GetMapWidth(),
	//	rand() % GetTileMap()->GetMapHeight(), eRed, TRUE);

	//----------------
	//create NPC /
	//----------------

	//----------------
	//set map sound /
	//----------------
	


	return TRUE;

}

bool CSceneMng::CS_InSmith()
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap(12, 16, 50);

	//-----------------
	//Set Tile at Map /
	//-----------------
	SetTileAll(enTile::eTile_Floor02, 0, 0);
	

	SetTileH(1, eTile_WoodWall02, 1, 1);
	SetTileH(2, eTile_WoodWall02, 1, 1);
	SetTileH(3, eTile_WoodWall02, 1, 1);
	//SetTileH(4, eTile_WoodWall02, 1, 1);


	SetTileMapEdge(enTile::eTile_Pillar02, 0, 1);

	SetTile({ 11,7 }, enTile::eTile_Floor02, 0, 0);
	SetTile({ 11,8 }, enTile::eTile_Floor02, 0, 0);
	
	CreateOrnament(eOrnament_COM, "COUNTER00", { 4,5 }, { 6,10 }, TRUE);

	CreateOrnament(eOrnament_COM, "WALL_SWORD", { 2,6 }, FALSE);
	CreateOrnament(eOrnament_COM, "WALL_SHIELD01", { 2,9 },  FALSE);

	CreateOrnament(eOrnament_COM, "SWORDTABLE", { 4,1 }, TRUE);
	CreateOrnament(eOrnament_COM, "SWORDTABLE", { 4,2 },  TRUE);
	CreateOrnament(eOrnament_COM, "FORGINGTABLE", { 4,3 },  TRUE);
	CreateOrnament(eOrnament_COM, "FORGINGTABLE", { 4,4 },  TRUE);



	//CreateOrnament(eOrnament_COM, "WALL_SHIELD00", { 2,9 }, { 2,9 }, FALSE);
	//CreateOrnament(eOrnament_COM, "WALL_SHIELD01", { 2,11 }, { 2,11 }, FALSE);
	
	CreateOrnament(eOrnament_COM, "SWORD00", { 6,2 }, TRUE);
	CreateOrnament(eOrnament_COM, "SWORD00", { 6,3 }, TRUE);
	CreateOrnament(eOrnament_COM, "SWORD00", { 8,2 }, TRUE);
	CreateOrnament(eOrnament_COM, "SWORD00", { 8,3 }, TRUE);
	CreateOrnament(eOrnament_COM, "SWORD00", { 10,2 }, TRUE);
	CreateOrnament(eOrnament_COM, "SWORD00", { 10,3 }, TRUE);
	
	CreateOrnament(eOrnament_COM, "ARMOUR00", { 6,12 },  TRUE);
	CreateOrnament(eOrnament_COM, "ARMOUR00", { 8,12 }, TRUE);
	CreateOrnament(eOrnament_COM, "ARMOUR00", { 10,12 }, TRUE);


	CreateOrnament(eOrnament_COM, "ARMOUR01", { 6,13 }, TRUE);
	CreateOrnament(eOrnament_COM, "ARMOUR01", { 8,13 }, TRUE);
	CreateOrnament(eOrnament_COM, "ARMOUR01", { 10,13 }, TRUE);



	CreateOrnament(eOrnament_COM, "ARMOURHUGE", {3,12}, { 4,13 },  TRUE);

	//CreateOrnament(eOrnament_COM, "CHEST00", { 4,7 }, { 7,12 }, TRUE);
	//CreateOrnament(eOrnament_COM, "FURNACE00", { 4,7 }, { 7,12 }, TRUE);
	//CreateOrnament(eOrnament_COM, "FURNACE01", { 4,7 }, { 7,12 }, TRUE);

	
	//CreateOrnament(eOrnament_COM, "WALL_SWORD", { 4,7 }, { 7,12 }, TRUE);
	//CreateOrnament(eOrnament_COM, "SWORD00", { 1,3 }, { 1,4 }, TRUE);

	CreateNpc({ 5,7 }, eNPC_OLDM);


	CNPC *pNPC = NULL;
	pNPC = m_pObjMng->CreateNpc(eNPC_03, 200, 400);
	pNPC->SetDialog(m_pDigDataMng->GetDialogChain("2"));

	return TRUE;
}

bool CSceneMng::CS_InGuild() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/hRoom3");
	//CreateTileMap(15, 20, 50);

	//-----------------
	//Set Tile at Map /
	//-----------------
	//SetTileAll(enTile::eTile_Floor03, 0, 0);
	//SetTileMapEdge(enTile::eTile_Pillar02, 0, 1);
	//SetTile({ 14,9 }, enTile::eTile_Floor03, 0, 0);
	//SetTile({ 14,10 }, enTile::eTile_Floor04, 0, 0);

	return TRUE;
}

bool CSceneMng::CS_InRoom01() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/sRoom1");

	return TRUE;
}
bool CSceneMng::CS_InRoom02() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/sRoom2");

	return TRUE;
}
bool CSceneMng::CS_InRoom03() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/sRoom3");

	return TRUE;
}
bool CSceneMng::CS_InRoom04() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/sRoom4");
	

	return TRUE;
}


bool CSceneMng::CS_Mountain() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/Mountain");

	return TRUE;
}
bool CSceneMng::CS_Coast() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/Harbour");
	CreateMonster({ 500,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 500,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 500,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 500,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 500,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 500,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1000,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1000,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1000,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1000,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1000,500 }, eMRabbit, eForHostile, FALSE);
	CreateMonster({ 1000,500 }, eMRabbit, eForHostile, FALSE);
	
	return TRUE;
}
bool CSceneMng::CS_InRoom05() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/hRoom1");

	return TRUE;

}
bool CSceneMng::CS_InRoom06() 
{
	//-----------------
	//Create Tile Map /
	//-----------------
	CreateTileMap("Data/Map/hRoom2");

	return TRUE;
}