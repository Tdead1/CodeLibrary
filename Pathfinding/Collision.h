#pragma once
#include <vector>
#include "Shape.h"
#include "QuadTree.h"
namespace rle
{

	//! The HitType enum indicates the sort of hit that was returned by the collision. It's either not a collision, collisions with or without positions, or multiple collisions.
	enum class HitType { NO_COLLISION = 0, COLLISION_WITH_POSITION, TWO_COLLISIONS, MULTIPLE_COLLISIONS, COLLISION_WITHOUT_POSITION };

	//! The Hit result struct can give a HitType, and the closest location of impact (location).
	struct Hit
	{
		HitType type;
		DirectX::XMFLOAT3 location;
	};

	//! The collision class includes all collision data.
	class Collision
	{
	public:
		//! Returns the distance between two points. A fast function, can be used a lot.
		static float GetDistance(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 target);
		//! Returns the closest position on a triangle from a certain origin.
		static DirectX::XMFLOAT3 ClosestPointOnTri(DirectX::XMFLOAT3 origin, shape::Triangle tri);
		//! Returns the closest point on a line, from a certain origin.
		static DirectX::XMFLOAT3 ClosestPointOnLine(DirectX::XMFLOAT3 origin, shape::Line line);
		//! Relatively medium-weight test. Returns a position of impact, if there is any.
		static Hit CapsuleTri(shape::Capsule capsule, shape::Triangle tri);
		//! Very fast collision check, can be resolved by a tunneling resolve. Returns the point ON the second sphere closest to the first sphere.
		static Hit SphereSphere(shape::Sphere sphere1, shape::Sphere sphere2);
		//! Relatively fast collision check, returns the closest point on the line to the sphere's center.
		static Hit LineSphere(shape::Line line, shape::Sphere sphere);
		//! Sphere triangle collision, returns the closest point to the sphere's center.
		static Hit SphereTri(shape::Sphere shpere, shape::Triangle tri);
		//! Sphere vs triangle list collision, returns the closest point to the sphere's center.
		static std::vector<Hit> SphereQuad(shape::Sphere shpere, QuadNode quad);
		//! Standard AABB check, doesn't return a location.
		static Hit AABB(shape::Cube cube1, shape::Cube cube2);
		//! Relatively slow check, returns the exact location of impact.
		static Hit LineTri(shape::Line Line, shape::Triangle tri);
		//! Does a line triangle check against each triangle, slows down if you have more triangles. Returns the closest collision.
		static Hit LineTriangleList(shape::Line line, std::vector<shape::Triangle> tris);

		//! Returns all overlapping quads within a sphere shape.
		static std::vector<QuadNode*> GetOverlappingQuads(shape::Sphere sphere, std::vector<QuadNode> &quads);

		//! So far we had no time to implement this. Contact Tom Kemna for extra information.
		static Hit SphereCubeAA(shape::Sphere sphere, shape::Cube cube);
	};
}