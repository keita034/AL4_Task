#pragma once
#include"Particle.h"
#include"AliceMathUtility.h"
#include"DirectX12Core.h"

class ParticleCreat
{
private:

	static ModelShareVaria modelShareVaria;

public:
	static Particle* CreateParticle(const wchar_t* filePath);

private:
	//パイプラインの生成
	static void CreatPipeline();
	//デスクプリタヒープの生成
	static void CreateDescriptorHeap();
	//デスクプリタレンジの生成
	static void CreateDescriptorRange();
};

