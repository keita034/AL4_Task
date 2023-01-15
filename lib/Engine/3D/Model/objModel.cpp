#include "objModel.h"
#include"TextureManager.h"

objModel::objModel()
{
}

objModel::~objModel()
{
}

void objModel::Create(const char* filePath,bool smoothing)
{
	std::vector<std::string> files;
	files = AliceMathUtility::getFileNames(filePath);
	std::string fileName;

	for (std::string file : files)
	{
		if (file.find("obj") != std::string::npos)
		{
			fileName = file;
		}

	}
	//ファイルストリーム
	std::ifstream file;
	//objファイルを開く
	file.open(fileName);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//頂点座標
	std::vector<AliceMathF::Vector3> positions;
	//法線ベクトル
	std::vector<AliceMathF::Vector3> normals;
	//テクスチャUV
	std::vector<AliceMathF::Vector2> texcoords;

	PosNormalUv tmp = { {},{},{} };

	//1行ずつ読み込む
	std::string line;
	while (std::getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字を取得
		std::string key;
		std::getline(line_stream, key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			std::string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(filePath, filename);

		}
		//先頭文字列がVなら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			AliceMathF::Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}
		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//U,V成分読み込み
			AliceMathF::Vector2 texcoord;
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			AliceMathF::Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		//先頭文字ならポリゴン(三角形)
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (std::getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1個分文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				uint16_t indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				//スラッシュを飛ばす
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				//スラッシュを飛ばす
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;

				//頂点データの追加
				tmp.pos = positions[static_cast<std::vector<AliceMathF::Vector2, std::allocator<AliceMathF::Vector2>>::size_type>(indexPosition) - 1];
				tmp.normal = normals[static_cast<std::vector<AliceMathF::Vector2, std::allocator<AliceMathF::Vector2>>::size_type>(indexNormal) - 1];
				tmp.uv = texcoords[static_cast<std::vector<AliceMathF::Vector2, std::allocator<AliceMathF::Vector2>>::size_type>(indexTexcoord) - 1];
				vertices.push_back(tmp);

				if (smoothing)
				{
					smoothData[static_cast<size_t>(indexPosition)].emplace_back(static_cast<uint16_t>(vertices.size()-1));
				}

				//インデックスデータの追加
				indices.emplace_back(static_cast<uint16_t>(indices.size()));

			}
		}
	}

	//ファイルを閉じる
	file.close();

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	}

	maxVert = static_cast<UINT>(vertices.size());
	maxIndex = static_cast<UINT>(indices.size());

	//頂点バッファ・インデックス生成
	CreatVertexIndexBuffer();

	//頂点バッファへのデータ転送
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	//インデックスバッファへのデータ転送
	for (size_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];   // インデックスをコピー
	}


	DirectX12Core::GetInstance()->CreateConstBuff(constMapMaterial, constBuffMaterial);
	constMapMaterial->ambient = modelMaterial.ambient;
	constMapMaterial->diffuse = modelMaterial.diffuse;
	constMapMaterial->specular = modelMaterial.specular;
	constMapMaterial->alpha = modelMaterial.alpha;
}

void objModel::Draw(Transform* transform, Material* material)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(material->pipelineState.Get());
	cmdList->SetGraphicsRootSignature(material->rootSignature.Get());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&ibView);

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, transform->GetconstBuff()->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial->GetGPUVirtualAddress());
	light->SetConstBufferView(cmdList.Get(), 3);

	// SRVヒープの設定コマンド
	cmdList->SetDescriptorHeaps(1, textureData.srvHeap.GetAddressOf());

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	cmdList->SetGraphicsRootDescriptorTable(2, textureData.gpuHandle);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(maxIndex, 1, 0, 0, 0);

}


void objModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	std::ifstream file;

	std::string filePath = directoryPath + "/" + filename;

	file.open(filePath);
	if (file.fail())
	{
		assert(0);
	}

	std::string line;
	while (std::getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字を取得
		std::string key;
		std::getline(line_stream, key, ' ');

		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		if (key == "newmtl")
		{
			line_stream >> modelMaterial.name;
		}
		if (key == "Ka")
		{
			line_stream >> modelMaterial.ambient.x;
			line_stream >> modelMaterial.ambient.y;
			line_stream >> modelMaterial.ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> modelMaterial.diffuse.x;
			line_stream >> modelMaterial.diffuse.y;
			line_stream >> modelMaterial.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> modelMaterial.specular.x;
			line_stream >> modelMaterial.specular.y;
			line_stream >> modelMaterial.specular.z;
		}
		if (key == "map_Kd")
		{
			line_stream >> modelMaterial.textureFiename;
			LoadTexture(directoryPath, modelMaterial.textureFiename);
		}
	}

	//ファイルを閉じる
	file.close();
}

void objModel::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	std::string filepath = directoryPath + "/" + filename;

	wchar_t wfilepath[128]{};
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	textureData = TextureManager::GetInstance()->LoadTexture(wfilepath);
}

void objModel::Initialize()
{
	ModelInitialize();
}

void objModel::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (;itr != smoothData.end(); itr++)
	{
		//各面用の共通点コレクション
		std::vector<uint16_t>& v = itr->second;
		//全頂点の法線を平均する
		AliceMathF::Vector3 normal = {};
		for (uint16_t index : v)
		{
			normal += AliceMathF::Vector3{vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z};
		}

		normal = normal / static_cast<float>(v.size());
		normal.normal();
		//共通法線を使用する全ての頂点データに書き込む
		for (uint16_t index : v)
		{
			vertices[index].normal = { normal.x,normal.y,normal.z };
		}

	}
}