#pragma once

#include"Vector3.h"
#include"Matrix4.h"
namespace AliceMathF
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		//クオータニオンを作成
		Quaternion(float x = 0.0f, float y = 0.0f, float z = 9.0f, float w = 1.0f);
		Quaternion(const Vector3& v, float angle);
		Quaternion(const Matrix4& m);

		//内積を求める
		float dot(const Quaternion& q1, const Quaternion& q2);

		//ノルムを求める
		float length(const Quaternion& q);
		//正規化する
		Quaternion normalize(const Quaternion& q);
		//単項演算子オーバーロード
		Quaternion operator + ()const;
		Quaternion operator - ()const;
		//代入演算子オーバーロード
		Quaternion& operator += (const Quaternion& q);
		Quaternion& operator -= (const Quaternion& q);
		Quaternion& operator *= (float s);
		Quaternion& operator /= (float s);
		Quaternion& operator *= (const Quaternion& q);

		Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
		Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t);
		Matrix4 rotate(const Quaternion& q);

		Vector3 getAxis(const Quaternion& q);
	};

	//2項演算子オーバーロード
	const Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
	const Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
	const Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
	const Quaternion operator * (const Quaternion& q, float s);
	const Quaternion operator * (float s, const Quaternion& q);
	const Quaternion operator / (const Quaternion& q, float s);

}