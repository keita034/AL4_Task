#pragma once
#include"Material.h"
#include"TextureManager.h"

//デフォルトのテクスチャ(白色)※改変しないこと
extern TextureData DEFAULT_TEXTURE;

//デフォルトのマテリアル※改変しないこと
extern Material DEFAULT_TEXTURE_MATERIAL;

//デフォルトのランバートマテリアル※改変しないこと
extern Material DEFAULT_LAMBERT_MATERIAL;

//デフォルトのフォングマテリアル※改変しないこと
extern Material DEFAULT_PHONG_MATERIAL;

//デフォルトのスプライト2Dマテリアル※改変しないこと
extern Material DEFAULT_SPRITE2D_MATERIAL;

//デフォルトのスプライト3Dマテリアル※改変しないこと
extern Material DEFAULT_SPRITE3D_MATERIAL;

void DefaultMaterialInitialize();
