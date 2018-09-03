#include "Collision.h"
#include "Bullet3Common/b3Vector3.h"

using namespace rle;
using namespace shape;
using namespace DirectX;

// FOR DEBUG DRAWING LINES:
// https://stackoverflow.com/questions/21649766/simplest-way-to-draw-line-in-directx-11-c

float Collision::GetDistance(XMFLOAT3 origin, XMFLOAT3 target)
{
	//return (float) b3Distance(b3MakeVector3(origin.x, origin.y, origin.z), b3MakeVector3(target.x, target.y, target.z));
	return sqrt(abs((origin.x - target.x) * (origin.x - target.x) +
		(origin.y - target.y) * (origin.y - target.y) +
		(origin.z - target.z) * (origin.z - target.z)));
}

XMFLOAT3 Collision::ClosestPointOnTri(XMFLOAT3 origin, Triangle tri)
{
	/////////////////////////////////////////////////////
	// Code checks for the nearest point on a triangle //
	/////////////////////////////////////////////////////

	// This code is from
	// Christer Ericsons Real-Time Collision Detection (2005)

	XMVECTOR a = XMLoadFloat3(&tri.p1);
	XMVECTOR b = XMLoadFloat3(&tri.p2);
	XMVECTOR c = XMLoadFloat3(&tri.p3);
	XMVECTOR p = XMLoadFloat3(&origin);

	// Check if P in vertex region outside A 
	XMVECTOR ab = XMVectorSubtract(b, a);
	XMVECTOR ac = XMVectorSubtract(c, a);
	XMVECTOR ap = XMVectorSubtract(p, a);
	XMVECTOR d1 = XMVector3Dot(ab, ap);
	XMVECTOR d2 = XMVector3Dot(ac, ap);
	
	float fd1, fd2, fd3, fd4;
	XMFLOAT3 f3a;

	XMStoreFloat(&fd1, d1);
	XMStoreFloat(&fd2, d2);
	XMStoreFloat3(&f3a, a);
	if (fd1 <= 0.0f && fd2 <= 0.0f)
		return f3a;

	// barycentric coordinates (1,0,0)
	// Check if P in vertex region outside B 
	XMVECTOR bp = XMVectorSubtract(p, b);
	XMVECTOR d3 = XMVector3Dot(ab, bp);
	XMVECTOR d4 = XMVector3Dot(ac, bp);
	XMFLOAT3 f3b;

	XMStoreFloat(&fd3, d3);
	XMStoreFloat(&fd4, d4);
	XMStoreFloat3(&f3b, b);

	if (fd3 >= 0.0f && fd4 <= fd3)
		return f3b;


	// barycentric coordinates (0,1,0)
	// Check if P in edge region of AB, if so return projection of P onto AB 
	float vc = fd1*fd4 - fd3*fd2;
	if (vc <= 0.0f && fd1 >= 0.0f && fd3 <= 0.0f)
	{
		float v = fd1 / (fd1 - fd3);
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVectorAdd(a, XMVectorScale(ab, v)));
		return result;
		// barycentric coordinates (1-v,v,0) 
	}

	// Check if P in vertex region outside C 
	XMVECTOR cp = XMVectorSubtract(p, c);
	XMVECTOR d5 = XMVector3Dot(ab, cp);
	XMVECTOR d6 = XMVector3Dot(ac, cp);
	XMFLOAT3 f3c;
	float fd5, fd6;
	XMStoreFloat3(&f3c, c);
	XMStoreFloat(&fd5, d5);
	XMStoreFloat(&fd6, d6);

	if (fd6 >= 0.0f && fd5 <= fd6)
		return f3c;

	// barycentric coordinates (0,0,1)
	float vb = fd5*fd2 - fd1*fd6;
	if (vb <= 0.0f && fd2 >= 0.0f && fd6 <= 0.0f)
	{
		float w = fd2 / (fd2 - fd6);
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVectorAdd(a, XMVectorScale(ac, w)));
		return result;
		// barycentric coordinates (1-w,0,w) 
	}
	// Check if P in edge region of BC, if so return projection of P onto BC 
	float va = fd3*fd6 - fd5*fd4;
	if (va <= 0.0f && (fd4 - fd3) >= 0.0f && (fd5 - fd6) >= 0.0f)
	{
		float w = (fd4 - fd3) / ((fd4 - fd3) + (fd5 - fd6));
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVectorAdd(b, XMVectorScale(XMVectorSubtract(c, b), w)));

		return result; // barycentric coordinates (0,1-w,w) 
	}
	// P inside face region. Compute Q through its barycentric coordinates (u,v,w) 
	float denom = 1.0f / (va + vb + vc);
	float v = vb *denom;
	float w = vc *denom;

	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVectorAdd(XMVectorAdd(a, XMVectorScale(ab, v)), XMVectorScale(ac, w)));
	return result;
}

