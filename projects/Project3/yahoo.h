#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "inet.h"
namespace V6 {

	struct Rousoku
	{
		float ymax,ymin;
		float ystart,yend;
		char date[20];


		float average[2]; // 0:7days, 1:120days
		WORD xpos;
	};
};

