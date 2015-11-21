// 

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
