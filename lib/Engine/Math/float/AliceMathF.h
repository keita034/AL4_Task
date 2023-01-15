#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Vector.h"
#include"Matrix4.h"
#include"Quaternion.h"

namespace AliceMathF
{
#pragma region 定数

	//π
	constexpr float AX_PI = 3.141592654f;

	//π/2
	constexpr float AX_PIHalf = 3.141592654f / 2.0f;

	// 2π
	constexpr float AX_2PI = 6.283185307f;

	//ラジアンから度に変換する定数
	constexpr float Rad2Deg = 57.29578f;

	//度からラジアンに変換する定数
	constexpr float Deg2Rad = AX_PI / 180.0f;

	//ε
	constexpr float EPSILON = 2.2204460492503131E-16f;

#pragma endregion 定数

#pragma region sin,cos,tan関数

#pragma region sin関数

	//sin関数
	float Sin(float sin);

	//asin関数
	float Asin(float sin);

#pragma endregion sin関数

#pragma region cos関数

	//cos関数
	float Cos(float cos);

	//cos関数
	float Acos(float cos);

#pragma endregion cos関数

#pragma region tan関数

	//tan関数
	float Tan(float tan);

	//atan関数
	float Atan(float tan);

	//atan2関数
	float Atan2(float y, float x);

#pragma endregion tan関数

	/// <summary>
	/// sin、cosを両方出す
	/// </summary>
	/// <param name="sin_"> : sinの結果を代入する</param>
	/// <param name="cos_"> : cosの結果を代入する</param>
	/// <param name="angle"> : ラジアン角</param>
	void SinCos(float& sin_, float& cos_, float angle);

#pragma endregion sin,cos,tan関数

#pragma region 角度変換

	/// <summary>
	/// 度数法からラジアン変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	float ChangeRadians(const float& angle);

	/// <summary>
	/// ラジアンから度数法変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	float ChangeDira(const float& angle);

#pragma endregion 角度変換

#pragma region sqrt関数

	//sqrt関数
	float Sqrt(float sqrt);


#pragma endregion sqrt関数

#pragma region pow関数

	//pow関数
	float Pow(float pow, float powNum);

#pragma endregion pow関数

#pragma region exp関数

	//exp関数
	float Exp(float exp);

#pragma endregion exp関数

#pragma region log関数

	//log関数
	float Log(float log);

	//log10関数
	float Log10(float log);

#pragma endregion log関数

#pragma region 絶対値の計算

	//絶対値の計算
	float Abs(float abs);

#pragma endregion 絶対値の計算

#pragma region ceil関数

	//ceil関数
	float Ceil(float ceil);

#pragma endregion ceil関数

#pragma region floor関数

	//floor関数
	float Floor(float floor);

#pragma endregion floor関数

#pragma region round関数

	//round関数
	float Round(float round);

#pragma endregion round関数

	/// <summary>
	/// 値の符号を返す
	/// </summary>
	/// <returns>値が正か0の場合は1を、負の場合は-1を返す</returns>
	float Sign(float sign);

	/// <summary>
	/// 値を範囲内に納める
	/// </summary>
	/// <param name="Value">値</param>
	/// <param name="low">最低値</param>
	/// <param name="high">最高値</param>
	float Clamp(float Value, const float low, const float high);

	/// <summary>
	/// 0と1の間に値を納め、その値を返す
	/// </summary>
	/// <param name="Value">値</param>
	float Clamp01(float Value);

	/// <summary>
	/// 線形補間(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	float Lerp(float a, float b, float t);

	/// <summary>
	/// 線形補間(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	Vector3 Lerp(Vector3 a, Vector3 b, float t);

	Vector3 Slerp(Vector3& v1, Vector3& v2, float t);

	/// <summary>
	/// 線形補間(0～)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	float LerpUnclamped(float a, float b, float t);

	/// <summary>
	/// Lerpの角度版(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された角度</returns>
	float LerpAngle(float a, float b, float t);

	/// <summary>
	/// tはlengthより大きくはならず0より小さくならず、その間をループ
	/// </summary>
	/// <param name="t">値</param>
	/// <param name="length">指定値</param>
	float Repeat(float t, float length);

	/// <summary>
	/// 現在地から目的地まで一定速度で移動
	/// </summary>
	/// <param name="current">現在地</param>
	/// <param name="target">目的地</param>
	/// <param name="maxDelta">速度</param>
	float MoveTowards(float current, float target, float maxDelta);

	/// <summary>
	/// 現在の角度から目的の角度まで一定速度で移動
	/// </summary>
	/// <param name="current">現在の角度</param>
	/// <param name="target">目的の角度</param>
	/// <param name="maxDelta">速度</param>
	float MoveTowardsAngle(float current, float target, float maxDelta);

	/// <summary>
	/// 角度の差を返す(度数法)
	/// </summary>
	/// <param name="current">現在の角度</param>
	/// <param name="target">目標となる角度</param>
	float DeltaAngle(float current, float target);

	/// <summary>
	/// スムージング補間
	/// </summary>
	/// <param name="from">開始値</param>
	/// <param name="to">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された角度</returns>
	float SmoothStep(float from, float to, float t);

	/// <summary>
	/// ガンマ補正
	/// </summary>
	float Gamma(float value, float absmax, float gamma);

	/// <summary>
	/// 何を何処に何秒で指定してオブジェクトを移動させる
	/// </summary>
	/// <param name="current">現在地</param>
	/// <param name="target">目的地</param>
	/// <param name="currentVelocity">事前に初期化した速度</param>
	/// <param name="smoothTime">目的までの到達時間</param>
	/// <param name="maxSpeed">最高速度</param>
	/// <param name="deltaTime">関数が前回実行されてからの経過時間</param>
	float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

	/// <summary>
	/// tが0とlengthの間で往復する(0～length)
	/// </summary>
	float PingPong(float t, float length);

	/// <summary>
	/// lerpの補間値を出す
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="value">開始と終了の間の値</param>
	/// <returns>補間値</returns>
	float InverseLerp(float a, float b, float value);

	/// <summary>
	/// 2本の線の交点を計算
	/// </summary>
	/// <param name="p1">始点1</param>
	/// <param name="p2">終点1</param>
	/// <param name="p3">始点2</param>
	/// <param name="p4">終点2</param>
	/// <param name="result">結果(出力用)</param>
	/// <returns>交差しているか</returns>
	bool LineIntersection(Vector2& p1, Vector2& p2, Vector2& p3, Vector2& p4, Vector2& result);

	bool LineSegmentIntersection(Vector2& p1, Vector2& p2, Vector2& p3, Vector2& p4, Vector2& result);

	/// <summary>
	/// 二つの値がほぼ等しいか
	/// </summary>
	bool Approximately(float a, float b);

	/// <summary>
	/// ランダムな値を取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	float GetRand(float min, float max);

	//補完関数
	void Complement(float& x1, float x2, float flame);

		/// <summary>
	/// エルミート曲線
	/// </summary>
	/// <param name="p0">制御点</param>
	/// <param name="p1">制御点</param>
	/// <param name="v0">制御点</param>
	/// <param name="v1">制御点</param>
	/// <param name="t">時間(0.0～1.0)</param>
	/// <returns></returns>
	Vector3 HermiteGetPoint(Vector3 p0, Vector3 p1, Vector3 v0, Vector3 v1, float t);

	/// <summary>
	/// キャットムルーロムスプライン 
	/// </summary>
	/// <param name="points">制御点</param>
	/// <param name="t">時間(0.0～1.0)</param>
	Vector3 CatmullRomSpline(std::vector<Vector3>& points, float t);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);
};

