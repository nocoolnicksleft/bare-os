#pragma once

#include "common.h"
#include "flipbook.h"
#include "window.h"
#include "bitmap.h"
#include "screen.h"
#include "../main.h"
#include "../bcm2835.h"


class ScreenRaspi : public Window
{
protected:
        unsigned int _width;
        unsigned int _height;
        unsigned int _colordepth;
        
        fbInfo * _fbinfo;
        
        bool _available;
        
        unsigned short int * _bufferptr;

public:
	ScreenRaspi(unsigned int pWidth, unsigned int pHeight, unsigned int pColorDepth);
	virtual ~ScreenRaspi(void);

public:	
	bool Init(void);
	virtual void Display();
};
