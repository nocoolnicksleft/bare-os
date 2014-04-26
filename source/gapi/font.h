#pragma once

#include "common.h"

enum FontType
{
	FontTypeLucidaConsole10,
	FontTypeComicSans10,
	FontTypeComicSans12,
        FontTypeVGA,
        FontTypeEGA,
        FontTypeNoritake,
        FontType6x8
};

enum CodePage
{
    CodePageASCII_8BIT,
    CodePageISO_8859
};

class Font
{

public:
    
        uint8 _char_definition_width;
        uint8 _char_definition_height;
	int8 _char_width;
	int8 _char_height;
	int8 _spacing_h;
	int8 _spacing_h_space;
	int8 _spacing_v;
	bool _fixed_width;
	uint8 * _map;
	uint8 * _width_map;
        int8 _first_charactercode;
        int8 _last_charactercode;
        CodePage _codepage;
        

public:
	Font(FontType type);

public:
	~Font(void);

};

