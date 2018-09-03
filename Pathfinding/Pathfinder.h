#pragma once
#include "vector"
#include "IValue.h"
#include "Shape.h"

namespace rle
{

	//! This indicates how far the A*  algorithm should look for a path.
#define MAX_PATH_RANGE float_MAX

//! A struct to hold the information for A* algorithm.
	class AStarTile : public IValue
	{
	public:
		AStarTile() {};
		~AStarTile() {};

		//! Returns the fcost of the tile.
		float getValue() override { return fCost; };

		//! Returns the hcost of the tile.
		float getSecondaryValue() override { return hCost; };

		//! Checks if the tile is closer to the target.
		bool isLess(IValue *tile) override {
			if (fCost == tile->getValue())
				if (hCost < tile->getSecondaryValue())
					return true;
				else
					return false;

			else return (fCost < tile->getValue());
		}

		//! Checks if the tile is further away from the target.
		bool isMore(IValue *tile) override {
			if (fCost == tile->getValue())
				return (hCost > tile->getSecondaryValue());

			else return (fCost > tile->getValue());
		}

		// Returns if the tile is equal to the other tile.
		bool isEqual(IValue *tile) override {
			return (this == tile);
		}

		//! The location.
		int x = -1, y = -1;
		//! G cost + F cost, results in the number that we're calculating.
		float fCost = -1.f;
		//! The distance from current to the start.
		float gCost = -1.f;
		//! The distance from current to destination.
		float hCost = -1.f;
		//! Indicates if a tile is a wall.
		bool wall = false;
		bool closed = false;
		//! Parent
		AStarTile * parent = nullptr;
	};

	//! A map of tiles, allows the pathfinding to happen on it.
	class TileMap
	{
	public:
		//! Constructor creates and allocates the memory needed for the map. 
		TileMap(DirectX::XMINT2 _size) : size(_size), tiles(std::vector<AStarTile>(size.x * size.y)) {	}

		//! Returns the tile at a specified width position (x) and height position (y)
		AStarTile &get(int x, int y)
		{
			return(tiles[(x % size.x) + (y % size.y) * getWidth()]);
		}

		//! Returns the map width.
		int getWidth() const
		{
			return size.x;
		};

		//! Returns the map height.
		int getHeight() const
		{
			return size.y;
		};


	private:
		//! The size of the map.
		DirectX::XMINT2 size;
		//! All of the actual tiles in the map.
		std::vector<AStarTile> tiles;
	};


	//! A struct to hold the pathfinding results.
	struct PathFindResult
	{
		//! Has the correct path been found?
		bool found;
		//! if a path has been found, returns a list of tiles.
		std::vector<DirectX::XMFLOAT3> path;
	};


	//! This class contains the pathfinding algorithms for the ai and player.
	class Pathfinder
	{
	public:
		Pathfinder(DirectX::XMINT2 size, const std::vector<shape::Triangle> &tris);
		~Pathfinder();

		//! Simply converts a position to a tile location.
		AStarTile *posToTile(const DirectX::XMFLOAT3 pos);
		DirectX::XMFLOAT3 tileToPos(AStarTile *tile);

		//! Finds the basic distance between tiles.
		unsigned int getTileDistance(AStarTile *start, AStarTile *end);

		/*!
		A star algorithm to find a path. Returns the positions that the character has to walk towards, in the right order.
		The input values are the current locations of the character and the destinations location.
		*/
		PathFindResult doAStar(DirectX::XMFLOAT3 charLoc, DirectX::XMFLOAT3 endLoc);

		//! sets up the tiles next to the current tile.
		void setupNeighbours(std::vector<AStarTile*> &tiles, AStarTile  *current);

		//! Resets all the tiles to their previous values.
		void resetValues();

		//! Returns the final path in a list of tiles.
		std::vector<DirectX::XMFLOAT3> retrievePath(AStarTile &lastTile, AStarTile &first);

		//! This is the map of the floor we can walk on.
		TileMap *map;
		DirectX::XMFLOAT3 map_size = DirectX::XMFLOAT3(0, 0, 0);

	private:
		//! finds the id of the tile in the list, returns -1 if not found.
		int searchForTileInList(AStarTile *tile, std::vector<AStarTile*> *list);
		std::vector<AStarTile*> neighbours = std::vector<AStarTile*>(8);

		AStarTile *first;
		AStarTile *last;
		AStarTile *current;
	};
}