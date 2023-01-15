#pragma once
#include "Model.h"

class ModelCone : public Model
{
private:
	//円錐のインデックス数と頂点数
	const UINT coneMaxIndex = 191;
	const UINT coneMaxVert = 66;

public:

	ModelCone();
	~ModelCone();

	//初期化
	virtual void Initialize()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void Draw(Transform* transform, Material* material)override;

private:


	/// <summary>
	/// オブジェクト生成
	/// </summary>
	virtual void Create(bool smoothing = false)override;

	virtual void Create(const char* filePath, bool smoothing = false);

	//コピーコンストラクタ・代入演算子削除
	ModelCone& operator=(const ModelCone&) = delete;
	ModelCone(const ModelCone&) = delete;
};

