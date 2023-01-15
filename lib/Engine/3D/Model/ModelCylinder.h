#pragma once
#include "Model.h"

class ModelCylinder : public Model
{
private:
	//円柱のインデックス数と頂点数
	const UINT cylinderMaxIndex = 384;
	const UINT cylinderMaxVert = 132;

public:

	ModelCylinder();
	~ModelCylinder();

	//初期化
	virtual void Initialize()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(Transform* transform, Material* material);

private:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	virtual void Create(bool smoothing = false)override;

	virtual void Create(const char* filePath, bool smoothing = false);

	//コピーコンストラクタ・代入演算子削除
	ModelCylinder& operator=(const ModelCylinder&) = delete;
	ModelCylinder(const ModelCylinder&) = delete;
};

