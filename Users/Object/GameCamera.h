#pragma once
#include"Camera.h"
#include"AliceMathUtility.h"
#include"Transform.h"
#include"ErrorException.h"
#include"Input.h"

class GameCamera
{
private:
	//三人称
	std::unique_ptr<Camera> thirdPersonCamera_;


	AliceMathF::Vector3 front_ = { 0.0f,0.0f,speed_ };
	char PADING[4]{};
	Transform thirdPersonTransform_;
	//スピード
	float speed_ = 12.0f;
	AliceMathF::Vector3 moveSpeed_ = { 0.0f,0.0f,speed_ };
	AliceMathF::Vector3 rotSpeed_ = { 0.0f,0.0f,0.0f };
	char PADING2[4]{};
	Input* input_ = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	Camera* GetThirdPersonCamera();
	float GetSpeed();
	Transform* GetFirstPersonTransform();
	AliceMathF::Vector3 GetWorldPosition();
	void SetPos(AliceMathF::Vector3 pos);

	GameCamera() = default;
	~GameCamera() = default;

	AliceMathF::Vector3 GetFront();
private:

	void ThirdPersonUpdate(AliceMathF::Vector3& move, AliceMathF::Vector3& rot);
	//コピーコンストラクタ・代入演算子削除
	GameCamera& operator=(const GameCamera&) = delete;
	GameCamera(const GameCamera&) = delete;
};

