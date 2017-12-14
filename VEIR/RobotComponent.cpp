#include "RobotComponent.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
RobotComponent::RobotComponent()
{
	this->transX = 0.0f;
	this->transX = 0.0f;
	this->transX = 0.0f;

	this->rotX = 0.0f;
	this->rotX = 0.0f;
	this->rotX = 0.0f;

	this->scaling = 0.01f;
}

void RobotComponent::SetParentOrigin(XMMATRIX NewOrigin)
{
	this->PARENT_ORIGIN = NewOrigin;
}

void RobotComponent::SetCurrentOrigin(XMMATRIX NewOrigin)
{
	this->CURRENT_ORIGIN = NewOrigin;
}

void RobotComponent::SetRotationX(float RotationX)
{
	this->rotX = RotationX;
}

void RobotComponent::SetRotationY(float RotationY)
{
	this->rotY = RotationY;
}

void RobotComponent::SetRotationZ(float RotationZ)
{
	this->rotZ = RotationZ;
}

void RobotComponent::SetOffsetX(float OffsetX)
{
	this->transX = OffsetX;
}

void RobotComponent::SetOffsetY(float OffsetY)
{
	this->transY = OffsetY;
}

void RobotComponent::SetOffsetZ(float OffsetZ)
{
	this->transZ = OffsetZ;
}

void RobotComponent::SetScaling(float Scale)
{
}

void RobotComponent::Render(ID3D11DeviceContext * pd3dImmediateContext, const CommonStates & states, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
}

void RobotComponent::Transform()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void RobotLeaf::SetModel(std::unique_ptr<Model> model)
{
	this->ptr_model=std::move(model);
}

void RobotLeaf::ReleaseModel()
{
	this->ptr_model.reset();
}

void RobotLeaf::Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	this->ptr_model->Draw(pd3dImmediateContext, states, world, view, proj);
}

void RobotLeaf::Transform()
{
	

	//this->CURRENT_ORIGIN = 0;

}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void RobotComposite::Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX world, XMMATRIX view, XMMATRIX proj)
{
	this->ptr_composite;
	for (std::vector< RobotComponent* >::size_type i = 0; i != this->ptr_composite.size(); i++)
	{
		this->ptr_composite.at(i)->Render(pd3dImmediateContext, states, world, view, proj);
	}
}

void RobotComposite::Transform()
{
	for (std::vector< RobotComponent* >::size_type i = 0; i != this->ptr_composite.size(); i++)
	{
		this->ptr_composite.at(i)->Transform();
	}
}

void RobotComposite::addComposite(RobotComposite& NewComposite)
{
	this->ptr_composite.push_back(&NewComposite);
}

void RobotComposite::addLeaf(RobotLeaf& NewLeaf)
{
	this->ptr_composite.push_back(&NewLeaf);
}
