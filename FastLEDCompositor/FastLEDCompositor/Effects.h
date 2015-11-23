// Definitions for all effect classes. Each effect just has to implement its specific draw function
// (should be put in a template)

#pragma once
#include "Effect.h"
class SolidEffect :
	public Effect
{
public:

	void draw();
};

class RainbowEffect :
	public Effect
{
public:
	void draw();
};

class ConfettiEffect :
	public Effect
{
public:
	void draw();
};

class BaseBeatEffect :
	public Effect
{
public:
	void draw();
};
