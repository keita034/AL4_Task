#pragma once

class DebugText
{
private:
	static DebugText* instance;

public:

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	static DebugText* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:

	DebugText() = default;
	~DebugText() = default;

};

