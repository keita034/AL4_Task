#include "DefaultMaterial.h"

Material DEFAULT_TEXTURE_MATERIAL{};
Material DEFAULT_LAMBERT_MATERIAL{};
Material DEFAULT_PHONG_MATERIAL{};
Material DEFAULT_SPRITE2D_MATERIAL{};
Material DEFAULT_SPRITE3D_MATERIAL{};
TextureData DEFAULT_TEXTURE{};

const Material* s;
void DefaultMaterialInitialize()
{

	DEFAULT_TEXTURE = TextureManager::GetInstance()->LoadTexture(L"Resources/Default/white1x1.png");


	D3D12_ROOT_PARAMETER tmp{};

	D3D12_RENDER_TARGET_BLEND_DESC tmpblend{};

#pragma region デフォルト

	DEFAULT_TEXTURE_MATERIAL.textureData = DEFAULT_TEXTURE;

	DEFAULT_TEXTURE_MATERIAL.vsFiledata.pFileName = L"Resources/Shaders/3D/Model/ModelBasicVS.hlsl";
	DEFAULT_TEXTURE_MATERIAL.psFiledata.pFileName = L"Resources/Shaders/3D/Model/ModelBasicPS.hlsl";

	DEFAULT_TEXTURE_MATERIAL.inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 0;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_TEXTURE_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 1;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_TEXTURE_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	tmp.DescriptorTable.pDescriptorRanges = &DEFAULT_TEXTURE_MATERIAL.textureData.descriptorRange;
	tmp.DescriptorTable.NumDescriptorRanges = 1;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_TEXTURE_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 2;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_TEXTURE_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmpblend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//共通設定(アルファ値)
	tmpblend.BlendEnable = true;// ブレンドを有効
	tmpblend.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	tmpblend.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	tmpblend.DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う
	// 半透明合成
	tmpblend.BlendOp = D3D12_BLEND_OP_ADD;// 加算
	tmpblend.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	tmpblend.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;// 1.0f-ソースのアルファ値

	DEFAULT_TEXTURE_MATERIAL.blenddesc = tmpblend;


	DEFAULT_TEXTURE_MATERIAL.Initialize();

#pragma endregion

#pragma region ランバート

	DEFAULT_LAMBERT_MATERIAL.textureData = DEFAULT_TEXTURE;

	DEFAULT_LAMBERT_MATERIAL.vsFiledata.pFileName = L"Resources/Shaders/3D/Model/LambertVS.hlsl";
	DEFAULT_LAMBERT_MATERIAL.psFiledata.pFileName = L"Resources/Shaders/3D/Model/LambertPS.hlsl";

	DEFAULT_LAMBERT_MATERIAL.inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 0;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_LAMBERT_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 1;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_LAMBERT_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	tmp.DescriptorTable.pDescriptorRanges = &DEFAULT_LAMBERT_MATERIAL.textureData.descriptorRange;
	tmp.DescriptorTable.NumDescriptorRanges = 1;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_LAMBERT_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 2;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_LAMBERT_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	DEFAULT_LAMBERT_MATERIAL.blenddesc = tmpblend;

	DEFAULT_LAMBERT_MATERIAL.Initialize();

#pragma endregion

#pragma region フォング

	DEFAULT_PHONG_MATERIAL.textureData = DEFAULT_TEXTURE;

	DEFAULT_PHONG_MATERIAL.vsFiledata.pFileName = L"Resources/Shaders/3D/Model/PhongVS.hlsl";
	DEFAULT_PHONG_MATERIAL.psFiledata.pFileName = L"Resources/Shaders/3D/Model/PhongPS.hlsl";

	DEFAULT_PHONG_MATERIAL.inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 0;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_PHONG_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 1;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_PHONG_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	tmp.DescriptorTable.pDescriptorRanges = &DEFAULT_PHONG_MATERIAL.textureData.descriptorRange;
	tmp.DescriptorTable.NumDescriptorRanges = 1;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_PHONG_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 2;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_PHONG_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	DEFAULT_PHONG_MATERIAL.blenddesc = tmpblend;

	DEFAULT_PHONG_MATERIAL.Initialize();

#pragma endregion

#pragma region スプライト

	DEFAULT_SPRITE2D_MATERIAL.textureData = DEFAULT_TEXTURE;

	DEFAULT_SPRITE2D_MATERIAL.vsFiledata.pFileName = L"Resources/Shaders/2D/SpriteVS.hlsl";
	DEFAULT_SPRITE2D_MATERIAL.psFiledata.pFileName = L"Resources/Shaders/2D/SpritePS.hlsl";

	DEFAULT_SPRITE2D_MATERIAL.inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 0;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_SPRITE2D_MATERIAL.rootParams.push_back(tmp);
	tmp={};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	tmp.DescriptorTable.pDescriptorRanges = &DEFAULT_SPRITE2D_MATERIAL.textureData.descriptorRange;
	tmp.DescriptorTable.NumDescriptorRanges = 1;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_SPRITE2D_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmpblend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//共通設定(アルファ値)
	tmpblend.BlendEnable = true;// ブレンドを有効
	tmpblend.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	tmpblend.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	tmpblend.DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う
	// 半透明合成
	tmpblend.BlendOp = D3D12_BLEND_OP_ADD;// 加算
	tmpblend.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	tmpblend.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;// 1.0f-ソースのアルファ値

	DEFAULT_SPRITE2D_MATERIAL.blenddesc = tmpblend;

	DEFAULT_SPRITE2D_MATERIAL.depthFlag = false;

	DEFAULT_SPRITE2D_MATERIAL.Initialize();

	DEFAULT_SPRITE3D_MATERIAL.textureData = DEFAULT_TEXTURE;

	DEFAULT_SPRITE3D_MATERIAL.vsFiledata.pFileName = L"Resources/Shaders/2D/SpriteVS.hlsl";
	DEFAULT_SPRITE3D_MATERIAL.psFiledata.pFileName = L"Resources/Shaders/2D/SpritePS.hlsl";

	DEFAULT_SPRITE3D_MATERIAL.inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	tmp.Descriptor.ShaderRegister = 0;
	tmp.Descriptor.RegisterSpace = 0;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_SPRITE3D_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	tmp.DescriptorTable.pDescriptorRanges = &DEFAULT_SPRITE3D_MATERIAL.textureData.descriptorRange;
	tmp.DescriptorTable.NumDescriptorRanges = 1;
	tmp.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	DEFAULT_SPRITE3D_MATERIAL.rootParams.push_back(tmp);
	tmp = {};

	tmpblend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//共通設定(アルファ値)
	tmpblend.BlendEnable = true;// ブレンドを有効
	tmpblend.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	tmpblend.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	tmpblend.DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う
	// 半透明合成
	tmpblend.BlendOp = D3D12_BLEND_OP_ADD;// 加算
	tmpblend.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	tmpblend.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;// 1.0f-ソースのアルファ値

	DEFAULT_SPRITE3D_MATERIAL.blenddesc = tmpblend;

	DEFAULT_SPRITE3D_MATERIAL.Initialize();

#pragma endregion
}
