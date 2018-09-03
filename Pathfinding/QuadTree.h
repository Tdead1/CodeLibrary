#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Shape.h"

#define MAX_TRIS_AMOUNT 50

namespace rle
{

	//! A node (possibly subdivided) containing triangle data of this part of the map.
	struct QuadNode {
		std::vector<shape::Triangle> tris;
		DirectX::XMFLOAT2 minPos, maxPos;
	};

	//! The tree data structure that holds all the quad nodes.
	class QuadTree
	{
	public:
		QuadTree() {};
		~QuadTree() {};

		//! Creates a quad tree of the specified triangles.
		static std::vector<QuadNode> MakeQuadTree(const std::vector<shape::Triangle> _tris);

	private:
		//! Sets the avarage location of the triangle list.
		static void setAvg(const shape::Triangle tri, DirectX::XMFLOAT3 &avg);
		//! Sets the miniumum and maximum values of the specified quad.
		static void setMinMax(QuadNode &quad);


	};
}