XMFLOAT3 Collision::ClosestPointOnLine(XMFLOAT3 origin, Line line)
{
	// From Christer Ericson: Real Time Collision detection (2005).
	XMFLOAT3 closest;
	XMVECTOR lineEnd = XMLoadFloat3(&line.end), lineOrigin = XMLoadFloat3(&line.origin);
	XMVECTOR start = XMLoadFloat3(&origin);
	XMVECTOR ab = XMVectorSubtract(lineEnd, lineOrigin);

	float t;
	XMStoreFloat(&t, XMVectorDivide(XMVector3Dot(XMVectorSubtract(start, lineOrigin), ab), XMVector3Dot(ab, ab)));
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;
	XMStoreFloat3(&closest, XMVectorAdd(lineOrigin, XMVectorScale(ab, t)));
	return closest;
}

// Returns the point on sphere2 that collides with sphere1.
Hit Collision::SphereSphere(Sphere sphere1, Sphere sphere2)
{
	Hit hit;
	if (GetDistance(sphere1.center, sphere2.center) < sphere1.range + sphere2.range)
	{
		XMFLOAT3 loc = XMFLOAT3(sphere1.center.x - sphere2.center.x, sphere1.center.y - sphere2.center.y, sphere1.center.z - sphere2.center.z);
		XMVector3Normalize(XMLoadFloat3(&loc));
		hit.location = XMFLOAT3(loc.x * sphere1.range, loc.y * sphere1.range, loc.z * sphere1.range);
		hit.type = HitType::COLLISION_WITHOUT_POSITION;
	}
	else
		hit.type = HitType::NO_COLLISION;

	return hit;
}

Hit Collision::LineSphere(Line line, Sphere sphere)
{
	XMFLOAT3 closest = ClosestPointOnLine(sphere.center, line);

	Hit hit;
	if (GetDistance(closest, sphere.center) < sphere.range)
	{
		hit.location = closest;
		hit.type = HitType::COLLISION_WITH_POSITION;
		return hit;
	}
	else
	{
		hit.type = HitType::NO_COLLISION;
		return hit;
	}
}

Hit Collision::SphereCubeAA(Sphere sphere, Cube cube)
{
	// https://stackoverflow.com/questions/4578967/cube-sphere-intersection-test
	Hit hit;
	hit.type = HitType::NO_COLLISION;
	return hit;
}

Hit Collision::AABB(Cube cube1, Cube cube2)
{
	Hit hit;
	if (abs(cube1.center.x - cube2.center.x) < (cube1.size.x + cube2.size.x) / 2
		&& abs(cube1.center.y - cube2.center.y) < (cube1.size.y + cube2.size.y) / 2
		&& abs(cube1.center.z - cube2.center.z) < (cube1.size.z + cube2.size.z) / 2)
		hit.type = HitType::COLLISION_WITHOUT_POSITION;
	else hit.type = HitType::NO_COLLISION;
	return hit;

}

