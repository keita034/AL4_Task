#include "Vector.h"
namespace AliceMathF
{

	const Vector4 operator+(const Vector3& v1, const Vector4& v2)
	{
		Vector4 tmp(v2);
		tmp.x += v1.x;
		tmp.y += v1.y;
		tmp.z += v1.z;
		tmp.w += 0;
		return tmp;
	}

	// Vector4をVector3に変換
	Vector3 TrnsVec3(const Vector4& vec)
	{
		return { vec.x,vec.y ,vec.z };
	}

	// Vector3をVector4に変換
	Vector4 TrnsVec4(const Vector3& vec, bool matFlag)
	{
		if (matFlag)
		{
			return { vec.x,vec.y,vec.z,1.0f };
		}
		return { vec.x,vec.y,vec.z,0.0f };
	}

	// ベクトルと行列の掛け算
	Vector3 Vec3Mat4Mul(Vector3& vec, Matrix4& mat)
	{
		Vector3 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

		return retVec;
	}

	Vector3& Vec3Mat4MulWdiv(Vector3& vec, Matrix4& mat)
	{
		Vector4 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + 1 * mat.m[3][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + 1 * mat.m[3][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + 1 * mat.m[3][2];

		retVec.w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + 1 * mat.m[3][3];

		Vector3 tmp = { retVec.x, retVec.y, retVec.z };

		return tmp /= tmp.z;
	}

	// ベクトルと行列の掛け算
	Vector4 Vec4Mat4Mul(Vector4& vec, Matrix4& mat)
	{
		Vector4 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2];

		return retVec;
	}
}