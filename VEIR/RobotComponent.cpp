#include "RobotComponent.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void RobotComponent::SetParentOrigin(XMMATRIX NewOrigin)
{
	this->PARENT_ORIGIN = NewOrigin;
}

void RobotComponent::Render(ID3D11DeviceContext * pd3dImmediateContext, const CommonStates & states,  XMMATRIX view, XMMATRIX proj)
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

void RobotLeaf::Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states,  XMMATRIX view, XMMATRIX proj)
{
	this->ptr_model->Draw(pd3dImmediateContext, states, this->PARENT_ORIGIN, view, proj);
}

void RobotLeaf::Transform()
{

}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void RobotComposite::Render(ID3D11DeviceContext* pd3dImmediateContext, const CommonStates &states, XMMATRIX view, XMMATRIX proj)
{
	this->ptr_composite;
	for (std::vector< RobotComponent* >::size_type i = 0; i != this->ptr_composite.size(); i++)
	{
		this->ptr_composite.at(i)->Render(pd3dImmediateContext, states, view, proj);
	}
}

void RobotComposite::Transform()
{
	XMMATRIX TRANSLATION = XMMatrixTranslation(this->transX, this->transY, this->transZ);
	//XMMATRIX TRANSLATION_Z = XMMatrixTranslation(0, 0, this->transZ);
	XMMATRIX ROTATION_X = XMMatrixRotationX(this->rotX);
	XMMATRIX ROTATION_Z = XMMatrixRotationZ(this->rotZ);
	XMMATRIX TRANSFORMATION = XMMatrixMultiply(XMMatrixMultiply(ROTATION_X, TRANSLATION), ROTATION_Z);
	this->CURRENT_ORIGIN = XMMatrixMultiply(TRANSFORMATION, this->PARENT_ORIGIN);


	for (std::vector< RobotComponent* >::size_type i = 0; i != this->ptr_composite.size(); i++)
	{
		this->ptr_composite.at(i)->SetParentOrigin(this->CURRENT_ORIGIN);
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

void RobotComposite::SetTxTzRxRz(float translationX, float translationZ, float rotationX, float rotationZ)
{
	this->transX = translationX;
	this->transZ = translationZ;
	this->rotX = rotationX*XM_PI / 180;
	this->rotZ = rotationZ*XM_PI / 180;
}

void RobotComposite::SetTranslationX(float translationX)
{this->transX = translationX;}

void RobotComposite::SetTranslationY(float translationY)
{this->transY = translationY;}

void RobotComposite::SetTranslationZ(float translationZ)
{this->transZ = translationZ;}

void RobotComposite::SetRotationX(float rotationX)
{this->rotX = rotationX*XM_PI / 180;}

void RobotComposite::SetRotationY(float rotationY)
{this->rotY = rotationY*XM_PI / 180;}

void RobotComposite::SetRotationZ(float rotationZ)
{this->rotZ = rotationZ*XM_PI / 180;}

float RobotComposite::GetTranslationX()
{return this->transX;}

float RobotComposite::GetTranslationY()
{return this->transY;}

float RobotComposite::GeTranslationZ()
{return this->transZ;}

float RobotComposite::GetRotationX()
{return this->rotX;}

float RobotComposite::GetRotationY()
{return this->rotY;}

float RobotComposite::GetRotationZ()
{return this->rotZ;}

XMMATRIX RobotComposite::GetCurrentOrigin()
{return this->CURRENT_ORIGIN;}
