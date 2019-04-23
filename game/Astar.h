#pragma once
#include"TileMap.h"
#include<set>
#include<map>
#include<list>
#include"DS\Heap.h"
#include"DS\HashTable.h"

struct STPathScore
{
	STPathScore()
		:m_dwG(0), m_dwH(0), m_dwF(0), 
		m_pTileRef(NULL), m_pParent(NULL),
		m_bFixed(FALSE){}

	STPathScore(CTile* Tile,DWORD G,DWORD H) :m_dwG(G),m_dwH(H),
		m_pTileRef(Tile), m_pParent(NULL),
		m_bFixed(FALSE)
	{
		CalculateF();
	}

	STPathScore(STPathScore &org) 
	{
		m_pTileRef = org.m_pTileRef;

		m_pParent = org.m_pParent;

		m_dwG = org.m_dwG;
		m_dwH = org.m_dwH;
		m_dwF = org.m_dwF;

		m_bFixed = org.m_bFixed;	
	}

	void CalculateF()
	{
		m_dwF = m_dwG + m_dwH;
	}

	CTile* m_pTileRef;

	STPathScore* m_pParent;

	DWORD m_dwG;
	DWORD m_dwH;
	DWORD m_dwF;

	bool m_bFixed;
};

//template<typename T>
struct FScore_COMPARE /*: public binary_function< T, T, bool >*/
{
	bool operator() (const STPathScore* PathA, const STPathScore* PathB)const
	{
		return PathA->m_dwF < PathB->m_dwF;
	}
};





void DeleteScore(STSlot<STPathScore*> deldata);



//------------------------------------------------------
//Struct path Finding
//------------------------------------------------------
struct Astar_PathFinding
{	
public:

	//creator
	Astar_PathFinding() :m_pRefMap(NULL){}

	//------------------------------------------------------
	//Path finding Function
	//------------------------------------------------------
	bool operator()(CTileMap*map, CObject *obj, POINTFLOAT Dest, std::list<POINT>&GetList)
	{
		if (map == NULL)return FALSE;
		//-------------------------------------------------------
		//Update Map State
		//-------------------------------------------------------
		UpdateMapState(map);

		//-------------------------------------------------------
		//Init 
		//-------------------------------------------------------
		//Get Object position
		POINTFLOAT PtStart = obj->GetPosition();

		//Get Object position in Tile map
		m_pStartTile = map->GetPointTilePos(PtStart);

		if (m_pStartTile == NULL)	return FALSE;
		//Get Dest Tile
		m_pDestTile = map->GetPointTilePos(Dest);
		if (m_pDestTile == NULL)	return FALSE;
		//Already in dest
		if (m_pDestTile == m_pStartTile)
			return TRUE;


		STPathScore *StartTileScore = new STPathScore();

		//Calculate Start Tile Score
		StartTileScore->m_dwG = 0;
		StartTileScore->m_dwH = Calculate_H(m_pStartTile, m_pDestTile);
		StartTileScore->CalculateF();
		StartTileScore->m_pTileRef = m_pStartTile;


		//Insert Start Tile Score
		m_OpenSet.insert(StartTileScore);

		DWORD key = m_pStartTile->GetTileID();
		m_OpenTable.Insert(key, StartTileScore);

		//-------------------------------------------------------
		//Main Loop
		//-------------------------------------------------------
		bool PathFound = FALSE;
		//get near tile score
		while (m_OpenSet.size() != 0)
		{
			if (FindPath())
			{
				PathFound = TRUE;
				break;
			}
		}

		if (PathFound)
		{
			GetList.push_front(m_pstCurPathScore->m_pTileRef->GetTileCenter());//

			while (m_pstCurPathScore->m_pParent != StartTileScore)
			{
				m_pstCurPathScore = m_pstCurPathScore->m_pParent;	//parent path score

				POINT Tiledest = m_pstCurPathScore->m_pTileRef->GetTileCenter();

				Tiledest.y -= 5;//some Adjust

				GetList.push_front(Tiledest);	//save path
			}

			ClearContainerData();
			return TRUE;
		}

		else
		{
			ClearContainerData();
			return FALSE;
		}
	}

private:
	void UpdateCurPath(STPathScore* pathScore)
	{
		m_pstCurPathScore = pathScore;
		m_pCurTile = pathScore->m_pTileRef;
		m_gdCurGrid = pathScore->m_pTileRef->GetGrid();
	}

	DWORD CalOffset(int offRow, int offColumn)
	{
		DWORD	oH = abs(offColumn);
		DWORD	oV = abs(offRow);

		if (oH || oV)
		{
			if (oH == oV)
			{
				return m_dwOffset_d;
			}
			else if (oH)
			{
				return  m_dwOffset_h;
			}
			else if (oV)
			{
				return  m_dwOffset_v;
			}
		}
		else return 0;

	}

