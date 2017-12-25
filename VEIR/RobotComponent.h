#pragma once
#include "Model.h"

#include <memory>
#include <vector>

using namespace DirectX;

class RobotComponent
{
protected:

	XMMATRIX PARENT_ORIGIN;

public:
	void SetParentOrigin(XMMATRIX NewOrigin);

	virtual void Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX view, XMMATRIX proj);
	virtual void Transform();

	
};

class RobotLeaf : public RobotComponent
{
	std::unique_ptr<Model> ptr_model;

public:

	void SetModel(std::unique_ptr<Model> model);
	void ReleaseModel();
	virtual void Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX view, XMMATRIX proj) override;
	virtual void Transform() override;	
};

class RobotComposite : public RobotComponent
{
protected:
	float transX;
	float transZ;
	float rotX;
	float rotZ;
	XMMATRIX CURRENT_ORIGIN;
	std::vector< RobotComponent * > ptr_composite;

public:
	void SetTxTzRxRz(float tx, float tz, float rx, float rz);
	void SetTranslationX(float tx);
	void SetTranslationZ(float tz);
	void SetRotationX(float rx);
	void SetRotationZ(float rz);

	float GetTranslationX();
	float GeTranslationZ();
	float GetRotationX();
	float GetRotationZ();


	virtual void Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX view, XMMATRIX proj) override;
	virtual void Transform() override;

	void addComposite(RobotComposite& NewComposite);
	void addLeaf(RobotLeaf& NewLeaf);
};