//Hit Collision::LineTri(Line line, Triangle tri)
//{
//	Hit hit;
//	XMFLOAT3 Normal, IntersectPos;
//	XMFLOAT3 vertex0, vertex1, vertex2;
//	vertex0 = tri.p1;
//	vertex1 = tri.p2;
//	vertex2 = tri.p3;
//	// Find Triangle Normal
//	Normal = (vertex1 - vertex0).Cross(vertex2 - vertex0);
//	if (Normal.Magnitude() > 0)
//		Normal.Normalize();
//	// Find distance from vertex0 and vertex1 to the plane defined by the triangle
//	float Dist1 = (line.origin - vertex0).Dot(Normal);
//	float Dist2 = (line.end - vertex0).Dot(Normal);
//	if ((Dist1 * Dist2) >= 0.0f) {
//		hit.type = HitType::NO_COLLISION;
//		return hit;
//	} // line doesn't cross the triangle.
//	if (Dist1 == Dist2) {
//		hit.type = HitType::NO_COLLISION;
//		return hit;
//	} // line and plane are parallel
//	  // Find point on the line that intersects with the plane
//	IntersectPos = line.origin + (line.end - line.origin) * (-Dist1 / (Dist2 - Dist1));
//	// Find if the interesection point lies inside the triangle by testing it against all edges
//	XMFLOAT3 vTest;
//	vTest = Normal.Cross(vertex1 - vertex0);
//	if (vTest.Dot(IntersectPos - vertex0) < 0.00001f)
//	{
//		hit.type = HitType::NO_COLLISION;
//		return hit;
//	}
//	vTest = Normal.Cross(vertex2 - vertex1);
//	if (vTest.Dot(IntersectPos - vertex1) < 0.00001f)
//	{
//		hit.type = HitType::NO_COLLISION;
//		return hit;
//	}
//
//	vTest = Normal.Cross(vertex0 - vertex2);
//	if (vTest.Dot(IntersectPos - vertex0) < 0.00001f)
//	{
//		hit.type = HitType::NO_COLLISION;
//		return hit;
//	}
//	hit.location = IntersectPos;
//	hit.type = HitType::COLLISION_WITH_POSITION;
//	return hit;
//}

//Hit Collision::LineTriangleList(Line line, std::vector<Triangle> tris)
//{
//	Hit hit;
//	bool anyHit = false;
//	XMFLOAT3 closestHit;
//	float dist1, dist2;
//
//	// ONLY USE THIS FOR SMALL TRIANGLE LISTS, VERY PERFORMANCE INTENSIVE!
//	for (int i = 0; i < (int)tris.size(); i++)
//	{
//		hit = LineTri(line, tris[i]);
//		if (hit.type == HitType::COLLISION_WITH_POSITION)
//		{
//			if (anyHit)
//			{
//				dist1 = GetDistance(line.origin, hit.location);
//				dist2 = GetDistance(line.origin, closestHit);
//				if (dist1 < dist2)
//				{
//					closestHit = hit.location;
//				}
//			}
//			else
//			{
//				closestHit = hit.location;
//			}
//			anyHit = true;
//		}
//	}
//	if (anyHit)
//	{
//		hit.location = closestHit;
//		hit.type = HitType::COLLISION_WITH_POSITION;
//	}
//	else hit.type = HitType::NO_COLLISION;
//	return hit;
//}

