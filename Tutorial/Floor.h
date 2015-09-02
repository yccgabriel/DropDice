#pragma once
#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_VERTICES {\
-2.0f, -2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,\
2.0f, -2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,\
2.0f, 2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,\
-2.0f, 2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}

#include "RectangularPrism.h"

class Floor : public RectangularPrism
{
public:
	Floor() : RectangularPrism(FLOOR_VERTICES){  }
	//~Floor();
};

#endif // FLOOR_H