#include"CollisionPrimitive.h"

void Triangle::ComputeNormal()
{
	AliceMathF::Vector4 p0_p1 = p1 - p0;
	AliceMathF::Vector4 p0_p2 = p2 - p0;

	//外積により、2辺に垂直なベクトルを算出する
	normal = p0_p1.cross(p0_p2);
	normal.normal();

}

const AliceMathF::Vector4& Triangle::GetP0()
{
	return p0;
}

void Triangle::SetP0(AliceMathF::Vector4& p0_)
{
	p0 = p0_;
}

const AliceMathF::Vector4& Triangle::GetP1()
{
	return p1;
}

void Triangle::SetP1(AliceMathF::Vector4& p1_)
{
	p1 = p1_;
}

const AliceMathF::Vector4& Triangle::GetP2()
{
	return p2;
}

void Triangle::SetP2(AliceMathF::Vector4& p2_)
{
	p2 = p2_;
}

const AliceMathF::Vector4& Triangle::GetNormal()
{
	return normal;
}

void Triangle::SetNormal(AliceMathF::Vector4& normal_)
{
	normal = normal_;
}

const AliceMathF::Vector3 OBB::GetDirect(uint16_t elem)
{
	return normaDirect[elem];
}

const float OBB::GetLen(uint16_t elem)
{
	return Length[elem];
}

const AliceMathF::Vector3 OBB::GetCenter()
{
	return center;
}

void OBB::CreateOBB(std::vector<PosNormalUv> vertex_, Transform& transform)
{
	AliceMathF::Matrix4	matRot;

	//最大値、最小値の初期値設定
	AliceMathF::Vector3 max = AliceMathF::Vector3(-10000.0f, -10000.0f, -10000.0f);
	AliceMathF::Vector3 min = AliceMathF::Vector3(10000.0f, 10000.0f, 10000.0f);

	//メッシュの頂点データ取得
	std::vector<PosNormalUv> vertex = vertex_;
	//最大値、最小値取得ループ
	for (size_t i = 0; i < vertex.size(); i++)
	{
		AliceMathF::Vector3 pos = vertex[i].pos;
		if (pos.x < min.x)min.x = pos.x;
		if (pos.x > max.x)max.x = pos.x;
		if (pos.y < min.y)min.y = pos.y;
		if (pos.y > max.y)max.y = pos.y;
		if (pos.z < min.z)min.z = pos.z;
		if (pos.z > max.z)max.z = pos.z;
	}

	//中心点取得
	center = transform.translation;

	//方向ベクトル取得
	matRot.MakeRotation(transform.rotation);
	normaDirect[0] = { matRot.m[0][0], matRot.m[0][1], matRot.m[0][2] };
	normaDirect[1] = { matRot.m[1][0], matRot.m[1][1], matRot.m[1][2] };
	normaDirect[2] = { matRot.m[2][0], matRot.m[2][1], matRot.m[2][2] };

	//長さ取得
	Length[0] = AliceMathF::Abs(max.x - min.x) * 0.5f;
	Length[1] = AliceMathF::Abs(max.y - min.y) * 0.5f;
	Length[2] = AliceMathF::Abs(max.z - min.z) * 0.5f;

	Length[0] *= transform.scale.x;
	Length[1] *= transform.scale.y;
	Length[2] *= transform.scale.z;
}

void OBB::UpdateOBB(Transform& transform)
{
	AliceMathF::Matrix4	matRot;

	//中心点取得
	center = AliceMathF::GetWorldPosition(transform);

	//方向ベクトル取得
	matRot.MakeRotation(transform.rotation);
	normaDirect[0] = { matRot.m[0][0], matRot.m[0][1], matRot.m[0][2] };
	normaDirect[1] = { matRot.m[1][0], matRot.m[1][1], matRot.m[1][2] };
	normaDirect[2] = { matRot.m[2][0], matRot.m[2][1], matRot.m[2][2] };
}

const uint32_t BaseCollsion::GetCollisionAttribute()
{
	return collisionAttribute;
}

void BaseCollsion::SetCollisionAttribute(uint32_t collisionAttribute_)
{
	collisionAttribute = collisionAttribute_;
}

const uint32_t BaseCollsion::GetCollisionMask()
{
	return CollisionMask;
}

void BaseCollsion::SetCollisionMask(uint32_t CollisionMask_)
{
	CollisionMask = CollisionMask_;
}

void BaseCollsion::SetCollsionName(std::string name)
{
	collsionName = name;
}

std::string BaseCollsion::GetCollsionName()
{
	return collsionName;
}

std::string BaseCollsion::GetOpponentCollsionName()
{
	return opponentCollsionName;
}

void BaseCollsion::SetOpponentCollsionName(std::string name)
{
	for (size_t i = 0; i < name.size(); i++)
	{
		opponentCollsionName[i] = name[i];
	}
}

const float Sphere::GetRadius()
{
	return radius;
}

void Sphere::SetRadius(float radius_)
{
	radius = radius_;
}

const AliceMathF::Vector4& Sphere::GetCenter()
{
	return center;
}

void Sphere::SetCenter(AliceMathF::Vector4& center_)
{
	center = center_;
}

const float Plane::GetDistance()
{
	return distance;
}

void Plane::SetDistance(float distance_)
{
	distance = distance_;
}

const AliceMathF::Vector4& Plane::GetNormal()
{
	return normal;
}

void Plane::SetNormal(AliceMathF::Vector4& normal_)
{
	normal = normal_;
}

const AliceMathF::Vector4& Ray::GetStart()
{
	return start;
}

void Ray::SetStart(AliceMathF::Vector4& start_)
{
	start = start_;
}

const AliceMathF::Vector4& Ray::GetDir()
{
	return dir;
}

void Ray::SetDir(AliceMathF::Vector4& dir_)
{
	dir = dir_;
}

const float RayCast::GetRange()
{
	return range;
}

void RayCast::SetRange(float range_)
{
	range = range_;
}
