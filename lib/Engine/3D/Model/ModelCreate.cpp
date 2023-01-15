#include "ModelCreate.h"


ModelCube* ModelCreate::CreateCubeModel()
{
	ModelCube* cube = new ModelCube;
	cube->Initialize();
	return cube;
}

ModelSphere* ModelCreate::CreateModelSphere()
{
	ModelSphere* sphere = new ModelSphere;
	sphere->Initialize();
	return sphere;
}

ModelCylinder* ModelCreate::CreateModelCylinder()
{
	ModelCylinder* cylinder = new ModelCylinder;
	cylinder->Initialize();
	return cylinder;
}

ModelCone* ModelCreate::CreateModelCone()
{
	ModelCone* cone = new ModelCone;
	cone->Initialize();
	return cone;
}


objModel* ModelCreate::CreateOBJModel(const char* filePath, bool smoothing)
{
	
	objModel* model = new objModel;
	model->Initialize();
	model->Create(filePath, smoothing);
	return model;

}