#pragma once
#include "BaseObject.h"

class CharacterController : public BaseObject
{
	CreateClassMetadata(CharacterController)

public:
	//TODO: Make it so we don't need any of these parameters.
	virtual bool Initialize(const char* imagePath, float inWidth, float inHeight, mathfu::Vector<float, 2> inPosition) override;

	virtual void UpdateInputs();

	inline mathfu::Vector<float, 2> GetLeftAxis() { return leftAxis; }
	inline mathfu::Vector<float, 2> GetrightAxis() { return rightAxis; }

private:
	//Input axes set by kbm or gamepad
	mathfu::Vector<float, 2> leftAxis, rightAxis;

	//TODO: Make project-defined inputs as defined below
	//List of project-defined inputs set in a data file somewhere.
	//These project-defined inputs can be mapped to individual controller or kbm inputs.
	//They can also be registered to specific macros (e.g. Press A + B)
};