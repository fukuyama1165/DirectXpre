#include "camera.h"

Camera* Camera::nowCamera = nullptr;

Camera::Camera()
{
	Win_width_ = (float)WinApp::SWindowWidth_;
	Win_height_ = (float)WinApp::SWindowHeight_;
}

Camera::Camera(float win_width,float win_height)
{
	
	Win_width_ = win_width;
	Win_height_ = win_height;
}

Camera:: ~Camera()
{

}

void Camera::upDate()
{
	matView_ = matViewGeneration(eye_, target_, up_);

	matProjection_ = perspectiveProjectionGeneration((45.0f * (PI / 180)), 0.1f, 1000000.0f);
}

void Camera::setDefCamera()
{
	nowCamera->eye_ = { 0,0,0 };
	nowCamera->target_ = { 0,0,1 };
	nowCamera->up_ = { 0,1,0 };

}

Matrix4x4 Camera::matViewGeneration(const Vector3& eye,const Vector3& target,const Vector3& up)
{

	Vector3 zVer = target - eye;

	zVer.normalize();

	forward_ = zVer;

	Vector3 xVer = Vector3::cross(up, zVer);

	xVer.normalize();

	rightDirection = xVer;

	Vector3 yVer = zVer.cross(xVer);

	yVer.normalize();

	Matrix4x4 cameraRotateMat = {};

	cameraRotateMat.IdentityMatrix();

	cameraRotateMat.m[0][0] = xVer.x;
	cameraRotateMat.m[0][1] = xVer.y;
	cameraRotateMat.m[0][2] = xVer.z;

	cameraRotateMat.m[1][0] = yVer.x;
	cameraRotateMat.m[1][1] = yVer.y;
	cameraRotateMat.m[1][2] = yVer.z;

	cameraRotateMat.m[2][0] = zVer.x;
	cameraRotateMat.m[2][1] = zVer.y;
	cameraRotateMat.m[2][2] = zVer.z;

	cameraRotateMat.m[3][0] = eye.x;
	cameraRotateMat.m[3][1] = eye.y;
	cameraRotateMat.m[3][2] = eye.z;

	Matrix4x4 ans = {};

	ans = cameraRotateMat.InverseMatrix();

	Vector3 eyeDis = {};
	eyeDis = { target.x - eye.x,target.y - eye.y,target.z - eye.z };

	Vector3 R2 = eyeDis.normalize();

	Vector3 R0 = Vector3::cross(up, R2);
	R0 = R0.normalize();

	Vector3 R1 = R2.cross(R0);

	Vector3 NegEyePosition = { -eye.x,-eye.y,-eye.z };

	Vector3 D0 = float3Dat(R0, NegEyePosition);
	Vector3 D1 = float3Dat(R1, NegEyePosition);
	Vector3 D2 = float3Dat(R2, NegEyePosition);

	Matrix4x4 M;
	M.m[0][0] = R0.x;
	M.m[0][1] = R0.y;
	M.m[0][2] = R0.z;
	M.m[0][3] = D0.x;

	M.m[1][0] = R1.x;
	M.m[1][1] = R1.y;
	M.m[1][2] = R1.z;
	M.m[1][3] = D1.x;

	M.m[2][0] = R2.x;
	M.m[2][1] = R2.y;
	M.m[2][2] = R2.z;
	M.m[2][3] = D2.x;

	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;

	//M = XMMatrixTranspose(M);

	//// x.x,x.y,y.x,y.y
	//XMVECTOR vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
	//// x.z,x.w,y.z,y.w
	//XMVECTOR vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	//// z.x,z.y,w.x,w.y
	//XMVECTOR vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
	//// z.z,z.w,w.z,w.w
	//XMVECTOR vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

	Vector4 vTemp1 = { M.m[0][0],M.m[0][1],M.m[1][0],M.m[1][1] };
	Vector4 vTemp2 = { M.m[0][2],M.m[0][3],M.m[1][2],M.m[1][3] };
	Vector4 vTemp3 = { M.m[2][0],M.m[2][1],M.m[3][0],M.m[3][1] };
	Vector4 vTemp4 = { M.m[2][2],M.m[2][3],M.m[3][2],M.m[3][3] };

	//XMMATRIX mResult;
	//// x.x,y.x,z.x,w.x
	//mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	//// x.y,y.y,z.y,w.y
	//mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	//// x.z,y.z,z.z,w.z
	//mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	//// x.w,y.w,z.w,w.w
	//mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

	Matrix4x4 mResult;
	mResult.m[0][0] = vTemp1.x;
	mResult.m[0][1] = vTemp1.z;
	mResult.m[0][2] = vTemp3.x;
	mResult.m[0][3] = vTemp3.z;

	mResult.m[1][0] = vTemp1.y;
	mResult.m[1][1] = vTemp1.w;
	mResult.m[1][2] = vTemp3.y;
	mResult.m[1][3] = vTemp3.w;

	mResult.m[2][0] = vTemp2.x;
	mResult.m[2][1] = vTemp2.z;
	mResult.m[2][2] = vTemp4.x;
	mResult.m[2][3] = vTemp4.z;

	mResult.m[3][0] = vTemp2.y;
	mResult.m[3][1] = vTemp2.w;
	mResult.m[3][2] = vTemp4.y;
	mResult.m[3][3] = vTemp4.w;

	//XMMATRIX matvi;

	//XMFLOAT3 eye2;
	//XMFLOAT3 target2;
	//XMFLOAT3 up2;

	////ビュー変換行列
	//eye2 = { eye.x, eye.y, eye.z };//視点座標
	//target2 = { target.x, target.y, target.z };//注視点座標
	//up2 = { up.x, up.y, up.z };//上方向ベクトル

	//matvi = XMMatrixLookAtLH(XMLoadFloat3(&eye2), XMLoadFloat3(&target2), XMLoadFloat3(&up2));

	return mResult;
}

Matrix4x4 Camera::perspectiveProjectionGeneration(float FovAngleY,float NearZ,float FarZ)
{
	Matrix4x4 ans = {};

	/*ans.IdentityMatrix();
	float fReciprocalWidth = 1.0f / (ViewRight - 0);
	float fReciprocalHeight = 1.0f / (0 - ViewBottom);
	float fRange = 1.0f / (FarZ - NearZ);
	ans.m[0][0] = {fReciprocalWidth * 2};
	ans.m[1][1] = { fReciprocalHeight * 2 };
	ans.m[2][2] = { fRange };
	ans.m[3][0] = { -(0 + ViewRight) * fReciprocalWidth };
	ans.m[3][1] = { -(0 + ViewBottom) * fReciprocalHeight };
	ans.m[3][2] = { fRange * -NearZ };*/

	float SinFov;
	float CosFov;
	float AspectRatio = Win_width_ / Win_height_;
	sinCos(SinFov, CosFov, 0.5f * FovAngleY);

	float fRange = FarZ / (FarZ - NearZ);

	float Height = CosFov / SinFov;


	ans.m[0][0] = Height / AspectRatio;

	ans.m[1][1] = Height;

	ans.m[2][2] = fRange;
	ans.m[2][3] = 1.0f;

	ans.m[3][2] = (-fRange) * NearZ;

	return ans;

}

void Camera::sinCos(float& Sin, float& Cos,const float& angle)
{
	Sin = sinf(angle);
	Cos = cosf(angle);
}

Vector3 Camera::float3Dat(const Vector3& A,const Vector3& B)
{
	Vector3 num = { A.x * B.x,A.y * B.y,A.z * B.z };

	num.x = num.x + num.y + num.z;

	num = { num.x,num.x,num.x };

	return num;

}