#include "Camera.h"


void Camera::Initialize(UpdateProjMatrixFunc matFunc)
{
	//?A?X?y?N?g?????v?Z????
	aspect =
		static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth()) /
		static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight());

	matrixFunc = matFunc;

	if (matFunc == UpdateProjMatrixFunc_Ortho)//???s???e
	{
		if (far_ == 0.0f)
		{
			far_ = 1.0f;
		}
		right = static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth());
		bottom = static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight());

		//???s???e?̌v?Z
		AliceMathF::MakeOrthogonalL(left, right, bottom, top, near_, far_, projectionMatrix);

	}
	else//???????e
	{
		if (near_ == 0.0f)
		{
			near_ = 0.1f;
		}
		if (far_ == 0.0f)
		{
			far_ = 1000.0f;
		}
		if (fovAngleY == 0.0f)
		{
			fovAngleY = AliceMathF::AX_PI / 4;
		}

		//???????e?s???̌v?Z
		AliceMathF::MakePerspectiveL(fovAngleY, aspect, near_, far_, projectionMatrix);

	}

	updateProjMatrix = false;

	//?r???[?s???̎Z?o
	AliceMathF::MakeLookL(eye, target, up, viewMatrix);
	//?r???[?s???̋t?s?????v?Z
	viewMatrixInv = AliceMathF::MakeInverse(&viewMatrix);
	//?r???[?v???W?F?N?V?????s???̍쐬
	viewProjectionMatrix = viewMatrix * projectionMatrix;

	//?J?????̑O?x?N?g???擾
	forward = { viewMatrixInv.m[2][0], viewMatrixInv.m[2][1], viewMatrixInv.m[2][2] };
	forward.normal();

	//?????_?Ǝ??_?̋????擾
	AliceMathF::Vector3 toPos;
	toPos = eye - target;
	tgtToPosLen = toPos.length();

	updateViewMatrix = false;
}

void Camera::Update()
{

	if (oldEye != eye || oldTarget != target || oldUp != up)
	{
		updateViewMatrix = true;

		oldEye = eye;
		oldTarget = target;
		oldUp = up;
	}


	if (updateProjMatrix)
	{
		if (matrixFunc == UpdateProjMatrixFunc_Ortho)
		{
			if (far_ == 0.0f)
			{
				far_ = 1.0f;
			}

			right = static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth());
			bottom = static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight());
			AliceMathF::MakeOrthogonalL(left, right, bottom, top, near_, far_, projectionMatrix);

		}
		else
		{
			if (near_ == 0.0f)
			{
				near_ = 0.1f;
			}
			if (far_ == 0.0f)
			{
				far_ = 1000.0f;
			}
			if (fovAngleY == 0.0f)
			{
				fovAngleY = AliceMathF::AX_PI / 4;
			}

			AliceMathF::MakePerspectiveL(fovAngleY, aspect, near_, far_, projectionMatrix);
		}
		updateProjMatrix = false;
	}

	if (updateViewMatrix)
	{
		//?r???[?s???̎Z?o
		AliceMathF::MakeLookL(eye, target, up, viewMatrix);
		//?r???[?v???W?F?N?V?????s???̍쐬
		viewProjectionMatrix = viewMatrix * projectionMatrix;
		//?r???[?s???̋t?s?????v?Z
		viewMatrixInv = AliceMathF::MakeInverse(&viewMatrix);

		forward = { viewMatrixInv.m[2][0], viewMatrixInv.m[2][1], viewMatrixInv.m[2][2] };

		AliceMathF::Vector3 toPos;
		toPos = eye - target;
		tgtToPosLen = toPos.length();

		updateViewMatrix = false;
	}
}

void Camera::Move(const AliceMathF::Vector3& move)
{
	eye += move;
	target += move;
	updateViewMatrix = true;
}

void Camera::MoveTarget(const AliceMathF::Vector3& move)
{
	target += move;
	updateViewMatrix = true;
}

