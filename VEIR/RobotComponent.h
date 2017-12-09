#pragma once
class RobotComponent
{
public:
	RobotComponent();
	~RobotComponent();
};

class RobotLeaf : public RobotComponent
{

};

class RobotComposite : public RobotComponent
{

};