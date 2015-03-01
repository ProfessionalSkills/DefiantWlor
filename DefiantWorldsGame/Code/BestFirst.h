#include "Grid.h"

using namespace std;

//Best First pathfinding algorithm 

struct SPathTile
{
	CTile* mTile;
	SPathTile* mParent;
	int mHeuristic;
	int ID;

	SPathTile()
	{

	}

	SPathTile(CTile* Tile, SPathTile* Parent)
	{
		mTile = Tile;
		mParent = Parent;
		ID = Tile->GetGridPos().mPosX * GRID_TILE_SIZE + Tile->GetGridPos().mPosY;
	}
	float ManHattanDistance(SPathTile nodeEnd) //The ManHattan distance between two nodes, determined by summing the difference between the nodes on both axis
	{
		float x = (float)(fabs((float)(this->mTile->GetWorldPos().x - nodeEnd.mTile->GetWorldPos().x)));
		float y = (float)(fabs((float)(this->mTile->GetWorldPos().z - nodeEnd.mTile->GetWorldPos().z)));

		return x + y;
	}
};

class CBestFirst
{
public:
	CBestFirst(CGrid* grid); //Constructor
	~CBestFirst();//De-structor
	
	void FindPath(DX::XMFLOAT3 currentNode, DX::XMFLOAT3 goalNode, CTile* (&mapNodes)[10][10]); //Generates a path from the current node to the target/goal node
	void ClearOpenList() { mOpenList.clear(); } //Clears the open list
	void ClearClosedList() { mClosedList.clear(); } //Clears the closed list
	void ClearGoalPath() { mGoalPath.clear(); } //Clears the goal path list

	DX::XMFLOAT3  NextNode(); //Obtains the next node in the goal path



private:
	void SetStartGoal(SPathTile &start, SPathTile &goal); //Sets up the start node and the goal node
	void PathOpened(float x, float y, SPathTile *parent); //Determines the cost of a child node to the goal node and whether it is the shortest path to the goal
	SPathTile GetNextTile(); //Reads through the possible next nodes in the path to the goal to determine which has the lowest total cost and returns that 
	void ContinuePath(); //Generated a path to the goal by determining which path has the smallest path cost using surrounding nodes to the current node

	SPathTile mStartingNode; //The node that the path will originate from
	std::vector<DX::XMFLOAT3*> mGoalPath; //The finalised list of nodes which will lead from the starting node to the target node
	SPathTile mTarget; //The node that the path will finish on 
	int mNodeIndex;
	bool mInitialized; //Determines whether the pathfinding setup has been initialised
	bool mGoalFound; //Determines whether the goal node has been found
	CGrid* mGrid;

	std::vector<SPathTile> mOpenList; //Will contain at some point all available nodes that could be used to construct a goal path
	std::vector<SPathTile> mClosedList;//Contains the nodes which have been determined to have the smallest cost to goal

};

bool costCompare(SPathTile a, SPathTile b)
{
	return a.mHeuristic < b.mHeuristic;
}

