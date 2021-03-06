#include "RobotMatrix.h"

void RobotMatrix::LoadXMMATRIX(XMMATRIX IN_MATRIX)
{
	XMStoreFloat4x4(&OUT_MATRIX, IN_MATRIX);
	r11 = OUT_MATRIX._11;
	r12 = OUT_MATRIX._21;
	r13 = OUT_MATRIX._31;
	px = OUT_MATRIX._41;

	r21 = OUT_MATRIX._12;
	r22 = OUT_MATRIX._22;
	r23 = OUT_MATRIX._32;
	py = OUT_MATRIX._42;

	r31 = OUT_MATRIX._13;
	r32 = OUT_MATRIX._23;
	r33 = OUT_MATRIX._33;
	pz = OUT_MATRIX._43;
}