	bool GetNearTile(int offset_H, int offset_V)
	{
		//------------------------------------------------------------
		//Step 1: Get Tile From Tilemap and Check if this is Valid
		//------------------------------------------------------------
		CTile* Tile = m_pRefMap->GetTile(m_gdCurGrid.wRow + offset_V, m_gdCurGrid.wColumn + offset_H);

		//Check if the tile is Valid 
		if (Tile)
		{
			if (offset_V && offset_H) //if a block beside you ,you can't go diagonally
			{
				CTile* TileA = m_pRefMap->GetTile(m_gdCurGrid.wRow + offset_V, m_gdCurGrid.wColumn);	//get the Tile Beside you

				if (TileA)//Check if the tile is Valid 
				{
					if (TileA->ifIncludeObstacle())return FALSE;	//check  if The Tile includes obstacle 
				}

				CTile* TileB = m_pRefMap->GetTile(m_gdCurGrid.wRow, m_gdCurGrid.wColumn + offset_H);	//get the Tile Beside you

				if (TileB)//Check if the tile is Valid 
				{
					if (TileB->ifIncludeObstacle())return FALSE;	//check  if The Tile includes obstacle 
				}
			}

			//------------------------------------------------------------
			//Step 2: Check if this Tile is the Destination
			//------------------------------------------------------------
			if (Tile == m_pDestTile)
			{
				STPathScore *DestPathScore = new STPathScore();

				DestPathScore->m_pParent = m_pstCurPathScore;				//Set cur tile as Parent

				DestPathScore->m_pTileRef = Tile;

				DWORD key = Tile->GetTileID();

				//m_OpenSet.insert(DestPathScore);
				m_OpenTable.Insert(key, DestPathScore);

				m_pstCurPathScore = DestPathScore;

				return TRUE;
			}

			//------------------------------------------------------------
			//Step 3: Check if this Tile includes Obstacle
			//------------------------------------------------------------
			if (!Tile->ifIncludeObstacle())
			{
				//for (SetIter = m_CloseSet.begin(); SetIter != m_CloseSet.end(); SetIter++)
				//{
				//	if ((*SetIter)->m_pTileRef == Tile)break;
				//}
				DWORD TileKey = Tile->GetTileID();//get Tile Index

												  //------------------------------------------------------------
												  //Step 4: Check if this Tile is in close Table
												  //------------------------------------------------------------
				if (!m_CloseTable.Search(TileKey))
				{

					//------------------------------------------------------------
					//Step 5: Check if this Tile is in Open Table
					//------------------------------------------------------------

					STPathScore* GetPathScore;
					//-------------------------------------------------------------
					//Branch 1: this tile has already in open Table
					//-------------------------------------------------------------
					if (m_OpenTable.Search(TileKey, GetPathScore))
					{
						//Calculate G Score
						GRID grid = GetPathScore->m_pTileRef->GetGrid();
						DWORD newG = CalOffset((grid.wRow - m_gdCurGrid.wRow), (grid.wColumn - m_gdCurGrid.wColumn));

						//if new G score is less, so it's better path
						if (newG < GetPathScore->m_dwG)
						{
							SetIter = m_OpenSet.find(GetPathScore);
							//Remove Score from open set
							for (; SetIter != m_OpenSet.end(); SetIter++)
							{
								if ((*SetIter)->m_pTileRef->GetTileID() == TileKey)
								{
									m_OpenSet.erase(SetIter);
									break;
								}
							}

							//change 
							GetPathScore->m_pParent = m_pstCurPathScore;	//change it's parent to cur path;
							GetPathScore->m_dwG = m_pstCurPathScore->m_dwG + newG;	//figure out new G Score
							GetPathScore->CalculateF();	//becuase of G Score has Changed ,you need to re calculate F Score

														//ReInsert
							m_OpenSet.insert(GetPathScore);//reInsert changed Score		
						}
					}
					//-------------------------------------------------------------
					//Branch 2: Can't find This tile in Open Table,it's New Tile 
					//-------------------------------------------------------------
					else
					{
						STPathScore *PathScore = new STPathScore();

						PathScore->m_dwG = m_pstCurPathScore->m_dwG + CalOffset(offset_V, offset_H);	//calculate G

						PathScore->m_dwH = Calculate_H(Tile, m_pDestTile);		//calculate H

						PathScore->CalculateF();								//F = G + H

						PathScore->m_pTileRef = Tile;

						PathScore->m_pParent = m_pstCurPathScore;				//Set cur tile as Parent

						m_OpenSet.insert(PathScore);							//insert Score to Open set

						m_OpenTable.Insert(TileKey, PathScore);					//insert Score to Open hash table
					}
				}
			}
		}
		return FALSE;
	}

	bool FindPath()
	{
		//Get The tile which F Value is least
		SetIter = m_OpenSet.begin();

		UpdateCurPath((*SetIter));

		//move this tile from open table to close table
		DWORD key = (*SetIter)->m_pTileRef->GetTileID();

		m_CloseTable.Insert(key, (*SetIter));
		m_OpenTable.Delete(key);

		m_OpenSet.erase(SetIter);

		//left Top Path Score
		if (GetNearTile(-1, -1)) return TRUE;

		//Top Path Score
		if (GetNearTile(0, -1)) return TRUE;

		//Right Top Path Score
		if (GetNearTile(1, -1)) return TRUE;

		//left Path Score
		if (GetNearTile(-1, 0)) return TRUE;

		//right Path Score 
		if (GetNearTile(1, 0)) return TRUE;

		//left bottom Path Score
		if (GetNearTile(-1, 1)) return TRUE;

		//bottom Path Score
		if (GetNearTile(0, 1)) return TRUE;

		//right bottom Path Score
		if (GetNearTile(1, 1)) return TRUE;


		return FALSE;
	}

