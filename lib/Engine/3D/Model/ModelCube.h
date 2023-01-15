#pragma once
#include"Model.h"


class ModelCube :public Model
{
private:
	//キューブのインデックス数と頂点数
	const UINT cubeMaxIndex = 36;
	const UINT cubeMaxVert = 24;

public:
	ModelCube();
	~ModelCube();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(Transform* transform, Material* material)override;

	//初期化
	void Initialize()override;

private:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	virtual void Create(bool smoothing = false)override;

	virtual void Create(const char* filePath, bool smoothing = false);

	//コピーコンストラクタ・代入演算子削除
	ModelCube& operator=(const ModelCube&) = delete;
	ModelCube(const ModelCube&) = delete;
};

