#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

class Writer
{
public:
    Writer();
    ~Writer();
    void renderText(const char* text, float x, float y, float sx, float sy);

private:
    FT_Library ft;
};
