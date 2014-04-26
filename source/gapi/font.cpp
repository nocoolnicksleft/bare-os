
#include "font.h"
#include "fonts/LucidaConsole10.h"
#include "fonts/LucidaConsole10.width.h"
#include "fonts/ComicSansMS10.h"
#include "fonts/ComicSansMS10.width.h"
#include "fonts/ComicSansMS12.h"
#include "fonts/ComicSansMS12.width.h"
#include "fonts/VGA_16x16.h"
#include "fonts/EGA_8x8.h"
#include "fonts/Noritake5x5.h"
#include "fonts/font6x8.h"

Font::Font(FontType type)
{
	if (type == FontTypeLucidaConsole10)
	{
		_fixed_width = true;
		_char_width = 7;
		_char_height = 10;
		_char_definition_width = 16;
		_char_definition_height = 16;
		_spacing_h = 0;
		_spacing_h_space = 0;
		_spacing_v = 11;
		_map = (uint8 *)LucidaConsole10;
		_width_map = (uint8 *)LucidaConsole10_width;
                _first_charactercode = 32;
                _last_charactercode = 255;
                _codepage = CodePageISO_8859;
	}
	else if (type == FontTypeComicSans10)
	{
		_fixed_width = false;
		_char_width = 16;
		_char_height = 16;
		_char_definition_width = 16;
		_char_definition_height = 16;
		_spacing_h = 1;
		_spacing_h_space = 2;
		_spacing_v = 14;
		_map = (uint8 *)ComicSansMS10;
		_width_map = (uint8 *)ComicSansMS10_width;
                _first_charactercode = 32;
                _last_charactercode = 255;
                _codepage = CodePageISO_8859;
	}
	else if (type == FontTypeComicSans12)
	{
		_fixed_width = false;
		_char_width = 16;
		_char_height = 16;
		_char_definition_width = 16;
		_char_definition_height = 16;
		_spacing_h = 2;
		_spacing_h_space = 5;
		_spacing_v = 15;
		_map = (uint8 *)ComicSansMS12;
		_width_map = (uint8 *)ComicSansMS12_width;
                _first_charactercode = 32;
                _last_charactercode = 255;
                _codepage = CodePageISO_8859;
	}
	else if (type == FontTypeVGA)
	{
		_fixed_width = true;
		_char_width = 9;
		_char_height = 16;
		_char_definition_width = 16;
		_char_definition_height = 16;
		_spacing_h = 0;
		_spacing_h_space = 0;
		_spacing_v = 14;
		_map = (uint8 *)VGA_16x16;
		_width_map = (uint8 *)0;
                _first_charactercode = 0;
                _last_charactercode = 255;
                _codepage = CodePageASCII_8BIT;
	}
	else if (type == FontTypeEGA)
	{
		_fixed_width = true;
		_char_width = 8;
		_char_height = 8;
		_char_definition_width = 8;
		_char_definition_height = 8;
		_spacing_h = 0;
		_spacing_h_space = 0;
		_spacing_v = 10;
		_map = (uint8 *)EGA_8x8;
		_width_map = (uint8 *)0;
                _first_charactercode = 0;
                _last_charactercode = 255;
                _codepage = CodePageASCII_8BIT;
	}
	else if (type == FontType6x8)
	{
		_fixed_width = true;
		_char_width = 6;
		_char_height = 8;
		_char_definition_width = 8;
		_char_definition_height = 8;
		_spacing_h = 0;
		_spacing_h_space = 0;
		_spacing_v = 10;
		_map = (uint8 *)font6x8;
		_width_map = (uint8 *)0;
                _first_charactercode = 0;
                _last_charactercode = 255;
                _codepage = CodePageASCII_8BIT;
	}
	else if (type == FontTypeNoritake)
	{
		_fixed_width = true;
		_char_width = 5;
		_char_height = 5;
		_char_definition_width = 8;
		_char_definition_height = 8;
		_spacing_h = 1;
		_spacing_h_space = 0;
		_spacing_v = 6;
		_map = (uint8 *)Noritake5x5;
		_width_map = (uint8 *)0;
                _first_charactercode = 32;
                _last_charactercode = 95;
                _codepage = CodePageASCII_8BIT;
	}

}

Font::~Font(void)
{
}


