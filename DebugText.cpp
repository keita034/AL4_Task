#include "DebugText.h"

DebugText* DebugText::instance = nullptr;

DebugText* DebugText::GetInstance()
{
	if (!instance)
	{
		instance = new DebugText();
	}

	return instance;
}

void DebugText::Initialize()
{
}

void DebugText::Update()
{
}
