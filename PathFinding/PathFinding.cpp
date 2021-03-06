// PathFinding.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include <vector>

//definitions of the maximum path finding nodes
#define X_MAX 1000
#define X_STEP 20
#define Y_MAX 500
#define Y_STEP 20

struct Node {
	int y;
	int x;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};

//represents the square map using the nodes
inline bool operator < (const Node & lhs, const Node & rhs)
{
	return lhs.fCost < rhs.fCost;
}

static bool isValid(int x, int y)
{
	int id = x + y * (X_MAX / X_STEP);
	if (world.obstacles.count(id) == 0)
	{
		if (x < 0 || y < 0 || x >= (X_MAX / X_STEP) || y >= (Y_MAX / Y_STEP))
		{
			return false;
		}
		return true;
	}
	return false;
}

static bool isDestination(int x, int y, Node dest)
{
	if (x == dest.x && y == dest.y)
	{
		return true;
	}
	return false;
}

static double calculateH(int x, int y, Node dest)
{
	double H = (sqrt((x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y)));
	return H;
}

static vector<Node> aStar(Node player, Node dest) {
	vector<Node> empty;
	if (isValid(dest.x, dest.y) == false) {
		cout << "Destination is an obstacle" << endl;
		return empty;
	}
	if (isDestination(player.x, player.y, dest)) {
		cout << "You are the destination" << endl;
		return empty;
	}
	bool closedList[(X_MAX / X_STEP)][(Y_MAX / Y_STEP)];

	array<array < Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> allMap;
	for (int x = 0; x < (X_MAX / X_STEP); x++) {
		for (int y = 0; y < (Y_MAX / Y_STEP); y++) {
			allMap[x][y].fCost = FLT_MAX;
			allMap[x][y].gCost = FLT_MAX;
			allMap[x][y].hCost = FLT_MAX;
			allMap[x][y].parentX = -1;
			allMap[x][y].parentY = -1;
			allMap[x][y].x = x;
			allMap[x][y].y = y;

			closedList[x][y] = false;
		}
	}

	int x = player.x;
	int y = player.y;
	allMap[x][y].fCost = 0.0;
	allMap[x][y].gCost = 0.0;
	allMap[x][y].hCost = 0.0;
	allMap[x][y].parentX = x;
	allMap[x][y].parentY = y;

	vector<Node> openList;
	openList.emplace_back(allMap[x][y]);
	bool destinationFound = false;

	while (!openList.empty() && openList.size()<(X_MAX / X_STEP)*(Y_MAX / Y_STEP)) {
		Node node;
		do {
			
			float temp = FLT_MAX;
			vector<Node>::iterator itNode;
			for (vector<Node>::iterator it = openList.begin();
				it != openList.end(); it = next(it)) {
				Node n = *it;
				if (n.fCost < temp) {
					temp = n.fCost;
					itNode = it;
				}
			}
			node = *itNode;
			openList.erase(itNode);
		} while (isValid(node.x, node.y) == false);

		x = node.x;
		y = node.y;
		closedList[x][y] = true;

		for (int newX = -1; newX <= 1; newX++) {
			for (int newY = -1; newY <= 1; newY++) {
				double gNew, hNew, fNew;
				if (isValid(x + newX, y + newY)) {
					if (isDestination(x + newX, y + newY, dest))
					{
						allMap[x + newX][y + newY].parentX = x;
						allMap[x + newX][y + newY].parentY = y;
						destinationFound = true;
						return makePath(allMap, dest);
					}
					else if (closedList[x + newX][y + newY] == false)
					{
						gNew = node.gCost + 1.0;
						hNew = calculateH(x + newX, y + newY, dest);
						fNew = gNew + hNew;
						
						if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
							allMap[x + newX][y + newY].fCost > fNew)
						{
							// Update the details of this neighbour node
							allMap[x + newX][y + newY].fCost = fNew;
							allMap[x + newX][y + newY].gCost = gNew;
							allMap[x + newX][y + newY].hCost = hNew;
							allMap[x + newX][y + newY].parentX = x;
							allMap[x + newX][y + newY].parentY = y;
							openList.emplace_back(allMap[x + newX][y + newY]);
						}
					}
				}
			}
		}
	}
	if (destinationFound == false) {
		cout << "Destination not found" << endl;
		return empty;
	}
}

static vector<Node> makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest) {
	try {
		cout << "Found a path" << endl;
		int x = dest.x;
		int y = dest.y;
		stack<Node> path;
		vector<Node> usablePath;

		while (!(map[x][y].parentX == x && map[x][y].parentY == y)
			&& map[x][y].x != -1 && map[x][y].y != -1)
		{
			path.push(map[x][y]);
			int tempX = map[x][y].parentX;
			int tempY = map[x][y].parentY;
			x = tempX;
			y = tempY;

		}
		path.push(map[x][y]);

		while (!path.empty()) {
			Node top = path.top();
			path.pop();
			usablePath.emplace_back(top);
		}
		return usablePath;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
}

int main()
{

	//todo make it useable 
    return 0;
}

