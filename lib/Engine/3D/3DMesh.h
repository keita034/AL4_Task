#pragma once
//#DirectXのやつ
#include"ErrorException.h"

//自作.h
#include "DirectX12Core.h"
#include"WindowsApp.h"
#include"AliceMathUtility.h"
#include"Camera.h"

//pragma comment
#pragma comment(lib, "d3dcompiler.lib")


//using namespace


class Mesh3D
{
private:

	DirectX12Core* directX12Core = nullptr;

	//定数バッファのGPUリソースのポインタ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform = nullptr;

	//ブレンドモード
	UINT blendMode;
	char PADDING[4];

	//三角形形状パイプラインセット
	std::array<std::unique_ptr<PipelineSet>, (size_t)BlendMode::AX_BLENDMODE_MAX>trianglePipelineSet;
	//ライン形状パイプラインセット
	std::array<std::unique_ptr<PipelineSet>, (size_t)BlendMode::AX_BLENDMODE_MAX>linePipelineSet;

	//三角形バッファ
	std::unique_ptr <Buff> triangleBuff;
	//三角形描画上限
	UINT triangleMaxCount = 4096;
	//三角形頂点数
	UINT triangleVertexCount = 3;
	//三角形インデックス数
	UINT triangleIndexCount = 0;
	//三角形使用数
	UINT triangleCount = 0;

	//線バッファ
	std::unique_ptr <Buff> lineBuff;
	//線描画上限
	UINT lineMaxCount = 4096;
	//線頂点数
	UINT lineVertexCount = 2;
	//線インデックス数
	UINT lineIndexCount = 0;
	//線使用数
	UINT lineCount = 0;

	//四角形バッファ
	std::unique_ptr <Buff> boxBuff;
	//四角形描画上限
	UINT boxMaxCount = 2048;
	//四角形頂点数
	UINT boxVertexCount = 4;
	//四角形インデックス数
	UINT boxIndexCount = 6;
	//四角形使用数
	UINT boxCount = 0;

	static Mesh3D* mesh;

	Camera* camera;

	AliceMathF::Matrix4 projectionMat;

public:

	/////<summary>
	/////三角形を描画する
	/////</summary>
	/////<param name="x1">: 描画する三角形の頂点1 X</param>
	/////<param name="y1">: 描画する三角形の頂点1 Y</param>
	/////<param name="x2">: 描画する三角形の頂点2 X</param>
	/////<param name="y2">: 描画する三角形の頂点2 Y</param>
	/////<param name="x3">: 描画する三角形の頂点3 X</param>
	/////<param name="y3">: 描画する三角形の頂点3 Y</param>
	/////<param name="color">: 描画する三角形の色</param>
	/////<param name="fillFlag">: 三角形の中身を塗りつぶすかフラグ</param>
	//void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, AliceMath::Vector4 color, int fillFlag);

	///<summary>
	///線を描画する
	///</summary>
	///<param name="x1">: 描画する線の始点座標 X</param>
	///<param name="y1">: 描画する線の始点座標 Y</param>
	///<param name="x2">: 描画する線の終点座標 X</param>
	///<param name="y2">: 描画する線の終点座標 Y</param>
	///<param name="color">: 描画する線の色</param>
	void DrawLine(AliceMathF::Vector3 start , AliceMathF::Vector3 end, AliceMathF::Vector4 color);

	/////<summary>
	/////四角形を描画する
	/////</summary>
	/////<param name="x">: 描画する四角形の中心座標 X</param>
	/////<param name="y">: 描画する四角形の中心座標 Y</param>
	/////<param name="radiusX">: 描画する四角形の半径 X</param>
	/////<param name="radiusY">: 描画する四角形の半径 Y</param>
	/////<param name="angle">: 描画する四角形の回転角(弧度法)</param>
	/////<param name="color">: 描画する四角形の色</param>
	/////<param name="fillFlag">: 四角形の中身を塗りつぶすかフラグ</param>
	//void DrawBox(float x, float y, float radiusX, float radiusY, float angle, AliceMath::Vector4 color, int fillFlag);

	/////<summary>
	/////楕円を描画する
	/////</summary>
	/////<param name="x">楕円の中心座標 X</param>
	/////<param name="y">楕円の中心座標 Y</param>
	/////<param name="radiusX">楕円の半径 X</param>
	/////<param name="radiusY">楕円の半径 Y</param>
	/////<param name="angle">楕円の回転角(rad)</param>
	/////<param name="color">楕円の色</param>
	/////<param name="fillFlag">: 四角形の中身を塗りつぶすかフラグ</param>
	//void DrawEllipse(float x, float y, float radiusX, float radiusY, float angle, AliceMath::Vector4 color, int fillMode);

	///<summary>
	///カウント初期化
	///</summary>
	void DrawReset();

	///<summary>
	///ブレンドモードを設定する
	///</summary>
	///<param name="mode">: ブレンドモード</param>
	void SetBlendMode(BlendMode mode);

	///<summary>
	///色コードを取得する
	///</summary>
	///<param name="red">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="blue">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="green">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="alpha">: 取得したい色の透過率 : 初期値255 (0～255)</param>
	///<returns>色コード</returns>
	AliceMathF::Vector4 GetColor(int red = 255, int blue = 255, int green = 255, int alpha = 255);

	///<summary>
	///色コードを取得する
	///</summary>
	///<param name="color">: 取得したい各色の輝度値 : 初期値255 (0～255)</param>
	///<returns>色コード</returns>
	AliceMathF::Vector4 GetColor(AliceMathF::Vector4 color);

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Mesh3D* GetInstance();

	/// <summary>
	/// インスタンスを解放
	/// </summary>
	void Destroy();

	void SetCamera(Camera* camera_);

private:

	//コンストラクタ
	Mesh3D();

	//三角形を描画する(中身塗りつぶし)
	void DrawTriangleFill(float x1, float y1, float x2, float y2, float x3, float y3, AliceMathF::Vector4 color);

	//四角形を描画する(中身塗りつぶし)
	void DrawBoxFill(float x, float y, float width, float height, float angle, AliceMathF::Vector4 color);

	//パイプライン作成
	std::unique_ptr < PipelineSet> CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode mode);

	///<summary>
	///バッファ作成
	///</summary>
	///<param name="vertexCount">: 頂点数</param>
	///<param name="indexCount">: インデックス数</param>
	///<returns>バッファ</returns>
	std::unique_ptr <Buff> CreateBuff(UINT vertexCount, UINT indexCount);


	//各種パイプラインセット生成
	void CreatArryPipeline();

	//定数バッファ生成(2D座標変換行列)
	void CreatConstBuff();

	//コピーコンストラクタ・代入演算子削除
	Mesh3D& operator=(const Mesh3D&) = delete;
	Mesh3D(const Mesh3D&) = delete;
};