void Camera::MovePosition(const AliceMathF::Vector3& move)
{
	eye += move;
	updateViewMatrix = true;
}

#pragma region ?Z?b?^?[

void Camera::SetAspect(float aspect_)
{
	aspect = aspect_;
	updateViewMatrix = true;
}

//void Camera::SetPosition(const AliceMathF::Vector3& pos)
//{
//	eye = pos;
//	updateViewMatrix = true;
//}
//
//void Camera::SetPosition(float x, float y, float z)
//{
//	SetPosition(AliceMathF::Vector3(x, y, z));
//}
//
//void Camera::SetTarget(const AliceMathF::Vector3& tgt)
//{
//	target = tgt;
//	updateViewMatrix = true;
//}
//
//void Camera::SetTarget(float x, float y, float z)
//{
//	SetTarget(AliceMathF::Vector3(x, y, z));
//}
//
//void Camera::SetUp(const AliceMathF::Vector3& upVec)
//{
//	up = upVec;
//	up.normal();
//	updateProjMatrix = true;
//}
//
//void Camera::SetUp(float x, float y, float z)
//{
//	SetUp(AliceMathF::Vector3(x, y, z));
//}

void Camera::SetFar(float fFar)
{
	far_ = fFar;
	updateProjMatrix = true;
}

void Camera::SetNear(float fNear)
{
	near_ = fNear;
	updateProjMatrix = true;
}

void Camera::SetFovAngleY(float fovAngle)
{
	this->fovAngleY = fovAngle;
	updateProjMatrix = true;
}

void Camera::SetUpdateProjMatrixFunc(UpdateProjMatrixFunc func)
{
	matrixFunc = func;
	updateProjMatrix = true;
}

void Camera::SetLeft(float left_)
{
	left = left_;
	updateProjMatrix = true;
}
void Camera::SetRight(float right_)
{
	right = right_;
	updateProjMatrix = true;
}

void Camera::SetTop(float top_)
{
	top = top_;
	updateProjMatrix = true;
}

void Camera::SetBottom(float bottom_)
{
	bottom = bottom_;
	updateProjMatrix = true;
}

#pragma endregion

#pragma region ?Q?b?^?[

//const AliceMathF::Vector3& Camera::GetPosition() const
//{
//	return eye;
//}
//
//const AliceMathF::Vector3& Camera::GetTarget() const
//{
//	return target;
//}
//
//const AliceMathF::Vector3& Camera::GetUp() const
//{
//	return up;
//}

const AliceMathF::Matrix4& Camera::GetViewMatrix()
{
	//?X?V
	Update();
	return viewMatrix;
}

const AliceMathF::Matrix4& Camera::GetViewMatrixInv()
{
	//?X?V
	Update();
	return viewMatrixInv;
}

const AliceMathF::Matrix4& Camera::GetProjectionMatrix()
{
	//?X?V
	Update();
	return projectionMatrix;
}

const AliceMathF::Matrix4& Camera::GetViewProjectionMatrix()
{
	//?X?V
	Update();
	return viewProjectionMatrix;
}

const AliceMathF::Matrix4& Camera::GetCameraRotation()
{
	//?X?V
	Update();
	return cameraRotation;
}

float Camera::GetFar() const
{
	return far_;
}

float Camera::GetNear() const
{
	return near_;
}

float Camera::GetFovAngleY() const
{
	return fovAngleY;
}

float Camera::GetTargetToPositionLength() const
{
	return tgtToPosLen;
}

const AliceMathF::Vector3& Camera::GetForward() const
{
	return forward;
}

float Camera::GetAspect() const
{
	return aspect;
}

float Camera::GetLeft()
{
	return left;
}

float Camera::GetRight()
{
	return right;
}

float Camera::GetTop()
{
	return top;
}

float Camera::GetBottom()
{
	return bottom;
}

#pragma endregion