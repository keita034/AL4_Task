#pragma once
#include"Model.h"
#include"AliceMathUtility.h"
#include"DefaultMaterial.h"

class objModel : public Model
{
private:

public:

	objModel();
	~objModel();

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	virtual void Create(const char* filePath, bool smoothing = false)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void Draw(Transform* transform, Material* material = &DEFAULT_TEXTURE_MATERIAL)override;

	//初期化
	virtual void Initialize()override;

private:

	void LoadMaterial(const std::string& directoryPath,const std::string& filename);
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	virtual void Create(bool smoothing)override
	{
		static_cast<void>(smoothing);
	};

	void CalculateSmoothedVertexNormals();

	//コピーコンストラクタ・代入演算子削除
	objModel& operator=(const objModel&) = delete;
	objModel(const objModel&) = delete;
};