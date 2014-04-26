#include "common.h"
#include "screen.h"
#include "screenraspi.h"
#include "bitmap.h"


ScreenRaspi::ScreenRaspi(unsigned int pWidth, unsigned int pHeight, unsigned int pColorDepth)
{
        _initialize();

	_width = pWidth;
        _height = pHeight;
        _colordepth = pColorDepth;
        
        _bufferptr = 0;
        
        _available = false;
        
        _fbinfo = 0;

        _fbinfo = InitialiseFrameBuffer(_width, _height, _colordepth);

        if (_fbinfo->pointer) {

         //   SetGraphicsAddress(_fbinfo);

            _bufferptr = (uint16 *)_fbinfo->pointer;
            _available = true;

        } else {

            _available = false;

        }    
        
        /* ATTN: From here, the Window::Window constructor is doubled up Find a better solution someday */
        
	_position = new Rectangle(0,0,_width,_height);

	_buffer = new Bitmap(_width,_height,_bufferptr);

	_textid = new char[20];
	strncpy(_textid,"screen",19);

	_inner_rect = new Rectangle(0,0,_position->Width, _position->Height);

	Clear();
}

bool ScreenRaspi::Init()
{
    _fbinfo = InitialiseFrameBuffer(_width, _height, _colordepth);

    if (_fbinfo->pointer) {

      //  SetGraphicsAddress(_fbinfo);
        
        _bufferptr = (uint16 *)_fbinfo->pointer;
        _available = true;
        
    } else {
    
        _available = false;
        
    }

    return _available;
    
}

ScreenRaspi::~ScreenRaspi(void)
{
	/* if (_background_buffer) delete _background_buffer; */
}

void ScreenRaspi::Display()
{
	/* _background_buffer->CopyBuffer(_buffer); */
	Window::Display();
}


