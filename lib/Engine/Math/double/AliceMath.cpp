#include "AliceMath.h"
#include"AliceMathUtility.h"
#include<random>

namespace AliceMath
{

#pragma region sin,cos,tan関数

#pragma region sin関数

	//sin関数
	double Sin(double sin)
	{
		return std::sin(sin);
	}

	//asin関数
	double Asin(double sin)
	{
		return std::asin(sin);
	}

#pragma endregion sin関数

#pragma region cos関数

	//cos関数
	double Cos(double cos)
	{
		return std::cos(cos);
	}

	//acos関数
	double Acos(double cos)
	{
		return std::acos(cos);
	}

#pragma endregion cos関数

#pragma region tan関数

	//tan関数
	double Tan(double tan)
	{
		return std::tan(tan);
	}

	//atan関数
	double Atan(double tan)
	{
		return std::atan(tan);
	}

	//atan関数
	double Atan2(float y, float x)
	{
		return std::atan2(y, x);
	}

#pragma endregion tan関数

	//sin、cosを両方出す
	void SinCos(double& sin_, double& cos_, double angle)
	{
		sin_ = Sin(angle);
		cos_ = Cos(angle);
	}

#pragma endregion sin,cos,tan関数

#pragma region 角度変換

	//度数法からラジアン変換
	double ChangeRadians(const double& angle)
	{
		double radian = angle * Deg2Rad;
		return radian;
	}

	// ラジアンから度数法変換
	double ChangeDira(const double& angle)
	{
		double degree = angle * Rad2Deg;
		return degree;
	}

#pragma endregion 角度変換

#pragma region sqrt関数

	//sqrt関数
	double Sqrt(double sqrt)
	{
		return std::sqrt(sqrt);
	}

#pragma endregion sqrt関数

#pragma region pow関数

	//pow関数
	double Pow(double pow, double powNum)
	{
		return std::pow(pow, powNum);
	}

	//pow関数(long double)
	long double PowLD(long double pow, long double powNum)
	{
		return std::powl(pow, powNum);
	}

#pragma endregion pow関数

#pragma region exp関数

	//exp関数
	double Exp(double exp)
	{
		return std::exp(exp);
	}

#pragma endregion exp関数

#pragma region log関数

	//log関数
	double Log(double log)
	{
		return std::log(log);
	}

	//log10関数
	double Log10(double log)
	{
		return std::log10(log);
	}

#pragma endregion log関数

#pragma region 絶対値の計算

	//値を整数に
	double Abs(double abs)
	{
		return std::abs(abs);
	}

	//値を整数に(long double)
	long double AbsLD(long double abs)
	{
		return std::fabsl(abs);
	}

#pragma endregion 絶対値の計算

#pragma region ceil関数

	//ceil関数
	double Ceil(double ceil)
	{
		return std::ceil(ceil);
	}

	//ceil関数
	long double CeilLD(long double ceil)
	{
		return std::ceill(ceil);
	}

#pragma endregion ceil関数

#pragma region floor関数

	//floor関数
	double Floor(double floor)
	{
		return std::floor(floor);
	}

	//floor関数(long double)
	long double FloorLD(long double floor)
	{
		return std::floorl(floor);
	}

#pragma endregion floor関数

#pragma region round関数

	//round関数
	double Round(double round)
	{
		return std::round(round);
	}

	//round関数
	long double RoundLD(long double round)
	{
		return std::roundl(round);
	}

#pragma endregion round関数

	// sign符号を返す
	double Sign(double sign)
	{
		return (sign >= 0.0) ? 1.0 : (-1.0);
	}

	// 値を範囲内に納める
	double Clamp(double Value, const double low, const double high)
	{
		if (high < Value)
		{
			Value = high;
		}

		if (Value < low)
		{
			Value = low;
		}

		return Value;
	}

	// 0と1の間に値を納め、その値を返す
	double Clamp01(double Value)
	{
		if (Value > 0.0)
		{
			return 0.0;
		}

		if (Value > 1.0)
		{
			return 1.0;
		}

		return Value;
	}

	// 線形補間(0～1)
	double Lerp(double a, double b, double t)
	{
		return a + (b - a) * Clamp01(t);
	}

