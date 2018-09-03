#include "Pathfinder.h"
#include "QuadTree.h"
#include "Collision.h"
#include "BinaryHeap.h"

#include <iostream>
#include <DirectXMath.h>

using namespace DirectX;
using namespace rle;

Pathfinder::Pathfinder(DirectX::XMINT2 size, const std::vector<shape::Triangle> &tris)
{
	map = new TileMap(size);
	map_size.x = (float) size.x;
	map_size.y = 0.f;
	map_size.z = (float) size.y;

	std::vector<QuadNode> quads = QuadTree::MakeQuadTree(tris);

	for (int y = 0; y < (int)map->getWidth(); y++)
	{
		for (int x = 0; x < (int)map->getHeight(); x++)
		{
			bool tileIsWall = false;
			bool closed = false;
			shape::Sphere sphere;
			// Correct rotation of the map generation. (+y = up, +x = right)
			int sphere_posx = (x * 2 - (int)map->getWidth());
			int sphere_posy = - (y * 2 - (int)map->getHeight());
			sphere.center = XMFLOAT3((float)sphere_posx, 1.f, (float)sphere_posy);
			sphere.range = 0.7f;

			std::vector<Hit> total_hits;
			std::vector<Hit> hits;

			std::vector<QuadNode*> overlappingQuads = Collision::GetOverlappingQuads(sphere, quads);
			for (int i = 0; i < overlappingQuads.size(); i++)
			{
				hits = Collision::SphereQuad(sphere, *overlappingQuads[i]);
				if ((int)hits.size() > 0)
				{
					tileIsWall = true;
				}
			}

			map->get(x, y).wall = tileIsWall;
			map->get(x, y).x = x;
			map->get(x, y).y = y;
		}
	}

	first = new AStarTile();
	last = new AStarTile();
	current = new AStarTile();

	for (int i = 0; i < 8; i++)
	{
		neighbours[i] = new AStarTile();
	}
	// PRINTING
	std::cout << "   ";
	for (size_t x = 0; x < map->getWidth(); x++)
	{
		if (x < 10)
			std::cout << x << " ";
		else if (x < 100)
			std::cout << x;
	}
	std::cout << std::endl;
	for(int y = 0; y < map->getHeight(); y++)
	{
		if (y < 10)
			std::cout << y << "  ";
		else if (y < 100)
			std::cout << y << " ";
		else if (y < 1000)
			std::cout << y << "";
	
		for (int x = 0; x < map->getWidth(); x++)
		{
			if (map->get(x, y).wall)
				std::cout << "[]";
			else
				std::cout << "  ";
		}
		std::cout << std::endl;
	}

}
Pathfinder::~Pathfinder()
{
	delete first;
	delete last;
	delete current;
	delete map;
	for (int i = 0; i < 8; i++)
	{
		delete neighbours[i];
	}
	//for (int i = 0; i < openList.items.size(); i++) { delete(openList.items[i]); }
}

AStarTile *Pathfinder::posToTile(const XMFLOAT3 pos)
{
	float x = (float)round(pos.x);
	float y = (float)round(pos.z);

	x = (x + map_size.x) * 0.5f;
	y = (y + map_size.z) * 0.5f;

	//x = ceil(Math::mclamp((float)x, 0, (float)map->getWidth() - 1));
	//y = ceil(Math::mclamp((float)y, 0, (float)map->getHeight() - 1));

	AStarTile* tile = &map->get((unsigned int)x, (unsigned int)y);
	return tile;
}
XMFLOAT3 Pathfinder::tileToPos(AStarTile * tile)
{
	float x = (float)tile->x;
	float y = (float)tile->y;

	x = (x * 2 - map_size.x);
	y = (y * 2 - map_size.z);

	return XMFLOAT3(x, 0, y);
}

void Pathfinder::setupNeighbours(std::vector<AStarTile*> &tiles, AStarTile *current)
{
	if (current->x + 1 >= 0 && current->x + 1 < (int)map->getWidth() && current->y >= 0 && current->y < (int)map->getHeight())
		tiles[0] = &map->get(current->x + 1, current->y);
	else
		tiles[0] = nullptr;

	if (current->x >= 0 && current->x < (int)map->getWidth() && current->y + 1 >= 0 && current->y + 1 < (int)map->getHeight())
		tiles[1] = &map->get(current->x, current->y + 1);
	else
		tiles[1] = nullptr;

	if (current->x + 1 >= 0 && current->x + 1 < (int)map->getWidth() && current->y + 1 >= 0 && current->y + 1 < (int)map->getHeight())
		tiles[2] = &map->get(current->x + 1, current->y + 1);
	else
		tiles[2] = nullptr;

	if (current->x - 1 >= 0 && current->x - 1 < (int)map->getWidth() && current->y + 1 >= 0 && current->y + 1 < (int)map->getHeight())
		tiles[3] = &map->get(current->x - 1, current->y + 1);
	else
		tiles[3] = nullptr;

	if (current->x - 1 >= 0 && current->x - 1 < (int)map->getWidth() && current->y >= 0 && current->y < (int)map->getHeight())
		tiles[4] = &map->get(current->x - 1, current->y);
	else
		tiles[4] = nullptr;

	if (current->x - 1 >= 0 && current->x - 1 < (int)map->getWidth() && current->y - 1 >= 0 && current->y - 1 < (int)map->getHeight())
		tiles[5] = &map->get(current->x - 1, current->y - 1);
	else
		tiles[5] = nullptr;

	if (current->x >= 0 && current->x < (int)map->getWidth() && current->y - 1 >= 0 && current->y - 1 < (int)map->getHeight())
		tiles[6] = &map->get(current->x, current->y - 1);
	else
		tiles[6] = nullptr;

	if (current->x + 1 >= 0 && current->x + 1 < (int)map->getWidth() && current->y - 1 >= 0 && current->y - 1 < (int)map->getHeight())
		tiles[7] = &map->get(current->x + 1, current->y - 1);
	else
		tiles[7] = nullptr;

	for (int i = 0; i < 8; i++)
	{
		if (tiles[i] != nullptr && tiles[i]->parent == nullptr)
		{
			tiles[i]->parent = current;
		}
	}
	return;
}

