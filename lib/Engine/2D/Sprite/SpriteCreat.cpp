#include "SpriteCreat.h"

Sprite2D* SpriteCreat::Create2DSprite()
{
	Sprite2D* s2D = new Sprite2D;
	s2D->Initialize();
	return s2D;
}

Sprite3D* SpriteCreat::Create3DSprite()
{
	Sprite3D* s3D = new Sprite3D;
	s3D->Initialize();
	return s3D;
}
