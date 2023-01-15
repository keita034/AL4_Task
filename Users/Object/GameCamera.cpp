#include "GameCamera.h"

void GameCamera::Initialize()
{

	input_ = Input::GetInstance();

	thirdPersonCamera_ = std::make_unique<Camera>();
	thirdPersonCamera_->SetFar(40000.0f);
	thirdPersonCamera_->Initialize(UpdateProjMatrixFunc_Perspective);
;
	thirdPersonTransform_.translation = {0.0f,3000.0f,0.0f};
}

void GameCamera::Update()
{
	AliceMathF::Vector3 move(0, 0, 0);
	AliceMathF::Vector3 front;
	AliceMathF::Vector3 frontVec;
	AliceMathF::Vector3 frontNormVec;

	float rotationSpeed = 0.01f;

	thirdPersonTransform_.rotation.y += rotationSpeed * input_->GetLeftStickVec().x;
	thirdPersonTransform_.rotation.x += rotationSpeed * -input_->GetLeftStickVec().y;

	thirdPersonTransform_.rotation.y += rotationSpeed * input_->GetKeyTime(DIK_D) / 254.0f;
	thirdPersonTransform_.rotation.y += rotationSpeed * -input_->GetKeyTime(DIK_A) / 254.0f;
	thirdPersonTransform_.rotation.x += rotationSpeed * -input_->GetKeyTime(DIK_W) / 254.0f;
	thirdPersonTransform_.rotation.x += rotationSpeed * input_->GetKeyTime(DIK_S) / 254.0f;

	front.x = thirdPersonTransform_.translation.x + cosf(thirdPersonTransform_.rotation.y - AliceMathF::AX_PIHalf);
	front.y = thirdPersonTransform_.translation.y + cosf(thirdPersonTransform_.rotation.x - AliceMathF::AX_PIHalf);
	front.z = thirdPersonTransform_.translation.z - sinf(thirdPersonTransform_.rotation.y - AliceMathF::AX_PIHalf);


	frontVec = front - thirdPersonTransform_.translation;
	frontNormVec = frontVec.normalization();
	front_ = frontNormVec;

	moveSpeed_ = speed_ * frontNormVec;

	ThirdPersonUpdate(moveSpeed_, rotSpeed_);
}

Camera* GameCamera::GetThirdPersonCamera()
{
	return thirdPersonCamera_.get();
}

float GameCamera::GetSpeed()
{
	return speed_;
}

Transform* GameCamera::GetFirstPersonTransform()
{
	return &thirdPersonTransform_;
}

AliceMathF::Vector3 GameCamera::GetWorldPosition()
{
	return AliceMathF::GetWorldPosition(thirdPersonTransform_);
}

void GameCamera::SetPos(AliceMathF::Vector3 pos)
{
	thirdPersonTransform_.translation = pos;
	thirdPersonTransform_.rotation = { 0.0f,0.0f,0.0f };
}

AliceMathF::Vector3 GameCamera::GetFront()
{
	return front_;
}

void GameCamera::ThirdPersonUpdate(AliceMathF::Vector3& move, AliceMathF::Vector3& rot)
{
	//ワールドトランスフォームの数値を加算
	thirdPersonTransform_.translation.x += move.x;
	thirdPersonTransform_.translation.y -= move.y;
	thirdPersonTransform_.translation.z += move.z;

	thirdPersonTransform_.rotation += rot;
	//ワールドトランスフォームの更新
	thirdPersonTransform_.matWorld = AliceMathF::MakeWorldMatrix4(thirdPersonTransform_);

	thirdPersonCamera_->eye = AliceMathF::GetWorldPosition(thirdPersonTransform_);
	//ワールド前方ベクトル
	AliceMathF::Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = AliceMathF::Vec3Mat4Mul(forward, thirdPersonTransform_.matWorld);
	//視点から前方に適当な距離進んだ位置が注視点
	thirdPersonCamera_->target = thirdPersonCamera_->eye + forward;
	//ワールド上方ベクトル
	AliceMathF::Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	thirdPersonCamera_->up = AliceMathF::Vec3Mat4Mul(up, thirdPersonTransform_.matWorld);
}
