#pragma once
#include "Model.h"

#include <memory>
#include <vector>

using namespace DirectX;

class RobotComponent
{
protected:
	float rotX;
	float rotY;
	float rotZ;

	float transX;
	float transY;
	float transZ;

	float scaling;

	XMMATRIX PARENT_ORIGIN;
	XMMATRIX CURRENT_ORIGIN;

public:
	RobotComponent();

	void SetParentOrigin(XMMATRIX NewOrigin);
	void SetCurrentOrigin(XMMATRIX NewOrigin);

	void SetRotationX(float RotationX);
	void SetRotationY(float RotationY);
	void SetRotationZ(float RotationZ);

	void SetOffsetX(float OffsetX);
	void SetOffsetY(float OffsetY);
	void SetOffsetZ(float OffsetZ);

	void SetScaling(float Scale);

	virtual void Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX world, XMMATRIX view, XMMATRIX proj);
	virtual void Transform();

	
};

class RobotLeaf : public RobotComponent
{
	std::unique_ptr<Model> ptr_model;

public:

	void SetModel(std::unique_ptr<Model> model);
	void ReleaseModel();
	virtual void Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX world, XMMATRIX view, XMMATRIX proj) override;
	virtual void Transform() override;

	
};

class RobotComposite : public RobotComponent
{
protected:
	std::vector< RobotComponent * > ptr_composite;

public:
	virtual void Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX world, XMMATRIX view, XMMATRIX proj) override;
	virtual void Transform() override;

	void addComposite(RobotComposite& NewComposite);
	void addLeaf(RobotLeaf& NewLeaf);

	//void getChild();
};