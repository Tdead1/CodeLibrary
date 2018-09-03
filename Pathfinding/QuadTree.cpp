#include "QuadTree.h"
using namespace rle;

std::vector<QuadNode> QuadTree::MakeQuadTree(const std::vector<shape::Triangle> map_tris)
{
	std::vector<QuadNode> quads;
	std::vector<QuadNode> quadsToDevide;
	QuadNode northEast, southEast, southWest, northWest;

	DirectX::XMFLOAT3 avg = DirectX::XMFLOAT3(0, 0, 0);
	bool keepGoing = true;

	// If this is the first time this function runs...
	if (quadsToDevide.size() == 0)
	{
		// Break early if the amount of triangles can fit in one node.
		if (map_tris.size() < MAX_TRIS_AMOUNT)
		{
			QuadNode wholeMap;
			for (size_t i = 0; i < map_tris.size(); i++)
			{
				wholeMap.tris.push_back(map_tris[i]);
			}
			quads.push_back(wholeMap);
			setMinMax(quads[0]);
			return quads;
		}

		// Do the first devision of the quads.
		// Calculate average
		for (size_t i = 0; i < map_tris.size(); i++)
		{
			setAvg(map_tris[i], avg);
		}
		avg = DirectX::XMFLOAT3(avg.x / ((float)map_tris.size() * 3.f), avg.y / ((float)map_tris.size() * 3.f), avg.z / ((float)map_tris.size() * 3.f));
		// Putting the triangles into the right quadrant(s)
		for (int i = 0; i < map_tris.size(); i++)
		{
			if (map_tris[i].p1.x > avg.x && map_tris[i].p1.z > avg.z)
				northEast.tris.push_back(map_tris[i]);
			if (map_tris[i].p1.x > avg.x && map_tris[i].p1.z < avg.z)
				southEast.tris.push_back(map_tris[i]);
			if (map_tris[i].p1.x < avg.x && map_tris[i].p1.z < avg.z)
				southWest.tris.push_back(map_tris[i]);
			if (map_tris[i].p1.x < avg.x && map_tris[i].p1.z > avg.z)
				northWest.tris.push_back(map_tris[i]);
		}
		
		quadsToDevide.push_back(northEast);
		quadsToDevide.push_back(southEast);
		quadsToDevide.push_back(southWest);
		quadsToDevide.push_back(northWest);

		northEast.tris.clear();
		southEast.tris.clear();
		southWest.tris.clear();
		northWest.tris.clear();
	}

	// Devide or add the newly made quads.
	while( (int)quadsToDevide.size() > 0)
	{
		for (int i = 0; i < (int)quadsToDevide.size(); i++)
		{
			QuadNode current = *quadsToDevide.begin();
			quadsToDevide.erase(quadsToDevide.begin());

			// Subdivide
			// Calculate avarage
			for (int j = 0; j < current.tris.size(); j++)
			{
				setAvg(current.tris[j], avg);
			}
			
			avg = DirectX::XMFLOAT3(avg.x / ((float)current.tris.size() * 3.f), avg.y / ((float)current.tris.size() * 3.f), avg.z / ((float)current.tris.size() * 3.f));
			// Adding triangles to their quad
			for (int j = 0; j < current.tris.size(); j++)
			{
				if (   current.tris[j].p1.x > avg.x && current.tris[j].p1.z > avg.z 
					|| current.tris[j].p2.x > avg.x && current.tris[j].p2.z > avg.z 
					|| current.tris[j].p3.x > avg.x && current.tris[j].p3.z > avg.z)
					northEast.tris.push_back(current.tris[j]);
				if (   current.tris[j].p1.x > avg.x && current.tris[j].p1.z < avg.z 
					|| current.tris[j].p2.x > avg.x && current.tris[j].p2.z < avg.z
					|| current.tris[j].p3.x > avg.x && current.tris[j].p3.z < avg.z)
					southEast.tris.push_back(current.tris[j]);
				if (   current.tris[j].p1.x < avg.x && current.tris[j].p1.z < avg.z 
					|| current.tris[j].p2.x < avg.x && current.tris[j].p2.z < avg.z 
					|| current.tris[j].p1.x < avg.x && current.tris[j].p3.z < avg.z)
					southWest.tris.push_back(current.tris[j]);
				if (   current.tris[j].p1.x < avg.x && current.tris[j].p1.z > avg.z 
					|| current.tris[j].p2.x < avg.x && current.tris[j].p2.z > avg.z 
					|| current.tris[j].p3.x < avg.x && current.tris[j].p3.z > avg.z)
					northWest.tris.push_back(current.tris[j]);
			}
			// Pushing  the quads into the right list.
			if (northEast.tris.size() > MAX_TRIS_AMOUNT)
				quadsToDevide.push_back(northEast);
			else {
				setMinMax(northEast);
				quads.push_back(northEast);
			}

			if (southEast.tris.size() > MAX_TRIS_AMOUNT)
				quadsToDevide.push_back(southEast);
			else {
				setMinMax(southEast);
				quads.push_back(southEast);
			}

			if (southWest.tris.size() > MAX_TRIS_AMOUNT)
				quadsToDevide.push_back(southWest);
			else {
				setMinMax(southWest);
				quads.push_back(southWest);
			}

			if (northWest.tris.size() > MAX_TRIS_AMOUNT)
				quadsToDevide.push_back(northWest);
			else {
				setMinMax(northWest);
				quads.push_back(northWest);
			}

			northEast.tris.clear();
			southEast.tris.clear();
			southWest.tris.clear();
			northWest.tris.clear();
		}
	}
	northEast.tris.clear();
	southEast.tris.clear();
	southWest.tris.clear();
	northWest.tris.clear();
	quadsToDevide.clear();
	return quads;
}

void QuadTree::setAvg(const shape::Triangle tri, DirectX::XMFLOAT3 &avg)
{
	avg.x += tri.p1.x;
	avg.z += tri.p1.z;

	avg.x += tri.p2.x;
	avg.z += tri.p2.z;

	avg.x += tri.p3.x;
	avg.z += tri.p3.z;
}
void QuadTree::setMinMax(QuadNode &quad)
{
	DirectX::XMFLOAT3 min = DirectX::XMFLOAT3(9999999, 0, 9999999);
	DirectX::XMFLOAT3 max = DirectX::XMFLOAT3(-9999999, 0, -9999999);
	for (int i = 0; i < (int) quad.tris.size(); i++)
	{
		shape::Triangle tri = quad.tris[i];
		if (tri.p1.x < min.x)
			min.x = tri.p1.x;
		else if (tri.p1.x > max.x)
			max.x = tri.p1.x;
		if (tri.p1.z < min.z)
			min.z = tri.p1.z;
		else if (tri.p1.z > max.z)
			max.z = tri.p1.z;

		if (tri.p2.x < min.x)
			min.x = tri.p2.x;
		else if (tri.p2.x > max.x)
			max.x = tri.p2.x;
		if (tri.p2.z < min.z)
			min.z = tri.p2.z;
		else if (tri.p2.z > max.z)
			max.z = tri.p2.z;

		if (tri.p3.x < min.x)
			min.x = tri.p3.x;
		else if (tri.p3.x > max.x)
			max.x = tri.p3.x;
		if (tri.p3.z < min.z)
			min.z = tri.p3.z;
		else if (tri.p3.z > max.z)
			max.z = tri.p3.z;
	}

	quad.maxPos = DirectX::XMFLOAT2(max.x, max.z);
	quad.minPos = DirectX::XMFLOAT2(min.x, min.z);
}