//Hit Collision::CapsuleTri(Capsule capsule, Triangle tri)
//{
//	XMFLOAT3 closestPointOnTri;
//	XMFLOAT3 closestPointOnLine;
//	Line line;
//	Hit hit;
//	line.origin = XMFLOAT3(capsule.center.x, capsule.center.y + capsule.height, capsule.center.z);
//	line.end = XMFLOAT3(capsule.center.x, capsule.center.y - capsule.height, capsule.center.z);
//
//
//	// check if the points of a triangle aren't in the cylinder first.
//	// Point one:
//	closestPointOnLine = ClosestPointOnLine(tri.p1, line);
//	if (GetDistance(closestPointOnLine, tri.p1) < capsule.radius)
//	{
//		hit.type = HitType::COLLISION_WITH_POSITION;
//		hit.location = tri.p1;
//		return hit;
//	}
//
//	// Point two:
//	closestPointOnLine = ClosestPointOnLine(tri.p2, line);
//	if (GetDistance(closestPointOnLine, tri.p2) < capsule.radius)
//	{
//		hit.type = HitType::COLLISION_WITH_POSITION;
//		hit.location = tri.p2;
//		return hit;
//	}
//
//	// Point three:
//	closestPointOnLine = ClosestPointOnLine(tri.p3, line);
//	if (GetDistance(closestPointOnLine, tri.p3) < capsule.radius)
//	{
//		hit.type = HitType::COLLISION_WITH_POSITION;
//		hit.location = tri.p3;
//		return hit;
//	}
//
//	/* If the closest points aren't the triangle corners: */
//	// Closest point to triangle from bottom
//	closestPointOnTri = ClosestPointOnTri(line.origin, tri);
//	closestPointOnLine = ClosestPointOnLine(closestPointOnTri, line);
//	if (GetDistance(closestPointOnLine, closestPointOnTri) < capsule.radius)
//	{
//		hit.type = HitType::COLLISION_WITH_POSITION;
//		hit.location = closestPointOnTri;
//		return hit;
//	}
//
//	// Closest point to triangle from top
//	closestPointOnTri = ClosestPointOnTri(line.end, tri);
//	closestPointOnLine = ClosestPointOnLine(closestPointOnTri, line);
//	if (GetDistance(closestPointOnLine, closestPointOnTri) < capsule.radius)
//	{
//		hit.type = HitType::COLLISION_WITH_POSITION;
//		hit.location = closestPointOnTri;
//		return hit;
//	}
//
//	else
//	{
//		hit.type = HitType::NO_COLLISION;
//	}
//
//	return hit;
//}

Hit Collision::SphereTri(Sphere sphere, Triangle tri)
{
	//////////////////////////////////////////////////////////
	// Code checks if sphere is hitting the closest point	//
	// on the triangle. If so, there is a collision on the	//
	// point closest to the triangle center.				//
	//////////////////////////////////////////////////////////

	Hit hit;

	XMFLOAT3 closest = ClosestPointOnTri(sphere.center, tri);
	if (GetDistance(closest, sphere.center) <= sphere.range)
	{
		hit.type = HitType::COLLISION_WITH_POSITION;
		hit.location = closest;
	}
	else
	{
		hit.type = HitType::NO_COLLISION;
	}
	return hit;
}

std::vector<Hit> Collision::SphereQuad(shape::Sphere sphere, QuadNode quad)
{
	std::vector<Hit> hits;
	Hit hit;
	for (size_t i = 0; i < quad.tris.size(); i++)
	{
		XMFLOAT3 closest = ClosestPointOnTri(sphere.center, quad.tris[i]);
		if (GetDistance(closest, sphere.center) <= sphere.range)
		{
			hit.type = HitType::COLLISION_WITH_POSITION;
			hit.location = closest;
			hits.push_back(hit);
		}
	}
	return hits;
}

std::vector<QuadNode*> Collision::GetOverlappingQuads(shape::Sphere sphere, std::vector<QuadNode> &quads)
{
	std::vector<QuadNode*> quadsToReturn;
	for (size_t i = 0; i < quads.size(); i++)
	{
		if (quads[i].minPos.x < sphere.center.x + sphere.range && quads[i].minPos.y < sphere.center.z + sphere.range && quads[i].maxPos.x > sphere.center.x - sphere.range && quads[i].maxPos.y > sphere.center.z - sphere.range)
		{
			quadsToReturn.push_back(&quads[i]);
		}
	}
	return quadsToReturn;
}



