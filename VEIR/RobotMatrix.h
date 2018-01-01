#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class RobotMatrix
{
public:
	void LoadXMMATRIX(XMMATRIX IN_MATRIX);

	float r11;
	float r12;
	float r13;
	float px;

	float r21;
	float r22;
	float r23;
	float py;

	float r31;
	float r32;
	float r33;
	float pz;


protected:
	XMFLOAT4X4 OUT_MATRIX;
};

