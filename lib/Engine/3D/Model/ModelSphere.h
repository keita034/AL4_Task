#pragma once
#include "Model.h"
#include"DirectX12Core.h"

class ModelSphere : public Model
{
private:
	//球のインデックス数と頂点数
	const UINT sphereMaxIndex = 3072;
	const UINT sphereMaxVert = 560;

public:

	ModelSphere();
	~ModelSphere();

	//初期化
	virtual void Initialize()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void Draw(Transform* transform, Material* material)override;

private:

	virtual void Create(const char* filePath, bool smoothing = false);

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	virtual void Create(bool smoothing = false)override;

	//コピーコンストラクタ・代入演算子削除
	ModelSphere& operator=(const ModelSphere&) = delete;
	ModelSphere(const ModelSphere&) = delete;
};

