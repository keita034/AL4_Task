#include "Controller.h"


bool Controller::StickInDeadZone(AliceMathF::Vector2& Thumb, const AliceMathF::Vector2& DeadRate)
{
	bool x = false;
	bool y = false;

	if ((Thumb.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.x
		&& Thumb.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.x))
	{
		Thumb.x = 0.0f;
		x = true;
	}
	if ((Thumb.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.y
		&& Thumb.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.y))
	{
		Thumb.y = 0.0f;
		y = true;
	}

	if (x && y)
	{
		return true;
	}

	return false;
}

void Controller::Update()
{
	oldXinputState = xinputState;
	ZeroMemory(&xinputState, sizeof(XINPUT_STATE));

	//コントローラー取得
	DWORD dwResult = XInputGetState(0, &xinputState);

	if (dwResult == ERROR_SUCCESS)
	{
		//コントローラーが接続されている
		if (0 < shakeTimer)
		{
			shakeTimer--;
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			if (shakeTimer == 0)
			{
				vibration.wLeftMotorSpeed = static_cast<WORD>(0.0f); // use any value between 0-65535 here
				vibration.wRightMotorSpeed = static_cast<WORD>(0.0f); // use any value between 0-65535 here
			}
			else
			{
				vibration.wLeftMotorSpeed = static_cast<WORD>(65535.0f * shakePower); // use any value between 0-65535 here
				vibration.wRightMotorSpeed = static_cast<WORD>(65535.0f * shakePower); // use any value between 0-65535 here
			}

			XInputSetState(dwResult, &vibration);
		}
	}
	else
	{
		//コントローラーが接続されていない
	}
}

bool Controller::ButtonTrigger(ControllerButton button)
{
	//トリガー
	if (button == LT)
	{
		return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && ButtonInput(button);
	}
	else if (button == RT)
	{
		return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && ButtonInput(button);
	}
	else
	{
		return !(oldXinputState.Gamepad.wButtons & button) && ButtonInput(button);
	}
}

bool Controller::StickTrigger(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
{
	AliceMathF::Vector2 oldVec;
	AliceMathF::Vector2 vec;
	bool isLeftStick = stickInput <= L_RIGHT;
	if (isLeftStick)
	{
		oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
		vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
	}
	else
	{
		oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
		vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
	}

	if (!StickInDeadZone(oldVec, deadRate))
	{
		return false;
	}

	if (StickInDeadZone(vec, deadRate))
	{
		return false;
	}

	bool result = false;

	if (stickInput % 4 == L_UP)
	{
		result = !(deadRange < (oldVec.y / STICK_INPUT_MAX)) && deadRange < (vec.y / STICK_INPUT_MAX);
	}
	else if (stickInput % 4 == L_DOWN)
	{
		result = !(oldVec.y / STICK_INPUT_MAX < -deadRange) && vec.y / STICK_INPUT_MAX < -deadRange;
	}
	else if (stickInput % 4 == L_RIGHT)
	{
		result = !(deadRange < (oldVec.x / STICK_INPUT_MAX)) && deadRange < (vec.x / STICK_INPUT_MAX);
	}
	else if (stickInput % 4 == L_LEFT)
	{
		result = !(oldVec.x / STICK_INPUT_MAX < -deadRange) && vec.x / STICK_INPUT_MAX < -deadRange;
	}
	else
	{
		assert(0);
	}

	return result;
}

bool Controller::ButtonInput(ControllerButton button)
{
	if (button == LT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
	}
	else if (button == RT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
	}
	else
	{
		return xinputState.Gamepad.wButtons & button;
	}
}

bool Controller::StickInput(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
{
	AliceMathF::Vector2 vec;
	bool isLeftStick = stickInput <= L_RIGHT;

	if (isLeftStick)
	{
		vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
	}
	else
	{
		vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
	}

	if (StickInDeadZone(vec, deadRate))return false;

	if (stickInput % 4 == L_UP)
	{
		return deadRange < (vec.y / STICK_INPUT_MAX);
	}
	else if (stickInput % 4 == L_DOWN)
	{
		return  vec.y / STICK_INPUT_MAX < -deadRange;
	}
	else if (stickInput % 4 == L_RIGHT)
	{
		return deadRange < (vec.x / STICK_INPUT_MAX);
	}
	else if (stickInput % 4 == L_LEFT)
	{
		return  vec.x / STICK_INPUT_MAX < -deadRange;
	}

	assert(0);
	return false;
}

bool Controller::ButtonOffTrigger(ControllerButton button)
{
	//トリガー
	if (button == LT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bLeftTrigger && !ButtonInput(button);
	}
	else if (button == RT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bRightTrigger && !ButtonInput(button);
	}
	//ボタン
	else
	{
		return (oldXinputState.Gamepad.wButtons & button) && !ButtonInput(button);
	}
}

bool Controller::StickOffTrigger(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
{
	AliceMathF::Vector2 oldVec;
	AliceMathF::Vector2 vec;
	bool isLeftStick = stickInput <= L_RIGHT;

	if (isLeftStick)
	{
		oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
		vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
	}
	else
	{
		oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
		vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
	}

	if (!StickInDeadZone(oldVec, deadRate))
	{
		return false;
	}

	if (StickInDeadZone(vec, deadRate))
	{
		return false;
	}

	bool result = false;

	if (stickInput % 4 == L_UP)
	{
		result = deadRange < (oldVec.y / STICK_INPUT_MAX) && !(deadRange < (vec.y / STICK_INPUT_MAX));
	}
	else if (stickInput % 4 == L_DOWN)
	{
		result = oldVec.y / STICK_INPUT_MAX < -deadRange && !(vec.y / STICK_INPUT_MAX < -deadRange);
	}
	else if (stickInput % 4 == L_RIGHT)
	{
		result = deadRange < (oldVec.x / STICK_INPUT_MAX) && !(deadRange < (vec.x / STICK_INPUT_MAX));
	}
	else if (stickInput % 4 == L_LEFT)
	{
		result = oldVec.x / STICK_INPUT_MAX < -deadRange && !(vec.x / STICK_INPUT_MAX < -deadRange);
	}
	else
	{
		assert(0);
	}

	return result;
}

AliceMathF::Vector2 Controller::GetLeftStickVec(const AliceMathF::Vector2& deadRate)
{

	AliceMathF::Vector2 result(static_cast<float>(xinputState.Gamepad.sThumbLX), static_cast<float>(-xinputState.Gamepad.sThumbLY));
	StickInDeadZone(result, deadRate);
	return result / STICK_INPUT_MAX;
}

AliceMathF::Vector2 Controller::GetRightStickVec(const AliceMathF::Vector2& deadRate)
{
	AliceMathF::Vector2 result(static_cast<float>(xinputState.Gamepad.sThumbRX), static_cast<float>(-xinputState.Gamepad.sThumbRY));
	StickInDeadZone(result, deadRate);
	return result / STICK_INPUT_MAX;;
}

void Controller::ShakeController(const float& power, const int& span)
{
	if (!(0 < power && power <= 1.0f))
	{
		assert(0);
	}

	shakePower = power;
	shakeTimer = span;
}