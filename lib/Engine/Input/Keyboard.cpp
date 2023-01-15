#include "Keyboard.h"

void Keyboard::Initialize(IDirectInput8* directInput)
{
	HRESULT result;

	WindowsApp* app = WindowsApp::GetInstance();

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(app->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Keyboard::Update()
{
	for (int i = 0; i < _countof(oldkey); i++)
	{
		oldkey[i] = key[i];
	}
	//キーボード情報の取得開始
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Keyboard::TriggerPush(int keyName)//押した瞬間
{
	return key[keyName] && !oldkey[keyName];
}

bool Keyboard::KeepPush(int keyName)//押されている間
{
	return static_cast<bool>(key[keyName]);
}

bool Keyboard::TriggerRelease(int keyName)//離された瞬間
{
	return !key[keyName] && oldkey[keyName];
}

bool Keyboard::KeepRelease(int keyName)//離している間
{
	return !key[keyName] && !oldkey[keyName];
}
