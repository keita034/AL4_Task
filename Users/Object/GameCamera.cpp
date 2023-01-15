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
	//���[���h�g�����X�t�H�[���̐��l�����Z
	thirdPersonTransform_.translation.x += move.x;
	thirdPersonTransform_.translation.y -= move.y;
	thirdPersonTransform_.translation.z += move.z;

	thirdPersonTransform_.rotation += rot;
	//���[���h�g�����X�t�H�[���̍X�V
	thirdPersonTransform_.matWorld = AliceMathF::MakeWorldMatrix4(thirdPersonTransform_);

	thirdPersonCamera_->eye = AliceMathF::GetWorldPosition(thirdPersonTransform_);
	//���[���h�O���x�N�g��
	AliceMathF::Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = AliceMathF::Vec3Mat4Mul(forward, thirdPersonTransform_.matWorld);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	thirdPersonCamera_->target = thirdPersonCamera_->eye + forward;
	//���[���h����x�N�g��
	AliceMathF::Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	thirdPersonCamera_->up = AliceMathF::Vec3Mat4Mul(up, thirdPersonTransform_.matWorld);
}
