#pragma once
#include "RobotObject.h"

class RobotComponent
{
public:
	RobotComponent();
	~RobotComponent();
};

class RobotLeaf : public RobotComponent
{
	RobotObject r_object;
};

class RobotComposite : public RobotComponent
{
	RobotObject r_object;
};