void Pathfinder::resetValues()
{
	for (int x = 0; x < map_size.x; x++)
	{
		for (int y = 0; y < map_size.z; y++)
		{
			map->get(x, y).fCost = -1.f;
			map->get(x, y).gCost = -1.f;
			map->get(x, y).hCost = -1.f;
			map->get(x, y).parent = nullptr;
			map->get(x, y).index = -1;
			map->get(x, y).closed = false;
		}
	}
}

std::vector<XMFLOAT3> Pathfinder::retrievePath(AStarTile &last_tile, AStarTile &first)
{
	std::vector<AStarTile> list;
	std::vector<XMFLOAT3> path;
	list.push_back(last_tile);
	AStarTile current = last_tile;
	if (current.parent != nullptr)
	{
		while (current.parent != nullptr && (current.parent->x != first.x || current.parent->y != first.y))
		{
			current = *current.parent;
			list.push_back(current);
		}

		for (int i = 1; i < list.size() + 1; i++)
		{
			XMFLOAT3 pathpoint = tileToPos(&list[list.size() - i]);
			path.push_back(pathpoint);
		}
	}
	else
	{
		XMFLOAT3 pathpoint = tileToPos(&current);
		path.push_back(pathpoint);
	}
	return path;
}

int Pathfinder::searchForTileInList(AStarTile *tile, std::vector<AStarTile*> *list)
{
	for (int i = 0; i < (int)list->size(); i++)
	{
		if (list->at(i)->x == tile->x && list->at(i)->y == tile->y)
		{
			return i;
		}
	}
	return -1;
}

unsigned int Pathfinder::getTileDistance(AStarTile *start, AStarTile *end)
{
	// (shortest one * 14), (longest - shortest) * 10 is the distance
	int dx, dy;
	dx = abs(start->x - end->x);
	dy = abs(start->y - end->y);

	if (dx > dy)
		return (int)((dy * 14) + (dx - dy) * 10);
	else
		return (int)((dx * 14) + (dy - dx) * 10);
}


PathFindResult Pathfinder::doAStar(XMFLOAT3 char_loc, XMFLOAT3 endLoc)
{
	resetValues();
	PathFindResult result;
	result.found = false;

	first = posToTile(char_loc);
	last = posToTile(endLoc);


	if (first->x == last->x && first->y == last->y)
	{
		result.found = true;
		result.path = retrievePath(*first, *first);
		return result;
	}


	BinaryHeap<AStarTile> openList;
	//std::vector<AStarTile*> closedList;

	first->fCost = (float)getTileDistance(first, last);
	first->gCost = 0;
	first->parent = nullptr;

	openList.addItem(first);
	float lowestF = 9999999;

	bool fininshed = false;
	while (!fininshed)
	{
		/*		removing current from open, adding it to closed.		*/
		if (openList.items.size() == 0 || openList.items.size() > 1000)
		{
			result.found = false;
			return result;
		}
		current = openList.removeFirst();
		current->closed = true;

		/*		check if current is target node, set found.				*/
		if (current->x == last->x && current->y == last->y)
		{
			result.found = true;
			result.path = retrievePath(*current, *first);
			return result;
		}


		if (!fininshed)
		{
			/*		For each neighbour of the node, set it up				*/
			setupNeighbours(neighbours, current);

			for (int i = 0; i < neighbours.size(); i++)
			{
				/*  if neighbour is wall or neighbour is in closed skip to the next neighbour.		*/
				if (neighbours[i] != nullptr)
				{
					if (neighbours[i]->wall || neighbours[i]->closed) continue;

					neighbours[i]->gCost = (float)getTileDistance(neighbours[i], first);
					neighbours[i]->hCost = (float)getTileDistance(neighbours[i], last);
					neighbours[i]->fCost = neighbours[i]->gCost + neighbours[i]->hCost;

					if (!openList.contains(neighbours[i]))
					{
						openList.addItem(neighbours[i]);
					}
				}
			}
		}
	}
	for (int i = 0; i < openList.items.size(); i++) { delete(openList.items[i]); }

	return result;
}
