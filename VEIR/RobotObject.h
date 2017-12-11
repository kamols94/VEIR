#pragma once
#include <memory>
#include "Model.h"

class RobotObject
{
private:
	std::unique_ptr<DirectX::Model>			m_model;
public:
	RobotObject();
	~RobotObject();
};

