#pragma once
#include"ErrorException.h"
#include"Keyboard.h"
#include"Mouse.h"
#include"Controller.h"

class Input
{
private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;

	//キーボードデバイス
	Keyboard* keyboard = nullptr;

	//マウス
	Mouse* mouse = nullptr;

	Controller* controller = nullptr;
	unsigned char keys[256];

public:
	/// <summary>
	/// インスタンスを所得
	/// </summary>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//キーボード

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

	unsigned char GetKeyTime(int keyName);
	//マウス

	/// <summary>
	/// マウスボタンのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool MouseButtonTrigger(MouseButton button);

	/// <summary>
	/// マウスボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool MouseButtonInput(MouseButton button);

	/// <summary>
	/// マウスボタンの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool MouseButtonOffTrigger(MouseButton button);

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns>マウスの位置</returns>
	const AliceMathF::Vector2 GetMousePos()const;

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns>マウスの位置</returns>
	const AliceMathF::Vector3 GetMouseMove();

	//コントローラ-

	/// <summary>
	/// コントローラーボタンのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool ButtonTrigger(ControllerButton button);

	/// <summary>
	/// コントローラースティックのトリガー入力
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool StickTrigger(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool ButtonInput(ControllerButton button);

	/// <summary>
	/// コントローラースティックの入力
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool StickInput(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーボタンの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool ButtonOffTrigger(ControllerButton button);

	/// <summary>
	/// コントローラースティックの離した瞬間
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>離したか</returns>
	bool StickOffTrigger(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーの左スティックのベクトル
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>ベクトル</returns>
	AliceMathF::Vector2 GetLeftStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーの右スティックのベクトル
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>ベクトル</returns>
	AliceMathF::Vector2 GetRightStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーを振動させる
	/// </summary>
	/// <param name="power">振動の強さ0.0f～1.0f</param>
	/// <param name="span">振動の時間フレーム</param>
	void ShakeController(const float& power, const int& span);
};