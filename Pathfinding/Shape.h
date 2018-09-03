#pragma once
#include <DirectXMath.h>

namespace rle
{


	namespace shape
	{
		struct Sphere
		{
			DirectX::XMFLOAT3 center;
			float range;
		};
		struct Line
		{

			DirectX::XMFLOAT3 origin, end;
		};
		struct Ray
		{

			DirectX::XMFLOAT3 origin;
			DirectX::XMFLOAT3 direction;
		};
		struct Capsule
		{

			DirectX::XMFLOAT3 center;
			float height;
			float radius;
		};
		struct Cube
		{
			DirectX::XMFLOAT3 center;
			DirectX::XMFLOAT3 size;
			DirectX::XMFLOAT3 rotation;
		};
		struct Triangle
		{
			Triangle() {};
			Triangle(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3) : p1(p1), p2(p2), p3(p3) {}
			DirectX::XMFLOAT3 p1;
			DirectX::XMFLOAT3 p2;
			DirectX::XMFLOAT3 p3;
		};
	}
}