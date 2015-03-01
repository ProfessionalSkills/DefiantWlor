#include "BestFirst.h"

CBestFirst::CBestFirst(CGrid* grid) //Constructor
{
	// Initialises and sets booleans
	mInitialized = false;
	mGoalFound = false;
	mGrid = grid;
}
CBestFirst::~CBestFirst(void) //Deconstructor
{
	for (int i = 0; i < mOpenList.size(); i++) //Delete all items in the open list
	{
		//delete openList[i];
	}

	mOpenList.clear(); //Clear the open list 

	for (int i = 0; i < mClosedList.size(); i++) //Delete all items in the closed list
	{
		//delete closedList[i];
	}

	mClosedList.clear(); //Clear the closed list

}


void CBestFirst::FindPath(DX::XMFLOAT3  currentNode, DX::XMFLOAT3  goalNode, CTile* (&mapNodes)[10][10])
{
	if (!mInitialized) //If the start has not been initialized
	{
		for (int i = 0; i < mOpenList.size(); i++) //Delete all items in the open list
		{
			//delete openList[i];
		}

		mOpenList.clear(); //Clear the open list 

		for (int i = 0; i < mClosedList.size(); i++) //Delete all items in the closed list
		{
			//delete closedList[i];
		}

		mClosedList.clear(); //Clear the closed list

		for (int i = 0; i < mGoalPath.size(); i++) //Delete the path to the goal
		{
			//delete mGoalPath[i];
		}

		mGoalPath.clear(); //Clear the goal path
		
		int startingNodeX = currentNode.x;
		int startingNodeZ = currentNode.z;
		//Initialize the start node
		SPathTile start( mapNodes[startingNodeX][startingNodeZ], nullptr);

		//Initialize goal node
		int goalNodeX = goalNode.x;
		int goalNodeZ = goalNode.z;
		SPathTile goal(mapNodes[goalNodeX][goalNodeZ], nullptr);


		SetStartGoal(start, goal); //Sets the start and the goal
		mInitialized = true; //Verifying that the start and goal have been initialized
	}
	if (mInitialized) //If the start and goal have been initialized
	{
		ContinuePath(); //Continue finding a path
	}
}

void CBestFirst::SetStartGoal(SPathTile &start, SPathTile &goal) //Sets up the starting and target/goal nodes
{
	mStartingNode = start; //Create the starting node
	mTarget = goal; //Create the target node 
	mStartingNode.mHeuristic = mStartingNode.ManHattanDistance(mTarget); //Determines the Heuristic distance to the target 
	mStartingNode.mParent = 0; //The starting node has no parents
	mOpenList.push_back(mStartingNode);
}

SPathTile CBestFirst::GetNextTile() // Determines the next node in the path to the goal
{
	SPathTile nextNode = mOpenList[0]; //the next node 
	return nextNode; //Returns the next node
}

void CBestFirst::PathOpened(float x, float y, SPathTile *parent) //Generates a new child node and determines the total cost to target/goal node 
{

	SPointData childPosition( x, y );

	int id = y * GRID_TILE_SIZE + x; //Generates an ID for the node
	for (int i = 0; i < mClosedList.size(); i++) // If the node is already in the closed list, end the function 
	{
		if (id == mClosedList[i].ID)
		{
			return;
		}
	}

	SPathTile child(mGrid->GetTileData(childPosition), parent); //creates a new child node
	child.mHeuristic = child.ManHattanDistance(mTarget); //Sets the ManHattanDistance to the target/goal node

	for (int i = 0; i < mOpenList.size(); i++) //For each item in the open list, checks the child ID against the list item ID
	{
		if (id == mOpenList[i].ID) //If the node is in the open list
		{

			if (mOpenList[i].mHeuristic > child.mHeuristic) //If the new cost is lower than current one, replace parent node and distance value of open list item with new node
			{
				mOpenList[i].mHeuristic = child.mHeuristic;
				mOpenList[i].mParent = &child;
			}
			else //Else if cost is higher or the same 
			{
				//delete child;
				return;
			}

		}
	}
	mOpenList.push_back(child); //Adds the new child node to the end of the open list
}

void CBestFirst::ContinuePath() //Determines whether the current node is the goal. If it is not then the child node of the current node is calculated 
{
	if (mOpenList.empty()) //If there is no longer a path to go expand, return
	{
		return;
	}

	std::sort(mOpenList.begin(), mOpenList.end(), costCompare);

	SPathTile currentNode = GetNextTile(); //The current node in the path
	if (currentNode.ID == mTarget.ID) //If the current node is the target/goal node
	{
		mTarget.mParent = currentNode.mParent; //Sets the parent of the target/goal node to be that of the current node, as they are the same node

		SPathTile* getNode; //The node that will obtain the path from the starting node to the target/goal node
		for (getNode = &mTarget; getNode != 0; getNode = getNode->mParent) //Looping through nodes and checking shortest path to target, which is added to the goal path
		{
			mGoalPath.push_back(new DX::XMFLOAT3(getNode->mTile->GetGridPos().mPosX, getNode->mTile->GetGridPos().mPosY, 0.0f));
		}
		//delete mTarget;
		mGoalFound = true; //Set that the goal has been found
		return;
	}

	mClosedList.push_back(currentNode);
	mOpenList.erase(mOpenList.begin());

	//If the current node is not the goal node, obtain all nodes surrounding the current node and determine the child node closest to the goal
	if (currentNode.mTile->GetGridPos().mPosY <= GRID_SIZE_Y) //If the node is within the coordinate limits
	{
		//up Node
		PathOpened(currentNode.mTile->GetGridPos().mPosX, currentNode.mTile->GetGridPos().mPosY + 1, &currentNode);
	}
	if (currentNode.mTile->GetGridPos().mPosY >= 0)
	{
		//down Node
		PathOpened(currentNode.mTile->GetGridPos().mPosX, currentNode.mTile->GetGridPos().mPosY - 1, &currentNode);
	}

	if (currentNode.mTile->GetGridPos().mPosX <= GRID_SIZE_X)
	{
		//right node
		PathOpened(currentNode.mTile->GetGridPos().mPosX + 1, currentNode.mTile->GetGridPos().mPosY, &currentNode);
	}

	if (currentNode.mTile->GetGridPos().mPosX >= 1)
	{
		PathOpened(currentNode.mTile->GetGridPos().mPosX - 1, currentNode.mTile->GetGridPos().mPosY, &currentNode);
	}
	ContinuePath();
}

DX::XMFLOAT3 CBestFirst::NextNode() //Gets first position from the shortest path in the list
{
	int index = 1; //Node increment value

	DX::XMFLOAT3 nextNode = { 0.0f, 0.0f, 0.0f }; //The position of the next node
	nextNode.x = mGoalPath[mGoalPath.size() - index]->x; //sets the x coordinate to that of the last item in the goal Path
	nextNode.y = mGoalPath[mGoalPath.size() - index]->y; //sets the y coordinate to that of the last item in the goal Path
	nextNode.z = 0.0f; //As 'z' is not used in our 2-dimensional setup, it is just set to 0.0f

	if (index <= mGoalPath.size()) //If there is more than one item in the goal path vector
	{
		delete mGoalPath[mGoalPath.size() - index]; //Erases the last item in the goal path 
		mGoalPath.resize(mGoalPath.size() - index);
	}
	return nextNode;
}
