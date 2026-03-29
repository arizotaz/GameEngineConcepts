#ifndef FREETYPE_SAFE_H
#define FREETYPE_SAFE_H

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Error Safe_Request_Size(FT_Face face, FT_Size_Request req);

#endif // FREETYPE_SAFE_H