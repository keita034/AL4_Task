#pragma once
#include"AliceMathUtility.h"
#include"DirectX12Core.h"

class ModelPipeLine
{
private:
	PipelineSet lambert{};
	PipelineSet phong{};
	PipelineSet dfault{};
	static ModelPipeLine* instance;
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	char pading[4]{};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelPipeLine* GetInstance();

	/// <summary>
	/// 解放
	/// </summary>
	void Destroy();

	/// <summary>
	/// デフォルト反射パイプライン取得
	/// </summary>
	/// <returns>パイプライン</returns>
	PipelineSet* GetDefaultPipeline();

	/// <summary>
	/// ランバート反射パイプライン取得
	/// </summary>
	/// <returns>パイプライン</returns>
	PipelineSet* GetLambertPipeline();

	/// <summary>
	/// フォング反射パイプライン取得
	/// </summary>
	/// <returns>パイプライン</returns>
	PipelineSet* GetPhongPipeline();

private:
	ModelPipeLine() = default;
	~ModelPipeLine() = default;

	//デフォルト
	void CreatDefaultPipeline();

	//ランバート
	void CreatLambertPipeline();

	//フォング
	void CreatPhongPipeline();
};

