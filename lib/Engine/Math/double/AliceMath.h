#pragma once
#include"ErrorException.h"

namespace AliceMath
{
#pragma region 定数

	// π
	constexpr double AX_PI = 3.141592654;

	//π/2
	constexpr double AX_PIHalf = 3.141592654 / 2.0;

	// 2π
	constexpr double AX_2PI = 6.283185307;

	//ε
	constexpr double EPSILON = 2.2204460492503131E-16;

	//度からラジアンに変換する定数
	constexpr double Deg2Rad = AX_PI / 180.0f;

	//ラジアンから度に変換する定数
	constexpr double Rad2Deg = 57.29578;

#pragma endregion 定数

#pragma region sin,cos,tan関数

#pragma region sin関数	

	//sin関数
	double Sin(double sin);

	//asin関数
	double Asin(double sin);

#pragma endregion sin関数

#pragma region cos関数

	//cos関数
	double Cos(double cos);

	//cos関数
	double Acos(double cos);

#pragma endregion cos関数

#pragma region tan関数

	//tan関数
	double Tan(double tan);

	//atan関数
	double Atan(double tan);

	//atan関数
	double Atan2(float y, float x);

#pragma endregion tan関数

	/// <summary>
	/// Sin、Cosを両方出す
	/// </summary>
	/// <param name="sin_"> : sinの結果を代入する</param>
	/// <param name="cos_"> : cosの結果を代入する</param>
	/// <param name="angle"> : ラジアン角</param>
	void SinCos(double& sin_, double& cos_, double angle);

#pragma endregion sin,cos,tan関数

#pragma region 角度変換

	/// <summary>
	/// 度数法からラジアン変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	double ChangeRadians(const double& angle);

	/// <summary>
	/// ラジアンから度数法変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	double ChangeDira(const double& angle);

#pragma endregion 角度変換

#pragma region sqrt関数

	//sqrt関数
	double Sqrt(double sqrt);

#pragma endregion sqrt関数

#pragma region pow関数

	//pow関数
	double Pow(double pow, double powNum);

	//pow関数(long double)
	long double PowLD(long double pow, long double powNum);

#pragma endregion pow関数

#pragma region exp関数

	//exp関数
	double Exp(double exp);

#pragma endregion exp関数

#pragma region log関数

	//log関数
	double Log(double log);

	//log10関数
	double Log10(double log);

#pragma endregion log関数

#pragma region 絶対値の計算

	//絶対値の計算
	double Abs(double abs);

	//絶対値の計算(long double)
	long double AbsLD(long double abs);

#pragma endregion 絶対値の計算

#pragma region ceil関数

	//ceil関数
	double Ceil(double ceil);

	//ceil関数
	long double CeilLD(long double ceil);

#pragma endregion ceil関数

#pragma region floor関数

	//floor関数
	double Floor(double floor);

	//floor関数
	long double FloorLD(long double floor);

#pragma endregion floor関数

#pragma region round関数

	//round関数
	double Round(double round);

	//round関数
	long double RoundLD(long double round);

#pragma endregion round関数

	/// <summary>
	/// 値の符号を返す
	/// </summary>
	/// <returns>値が正か0の場合は1を、負の場合は-1を返す</returns>
	double Sign(double sign);

	/// <summary>
	/// 値を範囲内に納める
	/// </summary>
	/// <param name="Value">値</param>
	/// <param name="low">最低値</param>
	/// <param name="high">最高値</param>
	double Clamp(double Value, const double low, const double high);

	/// <summary>
	/// 0と1の間に値を納め、その値を返す
	/// </summary>
	/// <param name="Value">値</param>
	double Clamp01(double Value);

	/// <summary>
	/// 線形補間(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	double Lerp(double a, double b, double t);

	/// <summary>
	/// 線形補間(0～)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された値</returns>
	double LerpUnclamped(double a, double b, double t);

	/// <summary>
	/// Lerpの角度版(0～1)
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された角度</returns>
	double LerpAngle(double a, double b, double t);

	/// <summary>
	/// tはlengthより大きくはならず0より小さくならず、その間をループ
	/// </summary>
	/// <param name="t">値</param>
	/// <param name="length">指定値</param>
	double Repeat(double t, double length);

	/// <summary>
	/// 現在地から目的地まで一定速度で移動
	/// </summary>
	/// <param name="current">現在地</param>
	/// <param name="target">目的地</param>
	/// <param name="maxDelta">速度</param>
	double MoveTowards(double current, double target, double maxDelta);

	/// <summary>
	/// 現在の角度から目的の角度まで一定速度で移動
	/// </summary>
	/// <param name="current">現在の角度</param>
	/// <param name="target">目的の角度</param>
	/// <param name="maxDelta">速度</param>
	double MoveTowardsAngle(double current, double target, double maxDelta);

	/// <summary>
	/// 角度の差を返す(度数法)
	/// </summary>
	/// <param name="current">現在の角度</param>
	/// <param name="target">目標となる角度</param>
	double DeltaAngle(double current, double target);

	/// <summary>
	/// スムージング補間
	/// </summary>
	/// <param name="from">開始値</param>
	/// <param name="to">終了値</param>
	/// <param name="t">補間値</param>
	/// <returns>補間された角度</returns>
	double SmoothStep(double from, double to, double t);

	/// <summary>
	/// ガンマ補正
	/// </summary>
	double Gamma(double value, double absmax, double gamma);

	/// <summary>
	/// 何を何処に何秒で指定してオブジェクトを移動させる
	/// </summary>
	/// <param name="current">現在地</param>
	/// <param name="target">目的地</param>
	/// <param name="currentVelocity">事前に初期化した速度</param>
	/// <param name="smoothTime">目的までの到達時間</param>
	/// <param name="maxSpeed">最高速度</param>
	/// <param name="deltaTime">関数が前回実行されてからの経過時間</param>
	double SmoothDamp(double current, double target, double& currentVelocity, double smoothTime, double maxSpeed, double deltaTime);

	/// <summary>
	/// tが0とlengthの間で往復する(0～length)
	/// </summary>
	double PingPong(double t, double length);

	/// <summary>
	/// lerpの補間値を出す
	/// </summary>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="value">開始と終了の間の値</param>
	/// <returns>補間値</returns>
	double InverseLerp(double a, double b, double value);

	/// <summary>
	/// 二つの値がほぼ等しいか
	/// </summary>
	bool Approximately(double a, double b);

	/// <summary>
	/// ランダムな値を取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	double GetRand(double min, double max);
}

