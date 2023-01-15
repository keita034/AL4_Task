#pragma once
#include"AliceMathUtility.h"
#include"DirectX12Core.h"
#include"Sprite2D.h"
#include"Sprite3D.h"

class SpriteCreat
{
private:

public:

	static Sprite2D* Create2DSprite();

	static Sprite3D* Create3DSprite();

private:
};