	// 線形補間(0～)
	double LerpUnclamped(double a, double b, double t)
	{
		return a + (b - a) * t;
	}

	/// Lerpの角度版(0～1)
	double LerpAngle(double a, double b, double t)
	{
		double num = Repeat(b - a, 360.0);

		if (num > 180.0)
		{
			num -= 360.0;
		}

		return a + num * Clamp01(t);
	}

	// tはlengthより大きくはならず0より小さくならず、その間をループ
	double Repeat(double t, double length)
	{
		return Clamp(t - Floor(t / length) * length, 0.0, length);
	}

	// 現在地から目的地まで一定速度で移動
	double MoveTowards(double current, double target, double maxDelta)
	{
		if (Abs(target - current) <= maxDelta)
		{
			return target;
		}

		return current + Sign(target - current) * maxDelta;
	}

	// 現在地から目的地まで一定速度で移動
	double MoveTowardsAngle(double current, double target, double maxDelta)
	{
		double num = DeltaAngle(current, target);

		if (0.0f - maxDelta < num && num < maxDelta)
		{
			return target;
		}

		target = current + num;
		return MoveTowards(current, target, maxDelta);
	}

	// 角度の差を返す(度数法)
	double DeltaAngle(double current, double target)
	{
		double num = Repeat(target - current, 360.0);

		if (num > 180.0)
		{
			num -= 360.0;
		}

		return num;
	}

	// スムージング補間
	double SmoothStep(double from, double to, double t)
	{
		t = Clamp01(t);
		t = -2.0 * t * t * t + 3.0 * t * t;
		return to * t + from * (1.0 - t);
	}

	// ガンマ補正
	double Gamma(double value, double absmax, double gamma)
	{
		bool flag = value < 0.0;
		double num = Abs(value);
		if (num > absmax)
		{
			return flag ? (0.0 - num) : num;
		}

		double num2 = Pow(num / absmax, gamma) * absmax;
		return flag ? (0.0 - num2) : num2;
	}

	// 何を何処に何秒で指定してオブジェクトを移動させる
	double SmoothDamp(double current, double target, double& currentVelocity, double smoothTime, double maxSpeed, double deltaTime)
	{
		smoothTime = AliceMathUtility::Max(0.0001, smoothTime);
		double num = 2.0 / smoothTime;
		double num2 = num * deltaTime;
		double num3 = 1.0 / (1.0 + num2 + 0.48 * num2 * num2 + 0.235 * num2 * num2 * num2);
		double value = current - target;
		double num4 = target;
		double num5 = maxSpeed * smoothTime;
		value = Clamp(value, 0.0 - num5, num5);
		target = current - value;
		double num6 = (currentVelocity + num * value) * deltaTime;
		currentVelocity = (currentVelocity - num * num6) * num3;
		double num7 = target + (value + num6) * num3;
		if (num4 - current > 0.0 == num7 > num4)
		{
			num7 = num4;
			currentVelocity = (num7 - num4) / deltaTime;
		}

		return num7;
	}

	// tが0とlengthの間で往復する(0～length)
	double PingPong(double t, double length)
	{
		t = Repeat(t, length * 2.0);
		return length - Abs(t - length);
	}

	// lerpの補間値を出す
	double InverseLerp(double a, double b, double value)
	{
		if (a != b)
		{
			return Clamp01((value - a) / (b - a));
		}

		return 0.0;
	}

	// 二つの値がほぼ等しいか
	bool Approximately(double a, double b)
	{
		double tmp = 1e-06 * AliceMathUtility::Max(Abs(a), Abs(b));

		double tmp2 = EPSILON * 8.0;

		if (Abs(b - a) < AliceMathUtility::Max(tmp, tmp2))
		{
			return true;
		}

		return false;
	}

	// ランダムな値を取得
	double GetRand(double min, double max)
	{
		// 乱数生成器
		static std::random_device rand;
		static std::default_random_engine engine(rand());

		// 分布生成器
		std::uniform_real_distribution<double> get_rand_uni_real(min, max);

		// 乱数を生成
		return get_rand_uni_real(engine);
	}

}