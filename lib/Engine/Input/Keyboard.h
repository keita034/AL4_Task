#pragma once
#include"ErrorException.h"
#include"WindowsApp.h"

class Keyboard
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE oldkey[256] = {};
	BYTE key[256] = {};
public:
	void Initialize(IDirectInput8* directInput);
	void Update();

	/// <summary>
	/// キーボードのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool TriggerPush(int keyName);

	/// <summary>
	/// キーボードの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool KeepPush(int keyName);

	/// <summary>
	/// キーボードの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool TriggerRelease(int keyName);

	/// <summary>
	/// キーボードの離してる間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離してるか</returns>
	bool KeepRelease(int keyName);
};

