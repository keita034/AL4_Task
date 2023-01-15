#include "Collision.h"

void Collision::ClosestPtPoint2Triangle(AliceMathF::Vector4& point, Triangle& triangle, AliceMathF::Vector4* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	AliceMathF::Vector4 p0_p1 = triangle.GetP1() - triangle.GetP1();
	AliceMathF::Vector4 p0_p2 = triangle.GetP2() - triangle.GetP0();
	AliceMathF::Vector4 p0_pt = point - triangle.GetP0();

	float d1 = p0_pt.dot(p0_p1);
	float d2 = p0_pt.dot(p0_p2);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.GetP0();
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	AliceMathF::Vector4 p1_pt = point - triangle.GetP1();

	float d3 = p1_pt.dot(p0_p1);
	float d4 = p1_pt.dot(p0_p2);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.GetP1();
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.GetP0() + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	AliceMathF::Vector4 p2_pt = point - triangle.GetP2();

	float d5 = p2_pt.dot(p0_p1);
	float d6 = p2_pt.dot(p0_p2);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.GetP2();
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.GetP0() + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.GetP1() + w * (triangle.GetP2() - triangle.GetP1());
		return;
	}

	// pointは面領域の中にある。closestを重心座標を用いて計算する
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.GetP0() + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Sphere(Sphere& sphereA, Sphere& sphereB, AliceMathF::Vector4* inter, AliceMathF::Vector4* reject)
{
	// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
	AliceMathF::Vector4 tmp;
	tmp = sphereA.GetCenter() - sphereB.GetCenter();
	float dist = tmp.lengthSquared();
	float radius2 = sphereA.GetRadius() + sphereB.GetRadius();
	radius2 *= radius2;

	if (dist <= radius2)
	{
		if (inter)
		{
			// Aの半径が0の時座標はBの中心　Bの半径が0の時座標はAの中心　となるよう補完
			float t = sphereB.GetRadius() / (sphereA.GetRadius() + sphereB.GetRadius());
			*inter = AliceMathF::Vector4Lerp(sphereA.GetCenter(), sphereB.GetCenter(), t);
		}
		// 押し出すベクトルを計算
		if (reject)
		{
			float rejectLen = sphereA.GetRadius() + sphereB.GetRadius() - std::sqrtf(dist);
			tmp = sphereA.GetCenter() - sphereB.GetCenter();
			*reject = tmp.normal();
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Plane(Sphere& sphere, Plane& plane, AliceMathF::Vector4* inter)
{
	// 座標系の原点から球の中心座標への距離から
	// 平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = sphere.GetCenter().dot(plane.GetNormal()) - plane.GetDistance();
	// 距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.GetRadius())	return false;

	// 擬似交点を計算
	if (inter)
	{
		// 平面上の再接近点を、疑似交点とする
		*inter = -dist * plane.GetNormal() + sphere.GetCenter() + sphere.GetCenter();
	}

	return true;
}

bool Collision::CheckSphere2Triangle(Sphere& sphere, Triangle& triangle, AliceMathF::Vector4* inter, AliceMathF::Vector4* reject)
{
	AliceMathF::Vector4 p;
	// 球の中心に対する最近接点である三角形上にある点pを見つける
	AliceMathF::Vector4 tmp = sphere.GetCenter();
	ClosestPtPoint2Triangle(tmp, triangle, &p);
	// 点pと球の中心の差分ベクトル
	AliceMathF::Vector4 v = p - sphere.GetCenter();
	// 距離の二乗を求める
	//（同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	float distanceSquare = v.dot(v);
	// 球と三角形の距離が半径以下なら当たっていない
	if (distanceSquare > sphere.GetRadius() * sphere.GetRadius())
	{
		return false;
	}

	// 擬似交点を計算
	if (inter)
	{
		// 三角形上の最近接点pを疑似交点とする
		*inter = p;
	}
	// 押し出すベクトルを計算
	if (reject)
	{
		float ds = sphere.GetCenter().dot(triangle.GetNormal());
		float dt = triangle.GetP0().dot(triangle.GetNormal());
		float rejectLen = dt - ds + sphere.GetRadius();
		*reject = triangle.GetNormal() * rejectLen;
	}
	return true;
}

bool Collision::CheckRay2Plane(Ray& lay, Plane& plane, float* distance, AliceMathF::Vector4* inter)
{
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値

	float d1 = plane.GetNormal().dot(lay.GetDir());
	// 裏面には当たらない
	if (d1 > -epsilon)
	{
		return false;
	}

	float d2 = plane.GetNormal().dot(lay.GetStart());
	float t = (plane.GetDistance() - d2) / d1;

	if (t < 0) return false;

	// 距離を書き込む
	if (distance)
	{
		*distance = t;
	}

	// 交点を計算
	if (inter)
	{
		*inter = lay.GetStart() + t * lay.GetDir();
	}

	return true;
}

bool Collision::CheckRay2Triangle(Ray& lay, Triangle& triangle, float* distance, AliceMathF::Vector4* inter)
{
	// 三角形が乗っている平面を算出
	Plane plane;
	AliceMathF::Vector4 interPlane;
	AliceMathF::Vector4 tmp = triangle.GetNormal();
	plane.SetNormal(tmp);
	plane.SetDistance(triangle.GetNormal().dot(triangle.GetP0()));
	// レイと平面が当たっていなければ、当たっていない	
	if (!CheckRay2Plane(lay, plane, distance, &interPlane))
	{
		return false;
	}
	// レイと平面が当たっていたので、距離と交点が書き込まれた

	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	AliceMathF::Vector4 m;
	// 辺p0_p1について
	AliceMathF::Vector4 pt_p0 = triangle.GetP0() - interPlane;
	AliceMathF::Vector4 p0_p1 = triangle.GetP1() - triangle.GetP0();
	m = pt_p0.cross(p0_p1);
	// 辺の外側
	if (m.dot(triangle.GetNormal()) < -epsilon)
	{
		return false;
	}

	// 辺p1_p2について
	AliceMathF::Vector4 pt_p1 = triangle.GetP1() - interPlane;
	AliceMathF::Vector4 p1_p2 = triangle.GetP2() - triangle.GetP1();
	m = pt_p1.cross(p1_p2);
	// 辺の外側
	if (m.dot(triangle.GetNormal()) < -epsilon)
	{
		return false;
	}

	// 辺p2_p0について
	AliceMathF::Vector4 pt_p2 = triangle.GetP2() - interPlane;
	AliceMathF::Vector4 p2_p0 = triangle.GetP0() - triangle.GetP2();
	m = pt_p2.cross(p2_p0);
	// 辺の外側
	if (m.dot(triangle.GetNormal()) < -epsilon)
	{
		return false;
	}

	if (inter)
	{
		*inter = interPlane;
	}

	// 内側なので、当たっている
	return true;
}

bool Collision::CheckRay2Sphere(Ray& lay, Sphere& sphere, float* distance, AliceMathF::Vector4* inter)
{
	AliceMathF::Vector4 m = lay.GetStart() - sphere.GetCenter();
	float b = m.dot(lay.GetDir());
	float c = m.dot(m) - sphere.GetRadius() * sphere.GetRadius();
	// layの始点がsphereの外側にあり(c > 0)、layがsphereから離れていく方向を
	// 差している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	// 負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f)
	{
		return false;
	}

	// レイは球と交差している。
	// 交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	// tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if (t < 0) t = 0.0f;

	if (distance)
	{
		*distance = t;
	}

	if (inter)
	{
		*inter = lay.GetStart() + t * lay.GetDir();
	}

	return true;
}

bool Collision::CheckRayCast2Sphere(RayCast& lay, Sphere& sphere, float* distance, AliceMathF::Vector4* inter)
{
	float dis = 0.0f;

	if (!CheckRay2Sphere(lay, sphere, &dis, inter))
	{
		return false;
	}

	if (dis > lay.GetRange())
	{
		return false;
	}

	if (distance)
	{
		*distance = dis;
	}

	return true;
}

bool Collision::CheckOBB2OBB(OBB& obb1, OBB& obb2)
{
	//各方向ベクトルの確保
	//(N***:標準化方向ベクトル)
	AliceMathF::Vector3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen(0);
	AliceMathF::Vector3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen(1);
	AliceMathF::Vector3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen(2);
	AliceMathF::Vector3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen(0);
	AliceMathF::Vector3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen(1);
	AliceMathF::Vector3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen(2);
	AliceMathF::Vector3 Interval = obb1.GetCenter() - obb2.GetCenter();

	//分離軸:Ae1
	float rA = Ae1.length();
	float rB = AliceMathF::LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = AliceMathF::Abs(Interval.dot(NAe1));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Ae2
	rA = Ae2.length();
	rB = AliceMathF::LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(NAe2));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Ae3
	rA = Ae3.length();
	rB = AliceMathF::LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(NAe3));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Be1
	rA = AliceMathF::LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Be1.length();
	L = AliceMathF::Abs(Interval.dot(NBe1));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Be2
	rA = AliceMathF::LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Be2.length();
	L = AliceMathF::Abs(Interval.dot(NBe2));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Be3
	rA = AliceMathF::LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Be3.length();
	L = AliceMathF::Abs(Interval.dot(NBe3));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C11
	AliceMathF::Vector3 Cross;
	Cross = NAe1.cross(NBe1);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C12
	Cross = NAe1.cross(NBe2);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C13
	Cross = NAe1.cross(NBe3);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C21
	Cross = NAe2.cross(NBe1);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C22
	Cross = NAe2.cross(NBe2);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C23
	Cross = NAe2.cross(NBe3);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C31
	Cross = NAe3.cross(NBe1);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C32
	Cross = NAe3.cross(NBe2);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C33
	Cross = NAe3.cross(NBe3);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離平面が存在しないので衝突している
	return true;
}

bool Collision::CheckOBB2OBB(OBB* obb1, OBB* obb2)
{
	if (!obb1||!obb2)
	{
		return false;
	}

	//各方向ベクトルの確保
	//(N***:標準化方向ベクトル)
	AliceMathF::Vector3 NAe1 = obb1->GetDirect(0), Ae1 = NAe1 * obb1->GetLen(0);
	AliceMathF::Vector3 NAe2 = obb1->GetDirect(1), Ae2 = NAe2 * obb1->GetLen(1);
	AliceMathF::Vector3 NAe3 = obb1->GetDirect(2), Ae3 = NAe3 * obb1->GetLen(2);
	AliceMathF::Vector3 NBe1 = obb2->GetDirect(0), Be1 = NBe1 * obb2->GetLen(0);
	AliceMathF::Vector3 NBe2 = obb2->GetDirect(1), Be2 = NBe2 * obb2->GetLen(1);
	AliceMathF::Vector3 NBe3 = obb2->GetDirect(2), Be3 = NBe3 * obb2->GetLen(2);
	AliceMathF::Vector3 Interval = obb1->GetCenter() - obb2->GetCenter();

	//分離軸:Ae1
	float rA = Ae1.length();
	float rB = AliceMathF::LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = AliceMathF::Abs(Interval.dot(NAe1));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Ae2
	rA = Ae2.length();
	rB = AliceMathF::LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(NAe2));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Ae3
	rA = Ae3.length();
	rB = AliceMathF::LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(NAe3));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Be1
	rA = AliceMathF::LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Be1.length();
	L = AliceMathF::Abs(Interval.dot(NBe1));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Be2
	rA = AliceMathF::LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Be2.length();
	L = AliceMathF::Abs(Interval.dot(NBe2));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:Be3
	rA = AliceMathF::LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Be3.length();
	L = AliceMathF::Abs(Interval.dot(NBe3));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C11
	AliceMathF::Vector3 Cross;
	Cross = NAe1.cross(NBe1);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C12
	Cross = NAe1.cross(NBe2);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C13
	Cross = NAe1.cross(NBe3);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C21
	Cross = NAe2.cross(NBe1);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C22
	Cross = NAe2.cross(NBe2);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C23
	Cross = NAe2.cross(NBe3);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C31
	Cross = NAe3.cross(NBe1);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C32
	Cross = NAe3.cross(NBe2);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離軸:C33
	Cross = NAe3.cross(NBe3);
	rA = AliceMathF::LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = AliceMathF::LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = AliceMathF::Abs(Interval.dot(Cross));
	if (L > rA + rB)
	{
		//衝突していない
		return false;
	}

	//分離平面が存在しないので衝突している
	return true;
}