	void ClearContainerData()
	{
		m_OpenTable.Traversal(DeleteScore);
		m_CloseTable.Traversal(DeleteScore);
	}

	DWORD Calculate_H(CTile* tile_A, CTile* tile_B)
	{
		if (tile_A == NULL || tile_B == NULL) return 0;

		GRID A = tile_A->GetGrid();
		GRID B = tile_B->GetGrid();

		return (abs(A.wRow - B.wRow))*m_dwOffset_v + (abs(A.wColumn - B.wColumn))*m_dwOffset_h;
	}

	void UpdateMapState(CTileMap* CurMap)
	{	
		//Get CurMap
		m_pRefMap = CurMap;	

		//calculate offset Horizontal Vertical
		m_dwOffset_h = m_dwOffset_v = CurMap->GetCelllength();

		//Diagonal offset
		m_dwOffset_d = (DWORD)sqrt((m_dwOffset_v *m_dwOffset_v) + (m_dwOffset_h*m_dwOffset_h));
	}

private:
	CTileMap *m_pRefMap;	//Current Map

	CTile *m_pStartTile;	//Start 

	CTile *m_pDestTile;		//Destination

	//Cur State
	CTile* m_pCurTile;
	GRID m_gdCurGrid;
	STPathScore *m_pstCurPathScore;


	//offset
	DWORD m_dwOffset_h;		//Horzontal Offset
	DWORD m_dwOffset_v;		//Vertical Offset
	DWORD m_dwOffset_d;		//Diagonal Offset

	

	CHashTable<STPathScore*, 100>m_OpenTable;	//open Hash Table

	CHashTable<STPathScore*, 100>m_CloseTable;	//Close Hash Table

	std::multiset<STPathScore*,FScore_COMPARE> m_OpenSet;	//Open MultiSet
	
	std::multiset<STPathScore*>::iterator SetIter;			//MultiSet Iterator
};


struct STPathUpdate
{
	enum enFindResult
	{
		eFailed,
		eSucceed,
		eDefault
	};

	bool operator() (CTileMap* Curmap, POINTFLOAT Dest)
	{
		if (UpdateCurScene(Curmap))
		{
			if (m_enFindResult != eDefault) return FALSE;

			//Astar_PathFinding PathFind;
			return 	PathFind(Dest);
		}
	}

	bool operator() (CTileMap* Curmap, CObject* DestObj)
	{
		m_pCurMap = Curmap;

		//------------------------------
		//Case: the Map has Changed
		//------------------------------
		if (UpdateCurScene(Curmap))
		{
			//Init
			m_pTarget = DestObj;

			POINTFLOAT dest = DestObj->GetPosition();

			m_pDestTile = Curmap->GetPointTilePos(dest);

			//Astar_PathFinding PathFind;
			return 	PathFind(dest);
		}
		//------------------------------
		//Case: the Map has not Changed
		//------------------------------
		else
		{	//------------------------------
			//Case: Target Has not Changed
			//------------------------------
			if (m_pTarget == DestObj)
			{
				//Check If Target's Position Has Changed
				POINTFLOAT dest = DestObj->GetPosition();

				CTile* DestTile = Curmap->GetPointTilePos(dest);

				//Case 1: the Target's Position not Changed
				if (m_pDestTile == DestTile)
					return FALSE;

				//Case 2:the Target's Position has Changed
				else
				{
					m_pDestTile = DestTile;
					//Astar_PathFinding PathFind;
					return 	PathFind(dest);
				}
			}
			//------------------------------
			//Case: Target Has Changed
			//------------------------------
			else
			{
				//Init
				m_pTarget = DestObj;

				POINTFLOAT dest = DestObj->GetPosition();

				m_pDestTile = Curmap->GetPointTilePos(dest);

				//Astar_PathFinding PathFind;
				return 	PathFind(dest);
			}
		}
	}


	STPathUpdate(CObject* Myself) :
		m_enFindResult(eDefault), m_pCurMap(NULL), m_pTarget(NULL), m_pDestTile(NULL),m_pMyself(Myself){}


	bool PathFind(POINTFLOAT Dest)
	{
		//-----------------
		//Path Find
		//-----------------
		m_listPath.clear();

		Astar_PathFinding PathFind;
		if (PathFind(m_pCurMap, m_pMyself, Dest, m_listPath))
		{
			m_enFindResult = eSucceed;
			return TRUE;
		}
		else
		{
			m_enFindResult = eFailed;
			return FALSE;
		}
	
	}

	bool UpdateCurScene(CTileMap* Curmap);



	std::list<POINT>m_listPath;

	CTileMap* m_pCurMap;

	enScene m_CurScene;
	
	CObject* m_pTarget;

	CObject* m_pMyself;

	CTile *m_pDestTile;

	enFindResult m_enFindResult;
};
