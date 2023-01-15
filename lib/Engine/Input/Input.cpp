
#include "Input.h"
#include"WindowsApp.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


Input* Input::GetInstance()
{
	static Input instance;
	return &instance;

}
void Input::Initialize()//初期化
{

	HRESULT result;

	WindowsApp* app = WindowsApp::GetInstance();

	//DirectInputの初期化
	result = DirectInput8Create(app->GetW().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)directInput.ReleaseAndGetAddressOf(), nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの初期化
	keyboard = new Keyboard;
	keyboard->Initialize(directInput.Get());

	//マウスデバイスの初期化
	mouse = new Mouse;
	mouse->Initialize(directInput.Get());

	//コントローラーの初期化
	controller = new Controller;
}

void Input::Update()//更新処理
{
	//キーボードデバイスの更新
	keyboard->Update();

	//マウスデバイスの更新
	mouse->Update();
	
	//コントローラーデバイスの更新
	controller->Update();
}

unsigned char Input::GetKeyTime(int keyName)
{
	if (KeepPush(keyName))
	{
		keys[keyName]++;

		if (keys[keyName] >= 255)
		{
			keys[keyName] = 254;
		}
	}
	else
	{
		keys[keyName] = 0;
	}

	return keys[keyName];
}

bool Input::TriggerPush(int keyName)
{
	return keyboard->TriggerPush(keyName);
}

bool Input::KeepPush(int keyName)
{
	return keyboard->KeepPush(keyName);
}

bool Input::TriggerRelease(int keyName)
{
	return keyboard->TriggerRelease(keyName);
}

bool Input::KeepRelease(int keyName)
{
	return keyboard->TriggerRelease(keyName);
}

bool Input::MouseButtonTrigger(MouseButton button)
{
	return mouse->MouseButtonTrigger(button);
}

bool Input::MouseButtonInput(MouseButton button)
{
	return mouse->MouseButtonInput(button);
}

bool Input::MouseButtonOffTrigger(MouseButton button)
{
	return mouse->MouseButtonOffTrigger(button);
}

const AliceMathF::Vector2 Input::GetMousePos()const
{
	return mouse->GetMousePos();
}

const AliceMathF::Vector3 Input::GetMouseMove()
{
	return mouse->GetMouseMove();
}

bool Input::ButtonTrigger(ControllerButton button)
{
	return controller->ButtonTrigger(button);
}

bool Input::StickTrigger(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
{
	return controller->StickTrigger(stickInput, deadRange, deadRate);
}

bool Input::ButtonInput(ControllerButton button)
{
	return controller->ButtonInput(button);
}

bool Input::StickInput(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
{
	return controller->StickInput(stickInput, deadRange, deadRate);
}

bool Input::ButtonOffTrigger(ControllerButton button)
{
	return controller->ButtonOffTrigger(button);
}

bool Input::StickOffTrigger(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
{
	return controller->StickOffTrigger(stickInput, deadRange, deadRate);
}

AliceMathF::Vector2 Input::GetLeftStickVec(const AliceMathF::Vector2& deadRate)
{
	return controller->GetLeftStickVec(deadRate);
}

AliceMathF::Vector2 Input::GetRightStickVec(const AliceMathF::Vector2& deadRate)
{
	return controller->GetRightStickVec(deadRate);
}

void Input::ShakeController(const float& power, const int& span)
{
	controller->ShakeController(power, span);
}