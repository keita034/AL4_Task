#pragma once
#include"ErrorException.h"
#include"DirectX12Core.h"
#include"ModelCube.h"
#include"ModelCone.h"
#include"ModelCylinder.h"
#include"ModelSphere.h"
#include"objModel.h"

class ModelCreate
{
private:

public:
	static ModelCube* CreateCubeModel();
	static ModelSphere* CreateModelSphere();
	static ModelCylinder* CreateModelCylinder();
	static ModelCone* CreateModelCone();
	static objModel* CreateOBJModel(const char* filePath, bool smoothing = false);

private:

};

