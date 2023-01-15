#pragma once
#include"AliceMathUtility.h"
#include"WindowsApp.h"

///<summary>
///射影行列の更新方法。
///</summary>
enum UpdateProjMatrixFunc
{
	UpdateProjMatrixFunc_Perspective,		//透視射影行列。遠近法が効いた絵を作りたいならこっち。
	UpdateProjMatrixFunc_Ortho,			//平行投影。２Ｄ的な表現がしたいならこっち。
};

class Camera
{
public:

	//視点座標
	AliceMathF::Vector3 eye = { 0.0f,0.0f,-50.0f };
	//注視点座標
	AliceMathF::Vector3 target = { 0.0f,0.0f,0.0f };
	//上方向ベクトル
	AliceMathF::Vector3 up = { 0.0f,1.0f,0.0f };

private:

	//視点座標
	AliceMathF::Vector3 oldEye = eye;
	//注視点座標
	AliceMathF::Vector3 oldTarget = target;
	//上方向ベクトル
	AliceMathF::Vector3 oldUp = up;

	//ビュー行列
	AliceMathF::Matrix4 viewMatrix;
	//プロジェクション行列
	AliceMathF::Matrix4 projectionMatrix;
	//ビュープロジェクション行列
	AliceMathF::Matrix4 viewProjectionMatrix;
	//ビュー行列の逆行列
	AliceMathF::Matrix4 viewMatrixInv;
	//カメラの回転行列
	AliceMathF::Matrix4 cameraRotation;

	//画角(ラジアン度)
	float fovAngleY = 0.0;
	//アスペクト比(画面横幅/画面縦幅)
	float aspect = 0.0f;
	//ニアクリップ(前端)
	float near_ = 0.0f;
	//ファークリップ(奥端)
	float far_ = 0.0f;
	//画面左側
	float left = 0.0f;
	//画面右端
	float right = 0.0f;
	//画面上側
	float top = 0;
	//画面下側
	float bottom = 0.0f;

	//プロジェクション行列の更新フラグ
	bool updateProjMatrix = true;
	//ビュー行列の更新フラグ
	bool updateViewMatrix = true;
	char PADDING[2]{};

	//注視点と視点の距離
	float tgtToPosLen = 0.0f;

	//カメラの正面ベクトル
	AliceMathF::Vector3 forward = { 0.0f,0.0f,0.0f };

	UpdateProjMatrixFunc matrixFunc = UpdateProjMatrixFunc_Perspective;

public:

	Camera() = default;
	~Camera() = default;
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(UpdateProjMatrixFunc matFunc);

	/// <summary>
	/// ビュー行列・プロジェクション行列を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// カメラを動かす
	/// </summary>
	/// <param name="move">動かす量</param>
	void Move(const AliceMathF::Vector3& move);

	/// <summary>
	/// 注視点を動かす
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveTarget(const AliceMathF::Vector3& move);

	/// <summary>
	/// 視点を動かす
	/// </summary>
	/// <param name="move">移動量</param>
	void MovePosition(const AliceMathF::Vector3& move);

#pragma region セッター

	/// <summary>
	/// アスペクト比を設定する
	/// </summary>
	void SetAspect(float aspect_);

	/// <summary>
	/// 遠平面までの距離を設定
	/// </summary>
	void SetFar(float fFar);

	/// <summary>
	/// 近平面までの距離を設定
	/// </summary>
	void SetNear(float fNear);

	/// <summary>
	/// 行列の計算方法を設定
	/// </summary>
	/// <param name="func"></param>
	void SetUpdateProjMatrixFunc(UpdateProjMatrixFunc func);

	/// <summary>
	/// 画角を設定
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Perspectiveが設定されているときに使用される。
	/// </remarks>
	/// <param name="viewAngle">画角、単位ラジアン</param>
	void SetFovAngleY(float fovAngle);

	/// <summary>
	/// 平行投影の左端を設定
	/// </summary>
	/// <param name="left_">左端</param>
	void SetLeft(float left_);

	/// <summary>
	/// 平行投影の右端を設定
	/// </summary>
	/// <param name="right_">右端</param>
	void SetRight(float right_);

	/// <summary>
	/// 平行投影の上端を設定
	/// </summary>
	/// <param name="top_">上端</param>
	void SetTop(float top_);

	/// <summary>
	/// 平行投影の下端を設定
	/// </summary>
	/// <param name="bottom_">下端</param>
	void SetBottom(float bottom_);
#pragma endregion

#pragma region ゲッター

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewMatrix();

	/// <summary>
	/// ビュー行列の逆行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewMatrixInv();

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetProjectionMatrix();

	/// <summary>
	/// ビュー×プロジェクション行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetViewProjectionMatrix();

	/// <summary>
	/// カメラの回転行列を取得
	/// </summary>
	const AliceMathF::Matrix4& GetCameraRotation();

	/// <summary>
	/// 遠平面までの距離を取得
	/// </summary>
	float GetFar() const;

	/// <summary>
	/// 近平面までの距離を取得
	/// </summary>
	float GetNear() const;

	/// <summary>
	/// アスペクト比を取得
	/// </summary>
	float GetAspect() const;

	/// <summary>
	/// 画角を取得
	/// </summary>
	/// <returns>画角。単位ラジアン</returns>
	float GetFovAngleY() const;

	/// <summary>
	/// 注視点と視点の距離を取得
	/// </summary>
	/// <returns></returns>
	float GetTargetToPositionLength() const;

	/// <summary>
	/// カメラの前方向を取得
	/// </summary>
	const AliceMathF::Vector3& GetForward() const;

	/// <summary>
	/// 平行投影の下端を取得
	/// </summary>
	float GetBottom();

	/// <summary>
	/// 平行投影の上端を取得
	/// </summary>
	float GetTop();

	/// <summary>
	/// 平行投影の右端を取得
	/// </summary>
	float GetRight();

	/// <summary>
	/// 平行投影の左端を取得
	/// </summary>
	float GetLeft();

#pragma endregion
};

